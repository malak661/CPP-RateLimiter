#pragma once

#include "models/Config.h"
#include "models/Response.h"
#include "storage/BucketStore.h"
#include <string>
#include <mutex>

class RateLimiter {
public:
    explicit RateLimiter(const Config& defaultConfig);

    Response check(const std::string& key);
    Response status(const std::string& key);
    void updateConfig(const Config& newConfig);
    Config getConfig();

private:
    Config _config;
    BucketStore _store;
    std::mutex _mutex;
};
