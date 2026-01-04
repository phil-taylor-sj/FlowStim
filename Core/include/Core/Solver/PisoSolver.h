#pragma once

#include <Core/Solver/SolverBase.h>

#include <Core/Field/FieldEquation.h>

#include <Core/Solver/JacobiMethod.h>

namespace fstim
{
    class PisoSolver : public SolverBase
    {
    public:

        bool compute(double deltaTime) override;

        void setPressure(std::unique_ptr<ScalarFieldEqu> pressure);

        const ScalarField* getPressure();

        PisoSolver() {};

        ~PisoSolver() {};

        PisoSolver(const PisoSolver& newSolver) = delete;

        PisoSolver& operator=(const PisoSolver& newSolver) = delete;

    private:

        std::unique_ptr<ScalarFieldEqu> m_pressure = nullptr;
        JacobiMethod<vecp::Vec2d> m_velocityIterator = JacobiMethod<vecp::Vec2d>();
        JacobiMethod<double> m_pressureIterator = JacobiMethod<double>();

        void m_solveMomentumPredictor();
        void m_discretisePressureEquation(const vecp::Vec2d* primaryCoeffsAtFaces);

        void m_solvePressureEquation(const vecp::Vec2d* primaryCoeffsAtFaces);
        void m_updateVelocityExplicitly();
    };
}