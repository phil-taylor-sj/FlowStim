#pragma once

#include <Core/Solver/JacobiMethod.h>

namespace fstim
{
    template <typename T>
    class JacobiVectorMethod : JacobiMethod<T>
    {
    public:
        JacobiVectorMethod() {};

        ~JacobiVectorMethod() {};

    protected:
        bool m_checkConvergence(Tolerance<T> criteria, const T* newValues, const T* oldValues) override;
    };
}