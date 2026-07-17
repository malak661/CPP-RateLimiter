#include "storage/BucketStore.h"
#include "models/Config.h"
#include <cassert>
#include <iostream>

void test_creates_bucket_for_new_key() {
    BucketStore store;
    Config config(10.0, 2.0);

    Bucket& bucket = store.getOrCreate("clientA", config);
    assert(bucket.clientKey == "clientA");
    assert(bucket.capacity == 10.0);
    assert(bucket.availableTokens == 10.0);

    std::cout << "test_creates_bucket_for_new_key passed\n";
}

void test_returns_same_bucket_on_second_call() {
    BucketStore store;
    Config config(10.0, 2.0);

    Bucket& first = store.getOrCreate("clientB", config);
    first.availableTokens = 3.0;

    Bucket& second = store.getOrCreate("clientB", config);
    assert(second.availableTokens == 3.0);

    std::cout << "test_returns_same_bucket_on_second_call passed\n";
}

void test_different_clients_have_independent_buckets() {
    BucketStore store;
    Config config(5.0, 1.0);

    Bucket& bucketA = store.getOrCreate("clientC", config);
    Bucket& bucketB = store.getOrCreate("clientD", config);

    bucketA.availableTokens = 1.0;
    assert(bucketB.availableTokens == 5.0);

    std::cout << "test_different_clients_have_independent_buckets passed\n";
}

int main() {
    test_creates_bucket_for_new_key();
    test_returns_same_bucket_on_second_call();
    test_different_clients_have_independent_buckets();

    std::cout << "\nAll BucketStore tests passed.\n";
    return 0;
}
