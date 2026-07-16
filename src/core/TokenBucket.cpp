#include "core/TokenBucket.h"
#include <algorithm>
#include <chrono>

using Clock = std::chrono::steady_clock;

void TokenBucket::refill(Bucket& bucket) {
    Clock::time_point now = Clock::now();

    std::chrono::duration<double> elapsed = now - bucket.lastUpdate;
    double elapsedSeconds = elapsed.count();

    if (elapsedSeconds > 0.0) {
        double tokensToAdd = elapsedSeconds * bucket.refillRate;
        bucket.availableTokens = std::min(
            bucket.capacity,
            bucket.availableTokens + tokensToAdd
        );
    }

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
        // Bucket will never refill; caller should treat this as blocked indefinitely.
        return -1.0;
    }

    double tokensNeeded = 1.0 - bucket.availableTokens;
    return tokensNeeded / bucket.refillRate;
}
