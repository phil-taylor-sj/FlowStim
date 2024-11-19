#include <Core/Field/FieldTolerance.h>

namespace fstim
{
    void FieldTolerance::setTolerance(Tolerance<double> newTolerance)
    {
        double newAbs = newTolerance.absolute;
        double newRel = newTolerance.relative;
        
        this->m_tolerance = Tolerance<double>(
            (newAbs > 0. && newAbs <= 1.) ? newAbs : this->DEFAULT_ABS,
            (newRel > 0. && newRel <= 1.) ? newRel : this->DEFAULT_REL
        );
    }

    Tolerance<double> FieldTolerance::getTolerance()
    {
        return this->m_tolerance;
    }
}