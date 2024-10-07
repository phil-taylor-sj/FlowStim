#include <Core/Solver/SolverBase.h>


namespace fstim
{
    void SolverBase::setMesh(std::unique_ptr<Mesh> mesh)
    {
        this->m_mesh = std::move(mesh);
    }

    void SolverBase::setVelocity(std::unique_ptr<VectorFieldEqu> velocity)
    {
        this->m_velocity = std::move(velocity);
    }

    const Mesh& SolverBase::getMesh()
    {
        return  *(this->m_mesh.get());
    }

    const VectorField& SolverBase::getVelocity()
    {
        return *(this->m_velocity.get());
    }

    void SolverBase::initialiseViscosity(double initVisc)
    {
        for (int id = 0; id < this->m_mesh->nCells; id++)
        {
            this->m_viscosity[id] = initVisc;
        }
    }

    double SolverBase::getMaxCFL()
    {
        Mesh& mesh = *(this->m_mesh.get());
        VectorField& velocity = *(this->m_velocity.get());

        return Courant::calculateMax(this->m_deltaTime, mesh, velocity);
    }

    double SolverBase::getAveCFL()
    {
        Mesh& mesh = *(this->m_mesh.get());
        VectorField& velocity = *(this->m_velocity.get());

        return Courant::calculateAve(this->m_deltaTime, mesh, velocity);
    }

    double SolverBase::calcRequiredTimeStep(double cfl)
    {
        double maxFraction = this->getMaxCFL() / this->m_deltaTime;
        return cfl / maxFraction;
    }

    bool SolverBase::m_discretise(VectorFieldEqu& field)
    {
        field.clear();

        Mesh& mesh = *(this->m_mesh.get());
        VectorField& velocity = *(this->m_velocity.get());
        double* viscosity = this->m_viscosity.get();

        if (field.divergence != nullptr)
        {
            (*field.divergence)(mesh, field, velocity);
        }
        if (field.laplacian != nullptr)
        {
            (*field.laplacian)(mesh, field, viscosity);
        }
        if (field.temporal != nullptr)
        {
            (*field.temporal)(this->m_deltaTime, mesh, field);
        }

        return true; 
    }
}