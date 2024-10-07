#pragma once

#include <Core/Domain/Mesh.h>
#include <Core/Field/Field.h>

namespace fstim
{
    class FaceFlux
    {
    public:
        static void Linear(
            double* flux,
            const Mesh& mesh, 
            const VectorField& velocity,
            const double* rho = nullptr
        );

        static std::unique_ptr<double[]> Linear(
            const Mesh& mesh, 
            const VectorField& velocity,
            const double* rho = nullptr
        );
    };
}