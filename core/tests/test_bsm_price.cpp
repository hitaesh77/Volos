#include <cmath>
#include <cstdlib>
#include <iostream>

#include "bsm/bsm_price.h"
#include "types.h"
#include "enums.h"

namespace {

void assert_near(double actual, double expected, double tolerance, const char* test_name) {
    if (std::fabs(actual - expected) > tolerance) {
        std::cerr << "FAIL: " << test_name << "\n"
                  << "  expected: " << expected << "\n"
                  << "  actual:   " << actual << "\n"
                  << "  diff:     " << std::fabs(actual - expected) << "\n";
        std::exit(1);
    }
}

void assert_true(bool condition, const char* test_name) {
    if (!condition) {
        std::cerr << "FAIL: " << test_name << "\n";
        std::exit(1);
    }
}

volos::OptionInput make_default_input() {
    volos::OptionInput input{};
    input.S = 100.0;
    input.K = 100.0;
    input.T = 1.0;
    input.r = 0.05;
    input.q = 0.0;
    input.sigma = 0.2;
    return input;
}

}  // namespace

void run_bsm_price_tests() {
    std::cout << "Running BSM price tests...\n";

    {
        auto input = make_default_input();

        const double call = volos::bsm::price(input, volos::OptionType::Call);
        const double put = volos::bsm::price(input, volos::OptionType::Put);

        assert_near(call, 10.4506, 1e-3, "ATM European call price");
        assert_near(put, 5.5735, 1e-3, "ATM European put price");
    }

    {
        auto input = make_default_input();

        const double call = volos::bsm::price(input, volos::OptionType::Call);
        const double put = volos::bsm::price(input, volos::OptionType::Put);

        const double lhs = call - put;
        const double rhs = input.S * std::exp(-input.q * input.T)
                         - input.K * std::exp(-input.r * input.T);

        assert_near(lhs, rhs, 1e-6, "Put-call parity with q = 0");
    }

    {
        auto input = make_default_input();
        input.q = 0.02;

        const double call = volos::bsm::price(input, volos::OptionType::Call);
        const double put = volos::bsm::price(input, volos::OptionType::Put);

        const double lhs = call - put;
        const double rhs = input.S * std::exp(-input.q * input.T)
                         - input.K * std::exp(-input.r * input.T);

        assert_near(lhs, rhs, 1e-6, "Put-call parity with dividend yield");
    }

    {
        auto low_spot = make_default_input();
        auto high_spot = make_default_input();

        low_spot.S = 90.0;
        high_spot.S = 110.0;

        const double low_call = volos::bsm::price(low_spot, volos::OptionType::Call);
        const double high_call = volos::bsm::price(high_spot, volos::OptionType::Call);

        assert_true(high_call > low_call, "Call price increases as spot increases");
    }

    {
        auto low_vol = make_default_input();
        auto high_vol = make_default_input();

        low_vol.sigma = 0.1;
        high_vol.sigma = 0.4;

        const double low_call = volos::bsm::price(low_vol, volos::OptionType::Call);
        const double high_call = volos::bsm::price(high_vol, volos::OptionType::Call);

        assert_true(high_call > low_call, "Call price increases as volatility increases");
    }

    {
        constexpr std::size_t n = 3;

        double S[n] = {100.0, 105.0, 110.0};
        double K[n] = {100.0, 100.0, 100.0};
        double T[n] = {1.0, 1.0, 1.0};
        double sigma[n] = {0.2, 0.25, 0.3};
        double out[n] = {0.0, 0.0, 0.0};

        volos::OptionInputBatch batch{};
        batch.S = S;
        batch.K = K;
        batch.T = T;
        batch.sigma = sigma;
        batch.r = 0.05;
        batch.q = 0.0;
        batch.n = n;

        volos::bsm::price_batch(batch, volos::OptionType::Call, out);

        for (std::size_t i = 0; i < n; ++i) {
            volos::OptionInput scalar{};
            scalar.S = S[i];
            scalar.K = K[i];
            scalar.T = T[i];
            scalar.r = batch.r;
            scalar.q = batch.q;
            scalar.sigma = sigma[i];

            const double expected = volos::bsm::price(scalar, volos::OptionType::Call);
            assert_near(out[i], expected, 1e-12, "Batch price matches scalar price");
        }
    }

    std::cout << "BSM price tests passed.\n";
}