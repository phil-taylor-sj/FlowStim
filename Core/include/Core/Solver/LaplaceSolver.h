#pragma once

#include <Core/Solver/SolverBase.h>
#include <Core/Solver/ExplicitIterator.h>

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

        ExplicitIterator<vecp::Vec2d> m_velocityIterator = ExplicitIterator<vecp::Vec2d>();

    };
}