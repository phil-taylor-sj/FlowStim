#pragma once

#include <algorithm>

namespace fstim 
{
    /**
     * \class FieldRelaxation
     * \brief Interface to manage the under-relaxation in the derived Field class.
     * 
     * An under-relaxation factor is used to stabilise a system of discretised scalar
     * or momentum transport equations. A factor of 1. provides no relaxation. Reducing
     * the factor increases the stability, but slows down the rate of convergence.
     */
    class FieldRelaxation
    {
    public:
        /**
         * \brief Set the values of the under-relaxation factor.
         * 
         * Verifies if the value is GT 0.0 and LE 1.0.
         * If invalid, set to default values of either 1.0 (GT 1.0) or
         * DEFAULT_RELAXATION (LE 0.0).
         * 
         * \param newFactor The new value of the under-relaxation factor.
         */
        void setRelaxation(double newFactor);

        /**
         * \brief Reteieve tha value of the under-relaxation factor. 
         * 
         * If m_isRelaxationActive is set to false, then a value of 1. 
         * if returned.
         * 
         * \return The value fo the under-relaxation factor.
         */
        double getRelaxation();

        /**
         * \brief Sets whether to override the under-relxation factor with its maximum value.
         * 
         * \param makeActive True to activate (return internal value), False to deactivate.
         */
        void activateRelaxation(bool makeActive);

        FieldRelaxation() {};

        ~FieldRelaxation() {};
    protected:
        const double DEFAULT_RELAXATION = 0.9;

        double m_relaxation{0.9};

        bool m_isRelaxationActive = true;
    };
}