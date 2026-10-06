#pragma once

#include <chrono>

namespace driver {

struct chrono_traits {
    static void init() noexcept {}

    static std::chrono::nanoseconds monotonic_ticks() noexcept {
        using namespace std::chrono;
        return steady_clock::now().time_since_epoch();
    }

    static std::chrono::nanoseconds wall_ticks() noexcept {
        using namespace std::chrono;
        return system_clock::now().time_since_epoch();
    }
};

}
