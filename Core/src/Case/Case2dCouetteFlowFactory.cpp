#include <Core/Case/Case2dCouetteFlowFactory.h>

#include <Core/Domain/FaceSetFactory.h>
#include <Core/Domain/CompassUtils.h>
#include <Core/Field/FieldEquation.h>
#include <Core/Field/BcType.h>
#include <Core/Solver/BurgersSolver.h>

#include <iostream>

namespace fstim
{
    std::unique_ptr<SolverBase> Case2dCouetteFlowFactory::buildCase()
    {
        if (!m_isReferenceDirectionValid(m_referenceDirection))
        {
            std::cout << "Reference direction invalid. \n"
                "Setting default direction of NORTH." << std::endl;
            m_referenceDirection = Compass::NORTH;
        }

        std::unique_ptr<Mesh2d> mesh = m_createMesh();
        m_assignMeshBoundaries(*mesh.get());
        std::unique_ptr<VectorFieldEqu> velocity = m_createVelocityField(*mesh.get());

        std::unique_ptr<BurgersSolver> solver = std::make_unique<BurgersSolver>();
        solver->setMesh(std::move(mesh));
        solver->setVelocity(std::move(velocity));

        return std::move(solver);
    }

    std::unique_ptr<VectorFieldEqu> Case2dCouetteFlowFactory::m_createVelocityField(Mesh2d& mesh)
    {
        std::unique_ptr<VectorFieldEqu> velocityField = std::make_unique<VectorFieldEqu>(mesh.nCells);
        velocityField->addBc(BcType::FIXEDVALUE, m_referenceVelocity); // Moving Wall
        velocityField->addBc(BcType::FIXEDVALUE, vecp::Vec2d(0., 0.)); // Static Wall
        velocityField->addBc(BcType::ZEROGRADIENT, vecp::Vec2d(0., 0.)); // Inflow/Outflow
        velocityField->addBc(BcType::ZEROGRADIENT, vecp::Vec2d(0., 0.)); // Inflow/Outflow
        return std::move(velocityField);
    }
    
    void Case2dCouetteFlowFactory::m_assignMeshBoundaries(Mesh2d& mesh)
    {
        std::set<Compass> m_directionsToAssign = {
            Compass::NORTH, Compass::SOUTH, Compass::WEST, Compass::EAST
        };
        Compass reversedReference = CompassUtils::getReverseDirection(m_referenceDirection);
        
        auto assignAndErase = [&](Compass directionToAssign) {
            FaceSetFactory::defineNewFaceSetByCompass(mesh, directionToAssign);
            m_directionsToAssign.erase(directionToAssign);
        };
        
        assignAndErase(m_referenceDirection);
        assignAndErase(reversedReference);
        for (Compass direction : m_directionsToAssign)
        {
            FaceSetFactory::defineNewFaceSetByCompass(mesh, direction);
        }
    }

}