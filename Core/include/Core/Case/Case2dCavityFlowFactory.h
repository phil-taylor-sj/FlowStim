#pragma once

#include <Core/Case/Case2dStructuredFactory.h>
#include <Core/Solver/SolverBase.h>
#include <Core/Field/FieldEquation.h>

#include <memory>

namespace fstim
{
    class Case2dCavityFlowFactory : public Case2dStructuredFactory
    {
    public:
        virtual std::unique_ptr<SolverBase> buildCase() override;

        Case2dCavityFlowFactory() {};

        ~Case2dCavityFlowFactory() {};

    private:
        std::unique_ptr<VectorFieldEqu> m_createVelocityField(Mesh2d& mesh);

        std::unique_ptr<ScalarFieldEqu> m_createPressureField(Mesh2d& mesh);
    
        void m_assignMeshBoundaries(Mesh2d& mesh);
    };
}