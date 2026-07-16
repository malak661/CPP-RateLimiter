#include "core/RateLimiter.h"
#include "core/DecisionEngine.h"

RateLimiter::RateLimiter(const Config& defaultConfig)
    : _config(defaultConfig) {}

Response RateLimiter::check(const std::string& key) {
    std::lock_guard<std::mutex> lock(_mutex);
    Bucket* bucket = _store.find(key);
    if (!bucket) bucket = &_store.create(key, _config);
    return DecisionEngine::evaluate(*bucket);
}

Response RateLimiter::status(const std::string& key) {
    std::lock_guard<std::mutex> lock(_mutex);
    Bucket* bucket = _store.find(key);
    if (!bucket) bucket = &_store.create(key, _config);
    return DecisionEngine::statusOf(*bucket);
}

void RateLimiter::updateConfig(const Config& newConfig) {
    std::lock_guard<std::mutex> lock(_mutex);
    _config = newConfig;
}

Config RateLimiter::getConfig() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _config;
}
