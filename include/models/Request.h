#pragma once

#include <string>

// Represents an incoming client request after basic parsing,
// before it reaches the core rate-limiting logic.
struct Request {
    std::string clientKey;   // identifier extracted from the request
    std::string endpoint;    // e.g. "/check", "/status", "/config"

    // Optional fields used only for /config requests.
    bool hasConfigUpdate;
    double newCapacity;
    double newRefillRate;

    Request()
        : clientKey(""),
          endpoint(""),
          hasConfigUpdate(false),
          newCapacity(0.0),
          newRefillRate(0.0) {}

    Request(const std::string& key, const std::string& endpoint_)
        : clientKey(key),
          endpoint(endpoint_),
          hasConfigUpdate(false),
          newCapacity(0.0),
          newRefillRate(0.0) {}
};
