#include "utils/Logger.h"
#include <iostream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>

static std::mutex g_logMutex;

static std::string timestamp() {
    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << std::put_time(std::localtime(&t), "%H:%M:%S");
    return ss.str();
}

void Logger::info(const std::string& message) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    std::cout << "[" << timestamp() << "] [INFO]  " << message << "\n";
}

void Logger::warn(const std::string& message) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    std::cout << "[" << timestamp() << "] [WARN]  " << message << "\n";
}

void Logger::error(const std::string& message) {
    std::lock_guard<std::mutex> lock(g_logMutex);
    std::cerr << "[" << timestamp() << "] [ERROR] " << message << "\n";
}
