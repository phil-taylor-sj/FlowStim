#pragma once

#include <Core/Solver/Tolerance.h>

namespace fstim
{
    class FieldTolerance
    {
    public:
        void setTolerance(Tolerance<double> newTolerance);

        Tolerance<double> getTolerance();

        FieldTolerance() {};

        ~FieldTolerance() {};
    protected:
        const double DEFAULT_ABS = 0.0001;
        const double DEFAULT_REL = 0.01;

        Tolerance<double> m_tolerance{0.0001, 0.01};
    };
}