#ifndef __BXG_SSEHELPER_H
#define __BXG_SSEHELPER_H

#pragma message("BXG warning: SSE-Helper is a feature for debug or test only")

#include <immintrin.h>
#include <iostream>
#include <bitset>

namespace [[deprecated("Features for debug or test only")]] bxg {

template <typename T = uint8_t>
inline void showM128i(const __m128i val, bool show_bin = false)
{
    constexpr int sz = sizeof(T);
    constexpr int N = 16 / sz;
    T* v = static_cast<T*>(std::aligned_alloc(16, 16));
    _mm_store_si128(reinterpret_cast<__m128i*>(v), val);
    for (int i = 0; i < N; ++i) {
        if (show_bin)
            std::cout << std::bitset<sz * 8>(v[i]) << " ";
        else
            printf("%3lld ", (long long)(v[i]));
    }
    std::cout << std::endl;
    std::free(v);
}

}

#endif // __BXG_SSEHELPER_H
