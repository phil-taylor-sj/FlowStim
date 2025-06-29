#pragma once

#include <Core/Solver/PointMethod.h>
#include <Core/Solver/Tolerance.h>
#include <iostream>


namespace fstim
{
    /**
     * \class JacobiMethod
     * \brief Abstract functor for applying Jacobi Point Iteration method to a Field object.
     * 
     * NOTE: Utilising an iterative method directly with an std::map data structure is computationally
     * inefficient. This class and its derivatives exist for preliminary testing and debugging purposes.
     * Future work will implement a dedicated linear algebra library for solving the discretised
     * equations of the Field class. All values and coefficients will be transferred to vector sparse 
     * matrix structures prior to iteration.
     */
    template <typename T>
    class JacobiMethod : public PointMethod<T>
    {
    public:
        int operator()(Field<T>& field, const T* source = nullptr) override;

        JacobiMethod() {};

        virtual ~JacobiMethod() = default;

    protected:
        void m_iteratorLoop(Field<T>& field, const T* source, T* newValues);

        Tolerance<T> virtual m_calcMaxErrors(size_t nCells, const T* newValues, const T* oldValues);

        T virtual m_calcGlobalResidual(Field<T>& field, const T* source = nullptr);

        T virtual m_calcNormFactor(Field<T>& field, const T* source = nullptr);

        bool virtual m_isConverged(Tolerance<T> errors, Tolerance<double> convergenceLimits);
    
        inline vecp::Vec2d m_getAbsolute(const vecp::Vec2d& vector) { return vector.abs(); }
        inline double m_getMax(const vecp::Vec2d& vector) { return vector.max(); }

        inline double m_getAbsolute(const double& value) { return std::abs(value); }
        inline double m_getMax(const double& input) { return input;}

    
    };

    extern template class JacobiMethod<double>;

    extern template class JacobiMethod<vecp::Vec2d>;
}