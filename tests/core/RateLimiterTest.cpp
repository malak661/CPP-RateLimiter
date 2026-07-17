#include "core/RateLimiter.h"
#include "models/Config.h"
#include <cassert>
#include <iostream>
#include <cmath>

static RateLimiter makeRateLimiter(double capacity = 5.0, double refillRate = 1.0) {
    return RateLimiter(Config(capacity, refillRate));
}

void test_rate_limits_after_exhaustion() {
    auto rl = makeRateLimiter(1.0, 0.001);
    rl.check("clientA");
    Response res = rl.check("clientA");
    assert(res.statusCode == 429);
    assert(res.allowed == false);
    std::cout << "test_rate_limits_after_exhaustion passed\n";
}

void test_status_returns_tokens() {
    auto rl = makeRateLimiter(5.0, 0.001);
    rl.check("clientB");
    Response res = rl.status("clientB");
    assert(res.statusCode == 200);
    assert(std::abs(res.remainingTokens - 4.0) < 0.01);
    std::cout << "test_status_returns_tokens passed\n";
}

void test_config_update_takes_effect() {
    auto rl = makeRateLimiter(5.0, 1.0);
    rl.check("clientC");

    rl.updateConfig(Config(2.0, 0.5));
    Config cfg = rl.getConfig();
    assert(cfg.capacity == 2.0);
    assert(cfg.refillRate == 0.5);

    Response statusRes = rl.status("clientC");
    assert(statusRes.limit == 2.0);

    std::cout << "test_config_update_takes_effect passed\n";
}

int main() {
    test_rate_limits_after_exhaustion();
    test_status_returns_tokens();
    test_config_update_takes_effect();

    std::cout << "\nAll RateLimiter tests passed.\n";
    return 0;
}
