#include <Core/Solver/JacobiVectorMethod.h>

#include <numeric>

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

    template <typename T>
    T JacobiVectorMethod<T>::m_calcGlobalResidual(Field<T>& field, const T* source)
    {
        T residualSum = T();

        T* values = field.writeValues();


        const std::map<int, T>* lhs = field.readLeft();
        const T* rhs = field.readRight();

        for (int cellId = 0; cellId < field.nCells; cellId++)
        {
            T localResidual = T();
            // Set the initial values of the new value.
            for (const std::pair<int, T> pair : lhs[cellId])
            {
                localResidual += pair.second * values[pair.first];
            }

            localResidual -= rhs[cellId];

            if (source != nullptr)
            {
                localResidual -= source[cellId];
            }

            // Add all contributions from the left hand side terms.
            residualSum += localResidual.abs();
        }
        return residualSum;

    };


    template class JacobiVectorMethod<vecp::Vec2d>;

}