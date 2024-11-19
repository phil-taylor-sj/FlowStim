#include <Core/Solver/JacobiVectorMethod.h>

namespace fstim
{
    template <typename T>
    Tolerance<double> JacobiVectorMethod<T>::m_calcMaxErrors(size_t nCells, const T* newValues, const T* oldValues)
    {
        // PLACEHOLDER...
        // TODO: write this function
        return Tolerance<double>(0., 0.);  
    }

    template class JacobiVectorMethod<vecp::Vec2d>;

}