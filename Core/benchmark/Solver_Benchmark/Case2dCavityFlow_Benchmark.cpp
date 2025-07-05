#include <benchmark/benchmark.h>

#include <Core/Case/Case2dCavityFlowFactory.h>
#include <Core/Solver/PisoSolver.h>

#include <Vecplus/Vec2.h>

#include <algorithm>
#include <memory>
#include <limits>
#include <map>

#include <omp.h>

using namespace fstim;

namespace bm_solver
{
	static void BM_Case2dCavityFlow(benchmark::State& state)
	{
        int nThreads = state.range(0);
        int nCells = state.range(1);
        omp_set_dynamic(0);
        omp_set_num_threads(nThreads);
        #pragma omp parallel
        #pragma omp single
        {
            state.counters["Threads"] = nThreads;
        }
        state.counters["Cells"] = std::pow(nCells, 2);


        for (auto _ : state)
        {
            state.PauseTiming();
            Case2dCavityFlowFactory caseFactory{};
            caseFactory.setDomainSize(vecp::Vec2i(nCells, nCells));
            caseFactory.setDomainLength(vecp::Vec2d(1., 1.));
            caseFactory.setReferenceVelocity(vecp::Vec2d(1.0, 0.0));
            caseFactory.setReferenceDirection(fstim::Compass::NORTH);
            std::unique_ptr<SolverBase> solver = caseFactory.buildCase();
            state.ResumeTiming();
            solver->compute(0.001);
             
            state.PauseTiming();
            solver.reset();
            state.ResumeTiming();
        }
	}
}

BENCHMARK(bm_solver::BM_Case2dCavityFlow)
->ArgsProduct({ {1, 2, 3, 4},
    {10, 50, 100, 500}})
->Unit(benchmark::kMillisecond);
