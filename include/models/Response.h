#pragma once

#include <string>

struct Response {
    int statusCode;
    bool allowed;
    double limit;
    double remainingTokens;
    double retryAfterSeconds;
    std::string message;

    Response()
        : statusCode(200),
          allowed(false),
          limit(0.0),
          remainingTokens(0.0),
          retryAfterSeconds(0.0),
          message("") {}

    Response(int statusCode_, bool allowed_, double limit_,
             double remainingTokens_, double retryAfterSeconds_,
             const std::string& message_)
        : statusCode(statusCode_),
          allowed(allowed_),
          limit(limit_),
          remainingTokens(remainingTokens_),
          retryAfterSeconds(retryAfterSeconds_),
          message(message_) {}
};
