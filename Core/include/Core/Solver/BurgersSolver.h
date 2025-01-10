#pragma once

#include <Core/Solver/SolverBase.h>
#include <Core/Solver/ExplicitMethod.h>

namespace fstim
{
    class BurgersSolver : public SolverBase
    {
    public:

        bool compute(double deltaTime) override;

        BurgersSolver() { }

        ~BurgersSolver() = default;

        BurgersSolver(const BurgersSolver& newSolver) = delete;

        BurgersSolver& operator=(const BurgersSolver& newSolver) = delete;

    private:

        ExplicitMethod<vecp::Vec2d> m_velocityIterator = ExplicitMethod<vecp::Vec2d>();

    };
}