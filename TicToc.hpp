/***************************************************************
 * Copyright 2020 (c) Xinguang Bian <victorbian@outlook.com>
 *
 * TicToc is part of TestFunc project.
 * By providing global labeled timers, TicToc can easily be used
 * to benchmark the time cost of algorithms.
 *
 * Note: C++17 feature is required.
 **************************************************************/

#ifndef __DBG_TICTOC_H
#define __DBG_TICTOC_H

#pragma message("dbg warning: TicToc is a feature for debug or test only")

#include "internal/WarningMessage.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>

namespace [[deprecated("Features for debug or test only")]] dbg {

class TicToc {
public:
    class ScopedTimer {
    public:
        explicit ScopedTimer(const std::string& label, bool showEachRun = false)
            : label_(label)
            , showEachRun_(showEachRun)
        {
            TicToc::start(label);
        }
        ~ScopedTimer()
        {
            if (showEachRun_)
                TicToc::showTime(label_, true);
            else
                TicToc::stop(label_);
        }

    private:
        const std::string label_;
        const bool showEachRun_ { false };
    };

public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    TicToc() = delete;

    inline static TimePoint now() { return Clock::now(); }
    inline static void start(const std::string& label)
    {
        timer(label).start();
    }
    /// @brief stop the timer and retrieve total time (seconds)
    inline static double stop(const std::string& label)
    {
        return timer(label).stop();
    }
    /// @brief retrieve total time (seconds)
    inline static double totalTime(const std::string& label)
    {
        return timer(label).totalTime();
    }
    inline static void showTime(const std::string& label, bool stop = false)
    {
        timer(label).showTime(stop);
    }
    inline static void reset(const std::string& label)
    {
        timer(label).reset();
    }
    inline static int count(const std::string& label)
    {
        return timer(label).count();
    }
    inline static double avgTime(const std::string& label)
    {
        return timer(label).avgTime();
    }
    inline static double lastTime(const std::string& label)
    {
        return timer(label).lastTime();
    }

private:
    class Timer {
    public:
        explicit inline Timer(std::string _label, bool seton = false)
            : label(std::move(_label))
        {
            reset(seton);
        }
        inline ~Timer()
        {
            showTime(true);
        }
        inline void start()
        {
            if (is_on)
                return;
            time_start = TicToc::now();
            is_on = true;
            ++count_;
        }
        inline double stop()
        {
            if (!is_on)
                return time_total;
            time_last = timeFromStart();
            time_total += time_last;
            time_start = TicToc::now();
            is_on = false;
            return time_total;
        }
        inline void reset(bool seton = false)
        {
            time_start = TicToc::now();
            time_total = 0.;
            time_last = 0.;
            count_ = 0;
            is_on = seton;
        }
        [[nodiscard]] inline double totalTime() const
        {
            if (!is_on)
                return time_total;
            return time_total + timeFromStart();
        }
        [[nodiscard]] inline double timeFromStart() const
        {
            return std::chrono::duration<double>(TicToc::now() - time_start).count();
        }
        [[nodiscard]] inline int count() const { return count_; }
        [[nodiscard]] inline double avgTime() const
        {
            if (count_ < 1)
                return 0.;
            return totalTime() / count_;
        }
        [[nodiscard]] inline double lastTime() const
        {
            return time_last;
        }

        inline void showTime(bool stop = false)
        {
            if (stop)
                this->stop();

            auto time2Str = [](double sec) -> std::string {
                std::string unit = " s";
                if (sec >= 10)
                    unit = " s";
                else if (sec >= 1e-4) {
                    sec *= 1e3;
                    unit = " ms";
                } else if (sec >= 1e-7) {
                    sec *= 1e6;
                    unit = " us";
                } else {
                    sec *= 1e9;
                    unit = " ns";
                }
                return std::to_string(sec) + unit;
            };

            printf("\33[33mtimer[%s]: cnt[%d] total[%s] avg[%s] last[%s]\33[0m\n",
                label.c_str(), this->count(), time2Str(this->totalTime()).c_str(),
                time2Str(this->avgTime()).c_str(), time2Str(this->lastTime()).c_str());
            std::cout.flush();
        }

    private:
        std::string label;
        TimePoint time_start;
        double time_total { 0 };
        double time_last { 0 }; // time between last start and last stop
        int count_ { 0 }; //!< started times
        volatile bool is_on { false };
    };

private:
    inline static std::unordered_map<std::string, Timer> global_timers;

private:
    inline static Timer& timer(const std::string& label)
    {
        auto res = global_timers.find(label);
        if (res != global_timers.end())
            return res->second;
        auto r = global_timers.emplace(label, label);
        return r.first->second;
    }
};

TESTFUNCS_CREATE_WANNING_MSG(TicToc)
} // namespace dbg

#endif // __DBG_TICTOC_H
