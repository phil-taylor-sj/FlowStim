#pragma once

#include <Core/Case/Case2dStructuredFactory.h>
#include <Core/Field/Field.h>
#include <Core/Domain/Mesh.h>

#include <memory>

namespace fstim
{
    class Case2dCouetteFlowFactory : public Case2dStructuredFactory
    {
    public:
        virtual std::unique_ptr<SolverBase> buildCase() override;

        Case2dCouetteFlowFactory() {};

        ~Case2dCouetteFlowFactory() {};
        
    private:
        std::unique_ptr<VectorFieldEqu> m_createVelocityField(Mesh2d& mesh);
    
        void m_assignMeshBoundaries(Mesh2d& mesh);

    };
}