/***************************************************************
 * Copyright 2020 (c) Xinguang Bian <victorbian@outlook.com>
 *
 * Benchmark is part of TestFunc project.
 * It's used for benchmarking performance of given function
 **************************************************************/

#ifndef __DBG_BENCHMARK_H
#define __DBG_BENCHMARK_H

#pragma message("dbg warning: Benchmark is a feature for debug or test only")

#include "internal/WarningMessage.hpp"
#include <algorithm>
#include <chrono>
#include <numeric>

namespace [[deprecated("Features for debug or test only")]] dbg {

struct EmptyFunc {
    void operator()() { }
};

/// @brief Do benchmark on given function
/// @param samples Number of samples that benchmark will test, the best result will be taken
/// @param iterations Number of iterations each sample will run, the result is average time of one iteration
/// @param func The function that benchmark tests on, it's signature should be void()
/// @param sync Optional function that will be called after each sample finished. It's used for ensuring
/// that all computation are finished (for example, GPU synchronization)
/// @return The best average time (in milliseconds) that the tested function costs
template <typename Func, typename Sync = EmptyFunc>
double benchmark(unsigned samples, unsigned iterations, Func func, Sync sync = Sync())
{
    using namespace std::chrono;
    samples = std::max(samples, 1u);
    iterations = std::max(iterations, 1u);
    double best_time = std::numeric_limits<double>::infinity();
    for (unsigned s = 0; s < samples; ++s) {
        auto t1 = high_resolution_clock::now();
        for (unsigned i = 0; i < iterations; ++i)
            func();
        sync();
        auto t2 = high_resolution_clock::now();
        best_time = std::min(best_time, duration<double>(t2 - t1).count());
    }
    best_time = best_time / iterations * 1e3; // milliseconds
    return best_time;
}

TESTFUNCS_CREATE_WANNING_MSG(Benchmark)
} //namespace dbg

#endif //__DBG_BENCHMARK_H
