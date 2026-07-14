#include "storage/BucketStore.h"

using namespace ratelimiter;

models::Bucket& BucketStore::create(const std::string& key,
                                     const models::Config& config) {
    _store.emplace(key, models::Bucket(key, config.capacity, config.refillRate));
    return _store.at(key);
}

models::Bucket* BucketStore::find(const std::string& key) {
    auto it = _store.find(key);
    if (it == _store.end()) return nullptr;
    return &it->second;
}
