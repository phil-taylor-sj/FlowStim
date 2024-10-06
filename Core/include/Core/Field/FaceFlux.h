#pragma once

#include <Core/Domain/Mesh.h>
#include <Core/Field/Field.h>

namespace fstim
{
    class FaceFlux
    {
    public:
        static void Linear(
            const Mesh& mesh, 
            const VectorField& velocity,
            const double* rho,
            double* fluxs
        );

        static std::unique_ptr<double[]> Linear(
            const Mesh& mesh, 
            const VectorField& velocity,
            const double* rho
            );
    };
}