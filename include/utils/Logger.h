#pragma once

#include <string>

// Simple synchronous logger. Thread-safe via internal mutex.
class Logger {
public:
    static void info(const std::string& message);
    static void warn(const std::string& message);
    static void error(const std::string& message);
};
