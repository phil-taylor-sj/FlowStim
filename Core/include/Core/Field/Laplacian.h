#pragma once

#include <Core/Field/Field.h>
#include <Core/Domain/Mesh.h>
#include <Core/Field/SparseMatrix.hpp>

namespace fstim
{
    template <typename T>
    class Laplacian
    {
    public:

        void operator()(const Mesh2d& mesh, Field<T>& field, const double* visc);

        Laplacian();

        virtual ~Laplacian() = default;
    };
}