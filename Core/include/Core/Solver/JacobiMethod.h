#pragma once

#include <Core/Solver/PointMethod.h>
#include <Core/Solver/Tolerance.h>
#include <iostream>


namespace fstim
{
    /**
     * \class JacobiMethod
     * \brief Abstract functor for appling Jacobi Point Iteration method to a Field object.
     * 
     * NOTE: Utilsing an iterative method directly with an std::map data structure is compuitationally
     * inefficient. This class and its derivatives exist for preliminary testing and debugging purposes.
     * Future work will implement a dedicated linear algebra library for solving the discretised
     * equations of the Field class. All values and coefficients will be transferred to vector sparse 
     * matrix structures prior to iteration.
     */
    template <typename T>
    class JacobiMethod : public PointMethod<T>
    {
    public:
        void operator()(Field<T>& field, const T* source = nullptr) override;

        JacobiMethod() {};

        virtual ~JacobiMethod() = 0;

    protected:
        void m_iteratorLoop(Field<T>& field, const T* source, T* newValues);

        Tolerance<T> virtual m_calcMaxErrors(size_t nCells, const T* newValues, const T* oldValues) = 0;

        bool virtual m_isConverged(Tolerance<T> errors, Tolerance<double> convergenceLimits) = 0;
    };

    extern template class JacobiMethod<double>;

    extern template class JacobiMethod<vecp::Vec2d>;
}