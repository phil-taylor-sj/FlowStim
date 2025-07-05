#include <Core/Solver/JacobiMethod.h>

#include <numeric>
#include <limits>

#include <omp.h>



namespace fstim
{

    template <typename T>
    int JacobiMethod<T>::operator()(Field<T>& field, const T* source)
    {
        constexpr double epsilon = std::numeric_limits<double>::epsilon();
        std::unique_ptr<T[]> newValues = std::make_unique<T[]>(field.nCells);
 

        //T normFactor = this->m_calcNormFactor(field, source) + epsilon;
        //normFactor = 1.;
        T initialResidual = this->m_calcGlobalResidual(field, source) / std::sqrt(field.nCells);// normFactor;

        for (int count = 0; count < 100; count++)
        {
            this->m_iteratorLoop(field, source, newValues.get());

            //Tolerance<T> errors = this->m_calcMaxErrors(field.nCells, newValues.get(), field.readValues());
   
            // Overwrite the current values with the new values.                      
            T* values = field.writeValues();
            #pragma omp parallel for
            for (int id = 0; id < field.nCells; id++)
            {
                values[id] = newValues[id];
            }

            T newResidual = this->m_calcGlobalResidual(field, source) / std::sqrt(field.nCells); // normFactor;
            T relativeResidual = newResidual / (initialResidual);
            // static_cast<double>(field.nCells);
            //Check errors against convergence criteria, and break loop if met.
            if (count != 0 && m_isConverged(Tolerance<T>(newResidual, relativeResidual), field.getTolerance()))
            {
                return count;
            }
        }
        return 100;
    }

    template <typename T>
    void JacobiMethod<T>::m_iteratorLoop(Field<T>& field, const T* source, T* newValues)
    {
        const T* values = field.readValues();
        const T* oldValues = field.readOldValues();
        const std::map<int, T>* lhs = field.readLeft();
        const T* rhs = field.readRight();

        #pragma omp parallel for
        for (int id = 0; id < field.nCells; id++)
        {
            // Set the initial values of the new value.
            newValues[id] = rhs[id];
            if (source != nullptr)
            {
                newValues[id] += source[id];
            }

            // Add all contributions from the left hand side terms.
            for (const std::pair<int, T>& pair : lhs[id])
            {
                if (pair.first == id) { continue; }
                newValues[id] -= pair.second * values[pair.first];
            }

            newValues[id] /= lhs[id].at(id);
        }
    }

    template <typename T>
    Tolerance<T> JacobiMethod<T>::m_calcMaxErrors(size_t nCells, const T* newValues, const T* oldValues)
    {
        return Tolerance<T>(0.1, 0.1);
    }

    template <typename T>
    bool JacobiMethod<T>::m_isConverged(Tolerance<T> errors, Tolerance<double> convergenceLimits)
    {
        return (
            this->m_getMax(errors.absolute - convergenceLimits.absolute) <= 0. ||
            this->m_getMax(errors.relative - convergenceLimits.relative) <= 0.
            );
    }


    template <typename T>
    T JacobiMethod<T>::m_calcGlobalResidual(Field<T>& field, const T* source)
    {
        T residualSum = T();

        T* values = field.writeValues();
        const std::map<int, T>* lhs = field.readLeft();
        const T* rhs = field.readRight();

        #pragma omp parallel
        {
            T localResidualSum = T();
            #pragma omp for nowait
            for (int cellId = 0; cellId < field.nCells; cellId++)
            {
                T localResidual = T();
                // Set the initial values of the new value.
                for (const std::pair<int, T>& pair : lhs[cellId])
                {
                    localResidual += pair.second * values[pair.first];
                }
                localResidual -= rhs[cellId];

                if (source != nullptr)
                {
                    localResidual -= source[cellId];
                }

                // Add all contributions from the left hand side terms.
                localResidualSum += this->m_getAbsolute(localResidual);
            }

            #pragma omp critical
            residualSum += localResidualSum;
        }
        return residualSum;
    }


    template <typename T>
    T JacobiMethod<T>::m_calcNormFactor(Field<T>& field, const T* source)
    {
        
        T normFactor = T();
        const T* values = field.readValues();
        T meanValue = std::accumulate(values, values + field.nCells, T())
            / static_cast<double>(field.nCells);

        const std::map<int, T>* lhs = field.readLeft();
        const T* rhs = field.readRight();

        for (int cellId = 0; cellId < field.nCells; cellId++)
        {

            T correction = T();
            T leftNorm = T();

            for (const std::pair<int, T>& pair : lhs[cellId])
            {
                leftNorm += pair.second * values[pair.first];
                correction += pair.second * meanValue;
            }

            T rightNorm = rhs[cellId];
            if (source != nullptr)
            {
                rightNorm += source[cellId];
            } 
            correction = T();

            normFactor += (this->m_getAbsolute(leftNorm - correction)
                + this->m_getAbsolute(rightNorm - correction));
        }
        return normFactor;
    }


    template class JacobiMethod<double>;

    template class JacobiMethod<vecp::Vec2d>;
}