#include "storage/BucketStore.h"
#include "core/TokenBucket.h"
#include <algorithm>

Bucket& BucketStore::getOrCreate(const std::string& key, const Config& config) {
    auto [it, inserted] = _store.try_emplace(key, key, config.capacity, config.refillRate);
    return it->second;
}

void BucketStore::updateAll(const Config& config) {
    for (auto& [key, bucket] : _store) {
        TokenBucket::refill(bucket);
        bucket.capacity = config.capacity;
        bucket.refillRate = config.refillRate;
        if (bucket.availableTokens > bucket.capacity) {
            bucket.availableTokens = bucket.capacity;
        }
    }
}
