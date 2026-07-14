#ifndef CORE_TOKENBUCKET_H
#define CORE_TOKENBUCKET_H

#include "models/Bucket.h"

namespace ratelimiter {
namespace core {

// Implements the Token Bucket algorithm on a single Bucket instance:
// lazy refill based on elapsed time, and token consumption.
class TokenBucket {
public:
    // Recalculates available tokens based on time elapsed since lastUpdate,
    // using the bucket's refillRate. Never exceeds bucket.capacity.
    // Updates bucket.lastUpdate to now.
    static void refill(models::Bucket& bucket);

    // Attempts to consume one token from the bucket.
    // Calls refill() first to bring the bucket up to date.
    // Returns true and decrements availableTokens if a token was available,
    // false otherwise (bucket left unchanged aside from the refill).
    static bool tryConsume(models::Bucket& bucket);

    // Returns the current token count after applying a refill,
    // without consuming a token. Useful for the /status endpoint.
    static double peekAvailableTokens(models::Bucket& bucket);

    // Calculates how many seconds the client must wait until at least
    // one token is available. Returns 0 if a token is already available.
    static double getRetryAfterSeconds(models::Bucket& bucket);
};

} // namespace core
} // namespace ratelimiter

#endif // CORE_TOKENBUCKET_H
