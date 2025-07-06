#include <Core/Case/Case2dCavityFlowFactory.h>
#include <Core/Solver/SolverBase.h>
#include <Core/Domain/Compass.h>

#include <time.h>
#include <thread>
#include <chrono>
#include <iostream>

#include <omp.h>

int main(int argc, char** argv)
{
    omp_set_dynamic(0);
    omp_set_num_threads(1);


    int nCells = 20;
    double deltaTime = 0.001;

    fstim::Case2dCavityFlowFactory caseFactory{};
    caseFactory.setDomainSize(vecp::Vec2i(nCells, nCells));
    caseFactory.setDomainLength(vecp::Vec2d(1., 1.));
    caseFactory.setReferenceVelocity(vecp::Vec2d(1.0, 0.0));
    caseFactory.setReferenceDirection(fstim::Compass::NORTH);
    std::unique_ptr<fstim::SolverBase> solver = caseFactory.buildCase();

    std::cout << "Solver constructed, beginning computation loop." << std::endl;

    int minLoopTime = 32;
    int loopTime = minLoopTime;
    for (int counter = 1; counter <= 1000; counter++)
    {
        clock_t startTime = std::clock();
        solver->compute(deltaTime);
        loopTime = 1000.0 * ((double)(std::clock() - startTime)) 
            / CLOCKS_PER_SEC;
        if (loopTime < minLoopTime)
        {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(minLoopTime - loopTime)
            );
        }

        if (counter % 100 == 0)
        {
            std::cout << counter << " loops completed." << std::endl;
        }
    }
}