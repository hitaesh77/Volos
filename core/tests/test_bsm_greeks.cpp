#include <gtest/gtest.h>
#include "bsm/bsm_greeks.h"

// Test ATM (At-The-Money) Call
TEST(BSMGreeksTest, CallPriceTest) {
    volos::OptionInput input{100.0, 100.0, 1.0, 0.05, 0.0, 0.2};
    auto res = volos::bsm::greeks(input, volos::OptionType::Call);

    // Expected: ~10.45
    EXPECT_NEAR(res.price, 10.4506, 0.001);
    // Expected Delta: ~0.637
    EXPECT_NEAR(res.delta, 0.6368, 0.001);
}

// Test ATM (At-The-Money) Put
TEST(BSMGreeksTest, PutPriceTest) {
    volos::OptionInput input{100.0, 100.0, 1.0, 0.05, 0.0, 0.2};
    auto res = volos::bsm::greeks(input, volos::OptionType::Put);

    // Expected: ~5.57
    EXPECT_NEAR(res.price, 5.5735, 0.001);
    // Expected Delta: ~ -0.363
    EXPECT_NEAR(res.delta, -0.3632, 0.001);
}

// Test Batch Processing
TEST(BSMGreeksTest, BatchTest) {
    double S[2] = {100.0, 100.0};
    double K[2] = {100.0, 100.0};
    double T[2] = {1.0, 1.0};
    double sigma[2] = {0.2, 0.2};

    volos::OptionInputBatch batch{S, K, T, sigma, 2, 0.05, 0.0};
    volos::GreeksResult results[2];

    volos::bsm::greeks_batch(batch, volos::OptionType::Call, results);

    EXPECT_NEAR(results[0].price, 10.4506, 0.001);
    EXPECT_NEAR(results[1].price, 10.4506, 0.001);
}