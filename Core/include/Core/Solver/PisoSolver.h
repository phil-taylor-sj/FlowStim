#pragma once

#include <Core/Solver/SolverBase.h>

#include <Core/Field/FieldEquation.h>

#include <Core/Solver/JacobiScalarMethod.h>
#include <Core/Solver/JacobiVectorMethod.h>

namespace fstim
{
    class PisoSolver : public SolverBase
    {
    public:

        bool compute(double deltaTime) override;

        void setPressure(std::unique_ptr<ScalarFieldEqu> pressure);

        const ScalarField* getPressure();

        PisoSolver() { }

        ~PisoSolver() = default;

        PisoSolver(const PisoSolver& newSolver) = delete;

        PisoSolver& operator=(const PisoSolver& newSolver) = delete;

    private:

        std::unique_ptr<ScalarFieldEqu> m_pressure = nullptr;
        JacobiVectorMethod<vecp::Vec2d> m_velocityIterator = JacobiVectorMethod<vecp::Vec2d>();
        JacobiScalarMethod<double> m_pressureIterator = JacobiScalarMethod<double>();

    };
}