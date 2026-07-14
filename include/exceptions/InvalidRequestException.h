#ifndef EXCEPTIONS_INVALIDREQUESTEXCEPTION_H
#define EXCEPTIONS_INVALIDREQUESTEXCEPTION_H

#include <exception>
#include <string>

namespace ratelimiter {
namespace exceptions {

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

} // namespace exceptions
} // namespace ratelimiter

#endif // EXCEPTIONS_INVALIDREQUESTEXCEPTION_H
