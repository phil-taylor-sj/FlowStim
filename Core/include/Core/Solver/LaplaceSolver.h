#pragma once

#include <Core/Solver/SolverBase.h>
#include <Core/Solver/ExplicitMethod.h>

namespace fstim
{
    class LaplaceSolver : public SolverBase
    {
    public:

        bool compute(double deltaTime) override;

        LaplaceSolver() { }

        ~LaplaceSolver() = default;

        LaplaceSolver(const LaplaceSolver& newSolver) = delete;

        LaplaceSolver& operator=(const LaplaceSolver& newSolver) = delete;

    private:

        ExplicitMethod<vecp::Vec2d> m_velocityIterator = ExplicitMethod<vecp::Vec2d>();

    };
}