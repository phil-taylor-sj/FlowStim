#pragma once

#include <Core/Solver/JacobiMethod.h>

namespace fstim
{
    /**
     * \class JacobiVectorMethod
     * 
     * \brief Expands JacobiMethod class to implement convergence error computation for a vector Field.
     * 
     * NOTE: Utilsing an iterative method directly with an std::map data structure is compuitationally
     * inefficient. This class and its derivatives exist for preliminary testing and debugging purposes.
     * Future work will implement a dedicated linear algebra library for solving the discretised
     * equations of the Field class. All values and coefficients will be transferred to vector sparse 
     * matrix structures prior to iteration.
     */
    template <typename T>
    class JacobiVectorMethod : public JacobiMethod<T>
    {
    public:
        JacobiVectorMethod() {};

        ~JacobiVectorMethod() {};

    protected:
        /** \brief Returns the maximum values of absolute and relative error for vector Field values.
         * 
         * \param newValues An array of values obtained produced by the most recent iteration loop.
         * \param oldValues An array of values from immediately prior to the most recent iteraiton loop.
         */
        Tolerance<T> m_calcMaxErrors(size_t nCells, const T* newValues, const T* oldValues) override;

        bool m_isConverged(Tolerance<T> errors, Tolerance<double> convergenceLimits) override;
    };
}