#pragma once

#include <Core/Field/Laplacian.h>

namespace fstim
{
    template <typename T>
    class Laplacian
    {
    public:

        void operator(const Mesh& mesh, FieldEquation<T>& field, PropertyField& viscosity) override;
        
        Laplacian();

        virtual ~Laplacian() = default;
    }
}