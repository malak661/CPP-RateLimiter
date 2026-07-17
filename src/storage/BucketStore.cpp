#include "storage/BucketStore.h"

Bucket& BucketStore::getOrCreate(const std::string& key, const Config& config) {
    auto [it, inserted] = _store.try_emplace(key, key, config.capacity, config.refillRate);
    return it->second;
}
