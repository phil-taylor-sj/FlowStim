#include <Core/Solver/LaplaceSolver.h>
#include <iostream>

namespace fstim
{
    bool LaplaceSolver::compute(double deltaTime)
    {
        this->m_velocity->updateOldValues();

        this->m_deltaTime = deltaTime;
        if (this->m_mesh == nullptr || this->m_velocity == nullptr)
        {
            return false;
        }

        this->m_velocity->clear();

        this->m_discretise(*(this->m_velocity.get()));
        
        this->m_velocityIterator(*(this->m_velocity.get()));
        
        double maxCo = Courant::calculateMax(
            deltaTime,
            *(this->m_mesh),
            *(this->m_velocity)
        );

        std::cout << maxCo << std::endl;
    }
}