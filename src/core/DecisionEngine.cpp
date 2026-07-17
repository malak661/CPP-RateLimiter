#include "core/DecisionEngine.h"
#include "core/TokenBucket.h"

Response DecisionEngine::evaluate(Bucket& bucket) {
    bool allowed = TokenBucket::tryConsume(bucket);

    if (allowed) {
        return Response(200, true, bucket.capacity, bucket.availableTokens, 0.0, "Request allowed.");
    }

    double retryAfter = TokenBucket::getRetryAfterSeconds(bucket);
    return Response(429, false, bucket.capacity, bucket.availableTokens, retryAfter, "Rate limit exceeded.");
}

Response DecisionEngine::statusOf(Bucket& bucket) {
    double tokens = TokenBucket::peekAvailableTokens(bucket);
    return Response(200, true, bucket.capacity, tokens, 0.0, "Status retrieved.");
}
