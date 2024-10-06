#pragma once

#include <Core/Domain/Mesh.h>
#include <Core/Field/Field.h>

namespace fstim
{
    template <typename T>
    class EulerExplicit
    {
    public:
       void operator()(
            const Mesh& mesh, 
            Field<T>& field, 
            const double* rho,
            double deltaT
            );

        EulerExplicit();

        virtual ~EulerExplicit() = default;
    };
}