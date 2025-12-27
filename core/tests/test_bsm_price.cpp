#include <gtest/gtest.h>
#include "bsm/bsm_price.h"

// ATM Call
TEST(BSMPriceTest, CallPriceATM) {
	volos::OptionInput input{100.0, 100.0, 1.0, 0.05, 0.0, 0.2};
	double p = volos::bsm::price(input, volos::OptionType::Call);

	EXPECT_NEAR(p, 10.4506, 0.001);
	EXPECT_GT(p, 0.0);
}

// ATM Put
TEST(BSMPriceTest, PutPriceATM) {
	volos::OptionInput input{100.0, 100.0, 1.0, 0.05, 0.0, 0.2};
	double p = volos::bsm::price(input, volos::OptionType::Put);

	EXPECT_NEAR(p, 5.5735, 0.001);
	EXPECT_GE(p, 0.0);
}

// Batch pricing matches individual calls
TEST(BSMPriceTest, BatchMatchesIndividual) {
	double S[2] = {100.0, 110.0};
	double K[2] = {100.0, 100.0};
	double T[2] = {1.0, 1.0};
	double sigma[2] = {0.2, 0.2};

	volos::OptionInputBatch batch{S, K, T, sigma, 2, 0.05, 0.0};
	double results[2];

	volos::bsm::price_batch(batch, volos::OptionType::Call, results);

	volos::OptionInput a{S[0], K[0], T[0], 0.05, 0.0, sigma[0]};
	volos::OptionInput b{S[1], K[1], T[1], 0.05, 0.0, sigma[1]};

	EXPECT_NEAR(results[0], volos::bsm::price(a, volos::OptionType::Call), 1e-12);
	EXPECT_NEAR(results[1], volos::bsm::price(b, volos::OptionType::Call), 1e-12);
}

// Put-Call parity: C - P = S - K * exp(-rT)
TEST(BSMPriceTest, PutCallParity) {
	volos::OptionInput input{120.0, 100.0, 0.5, 0.03, 0.0, 0.25};

	double C = volos::bsm::price(input, volos::OptionType::Call);
	double P = volos::bsm::price(input, volos::OptionType::Put);

	double rhs = input.S - (input.K * std::exp(-input.r * input.T));
	EXPECT_NEAR(C - P, rhs, 1e-8);
}

// Price monotonicity: price increases with spot S
TEST(BSMPriceTest, MonotonicityWithSpot) {
	volos::OptionInput low{90.0, 100.0, 1.0, 0.05, 0.0, 0.2};
	volos::OptionInput high{110.0, 100.0, 1.0, 0.05, 0.0, 0.2};

	double p_low = volos::bsm::price(low, volos::OptionType::Call);
	double p_high = volos::bsm::price(high, volos::OptionType::Call);

	EXPECT_GT(p_high, p_low);
}

// Price increases with volatility (call)
TEST(BSMPriceTest, MonotonicityWithVolatility) {
	volos::OptionInput lowSigma{100.0, 100.0, 1.0, 0.05, 0.0, 0.1};
	volos::OptionInput highSigma{100.0, 100.0, 1.0, 0.05, 0.0, 0.5};

	double p_low = volos::bsm::price(lowSigma, volos::OptionType::Call);
	double p_high = volos::bsm::price(highSigma, volos::OptionType::Call);

	EXPECT_GT(p_high, p_low);
}

// Deep ITM call respects lower bound: C >= S - K*exp(-rT)
TEST(BSMPriceTest, DeepITMCallLowerBound) {
	volos::OptionInput input{150.0, 100.0, 1.0, 0.05, 0.0, 0.2};

	double C = volos::bsm::price(input, volos::OptionType::Call);
	double lower = input.S - (input.K * std::exp(-input.r * input.T));

	EXPECT_GE(C + 1e-12, lower);
}
