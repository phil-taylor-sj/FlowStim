#include <Core/Solver/FaceValues.h>

namespace fstim
{
    template <typename T>
    std::unique_ptr<T[]> interpolate(const Field<T>& field, const Mesh& mesh)
    {
        std::unique_ptr<T[]> faceValues = std::make_unique<T[]>(mesh.nFaces)

        const T* cellValues = field.getValues();

        for (int faceId = 0; faceId < mesh.nFaces; faceId++)
        {
            const Face& face = mesh.faces[faceId];

            // TODO: Add an owner weight property to each cell to remove need
            // for weight to be recaluclated during simulations.
            if (face.neighId != -1)
            {
                // --- Replace ---
                T ownerCenter = mesh.cells[face.ownerId].center;
                T neighCenter = mesh.cells[face.neighId].center;
                
                double internalWeight = (ownerCenter - neighCenter).mag() / 
                    (ownerCenter - face.center).mag();
                // --------------

                faceValues[faceId] = momentum[face.ownerId] * internalWeight
                    + momentum[face.neighId] * (1. - internalWeight);          

                continue;
            }

            std::tuple<BcType, T> bc = field.getBc(mesh.getFaceSetId(face.id));              
            switch (std::get<0>(bc)) {
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
        
        return std::move(faceValues)
    }
}