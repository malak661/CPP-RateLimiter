#include "core/TokenBucket.h"
#include "models/Bucket.h"
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

void test_consume_reduces_tokens() {
    Bucket bucket("client1", 5.0, 1.0);
    bool result = TokenBucket::tryConsume(bucket);
    assert(result == true);
    assert(bucket.availableTokens == 4.0);
    std::cout << "test_consume_reduces_tokens passed\n";
}

void test_consume_fails_when_empty() {
    Bucket bucket("client2", 1.0, 0.001);
    bool first = TokenBucket::tryConsume(bucket);
    bool second = TokenBucket::tryConsume(bucket);
    assert(first == true);
    assert(second == false);
    std::cout << "test_consume_fails_when_empty passed\n";
}

void test_refill_over_time() {
    Bucket bucket("client3", 5.0, 5.0);
    while (TokenBucket::tryConsume(bucket)) {}
    assert(bucket.availableTokens < 1.0);

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    double tokens = TokenBucket::peekAvailableTokens(bucket);
    assert(tokens > 1.0);
    std::cout << "test_refill_over_time passed\n";
}

void test_never_exceeds_capacity() {
    Bucket bucket("client4", 3.0, 100.0);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    double tokens = TokenBucket::peekAvailableTokens(bucket);
    assert(tokens <= 3.0);
    std::cout << "test_never_exceeds_capacity passed\n";
}

void test_retry_after_seconds() {
    Bucket bucket("client5", 1.0, 1.0);
    TokenBucket::tryConsume(bucket);
    double retryAfter = TokenBucket::getRetryAfterSeconds(bucket);
    assert(retryAfter > 0.0 && retryAfter <= 1.0);
    std::cout << "test_retry_after_seconds passed\n";
}

int main() {
    test_consume_reduces_tokens();
    test_consume_fails_when_empty();
    test_refill_over_time();
    test_never_exceeds_capacity();
    test_retry_after_seconds();

    std::cout << "\nAll TokenBucket tests passed.\n";
    return 0;
}
