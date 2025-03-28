#pragma once

#include <Core/Solver/PointMethod.h>

namespace fstim
{
    template <typename T>
    class ExplicitMethod : public PointMethod<T>
    {
    public:

        void operator()(Field<T>& field, const T* source = nullptr) override;

        ExplicitMethod();

        ~ExplicitMethod();
    };
}

