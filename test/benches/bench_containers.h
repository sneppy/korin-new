#pragma once

#include "benchmark/benchmark.h"
#include "testing.h"

#include "containers/containers.h"

using namespace Korin;

#include <map>

static char const* names[16] = {
	"sneppy",
	"fmonz",
	"lorecri96",
	"lpraat",
	"nondecidibile",
	"cam",
	"nicofico",
	"nonswiss",
	"turingcomplete",
	"nov4chip",
	"samn844",
	"zerry",
	"sgherry",
	"frabellazio",
	"fryscan",
	"ananasso"
};

static void BM_containers_Korin_Map(benchmark::State& state)
{
	Map<String, Testing::Object> map;
	const int32 numItems = state.range(0);

	for (auto _ : state)
	{
		for (int32 i = 0; i < numItems; ++i)
		{
			int32 nameIdx = rand() & 0xf;
			map.insert(names[nameIdx], 0x1ull << (i & 0xf));
		}

		for (int32 i = 0; i < numItems; ++i)
		{
			int32 nameIdx = rand() & 0xf;
			auto it = map.find(names[nameIdx]);
			benchmark::DoNotOptimize(it);
		}
	}
}
BENCHMARK(BM_containers_Korin_Map)->Range(8, 8 << 10);

static void BM_containers_std_map(benchmark::State& state)
{
	std::map<String, Testing::Object> map;
	const int32 numItems = state.range(0);

	for (auto _ : state)
	{
		for (int32 i = 0; i < numItems; ++i)
		{
			int32 nameIdx = rand() & 0xf;
			map.insert(std::make_pair(names[nameIdx], 0x1ull << (i & 0xf)));
		}

		for (int32 i = 0; i < numItems; ++i)
		{
			int32 nameIdx = rand() & 0xf;
			auto it = map.find(names[nameIdx]);
			benchmark::DoNotOptimize(it);
		}
	}
}
BENCHMARK(BM_containers_std_map)->Range(8, 8 << 10);
