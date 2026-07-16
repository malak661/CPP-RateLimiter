#pragma once

#include "models/Bucket.h"
#include "models/Response.h"

class DecisionEngine {
public:
    static Response evaluate(Bucket& bucket);
    static Response statusOf(Bucket& bucket);
};
