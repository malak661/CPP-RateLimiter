#pragma once

#include <string>

// Represents the outgoing response returned to the client,
// built by the API layer from the core logic's decision.
struct Response {
    int statusCode;          // HTTP status code (e.g. 200, 429, 400)
    bool allowed;            // whether the request was accepted
    double remainingTokens;  // tokens left in the bucket after this request
    double retryAfterSeconds; // time to wait before retrying (0 if allowed)
    std::string message;     // human-readable message / error detail

    Response()
        : statusCode(200),
          allowed(false),
          remainingTokens(0.0),
          retryAfterSeconds(0.0),
          message("") {}

    Response(int statusCode_, bool allowed_, double remainingTokens_,
              double retryAfterSeconds_, const std::string& message_)
        : statusCode(statusCode_),
          allowed(allowed_),
          remainingTokens(remainingTokens_),
          retryAfterSeconds(retryAfterSeconds_),
          message(message_) {}
};
