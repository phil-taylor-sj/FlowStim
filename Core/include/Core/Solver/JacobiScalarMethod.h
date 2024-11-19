#pragma once

#include <Core/Solver/JacobiMethod.h>

namespace fstim
{
    /**
     * \class JacobiScalarMethod
     * 
     * \brief Expands JacobiMethod class to implement convergence error computation for a scalar Field.
     * 
     * NOTE: Utilsing an iterative method directly with an std::map data structure is compuitationally
     * inefficient. This class and its derivatives exist for preliminary testing and debugging purposes.
     * Future work will implement a dedicated linear algebra library for solving the discretised
     * equations of the Field class. All values and coefficients will be transferred to vector sparse 
     * matrix structures prior to iteration.
     */
    template <typename T>
    class JacobiScalarMethod : public JacobiMethod<T>
    {
    public:
        JacobiScalarMethod() {};

        ~JacobiScalarMethod() {};

    protected:
        /**
         * \brief Returns the maximum values of absolute and relative error for scalar Field values.
         * 
         * \param newValues An array of values obtained produced by the most recent iteration loop.
         * \param oldValues An array of values from immediately prior to the most recent iteraiton loop.
         */
        Tolerance<double> m_calcMaxErrors(size_t nCells, const T* newValues, const T* oldValues) override;
    };
}