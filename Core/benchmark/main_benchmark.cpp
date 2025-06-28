#include <benchmark/benchmark.h>

//#include <iostream>

/**
void SomeFunction()
{  
    for (int i = 0; i < 1000; i++)
    {
        std::cout << i << ", " << std::endl;
    }
}

static void BM_SomeFunction(benchmark::State& state) {
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        SomeFunction();
    }
}
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
// Run the benchmark
*/
BENCHMARK_MAIN();