#pragma once

#include <Core/Solver/SolverBase.h>
#include <Core/Domain/Compass.h>

#include <VecPlus/Vec2.h>

#include <memory>
#include <set>

namespace fstim
{
    template <typename T>
    class CaseFactory
    {
    public:
        virtual std::unique_ptr<SolverBase> buildCase() = 0;

        void setReferenceDirection(Compass direction);

        Compass getReferenceDirection();

        void setReferenceVelocity(T velocity);

        T getReferenceVelocity();

        CaseFactory() {};

        virtual ~CaseFactory() {};

    protected:
        Compass m_referenceDirection = Compass::NORTH;
        T m_referenceVelocity = T();

        virtual bool m_isReferenceDirectionValid(Compass direction) = 0;

    };

    extern template class CaseFactory<vecp::Vec2d>;

    extern template class CaseFactory<vecp::Vec3d>;
}