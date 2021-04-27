/***************************************************************
 * Copyright 2020 (c) Xinguang Bian <victorbian@outlook.com>
 *
 * SseHelper is part of TestFunc project.
 * It provides helper functions to debug SSE codes.
 **************************************************************/

#if not defined(__DBG_SSEHELPER_H) and defined(__x86_64__)
#define __DBG_SSEHELPER_H

#pragma message("DBG warning: SSE-Helper is a feature for debug or test only")

#include "WarningMessage.hpp"
#include <bitset>
#include <immintrin.h>
#include <iostream>

namespace [[deprecated("Features for debug or test only")]] dbg {

template <typename T = uint8_t>
inline void showM128i(const __m128i val, bool show_bin = false)
{
    constexpr int sz = sizeof(T);
    constexpr int N = 16 / sz;
    alignas(16) T v[N];
    _mm_store_si128(reinterpret_cast<__m128i*>(v), val);
    for (int i = 0; i < N; ++i) {
        if (show_bin)
            std::cout << std::bitset<sz * 8>(v[i]) << " ";
        else
            printf("%3lld ", (long long)(v[i]));
    }
    std::cout << std::endl;
}

inline void showM128(const __m128 val, const char* format = "%4.2f ")
{
    constexpr int sz = sizeof(float);
    constexpr int N = 16 / sz;
    alignas(16) float v[N];
    _mm_store_ps(v, val);
    for (int i = 0; i < N; ++i) {
        printf(format, v[i]);
    }
    std::cout << std::endl;
}

TESTFUNCS_CREATE_WANNING_MSG(SseHelper)
} // namespace dbg

#endif // __DBG_SSEHELPER_H
