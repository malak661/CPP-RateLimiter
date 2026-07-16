#pragma once

#include <string>
#include <chrono>

// Represents a single client's token bucket state.
struct Bucket {
    std::string clientKey;

    double availableTokens;   // current number of tokens available
    double capacity;          // max tokens the bucket can hold
    double refillRate;        // tokens added per second

    // Timestamp of the last time this bucket was refilled/updated.
    std::chrono::steady_clock::time_point lastUpdate;

    Bucket()
        : clientKey(""),
          availableTokens(0.0),
          capacity(0.0),
          refillRate(0.0),
          lastUpdate(std::chrono::steady_clock::now()) {}

    Bucket(const std::string& key, double capacity_, double refillRate_)
        : clientKey(key),
          availableTokens(capacity_),   // start full
          capacity(capacity_),
          refillRate(refillRate_),
          lastUpdate(std::chrono::steady_clock::now()) {}
};
