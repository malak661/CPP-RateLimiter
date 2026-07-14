#include "core/DecisionEngine.h"
#include "core/TokenBucket.h"
#include "constants/Constants.h"

using namespace ratelimiter;

models::Response DecisionEngine::evaluate(models::Bucket& bucket) {
    bool allowed = core::TokenBucket::tryConsume(bucket);

    if (allowed) {
        return models::Response(
            constants::HTTP_OK,
            true,
            bucket.availableTokens,
            0.0,
            "Request allowed."
        );
    }

    double retryAfter = core::TokenBucket::getRetryAfterSeconds(bucket);
    return models::Response(
        constants::HTTP_TOO_MANY_REQUESTS,
        false,
        bucket.availableTokens,
        retryAfter,
        "Rate limit exceeded."
    );
}

models::Response DecisionEngine::statusOf(models::Bucket& bucket) {
    double tokens = core::TokenBucket::peekAvailableTokens(bucket);
    return models::Response(
        constants::HTTP_OK,
        true,
        tokens,
        0.0,
        "Status retrieved."
    );
}
