#include <Core/Solver/FaceValues.h>

namespace fstim
{
    template <typename T>
    std::unique_ptr<T[]> FaceValues<T>::interpolate(const Field<T>& field, const Mesh& mesh)
    {

        std::unique_ptr<T[]> faceValues = std::make_unique<T[]>(mesh.nFaces);

        const T* cellValues = field.readValues();

        for (int faceId = 0; faceId < mesh.nFaces; faceId++)
        {
            const Face& face = mesh.faces[faceId];

            // TODO: Add an owner weight property to each cell to remove need
            // for weight to be recaluclated during simulations.
            if (face.neighId != -1)
            {
                // --- Replace ---
                vecp::Vec2d ownerCenter = mesh.cells[face.ownerId].center;
                vecp::Vec2d neighCenter = mesh.cells[face.neighId].center;
                
                double internalWeight = (ownerCenter - face.center).mag() /
                    (ownerCenter - neighCenter).mag();
                // --------------

                faceValues[faceId] = cellValues[face.ownerId] * internalWeight
                    + cellValues[face.neighId] * (1. - internalWeight);          
                continue;
            }

            std::tuple<BcType, T> bc = field.getBc(mesh.getFaceSetId(face.id));              
            switch (std::get<0>(bc)) {
                case BcType::NONE:
                    [[fallthrough]];
                case BcType::ZEROGRADIENT:
                    faceValues[faceId] = cellValues[face.ownerId];
                    break;
                case BcType::FIXEDVALUE:
                    faceValues[faceId] = std::get<1>(bc);
                    break;
                default:
                    break;
            }
        }

        return std::move(faceValues);
    }

    template <typename T>
    std::unique_ptr<T[]> FaceValues<T>::interpolate(const T* cellValues, const Mesh& mesh)
    {
        std::unique_ptr<T[]> faceValues = std::make_unique<T[]>(mesh.nFaces);

        for (int faceId = 0; faceId < mesh.nFaces; faceId++)
        {
            const Face& face = mesh.faces[faceId];

            // TODO: Add an owner weight property to each cell to remove need
            // for weight to be recaluclated during simulations.
            if (face.neighId != -1)
            {
                // --- Replace ---
                vecp::Vec2d ownerCenter = mesh.cells[face.ownerId].center;
                vecp::Vec2d neighCenter = mesh.cells[face.neighId].center;
                
                double internalWeight = (ownerCenter - face.center).mag() /
                    (ownerCenter - neighCenter).mag();
                // --------------

                faceValues[faceId] = cellValues[face.ownerId] * internalWeight
                    + cellValues[face.neighId] * (1. - internalWeight);          

                continue;
            }

            faceValues[faceId] = cellValues[face.ownerId];
        }

        return std::move(faceValues);
    }

    template class FaceValues<double>;
    
    template class FaceValues<vecp::Vec2d>;
}