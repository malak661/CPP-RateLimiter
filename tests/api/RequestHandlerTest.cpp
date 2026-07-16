#include "core/RateLimiter.h"
#include "models/Config.h"
#include <cassert>
#include <iostream>

static RateLimiter makeRateLimiter(double capacity = 5.0, double refillRate = 1.0) {
    return RateLimiter(Config(capacity, refillRate));
}

void test_check_allows_first_request() {
    auto rl = makeRateLimiter();
    Response res = rl.check("clientA");
    assert(res.statusCode == 200);
    assert(res.allowed == true);
    std::cout << "test_check_allows_first_request passed\n";
}

void test_check_rate_limits_after_exhaustion() {
    auto rl = makeRateLimiter(1.0, 0.0);
    rl.check("clientX");
    Response res = rl.check("clientX");
    assert(res.statusCode == 429);
    assert(res.allowed == false);
    std::cout << "test_check_rate_limits_after_exhaustion passed\n";
}

void test_status_returns_tokens() {
    auto rl = makeRateLimiter(5.0, 0.0);
    rl.check("clientB");
    Response res = rl.status("clientB");
    assert(res.statusCode == 200);
    assert(res.remainingTokens == 4.0);
    std::cout << "test_status_returns_tokens passed\n";
}

void test_config_update_takes_effect() {
    auto rl = makeRateLimiter(5.0, 1.0);
    rl.updateConfig(Config(20.0, 2.0));
    Config cfg = rl.getConfig();
    assert(cfg.capacity == 20.0);
    assert(cfg.refillRate == 2.0);
    std::cout << "test_config_update_takes_effect passed\n";
}

void test_different_clients_are_independent() {
    auto rl = makeRateLimiter(1.0, 0.0);
    rl.check("clientC");
    Response resC = rl.check("clientC");
    Response resD = rl.check("clientD");
    assert(resC.allowed == false);
    assert(resD.allowed == true);
    std::cout << "test_different_clients_are_independent passed\n";
}

int main() {
    test_check_allows_first_request();
    test_check_rate_limits_after_exhaustion();
    test_status_returns_tokens();
    test_config_update_takes_effect();
    test_different_clients_are_independent();

    std::cout << "\nAll RateLimiter integration tests passed.\n";
    return 0;
}
