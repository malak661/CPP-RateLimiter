#include "storage/BucketStore.h"

Bucket& BucketStore::create(const std::string& key,
                             const Config& config) {
    auto [it, _] = _store.emplace(key, Bucket(key, config.capacity, config.refillRate));
    return it->second;
}

Bucket* BucketStore::find(const std::string& key) {
    auto it = _store.find(key);
    if (it == _store.end()) return nullptr;
    return &it->second;
}
