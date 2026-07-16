#include "config/ConfigManager.h"
#include "exceptions/InvalidRequestException.h"
#include "constants/Constants.h"

namespace ratelimiter {
namespace config {

ConfigManager::ConfigManager()
    : config_(constants::DEFAULT_BUCKET_CAPACITY, constants::DEFAULT_REFILL_RATE) {}

ConfigManager::ConfigManager(const models::Config& initialConfig)
    : config_(initialConfig) {}

models::Config ConfigManager::getConfig() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return config_;
}

void ConfigManager::updateConfig(const models::Config& newConfig) {
    if (newConfig.capacity <= 0.0 || newConfig.refillRate < 0.0) {
        throw exceptions::InvalidRequestException(
            "Invalid config: capacity must be > 0 and refillRate must be >= 0"
        );
    }

    std::lock_guard<std::mutex> lock(mutex_);
    config_ = newConfig;
}

} // namespace config
} // namespace ratelimiter
