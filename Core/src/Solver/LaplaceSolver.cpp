#include <Core/Solver/LaplaceSolver.h>

namespace fstim
{
    bool LaplaceSolver::compute(double deltaTime)
    {
        if (this->m_mesh == nullptr || this->m_velocity == nullptr)
        {
            return false;
        }

        this->m_velocity->clear();
        
        this->m_discretise(*(this->m_velocity.get()));
        
        this->m_velocityIterator(*(this->m_velocity.get()));
    }
}