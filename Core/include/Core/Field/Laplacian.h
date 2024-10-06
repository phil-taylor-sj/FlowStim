#pragma once

#include <Core/Field/Field.h>
#include <Core/Domain/Mesh.h>

namespace fstim
{
    template <typename T>
    class Laplacian
    {
    public:

        void operator()(const Mesh& mesh, Field<T>& field, const double* mu);
        
        Laplacian();

        virtual ~Laplacian() = default;
    };
}