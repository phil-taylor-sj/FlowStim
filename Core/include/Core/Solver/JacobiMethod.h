#pragma once

#include <Core/Solver/PointMethod.h>
#include <Core/Solver/Tolerance.h>

namespace fstim
{
    template <typename T>
    class JacobiMethod : public PointMethod<T>
    {
    public:
        void operator()(Field<T>& field, const T* source = nullptr) override;

        JacobiMethod() {};

        virtual ~JacobiMethod() = 0;

    protected:
        void m_iteratorLoop(Field<T>& field, const T* source, T* newValues);

        bool virtual m_checkConvergence(Tolerance<T> criteria, const T* newValues, const T* oldValues) = 0;
    };
}