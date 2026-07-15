#pragma once

#include <chrono>

class TimeUtils {
public:
    static std::chrono::steady_clock::time_point now();
    static double secondsElapsed(std::chrono::steady_clock::time_point from,
                                  std::chrono::steady_clock::time_point to);
};
