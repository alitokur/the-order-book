#include <benchmark/benchmark.h>
#include <cstdint>
#include "order_book.h"   // whatever your header is

static void BM_AddBuy_NoCross_Batch(benchmark::State& state)
{

}
BENCHMARK(BM_AddBuy_NoCross_Batch)->Arg(1'000)->Arg(10'000)->Arg(100'000);

static void BM_AddSell_NoCross_Batch(benchmark::State& state)
{

}
BENCHMARK(BM_AddSell_NoCross_Batch)->Arg(1'000)->Arg(10'000)->Arg(100'000);

BENCHMARK_MAIN();
