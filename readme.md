# Test Functions for Developing
This project aims to provide some commonly used test functions while developing with C++.

Put the folder `TestFuncs` to `/usr/local/include` so that you can use the test functions by adding `#include <TestFuncs/...>` to your project (C++17 is required). The files and functions in this repository are not supposed to be used in projects which are to be released!

## Modules

### TicToc

Global timer for timing test, managed with labels.

1. Measure time elapsed between two time points:
```cpp
auto someFunc = [] { this_thread::sleep_for(12ms); };

for (int i = 0; i < 20; ++i) {
    dbg::TicToc::start("tm");
    someFunc();
    dbg::TicToc::stop("tm");
}

/* output:
timer[tm]: cnt[20] total[241.692379 ms] avg[12.084619 ms] last[12.090958 ms]
*/
```

When `dbg::TicToc::start(string label)` is called for the first time, a global timer named `label` is created. The timer records time elapsed between a pair of `start()` and `stop()`, and automatically output the average and last value when it's destructed (the executable exits).

2. Measure and output time in each run:
```cpp
auto someFunc = [] { this_thread::sleep_for(12ms); };

for (int i = 0; i < 3; ++i) {
    dbg::TicToc::start("tm");
    someFunc();
    dbg::TicToc::showTime("tm", true);
}

/* output:
timer[tm]: cnt[1] total[12.096808 ms] avg[12.096808 ms] last[12.096808 ms]
timer[tm]: cnt[2] total[24.186642 ms] avg[12.093321 ms] last[12.089834 ms]
timer[tm]: cnt[3] total[36.275821 ms] avg[12.091940 ms] last[12.089179 ms]
timer[tm]: cnt[3] total[36.275821 ms] avg[12.091940 ms] last[12.089179 ms]
*/
```

`dbg::TicToc::showTime(string label, bool stop)` output the timer info immediately. If the argument `stop` is set to `true`, it will stop the timer in the meantime.

3. Measure time elapsed in a scope
```cpp
auto someFunc = [](int i) {
    dbg::TicToc::ScopedTimer st("tm", true);
    if (i & 1)
        this_thread::sleep_for(12ms);
};

for (int i = 0; i < 4; ++i)
    someFunc(i);

/* output:
timer[tm]: cnt[1] total[0.682000 us] avg[0.682000 us] last[0.682000 us]
timer[tm]: cnt[2] total[12.090938 ms] avg[6.045469 ms] last[12.090256 ms]
timer[tm]: cnt[3] total[12.090991 ms] avg[4.030330 ms] last[53.000000 ns]
timer[tm]: cnt[4] total[24.182239 ms] avg[6.045560 ms] last[12.091248 ms]
timer[tm]: cnt[4] total[24.182239 ms] avg[6.045560 ms] last[12.091248 ms]
*/
```

`dbg::TicToc::ScopedTimer` start a timer when it's constructed and stop the timer when it's destructed. It's useful in functions with multiple `return`.

### Benchmark

Latency test for given functions.

```cpp
auto someFunc = [] { this_thread::sleep_for(12ms); };

auto time = dbg::benchmark(3, 100, someFunc);
cout << "time cost: " << time << " ms\n";

/* output:
time cost: 12.0641 ms
*/
```

`dbg::benchmark()` runs given function for specific iterations in each sample. The best average time is returned.

### SseHelper

Show SSE packed variables.

1. Show integer variables:

```cpp
auto val = _mm_set_epi32(1, -2, 0, 4);
dbg::showM128i<int>(val);

/* output:
  4   0  -2   1 
*/
```

2. Show float variables:

```cpp
auto val = _mm_set_ps(3.14, 10, -0.001, 0);
dbg::showM128(val, "%4.3f ");

/* output
0.000 -0.001 10.000 3.140 
*/
```

