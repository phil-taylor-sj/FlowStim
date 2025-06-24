#pragma once

#include <Core/Domain/Mesh.h>
#include <Core/Field/FieldEquation.h>
#include <Core/Solver/Courant.h>

namespace fstim
{
    class SolverBase
    {
    public:
        bool virtual compute(double deltaTime) = 0;

        void setMesh(std::unique_ptr<Mesh2d> mesh);

        void setVelocity(std::unique_ptr<VectorFieldEqu> velocity);

        const Mesh2d* getMesh();

        const VectorField* getVelocity();

        void initialiseViscosity(double initVisc);

        double getMaxCFL();

        double getAveCFL();

        double calcRequiredTimeStep(double cfl);

        SolverBase() { }

        virtual ~SolverBase() = default;

        SolverBase(const SolverBase& newSolver) = delete;

        SolverBase& operator=(const SolverBase& newSolver) = delete;

        
    protected:
        std::unique_ptr<Mesh2d> m_mesh = nullptr;
        std::unique_ptr<VectorFieldEqu> m_velocity = nullptr;
        std::unique_ptr<double[]> m_viscosity = nullptr;
        double m_deltaTime = 1.;

        bool m_discretise(VectorFieldEqu& field);

        bool m_solveEquation(VectorFieldEqu& field);
    };
}