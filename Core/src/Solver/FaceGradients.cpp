#include <Core/Solver/FaceGradients.h>

namespace fstim
{
    template <typename T>
    std::unique_ptr<vecp::Vec2d[]> FaceGradients<T>::calculate(const Field<T>& field, const Mesh2d& mesh)
    {
        std::unique_ptr<vecp::Vec2d[]> faceGradients = std::make_unique<vecp::Vec2d[]>(mesh.nFaces);

        const T* cellValues = field.readValues();

        for (int faceId = 0; faceId < mesh.nFaces; faceId++)
        {
            const Face2d& face = mesh.faces[faceId];

            if (face.neighId != -1)
            {
                vecp::Vec2d ownerCenter = mesh.cells[face.ownerId].center;
                vecp::Vec2d neighCenter = mesh.cells[face.neighId].center;
                
                T gradient = (cellValues[face.neighId] - cellValues[face.ownerId])
                    / (neighCenter - ownerCenter).mag();

                faceGradients[faceId] = face.normal.normalise() * gradient;
                continue;
            }

            std::tuple<BcType, T> bc = field.getBc(mesh.getFaceSetId(face.id));              
            switch (std::get<0>(bc)) {
                case BcType::NONE:
                    [[fallthrough]];
                case BcType::ZEROGRADIENT:
                    faceGradients[faceId] = 0.;
                    break;
                case BcType::FIXEDVALUE: {
                    vecp::Vec2d ownerCenter = mesh.cells[face.ownerId].center;
                    T faceValue = std::get<1>(bc);
                    T gradient = (faceValue - cellValues[face.ownerId])
                        / (face.center - ownerCenter).mag();
                    faceGradients[faceId] = face.normal.normalise() * gradient;
                    break;
                }
                default:
                    break;
            }
        }

        return std::move(faceGradients);
    }

    template class FaceGradients<double>;
}