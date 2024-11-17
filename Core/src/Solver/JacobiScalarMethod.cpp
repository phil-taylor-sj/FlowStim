#include <Core/Solver/JacobiScalarMethod.h>

namespace fstim
{

    template <typename T>
    bool JacobiScalarMethod<T>::m_checkConvergence(Tolerance<T> criteria, const T* newValues, const T* oldValues)
    {
        // PLACEHOLDER...
        // TODO: write this function
        return true;  
    }

    template class JacobiScalarMethod<double>;
}