#include "core/RateLimiter.h"
#include "core/DecisionEngine.h"

using namespace ratelimiter;

RateLimiter::RateLimiter(const models::Config& defaultConfig)
    : _config(defaultConfig) {}

models::Response RateLimiter::check(const std::string& key) {
    std::lock_guard<std::mutex> lock(_mutex);
    models::Bucket* bucket = _store.find(key);
    if (!bucket) bucket = &_store.create(key, _config);
    return DecisionEngine::evaluate(*bucket);
}

models::Response RateLimiter::status(const std::string& key) {
    std::lock_guard<std::mutex> lock(_mutex);
    models::Bucket* bucket = _store.find(key);
    if (!bucket) bucket = &_store.create(key, _config);
    return DecisionEngine::statusOf(*bucket);
}

void RateLimiter::updateConfig(const models::Config& newConfig) {
    std::lock_guard<std::mutex> lock(_mutex);
    _config = newConfig;
}

models::Config RateLimiter::getConfig() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _config;
}
