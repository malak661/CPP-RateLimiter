#pragma once

#include <string>
#include "models/Request.h"
#include "models/Response.h"
#include "core/RateLimiter.h"

// Sits between the raw HTTP layer (HttpServer) and the core rate-limiting
// logic. Validates input, extracts the client key, and delegates to RateLimiter.
class RequestHandler {
public:
    explicit RequestHandler(RateLimiter& rateLimiter);

    // path: e.g. "/check", "/status", "/config"
    // clientKeyHeader: raw value of the client identifier header (may be empty)
    // body: raw request body, only used for /config updates
    // Throws InvalidRequestException on missing/invalid input.
    Response handle(const std::string& path,
                     const std::string& clientKeyHeader,
                     const std::string& body);

private:
    RateLimiter& rateLimiter_;

    std::string extractClientKey(const std::string& clientKeyHeader) const;

    // Parses a simple "capacity=<num>&refillRate=<num>" body for /config.
    Request parseConfigBody(const std::string& clientKey,
                             const std::string& body) const;
};
