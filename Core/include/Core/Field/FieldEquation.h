#pragma once

#include <VecPlus/Vec2.h>

namespace fstim
{
    template<typename T>
    class FieldEquation
    {
    public:
        const int nCells;

        std::unique_ptr<int> laplacian = nullptr;
        std::unique_ptr<int> divergence = nullptr;
        std::unique_ptr<int> temporal = nullptr;

        /*!
         * \brief Provides read only access to the cell center values.
         *
         * \return A const pointer to the array of cell center values.
        */
        const T* readValues() const { return this->m_values.get() };
          
        /*!
         * \brief Provides read only access to the equation coefficients.
         *
         * These coefficients form the left hand side.
         *
         * \return A const pointer to the array of equation coefficients.
        */        
        const T* readLeft() const { return this->m_rhs.get() };
        
        /*!
         * \brief Provides read only access to the equation constants.
         *
         * These constants form the right hand side.
         * 
         * \return A const pointer to the array of equation constants.
        */    
        const T* readRight() const { return this->m_lhs.get() };
        
        /*!
         * \brief Provides read and write access to the cell center values.
         *
         * \return A non-const pointer to the array of cell center values.
        */
        T* writeValues();

        /*!
         * \brief Provides read and write access to the equation coefficients.
         *
         * These coefficients form the left hand side.
         * 
         * \return A non-const pointer to the array of equation coefficients.
        */   
        T* writeLeft();

        /*!
         * \brief Provides read only access to the equation constants.
         *
         * These constants form the right hand side.
         * 
         * \return A const pointer to the array of equation constants.
        */    
        T* writeRight();

        EquationField(int nCells) : nCells(nCells),
          m_lhs(std::make_unique<T[]>(nCells)),
          m_rhs(std::make_unique<T[]>(nCells)),
          m_values(std::make_unique<T[]>(nCells)) { }

        ~FieldEquation() = default;

        FieldEquation(const FieldEquation& newField) = delete;

        FieldEquation& operator=(const FieldEquation& newField) = delete;

    private:
        std::unique_ptr<std::map<int, T>> m_lhs[];
        std::unique_ptr<T> m_rhs[];
        std::unique_ptr<T> m_values[];
    };

    using ScalarFieldEqu = FieldEquation<double>;

    using VectorFieldEqu = FieldEquation<vecp::Vec2d>;
}
