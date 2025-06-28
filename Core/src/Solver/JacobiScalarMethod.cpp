#include <Core/Solver/JacobiScalarMethod.h>
#include <Core/Solver/Tolerance.h>

#include <algorithm>
#include <cstdlib>

namespace fstim
{

    template <typename T>
    Tolerance<T> JacobiScalarMethod<T>::m_calcMaxErrors(size_t nCells, const T* newValues, const T* oldValues)
    {
        T maxAbs = 0.;
        T maxRel = 0.;
        for (size_t id = 0; id < nCells; id++)
        {
            T absErr = std::abs(newValues[id] - oldValues[id]);

            T relErr = (oldValues[id] != 0.) 
                ? absErr / std::abs(oldValues[id])
                : 0.;

            maxAbs = std::max(maxAbs, absErr);
            maxRel = std::max(maxRel, relErr);
        }
        
        return Tolerance<T>(maxAbs, maxRel);  
    }

    template <typename T>
    bool JacobiScalarMethod<T>::m_isConverged(Tolerance<T> errors, Tolerance<double> convergenceLimits)
    {
        return (errors.absolute <= convergenceLimits.absolute ||
                errors.relative <= convergenceLimits.relative);
 
    }
        
    template class JacobiScalarMethod<double>;
}