#include <benchmark/benchmark.h>

#include <Core/Solver/JacobiVectorMethod.h>
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

    static std::unique_ptr<Field<vecp::Vec2d>> createVectorField(int nCells, double initialValue)
    {
        std::unique_ptr<Field<vecp::Vec2d>> field = std::make_unique<Field<vecp::Vec2d>>(nCells);
        std::map<int, vecp::Vec2d>* lhs = field->writeLeft();
        vecp::Vec2d* rhs = field->writeRight();
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
                    ? vecp::Vec2d((double)cellId + 10., (double)cellId + 10.)
                    : vecp::Vec2d(-1.,-1.);
            }
            rhs[cellId] = vecp::Vec2d(
                -2. * (double)(cellId + 1),
                -2. * (double)(cellId + 1)
            );
        }

        vecp::Vec2d* values = field->writeValues();
        std::fill(values, values + field->nCells, initialValue);

        return field;

    }

    static void BM_JacobiVectorMethod_OneCycle(benchmark::State& state)
    {
        int nCells = state.range(0);
        double initialValue = 20.;
        for (auto _ : state)
        {
            state.PauseTiming();
            std::unique_ptr<Field<vecp::Vec2d>> field = createVectorField(nCells, initialValue);
            field->setTolerance(Tolerance<double>(std::numeric_limits<double>::max(),
                std::numeric_limits<double>::max()));

            JacobiMethod<vecp::Vec2d> fieldIterator{};
            state.ResumeTiming();

            state.counters["Loops"] = fieldIterator(*(field.get()));
            state.PauseTiming();
            field.reset();
            state.ResumeTiming();
        }
        state.counters["Total Cells"] = nCells;
    }

    static void BM_JacobiVectorMethod_MaxCycles(benchmark::State& state)
    {
        int nCells = state.range(0);
        double initialValue = -20.;
        for (auto _ : state)
        {
            state.PauseTiming();
            std::unique_ptr<Field<vecp::Vec2d>> field = createVectorField(nCells, initialValue);
            field->setTolerance(Tolerance<double>(0., 0.));

            JacobiMethod<vecp::Vec2d> fieldIterator{};
            state.ResumeTiming();

            state.counters["Loops"] = fieldIterator(*(field.get()));
            state.PauseTiming();
            field.reset();
            state.ResumeTiming();
        }
        state.counters["Total Cells"] = nCells;
    }

}

BENCHMARK(bm_solver::BM_JacobiVectorMethod_OneCycle)
->RangeMultiplier(10)
->Range(100, 100000)
->Unit(benchmark::kMillisecond);


BENCHMARK(bm_solver::BM_JacobiVectorMethod_MaxCycles)
->RangeMultiplier(10)
->Range(100, 100000)
->Unit(benchmark::kMillisecond);

