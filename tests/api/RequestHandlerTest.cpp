#include "api/RequestHandler.h"
#include "core/RateLimiter.h"
#include "models/Config.h"
#include "constants/Constants.h"
#include "exceptions/InvalidRequestException.h"
#include <cassert>
#include <iostream>

// --- helpers ---

static RateLimiter makeRateLimiter(double capacity = 5.0, double refillRate = 1.0) {
    return RateLimiter(Config(capacity, refillRate));
}

// --- tests ---

void test_check_allows_first_request() {
    auto rl = makeRateLimiter();
    RequestHandler handler(rl);

    Response res = handler.handle(ENDPOINT_CHECK, "clientA", "");
    assert(res.statusCode == HTTP_OK);
    assert(res.allowed == true);

    std::cout << "test_check_allows_first_request passed\n";
}

void test_check_rate_limits_after_exhaustion() {
    auto rl = makeRateLimiter(1.0, 0.0); // 1 token, no refill
    RequestHandler handler(rl);

    handler.handle(ENDPOINT_CHECK, "clientX", "");           // consumes the 1 token
    Response res = handler.handle(ENDPOINT_CHECK, "clientX", "");
    assert(res.statusCode == HTTP_TOO_MANY_REQUESTS);
    assert(res.allowed == false);

    std::cout << "test_check_rate_limits_after_exhaustion passed\n";
}

void test_status_returns_ok() {
    auto rl = makeRateLimiter();
    RequestHandler handler(rl);

    Response res = handler.handle(ENDPOINT_STATUS, "clientB", "");
    assert(res.statusCode == HTTP_OK);

    std::cout << "test_status_returns_ok passed\n";
}

void test_config_update_succeeds() {
    auto rl = makeRateLimiter();
    RequestHandler handler(rl);

    Response res = handler.handle(ENDPOINT_CONFIG, "clientC", "capacity=20&refillRate=5");
    assert(res.statusCode == HTTP_OK);

    std::cout << "test_config_update_succeeds passed\n";
}

void test_config_invalid_body_throws() {
    auto rl = makeRateLimiter();
    RequestHandler handler(rl);

    bool threw = false;
    try {
        handler.handle(ENDPOINT_CONFIG, "clientD", "capacity=-1&refillRate=1");
    } catch (const InvalidRequestException&) {
        threw = true;
    }
    assert(threw);

    std::cout << "test_config_invalid_body_throws passed\n";
}

void test_missing_client_key_throws() {
    auto rl = makeRateLimiter();
    RequestHandler handler(rl);

    bool threw = false;
    try {
        handler.handle(ENDPOINT_CHECK, "", "");
    } catch (const InvalidRequestException&) {
        threw = true;
    }
    assert(threw);

    std::cout << "test_missing_client_key_throws passed\n";
}

void test_unknown_endpoint_throws() {
    auto rl = makeRateLimiter();
    RequestHandler handler(rl);

    bool threw = false;
    try {
        handler.handle("/unknown", "clientE", "");
    } catch (const InvalidRequestException&) {
        threw = true;
    }
    assert(threw);

    std::cout << "test_unknown_endpoint_throws passed\n";
}

int main() {
    test_check_allows_first_request();
    test_check_rate_limits_after_exhaustion();
    test_status_returns_ok();
    test_config_update_succeeds();
    test_config_invalid_body_throws();
    test_missing_client_key_throws();
    test_unknown_endpoint_throws();

    std::cout << "\nAll API tests passed.\n";
    return 0;
}
