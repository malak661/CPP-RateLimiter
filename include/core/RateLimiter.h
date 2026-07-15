#pragma once

#include "models/Config.h"
#include "models/Response.h"
#include "storage/BucketStore.h"
#include <string>
#include <mutex>

class RateLimiter {
public:
    explicit RateLimiter(const ratelimiter::models::Config& defaultConfig);

    ratelimiter::models::Response check(const std::string& key);
    ratelimiter::models::Response status(const std::string& key);
    void updateConfig(const ratelimiter::models::Config& newConfig);
    ratelimiter::models::Config getConfig();

private:
    ratelimiter::models::Config _config;
    BucketStore _store;
    std::mutex _mutex;
};
