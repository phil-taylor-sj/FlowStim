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
            double deltaT,
            const Mesh& mesh, 
            Field<T>& field, 
            const double* rho = nullptr
            );

        EulerExplicit();

        virtual ~EulerExplicit() = default;
    };
}