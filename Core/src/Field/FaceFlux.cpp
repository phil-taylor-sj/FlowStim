#include <Core/Field/FaceFlux.h>

namespace fstim
{
    void FaceFlux::Linear(
            double* flux,
            const Mesh& mesh, 
            const VectorField& velocity,
            const double* rho)
    {
        const vecp::Vec2d* values = velocity.readValues();

        for (int faceId = 0; faceId < mesh.nFaces; faceId++)
        {
            const Face& face = mesh.faces[faceId];

            if (face.neighId != -1)
            {
                vecp::Vec2d ownerCenter = mesh.cells[face.ownerId].center;
                vecp::Vec2d neighCenter = mesh.cells[face.neighId].center;
                
                double internalWeight = (ownerCenter - neighCenter).mag() / 
                    (ownerCenter - face.center).mag();

                vecp::Vec2d faceVelocity = values[face.ownerId] * internalWeight
                    + values[face.neighId] * (1. - internalWeight);

                double faceRho = (rho == nullptr) 
                    ? 1.
                    : rho[face.ownerId] * internalWeight
                        + rho[face.neighId] * (1. - internalWeight);

                flux[faceId] = face.normal.dot(faceVelocity * faceRho);

                continue;
            }

            // Calculate face flux
            double cellRho = (rho == nullptr) ? 1. : rho[face.ownerId];
            std::tuple<BcType, vecp::Vec2d> fluxBc = velocity.getBc(mesh.getFaceSetId(faceId));
            switch (std::get<0>(fluxBc)) {
                case BcType::NONE:
                    [[fallthrough]];
                case BcType::ZEROGRADIENT:
                    flux[faceId] = face.normal.dot(values[face.ownerId] * cellRho);
                    break;
                case BcType::FIXEDVALUE:
                    flux[faceId] = face.normal.dot(std::get<1>(fluxBc) * cellRho);
                    break;
                default:
                    break;
            }
        
        }
        
    }

    std::unique_ptr<double[]> FaceFlux::Linear(
        const Mesh& mesh, 
        const VectorField& velocity,
        const double* rho
        )
    {
        std::unique_ptr<double[]> flux = std::make_unique<double[]>(mesh.nFaces);
        FaceFlux::Linear(flux.get(), mesh, velocity, rho);
        return std::move(flux);
    }
}