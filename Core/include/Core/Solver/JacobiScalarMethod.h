#pragma once

#include <Core/Solver/JacobiMethod.h>

namespace fstim
{
    template <typename T>
    class JacobiScalarMethod : public JacobiMethod<T>
    {
    public:
        JacobiScalarMethod() {};

        ~JacobiScalarMethod() {};

    protected:
        bool m_checkConvergence(Tolerance<T> criteria, const T* newValues, const T* oldValues) override;
    };
}