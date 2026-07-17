#pragma once

#include "models/Bucket.h"

class TokenBucket {
public:
    static void refill(Bucket& bucket);
    static bool tryConsume(Bucket& bucket);
    static double peekAvailableTokens(Bucket& bucket);
    static double getRetryAfterSeconds(Bucket& bucket);
};
