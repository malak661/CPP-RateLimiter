#include "storage/BucketStore.h"
#include "models/Bucket.h"
#include "models/Config.h"
#include <cassert>
#include <iostream>

void test_creates_new_bucket_if_not_exists() {
    BucketStore store;
    Config config(10.0, 2.0);

    assert(store.find("clientA") == nullptr);

    Bucket& bucket = store.create("clientA", config);
    assert(bucket.clientKey == "clientA");
    assert(bucket.capacity == 10.0);
    assert(store.find("clientA") != nullptr);

    std::cout << "test_creates_new_bucket_if_not_exists passed\n";
}

void test_returns_same_bucket_on_second_call() {
    BucketStore store;
    Config config(10.0, 2.0);

    Bucket& first = store.create("clientB", config);
    first.availableTokens = 3.0; // simulate prior consumption

    Bucket* second = store.find("clientB");
    assert(second != nullptr);
    assert(second->availableTokens == 3.0);

    std::cout << "test_returns_same_bucket_on_second_call passed\n";
}

void test_different_clients_have_independent_buckets() {
    BucketStore store;
    Config config(5.0, 1.0);

    Bucket& bucketA = store.create("clientC", config);
    Bucket& bucketB = store.create("clientD", config);

    bucketA.availableTokens = 1.0;
    assert(bucketB.availableTokens == 5.0);

    std::cout << "test_different_clients_have_independent_buckets passed\n";
}

int main() {
    test_creates_new_bucket_if_not_exists();
    test_returns_same_bucket_on_second_call();
    test_different_clients_have_independent_buckets();

    std::cout << "\nAll BucketStore tests passed.\n";
    return 0;
}
