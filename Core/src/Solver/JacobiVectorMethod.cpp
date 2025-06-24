#include <Core/Solver/JacobiVectorMethod.h>

namespace fstim
{
    template <typename T>
    Tolerance<T> JacobiVectorMethod<T>::m_calcMaxErrors(size_t nCells, const T* newValues, const T* oldValues)
    {
        T maxAbs = T();
        T maxRel = T();
        for (size_t id = 0; id < nCells; id++)
        {
            T absErr = (newValues[id] - oldValues[id]).abs();

            T relErr = ((oldValues[id].abs()).min() != 0.)
                ? absErr / oldValues[id].abs()
                : T();

            maxAbs = maxAbs.max(absErr);
            maxRel = maxRel.max(relErr);
        }
        
        return Tolerance<T>(maxAbs, maxRel); 
    }

    template <typename T>
    bool JacobiVectorMethod<T>::m_isConverged(Tolerance<T> errors, Tolerance<double> convergenceLimits)
    {
        return ((errors.absolute - convergenceLimits.absolute).max() <= 0. ||
                (errors.relative - convergenceLimits.relative).max() <= 0.);
    }

    template class JacobiVectorMethod<vecp::Vec2d>;

}