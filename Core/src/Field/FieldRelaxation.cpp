#include <Core/Field/FIeldRelaxation.h>

namespace fstim
{
    void FieldRelaxation::setRelaxation(double newFactor)
    {
        if (newFactor < 0.)
        {
            newFactor = this->DEFAULT_RELAXATION;
        }
        this->m_relaxation = std::min(newFactor, 1.);
    }

    double FieldRelaxation::getRelaxation()
    {
        return (this->m_isRelaxationActive) ? this->m_relaxation : 1.;
    }

    void FieldRelaxation::activateRelaxation(bool makeActive)
    {
        this->m_isRelaxationActive = makeActive;
    }
}