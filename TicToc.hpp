#ifndef __BXG_TICTOC_H
#define __BXG_TICTOC_H

#pragma message("BXG warning: TicToc is a feature for debug or test only")

#include <atomic>
#include <chrono>
#include <iostream>
#include <unordered_map>

namespace [[deprecated("Features for debug or test only")]] bxg {

class TicToc {
public:
    using Clock = std::chrono::system_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    TicToc() = delete;

    inline static TimePoint now() { return Clock::now(); }
    inline static void start(const std::string& label)
    {
        global_timers[label].start();
    }
    /// @brief stop the timer and retrieve total time (seconds)
    inline static double stop(const std::string& label)
    {
        return global_timers[label].stop();
    }
    /// @brief retrieve total time (seconds)
    inline static double totalTime(const std::string& label)
    {
        return global_timers[label].totalTime();
    }
    inline static double showTime(const std::string& label, bool stop = false)
    {
        auto& timer = global_timers[label];
        if (stop)
            timer.stop();
        auto t = timer.totalTime();
        std::cout << "timer [" << label << "]: " << t << " s" << std::endl;
        return t;
    }
    inline static void reset(const std::string& label)
    {
        global_timers[label].reset();
    }
    inline static int count(const std::string& label)
    {
        return global_timers[label].count();
    }
    inline static double avgTime(const std::string& label)
    {
        return global_timers[label].avgTime();
    }

private:
    class Timer {
    public:
        inline Timer(bool seton = true)
        {
            reset(seton);
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
            time_total += timeFromStart();
            time_start = TicToc::now();
            is_on = false;
            return time_total;
        }
        inline void reset(bool seton = true)
        {
            time_start = TicToc::now();
            time_total = 0.;
            count_ = 0;
            is_on = seton;
        }
        inline double totalTime() const
        {
            if (!is_on)
                return time_total;
            return time_total + timeFromStart();
        }
        inline double timeFromStart() const
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(TicToc::now() - time_start).count() * 1e-9;
        }
        inline int count() const { return count_; }
        inline double avgTime() const
        {
            if (count_ < 1)
                return 0.;
            return totalTime() / count_;
        }

    private:
        TimePoint time_start;
        double time_total;
        int count_; //!< started times
        std::atomic_bool is_on;
    };

private:
    inline static std::unordered_map<std::string, Timer> global_timers;
};

namespace details {
    class TicTocDummy final {
        struct Msg {
            Msg()
            {
                std::cout << "\33[33mWarning: Debug feature \"TicToc\" is included.\33[0m\n"
                          << std::endl;
            };
        };
        inline static Msg msg;
    };
}
} // namespace bxg

#endif
