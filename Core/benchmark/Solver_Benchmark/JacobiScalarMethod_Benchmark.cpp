#include <benchmark/benchmark.h>

#include <Core/Solver/JacobiScalarMethod.h>
#include <Core/Solver/Tolerance.h>
#include <Core/Field/Field.h>

#include <Vecplus/Vec2.h>

#include <algorithm>
#include <memory>
#include <limits>
#include <map>


using namespace fstim;

namespace bm_solver
{

    static std::unique_ptr<Field<double>> createScalarField(int nCells, double initialValue)
    {
        std::unique_ptr<Field<double>> field = std::make_unique<Field<double>>(nCells);
        std::map<int, double>* lhs = field->writeLeft();
        double* rhs = field->writeRight();
        for (int cellId = 0; cellId < field->nCells; cellId++)
        {
            // Sample Coefficents
            // [10., -1., -1., 0., 0., ...]
            // [-1., 11., -1., -1., 0., ...]
            // [-1., -1., 12., -1., -1., ...]
            // [0., -1., -1., 13., -1., -1., ...]
            // [0., 0., -1., -1., 14.., -1., ...]
            // ....
            int minIndex = std::max(cellId - 2, 0);
            int maxIndex = std::min(cellId + 2, field->nCells - 1);
            for (int index = minIndex; index <= maxIndex; index++)
            {
                lhs[cellId][index] = (cellId == index)
                    ? (double)cellId + 10.
                    : -1.;
            }
            rhs[cellId] = -2. * (double)(cellId + 1);
        }

        double* values = field->writeValues();
        std::fill(values, values + field->nCells, initialValue);

        return field;

    }

	static void BM_JacobiScalarMethod_OneCycle(benchmark::State& state)
	{	
        int nCells = state.range(0);
        double initialValue = 20.;
        for (auto _ : state)
        {
		    state.PauseTiming();
            std::unique_ptr<Field<double>> field = createScalarField(nCells, initialValue);
            field->setTolerance(Tolerance<double>(std::numeric_limits<double>::max(), 
                std::numeric_limits<double>::max()));
            state.ResumeTiming();

            JacobiMethod<double> fieldIterator {};
            state.counters["Loops"] =  fieldIterator(*(field.get()));
        }        
        state.counters["Total Cells"] = nCells;
    }

    static void BM_JacobiScalarMethod_MaxCycles(benchmark::State& state)
    {
        int nCells = state.range(0);
        double initialValue = -20.;
        for (auto _ : state)
        {
            state.PauseTiming();
            std::unique_ptr<Field<double>> field = createScalarField(nCells, initialValue);
            field->setTolerance(Tolerance<double>(0., 0.));

            state.ResumeTiming();

            JacobiMethod<double> fieldIterator{};
            state.counters["Loops"] = fieldIterator(*(field.get()));
        }
        state.counters["Total Cells"] = nCells;
    }

}

BENCHMARK(bm_solver::BM_JacobiScalarMethod_OneCycle)
    ->RangeMultiplier(2)
    ->Range(32, 1024)
    ->Unit(benchmark::kMicrosecond);


BENCHMARK(bm_solver::BM_JacobiScalarMethod_MaxCycles)
->RangeMultiplier(2)
->Range(32, 1024)
->Unit(benchmark::kMicrosecond);

