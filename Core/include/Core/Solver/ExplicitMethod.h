#pragma once

#include <Core/Solver/PointMethod.h>
#include <Core/Field/SparseMatrix.hpp>

namespace fstim
{
    template <typename T>
    class ExplicitMethod : public PointMethod<T>
    {
    public:

        int operator()(Field<T>& field, const T* source = nullptr) override;

        ExplicitMethod();

        ~ExplicitMethod();
    };
}

