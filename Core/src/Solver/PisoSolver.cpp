#include <Core/Solver/PisoSolver.h>

#include <Core/Solver/PressureExplicit.h>
#include <Core/Field/PressureLinear.h>
#include <Core/Solver/RhieChow.h>
#include <Core/Solver/ExplicitMethod.h>

#include <iostream>
#include <stdexcept>

namespace fstim
{
    bool PisoSolver::compute(double deltaTime)
    {
        this->m_velocity->updateOldValues();

        this->m_deltaTime = deltaTime;
        if (this->m_mesh == nullptr || 
            this->m_velocity == nullptr || 
            this->m_pressure == nullptr)
        {
            return false;
        }

        this->m_velocity->clear();
        this->m_pressure->clear();

        // Discretise all equations.
        this->m_discretise(*(this->m_velocity.get()));
        std::unique_ptr<vecp::Vec2d[]> primaryCoeffsAtFaces = 
            RhieChow<vecp::Vec2d>::interpolatePrimaryCoefficients(*(this->m_velocity), *(this->m_mesh));
        this->m_discretisePressureEquation(primaryCoeffsAtFaces.get());
        // Solve velocity field implicitly.
        this->m_solveMomentumPredictor();
        // Solve pressure field and correct velocity.
        for (int count = 0; count < 4; count++)
        {
            this->m_solvePressureEquation(primaryCoeffsAtFaces.get());
            this->m_updateVelocityExplicitly();        
        }

        double maxCo = Courant::calculateMax(deltaTime, *(this->m_mesh), *(this->m_velocity));


        return true;
    };

    void PisoSolver::setPressure(std::unique_ptr<ScalarFieldEqu> pressure)
    {
        if (this->m_mesh == nullptr)
        {
            return;
        }
        if (this->m_mesh->nCells == pressure->nCells)
        {
            this->m_pressure = std::move(pressure);
        }
    }

    const ScalarField* PisoSolver::getPressure()
    {
        return this->m_pressure.get();
    }

    void PisoSolver::m_discretisePressureEquation(const vecp::Vec2d* primaryCoeffsAtFaces)
    {
        PressureLinear2d pressureLinear {};
        pressureLinear(*(this->m_mesh.get()), *(this->m_pressure.get()), primaryCoeffsAtFaces);
    }

    void PisoSolver::m_solveMomentumPredictor()
    {
        std::unique_ptr<vecp::Vec2d[]> pressureSource = PressureExplicit::calculateStructured(
            *(this->m_pressure.get()), *(this->m_mesh.get())
        );
        this->m_velocityIterator(*(this->m_velocity.get()), pressureSource.get());
    }

    void PisoSolver::m_solvePressureEquation(const vecp::Vec2d* primaryCoeffsAtFaces)
    {
        std::unique_ptr<vecp::Vec2d[]> momentumTermsAtFaces
            = RhieChow<vecp::Vec2d>::interpolate(*(this->m_velocity), *(this->m_mesh));

        std::unique_ptr<double[]> momentumSumAtCells = std::make_unique<double[]>(this->m_mesh->nCells);
        for (int cellId = 0; cellId < this->m_mesh->nCells; cellId++)
        {
            const Cell2d& cell = this->m_mesh->cells[cellId];
            double momentumSum = 0.;
            for (int faceId : cell.faceId)
            {
                const Face2d& face = this->m_mesh->faces[faceId];
                vecp::Vec2d normalVector = face.normal.normalise();
                if (face.ownerId != cellId)
                {
                    normalVector *= -1.;
                }
                momentumSum -= normalVector.dot((momentumTermsAtFaces[faceId] / primaryCoeffsAtFaces[faceId]));
            }
            momentumSumAtCells[cellId] = momentumSum;      
        }
        
        this->m_pressureIterator(*(this->m_pressure.get()), momentumSumAtCells.get());
    }

    void PisoSolver::m_updateVelocityExplicitly()
    {
        std::unique_ptr<vecp::Vec2d[]> pressureSource = PressureExplicit::calculateStructured(
            *(this->m_pressure.get()), *(this->m_mesh.get())
        );
        ExplicitMethod<vecp::Vec2d> velocityIterator {};
        velocityIterator(*(this->m_velocity.get()), pressureSource.get());
    }


}