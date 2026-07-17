#pragma once

#include "models/Bucket.h"
#include "models/Config.h"
#include <string>
#include <unordered_map>

class BucketStore {
public:
    Bucket& getOrCreate(const std::string& key, const Config& config);

private:
    std::unordered_map<std::string, Bucket> _store;
};
