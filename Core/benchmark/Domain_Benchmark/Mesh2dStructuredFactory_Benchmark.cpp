#include <benchmark/benchmark.h>

#include <Core/Domain/Mesh2dStructuredFactory.h>
#include <Core/Domain/Mesh.h>

#include <Vecplus/Vec2.h>

#include <memory>

using namespace fstim;

static void BM_Mesh2dStructuredFactory_Domain(benchmark::State& state)
{	
	vecp::Vec2i size = vecp::Vec2i(state.range(0), state.range(1));
	Mesh2dStructuredFactory factory = Mesh2dStructuredFactory();
	for (auto _ : state) 
	{
		std::unique_ptr<Mesh2d> mesh = factory(
			size, vecp::Vec2d(10., 10.)
		);
	}

	state.counters["Total Cells"] = size.x * size.y;
}

BENCHMARK(BM_Mesh2dStructuredFactory_Domain)
	->Args({ 8,  8})
	->Args({ 16, 16})
	->Args({ 32, 32})
	->Args({ 64, 64})
	->Args({ 128, 128})
	->Args({ 256, 256})
	->Args({ 512, 512 })
	->Args({ 1024, 1024 })
	->Args({ 2,   32 })
	->Args({ 4,   64 })
	->Args({ 8,  128 })
	->Args({ 16,  256 })
	->Args({ 32,  512 })
	->Args({ 64, 1024 })
	->Args({ 128, 2048 })
	->Args({ 256, 4096 })
	->Unit(benchmark::kMillisecond);
