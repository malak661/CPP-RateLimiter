#ifndef API_REQUESTHANDLER_H
#define API_REQUESTHANDLER_H

#include <string>
#include "models/Request.h"
#include "models/Response.h"
#include "core/RateLimiter.h"

// NOTE ON DEPENDENCY:
// This file assumes core::RateLimiter exposes the following interface
// (this is your teammate's Day 1 deliverable):
//
//   class RateLimiter {
//   public:
//       models::Response checkRequest(const models::Request& request);
//       models::Response getStatus(const models::Request& request);
//       models::Response updateConfig(const models::Request& request);
//   };
//
// If the real signatures differ, update the calls in RequestHandler.cpp
// to match.

namespace ratelimiter {
namespace api {

// Sits between the raw HTTP layer (HttpServer) and the core rate-limiting
// logic. Responsible for validating incoming data, extracting the client
// key, building a models::Request, and turning the core's decision into
// a models::Response.
class RequestHandler {
public:
    explicit RequestHandler(core::RateLimiter& rateLimiter);

    // path: e.g. "/check", "/status", "/config"
    // clientKeyHeader: raw value of the client identifier header/param (may be empty)
    // body: raw request body, only used for /config updates
    // Throws exceptions::InvalidRequestException on missing/invalid input.
    models::Response handle(const std::string& path,
                             const std::string& clientKeyHeader,
                             const std::string& body);

private:
    core::RateLimiter& rateLimiter_;

    std::string extractClientKey(const std::string& clientKeyHeader) const;

    // Parses a simple "capacity=<num>&refillRate=<num>" body for /config.
    models::Request parseConfigBody(const std::string& clientKey,
                                     const std::string& body) const;
};

} // namespace api
} // namespace ratelimiter

#endif // API_REQUESTHANDLER_H
