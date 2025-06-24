#include <Core/Case/CaseFactory.h>

namespace fstim
{
    template <typename T>
    void CaseFactory<T>::setReferenceDirection(Compass direction)
    {
        if (m_isReferenceDirectionValid(direction))
        {
            this->m_referenceDirection = direction;   
        }
    }
    
    template <typename T>
    Compass CaseFactory<T>::getReferenceDirection()
    {
        return this->m_referenceDirection;
    }

    template <typename T>
    void CaseFactory<T>::setReferenceVelocity(T velocity)
    {
        this->m_referenceVelocity = velocity;
    }
    
    template<typename T>
    T CaseFactory<T>::getReferenceVelocity()
    {
        return this->m_referenceVelocity;
    }

    template class CaseFactory<vecp::Vec2d>;

    template class CaseFactory<vecp::Vec3d>;
}