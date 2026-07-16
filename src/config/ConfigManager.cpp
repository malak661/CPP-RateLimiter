#include "config/ConfigManager.h"
#include "exceptions/InvalidRequestException.h"
#include "constants/Constants.h"

ConfigManager::ConfigManager()
    : config_(DEFAULT_BUCKET_CAPACITY, DEFAULT_REFILL_RATE) {}

ConfigManager::ConfigManager(const Config& initialConfig)
    : config_(initialConfig) {}

Config ConfigManager::getConfig() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return config_;
}

void ConfigManager::updateConfig(const Config& newConfig) {
    if (newConfig.capacity <= 0.0 || newConfig.refillRate < 0.0) {
        throw InvalidRequestException(
            "Invalid config: capacity must be > 0 and refillRate must be >= 0"
        );
    }

    std::lock_guard<std::mutex> lock(mutex_);
    config_ = newConfig;
}
