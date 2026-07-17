#include "core/TokenBucket.h"
#include <algorithm>
#include <chrono>

using Clock = std::chrono::steady_clock;

void TokenBucket::refill(Bucket& bucket) {
    Clock::time_point now = Clock::now();
    double elapsed = std::chrono::duration<double>(now - bucket.lastUpdate).count();

    bucket.availableTokens = std::min(
        bucket.capacity,
        bucket.availableTokens + elapsed * bucket.refillRate
    );
    bucket.lastUpdate = now;
}

bool TokenBucket::tryConsume(Bucket& bucket) {
    refill(bucket);

    if (bucket.availableTokens >= 1.0) {
        bucket.availableTokens -= 1.0;
        return true;
    }

    return false;
}

double TokenBucket::peekAvailableTokens(Bucket& bucket) {
    refill(bucket);
    return bucket.availableTokens;
}

double TokenBucket::getRetryAfterSeconds(Bucket& bucket) {
    refill(bucket);

    if (bucket.availableTokens >= 1.0) {
        return 0.0;
    }

    if (bucket.refillRate <= 0.0) {
        return 0.0;
    }

    return (1.0 - bucket.availableTokens) / bucket.refillRate;
}
