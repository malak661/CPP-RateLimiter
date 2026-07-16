#ifndef CONFIG_CONFIGMANAGER_H
#define CONFIG_CONFIGMANAGER_H

#include <mutex>
#include "models/Config.h"

namespace ratelimiter {
namespace config {

// Thread-safe holder for the current rate-limiting configuration
// (default bucket capacity + refill rate). Read by the core layer
// when creating new client buckets, and updated via the /config endpoint.
class ConfigManager {
public:
    ConfigManager();
    explicit ConfigManager(const models::Config& initialConfig);

    models::Config getConfig() const;

    // Validates and applies a new configuration.
    // Throws exceptions::InvalidRequestException if capacity <= 0
    // or refillRate < 0.
    void updateConfig(const models::Config& newConfig);

private:
    mutable std::mutex mutex_;
    models::Config config_;
};

} // namespace config
} // namespace ratelimiter

#endif // CONFIG_CONFIGMANAGER_H
