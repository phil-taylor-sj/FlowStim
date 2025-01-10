#pragma once

#include <Core/Solver/Tolerance.h>

namespace fstim
{
    /**
     * \class Field Tolerence
     * \brief Interface to manage the relative an absolute convergence tolerences for the derived Field class
     *
     * The tolerance values provide the required relative and absolute errors at which the system of 
     * linear equation is considered to be converged.
     */
    class FieldTolerance
    {
    public:

        const double DEFAULT_ABS = 0.0001;

        const double DEFAULT_REL = 0.01;

        /**
         * \brief Set new absolute and relative tolerance values for the field.
         * 
         * Validates that both values lies within the range of 0.0 and 1.0.
         * If a values is invalid, it is replaced by its repsective default value.
         * 
         * \param newTolerance An instance containng new values for absolute and relative tolerances. 
         */
        void setTolerance(Tolerance<double> newTolerance);

        /**
         * \brief Returns the values of the absolute and relative tolerances.
         * 
         * \return An instance containng new values for absolute and relative tolerances. 
         */
        Tolerance<double> getTolerance();

        FieldTolerance() {};

        ~FieldTolerance() {};
    protected:

        Tolerance<double> m_tolerance{DEFAULT_ABS, DEFAULT_REL};
    };
}