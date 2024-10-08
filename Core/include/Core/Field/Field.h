#pragma once

#include <map>

#include <VecPlus/Vec2.h>
#include <Core/Field/BcType.h>


namespace fstim
{

    template<typename T>
    class Field
    {
    public:
        const int nCells;

        /*!
         * \brief Provides read only access to the cell center values.
         *
         * \return A const pointer to the array of cell center values.
        */
        const T* readValues() const { return this->m_values.get(); }
          
        /*!
         * \brief Provides read only access to the equation coefficients.
         *
         * These coefficients form the left hand side.
         *
         * \return A const pointer to the array of equation coefficients.
        */        
        const std::map<int, T>* readLeft() const { return this->m_lhs.get(); }
        
        /*!
         * \brief Provides read only access to the equation constants.
         *
         * These constants form the right hand side.
         * 
         * \return A const pointer to the array of equation constants.
        */    
        const T* readRight() const { return this->m_rhs.get(); }
        
        /*!
         * \brief Provides read and write access to the cell center values.
         *
         * \return A non-const pointer to the array of cell center values.
        */
        T* writeValues() { return this->m_values.get(); };

        /*!
         * \brief Provides read and write access to the equation coefficients.
         *
         * These coefficients form the left hand side.
         * 
         * \return A non-const pointer to the array of equation coefficients.
        */   
        std::map<int, T>* writeLeft() { return this->m_lhs.get(); };

        /*!
         * \brief Provides read only access to the equation constants.
         *
         * These constants form the right hand side.
         * 
         * \return A const pointer to the array of equation constants.
        */    
        T* writeRight() { return this->m_rhs.get(); };

        const T* readOldValues() { return this->m_oldValues.get(); }

        void updateOldValues();

        std::tuple<BcType, T> getBc(int faceSetId) const;

        int addBc(BcType type, T value);

        bool updateBc(int faceSetId, T newValue);

        void clear();

        void initialise();

        Field(int nCells) : nCells(nCells),
          m_lhs(std::make_unique<std::map<int, T>[]>(nCells)),
          m_rhs(std::make_unique<T[]>(nCells)),
          m_values(std::make_unique<T[]>(nCells)) { }

        ~Field() = default;

        Field(const Field& newField) = delete;

        Field& operator=(const Field& newField) = delete;


    protected:
        std::unique_ptr<std::map<int, T>[]> m_lhs;
        std::unique_ptr<T[]> m_rhs;
        std::unique_ptr<T[]> m_values;
        std::unique_ptr<T[]> m_oldValues = nullptr;
        std::vector<std::tuple<BcType, T>> m_boundaries{};
    };

    using ScalarField = Field<double>;

    using VectorField = Field<vecp::Vec2d>;
}
