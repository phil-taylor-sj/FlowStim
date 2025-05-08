#pragma once

#include <Core/Domain/Mesh.h>
#include <Core/Field/Field.h>
#include <Core/Field/FaceFlux.h>

namespace fstim
{
    template <typename T>
    class Divergence
    {
    public:

       void operator()(
            const Mesh2d& mesh, 
            Field<T>& field, 
            const VectorField& velocity,
            const double* rho = nullptr
            );

        Divergence();

        virtual ~Divergence() = default;
    };
}