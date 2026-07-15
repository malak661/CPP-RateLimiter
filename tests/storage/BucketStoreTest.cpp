// NOTE ON DEPENDENCY:
// This test assumes storage::BucketStore exposes the following interface
// (your teammate's Day 1 deliverable):
//
//   class BucketStore {
//   public:
//       models::Bucket& getOrCreate(const std::string& clientKey,
//                                   double capacity, double refillRate);
//       bool exists(const std::string& clientKey) const;
//   };
//
// If the real method names/signatures differ, update the calls below
// to match the actual BucketStore.h.

#include "storage/BucketStore.h"
#include "models/Bucket.h"
#include <cassert>
#include <iostream>

using namespace ratelimiter;

void test_creates_new_bucket_if_not_exists() {
    storage::BucketStore store;
    assert(store.exists("clientA") == false);

    models::Bucket& bucket = store.getOrCreate("clientA", 10.0, 2.0);
    assert(bucket.clientKey == "clientA");
    assert(bucket.capacity == 10.0);
    assert(store.exists("clientA") == true);

    std::cout << "test_creates_new_bucket_if_not_exists passed\n";
}

void test_returns_same_bucket_on_second_call() {
    storage::BucketStore store;
    models::Bucket& first = store.getOrCreate("clientB", 10.0, 2.0);
    first.availableTokens = 3.0; // simulate prior consumption

    models::Bucket& second = store.getOrCreate("clientB", 10.0, 2.0);
    assert(second.availableTokens == 3.0);

    std::cout << "test_returns_same_bucket_on_second_call passed\n";
}

void test_different_clients_have_independent_buckets() {
    storage::BucketStore store;
    models::Bucket& bucketA = store.getOrCreate("clientC", 5.0, 1.0);
    models::Bucket& bucketB = store.getOrCreate("clientD", 5.0, 1.0);

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
