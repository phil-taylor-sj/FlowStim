#include <Core/Solver/PisoSolver.h>

#include <Core/Solver/PressureExplicit.h>

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

        this->m_discretise(*(this->m_velocity.get()));
        
        this->m_velocityIterator(*(this->m_velocity.get()));
        
        std::unique_ptr<vecp::Vec2d[]> pressureSource = PressureExplicit::calculateStructured(
            *(this->m_pressure.get()), *(this->m_mesh.get())
        );
        
        this->m_pressureIterator(*(this->m_pressure.get()));



        //double maxCo = Courant::calculateMax(
        //    deltaTime,
        //    *(this->m_mesh),
        //    *(this->m_velocity)
        //);

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


}