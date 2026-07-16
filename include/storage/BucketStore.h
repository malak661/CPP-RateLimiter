#pragma once

#include "models/Bucket.h"
#include "models/Config.h"
#include <string>
#include <unordered_map>

class BucketStore {
public:
    Bucket& create(const std::string& key,
                   const Config& config);

    Bucket* find(const std::string& key);

private:
    std::unordered_map<std::string, Bucket> _store;
};
