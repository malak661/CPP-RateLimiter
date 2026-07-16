#pragma once

#include <exception>
#include <string>

// Thrown when an incoming request is missing required fields,
// has an invalid client key, or contains invalid config values.
class InvalidRequestException : public std::exception {
public:
    explicit InvalidRequestException(const std::string& message)
        : message_(message) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};
