#include "core/TokenBucket.h"
#include "models/Bucket.h"
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

using namespace ratelimiter;

void test_initial_bucket_is_full() {
    models::Bucket bucket("client1", 5.0, 1.0);
    assert(bucket.availableTokens == 5.0);
    std::cout << "test_initial_bucket_is_full passed\n";
}

void test_consume_reduces_tokens() {
    models::Bucket bucket("client2", 5.0, 1.0);
    bool result = core::TokenBucket::tryConsume(bucket);
    assert(result == true);
    assert(bucket.availableTokens == 4.0);
    std::cout << "test_consume_reduces_tokens passed\n";
}

void test_consume_fails_when_empty() {
    models::Bucket bucket("client3", 1.0, 0.0); // no refill
    bool first = core::TokenBucket::tryConsume(bucket);
    bool second = core::TokenBucket::tryConsume(bucket);
    assert(first == true);
    assert(second == false);
    std::cout << "test_consume_fails_when_empty passed\n";
}

void test_refill_over_time() {
    models::Bucket bucket("client4", 5.0, 5.0); // 5 tokens/sec
    while (core::TokenBucket::tryConsume(bucket)) {} // drain it
    assert(bucket.availableTokens < 1.0);

    std::this_thread::sleep_for(std::chrono::milliseconds(300)); // ~1.5 tokens
    double tokens = core::TokenBucket::peekAvailableTokens(bucket);
    assert(tokens > 1.0);
    std::cout << "test_refill_over_time passed\n";
}

void test_never_exceeds_capacity() {
    models::Bucket bucket("client5", 3.0, 100.0); // fast refill
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    double tokens = core::TokenBucket::peekAvailableTokens(bucket);
    assert(tokens <= 3.0);
    std::cout << "test_never_exceeds_capacity passed\n";
}

void test_retry_after_seconds() {
    models::Bucket bucket("client6", 1.0, 1.0); // 1 token/sec
    core::TokenBucket::tryConsume(bucket); // drain the single token
    double retryAfter = core::TokenBucket::getRetryAfterSeconds(bucket);
    assert(retryAfter > 0.0 && retryAfter <= 1.0);
    std::cout << "test_retry_after_seconds passed\n";
}

int main() {
    test_initial_bucket_is_full();
    test_consume_reduces_tokens();
    test_consume_fails_when_empty();
    test_refill_over_time();
    test_never_exceeds_capacity();
    test_retry_after_seconds();

    std::cout << "\nAll TokenBucket tests passed.\n";
    return 0;
}
