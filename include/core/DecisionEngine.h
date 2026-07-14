#pragma once

#include "models/Bucket.h"
#include "models/Response.h"

class DecisionEngine {
public:
    static ratelimiter::models::Response evaluate(ratelimiter::models::Bucket& bucket);
    static ratelimiter::models::Response statusOf(ratelimiter::models::Bucket& bucket);
};
