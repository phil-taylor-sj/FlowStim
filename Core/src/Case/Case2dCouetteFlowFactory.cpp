#include <Core/Case/Case2dCouetteFlowFactory.h>

namespace fstim
{
    std::unique_ptr<SolverBase> Case2dCouetteFlowFactory::buildCase()
    {
        std::unique_ptr<Mesh2d> mesh = m_createMesh();
        m_assignMeshBoundaries(*mesh.get());
    }

    std::unique_ptr<VectorField> Case2dCouetteFlowFactory::m_createVelocityField()
    {

    }
    
    void Case2dCouetteFlowFactory::m_assignMeshBoundaries(Mesh2d& mesh)
    {

    }
}