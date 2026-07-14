#pragma once

#include "models/Bucket.h"
#include "models/Config.h"
#include <string>
#include <unordered_map>

class BucketStore {
public:
    ratelimiter::models::Bucket& create(const std::string& key,
                                        const ratelimiter::models::Config& config);

    ratelimiter::models::Bucket* find(const std::string& key);

private:
    std::unordered_map<std::string, ratelimiter::models::Bucket> _store;
};
