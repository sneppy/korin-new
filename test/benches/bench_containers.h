#pragma once

#include "benchmark/benchmark.h"
#include "testing.h"

#include "containers/containers.h"

using namespace Korin;

// STL includes
#include <map>
#include <unordered_map>

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
	Map<int32, Testing::Object> map;
	const int32 numItems = state.range(0);

	for (auto _ : state)
	{
		for (int32 i = 0; i < numItems; ++i)
		{
			int32 r = rand() & 0xff;
			map.insert(r, 0x1ull << (i & 0xf));
		}

		for (int32 i = 0; i < numItems; ++i)
		{
			int32 r = rand() & 0xff;
			auto it = map.find(r);
			benchmark::DoNotOptimize(it);
		}
	}
}
BENCHMARK(BM_containers_Korin_Map)->Range(8, 8 << 10);

static void BM_containers_std_map(benchmark::State& state)
{
	std::map<int32, Testing::Object> map;
	const int32 numItems = state.range(0);

	for (auto _ : state)
	{
		for (int32 i = 0; i < numItems; ++i)
		{
			int32 r = rand() & 0xff;
			map.insert({r, 0x1ull << (i & 0xf)});
		}

		for (int32 i = 0; i < numItems; ++i)
		{
			int32 r = rand() & 0xff;
			auto it = map.find(r);
			benchmark::DoNotOptimize(it);
		}
	}
}
BENCHMARK(BM_containers_std_map)->Range(8, 8 << 10);

static void BM_containers_Korin_Map2(benchmark::State& state)
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
BENCHMARK(BM_containers_Korin_Map2)->Range(8, 8 << 10);

static void BM_containers_std_map2(benchmark::State& state)
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
BENCHMARK(BM_containers_std_map2)->Range(8, 8 << 10);

static void BM_containers_Korin_HashMap(benchmark::State& state)
{
	HashMap<String, Testing::Object> map;
	const int32 numItems = state.range(0);

	for (auto _ : state)
	{
		for (int32 i = 0; i < numItems; ++i)
		{
			int32 nameIdx = rand() & 0xf;
			map.emplace(names[nameIdx], 0x1ull << (i & 0xf));
		}

		for (int32 i = 0; i < numItems; ++i)
		{
			int32 nameIdx = rand() & 0xf;
			auto it = map.find(names[nameIdx]);
			benchmark::DoNotOptimize(it);
		}
	}
}
BENCHMARK(BM_containers_Korin_HashMap)->Range(8, 8 << 10);

static void BM_containers_std_unordered_map(benchmark::State& state)
{
	struct HashStringSTL
	{
		std::size_t operator()(String const& key) const
		{
			return murmur(*key, key.getNumBytes());
		}
	};

	std::unordered_map<String, Testing::Object, HashStringSTL> map;
	const int32 numItems = state.range(0);

	for (auto _ : state)
	{
		for (int32 i = 0; i < numItems; ++i)
		{
			int32 nameIdx = rand() & 0xf;
			map.emplace(std::make_pair(names[nameIdx], 0x1ull << (i & 0xf)));
		}

		for (int32 i = 0; i < numItems; ++i)
		{
			int32 nameIdx = rand() & 0xf;
			auto it = map.find(names[nameIdx]);
			benchmark::DoNotOptimize(it);
		}
	}
}
BENCHMARK(BM_containers_std_unordered_map)->Range(8, 8 << 10);
