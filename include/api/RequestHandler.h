#pragma once

#include <string>
#include "models/Request.h"
#include "models/Response.h"
#include "core/RateLimiter.h"

// NOTE ON DEPENDENCY:
// This file assumes RateLimiter exposes the following interface
// (this is your teammate's Day 1 deliverable):
//
//   class RateLimiter {
//   public:
//       Response checkRequest(const Request& request);
//       Response getStatus(const Request& request);
//       Response updateConfig(const Request& request);
//   };
//
// If the real signatures differ, update the calls in RequestHandler.cpp
// to match.

// Sits between the raw HTTP layer (HttpServer) and the core rate-limiting
// logic. Responsible for validating incoming data, extracting the client
// key, building a Request, and turning the core's decision into
// a Response.
class RequestHandler {
public:
    explicit RequestHandler(RateLimiter& rateLimiter);

    // path: e.g. "/check", "/status", "/config"
    // clientKeyHeader: raw value of the client identifier header/param (may be empty)
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
