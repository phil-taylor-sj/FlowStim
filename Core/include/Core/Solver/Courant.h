#pragma once

#include <Core/Domain/Mesh.h>
#include <Core/Field/Field.h>

namespace fstim
{
    class Courant
    {
    public:
        double static calculateMax(
            double timeStep,
            const Mesh2d& mesh,
            const VectorField& velocity
        );

        double static calculateAve(
            double timeStep,
            const Mesh2d& mesh,
            const VectorField& velocity
        );   
    };
}