#pragma once

#include <mutex>
#include "models/Config.h"

// Thread-safe holder for the current rate-limiting configuration
// (default bucket capacity + refill rate). Read by the core layer
// when creating new client buckets, and updated via the /config endpoint.
class ConfigManager {
public:
    ConfigManager();
    explicit ConfigManager(const Config& initialConfig);

    Config getConfig() const;

    // Validates and applies a new configuration.
    // Throws InvalidRequestException if capacity <= 0
    // or refillRate < 0.
    void updateConfig(const Config& newConfig);

private:
    mutable std::mutex mutex_;
    Config config_;
};
