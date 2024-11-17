#include <Core/Solver/JacobiVectorMethod.h>

namespace fstim
{
    template <typename T>
    bool JacobiVectorMethod<T>::m_checkConvergence(Tolerance<T> criteria, const T* newValues, const T* oldValues)
    {
        // PLACEHOLDER...
        // TODO: write this function
        return true;  
    }

    template class JacobiVectorMethod<vecp::Vec2d>;

}