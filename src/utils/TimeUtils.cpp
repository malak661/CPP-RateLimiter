#include "utils/TimeUtils.h"

std::chrono::steady_clock::time_point TimeUtils::now() {
    return std::chrono::steady_clock::now();
}

double TimeUtils::secondsElapsed(std::chrono::steady_clock::time_point from,
                                  std::chrono::steady_clock::time_point to) {
    return std::chrono::duration<double>(to - from).count();
}
