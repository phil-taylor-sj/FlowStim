#include <benchmark/benchmark.h>

#include <Core/Domain/Mesh2dStructuredFactory.h>
#include <Core/Domain/Mesh.h>

#include <Vecplus/Vec2.h>

#include <memory>

using namespace fstim;

static void BM_Mesh2dStructuredFactory_Domain(benchmark::State& state)
{	
	int cellsPerSide = state.range(0);
	Mesh2dStructuredFactory factory = Mesh2dStructuredFactory();
	for (auto _ : state) 
	{
		std::unique_ptr<Mesh2d> mesh = factory(
			vecp::Vec2i(cellsPerSide, cellsPerSide), vecp::Vec2d(10., 10.)
		);
	}

	state.counters["Total Cells"] = cellsPerSide * cellsPerSide;
}

BENCHMARK(BM_Mesh2dStructuredFactory_Domain)
	->RangeMultiplier(2)
	->Range(8, 1024)
	->Unit(benchmark::kMillisecond);
