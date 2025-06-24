#include <Core/Case/Case2dCavityFlowFactory.h>

#include <Core/Solver/PisoSolver.h>
#include <Core/Domain/FaceSetFactory.h>

#include <iostream>

namespace fstim
{
    std::unique_ptr<SolverBase> Case2dCavityFlowFactory::buildCase()
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
        std::unique_ptr<ScalarFieldEqu> pressure = m_createPressureField(*mesh.get());

        std::unique_ptr<PisoSolver> solver = std::make_unique<PisoSolver>();
        solver->setMesh(std::move(mesh));
        solver->setVelocity(std::move(velocity));
        solver->setPressure(std::move(pressure));

        return std::move(solver);
    }

    std::unique_ptr<VectorFieldEqu> Case2dCavityFlowFactory::m_createVelocityField(Mesh2d& mesh)
    {
        std::unique_ptr<VectorFieldEqu> velocityField = std::make_unique<VectorFieldEqu>(mesh.nCells);
        velocityField->addBc(BcType::FIXEDVALUE, m_referenceVelocity); // Moving Wall
        velocityField->addBc(BcType::FIXEDVALUE, vecp::Vec2d(0., 0.)); // Static Wall
        velocityField->addBc(BcType::FIXEDVALUE, vecp::Vec2d(0., 0.)); // Static Wall
        velocityField->addBc(BcType::FIXEDVALUE, vecp::Vec2d(0., 0.)); // Static Wall
        return std::move(velocityField);
    }

    std::unique_ptr<ScalarFieldEqu> Case2dCavityFlowFactory::m_createPressureField(Mesh2d& mesh)
    {
        std::unique_ptr<ScalarFieldEqu> pressureField = std::make_unique<ScalarFieldEqu>(mesh.nCells);
        pressureField->addBc(BcType::ZEROGRADIENT, 0.); // Moving Wall
        pressureField->addBc(BcType::ZEROGRADIENT, 0.); // Static Wall
        pressureField->addBc(BcType::ZEROGRADIENT, 0.); // Static Wall
        pressureField->addBc(BcType::ZEROGRADIENT, 0.); // Static Wall
        return std::move(pressureField);
    }
    
    void Case2dCavityFlowFactory::m_assignMeshBoundaries(Mesh2d& mesh)
    {
        std::set<Compass> m_directionsToAssign = {
            Compass::NORTH, Compass::SOUTH, Compass::WEST, Compass::EAST
        };

        FaceSetFactory::defineNewFaceSetByCompass(mesh, m_referenceDirection);
        m_directionsToAssign.erase(m_referenceDirection);

        for (Compass direction : m_directionsToAssign)
        {
            FaceSetFactory::defineNewFaceSetByCompass(mesh, direction);
        }
    }
}