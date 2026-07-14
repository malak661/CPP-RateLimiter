#ifndef MODELS_CONFIG_H
#define MODELS_CONFIG_H

namespace ratelimiter {
namespace models {

// Holds the rate-limiting configuration values.
// Used both as the default/global config and for per-request config updates.
struct Config {
    double capacity;     // maximum tokens a bucket can hold
    double refillRate;   // tokens added per second

    Config()
        : capacity(0.0), refillRate(0.0) {}

    Config(double capacity_, double refillRate_)
        : capacity(capacity_), refillRate(refillRate_) {}
};

} // namespace models
} // namespace ratelimiter

#endif // MODELS_CONFIG_H
