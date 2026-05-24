#include <cmath>
#include <cstdlib>
#include <iostream>

#include "bsm/bsm_greeks.h"
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

void run_bsm_greeks_tests() {
    std::cout << "Running BSM Greeks tests...\n";

    {
        auto input = make_default_input();

        const volos::GreeksResult call = volos::bsm::greeks(input, volos::OptionType::Call);

        assert_near(call.price, 10.4506, 1e-3, "Call Greeks result price");
        assert_near(call.delta, 0.6368, 1e-3, "Call delta");
        assert_near(call.gamma, 0.0188, 1e-3, "Call gamma");
        assert_near(call.vega, 37.5240, 1e-2, "Call vega");
        assert_near(call.theta, -6.4140, 1e-2, "Call theta");
        assert_near(call.rho, 53.2325, 1e-2, "Call rho");
    }

    {
        auto input = make_default_input();

        const volos::GreeksResult put = volos::bsm::greeks(input, volos::OptionType::Put);

        assert_near(put.price, 5.5735, 1e-3, "Put Greeks result price");
        assert_near(put.delta, -0.3632, 1e-3, "Put delta");
        assert_near(put.gamma, 0.0188, 1e-3, "Put gamma");
        assert_near(put.vega, 37.5240, 1e-2, "Put vega");
        assert_near(put.theta, -1.6579, 1e-2, "Put theta");
        assert_near(put.rho, -41.8905, 1e-2, "Put rho");
    }

    {
        auto input = make_default_input();

        const volos::GreeksResult call = volos::bsm::greeks(input, volos::OptionType::Call);
        const volos::GreeksResult put = volos::bsm::greeks(input, volos::OptionType::Put);

        assert_true(call.delta > 0.0 && call.delta < 1.0, "Call delta is between 0 and 1");
        assert_true(put.delta > -1.0 && put.delta < 0.0, "Put delta is between -1 and 0");
        assert_true(call.gamma > 0.0, "Call gamma is positive");
        assert_true(put.gamma > 0.0, "Put gamma is positive");
        assert_true(call.vega > 0.0, "Call vega is positive");
        assert_true(put.vega > 0.0, "Put vega is positive");
    }

    {
        auto input = make_default_input();

        const volos::GreeksResult call = volos::bsm::greeks(input, volos::OptionType::Call);
        const volos::GreeksResult put = volos::bsm::greeks(input, volos::OptionType::Put);

        const double expected_delta_diff = std::exp(-input.q * input.T);
        const double actual_delta_diff = call.delta - put.delta;

        assert_near(actual_delta_diff, expected_delta_diff, 1e-6, "Call-put delta parity");
    }

    {
        auto input = make_default_input();
        input.q = 0.02;

        const volos::GreeksResult call = volos::bsm::greeks(input, volos::OptionType::Call);
        const volos::GreeksResult put = volos::bsm::greeks(input, volos::OptionType::Put);

        const double expected_delta_diff = std::exp(-input.q * input.T);
        const double actual_delta_diff = call.delta - put.delta;

        assert_near(actual_delta_diff, expected_delta_diff, 1e-6, "Call-put delta parity with q");
    }

    {
        constexpr std::size_t n = 3;

        double S[n] = {100.0, 105.0, 110.0};
        double K[n] = {100.0, 100.0, 100.0};
        double T[n] = {1.0, 1.0, 1.0};
        double sigma[n] = {0.2, 0.25, 0.3};

        volos::GreeksResult out[n]{};

        volos::OptionInputBatch batch{};
        batch.S = S;
        batch.K = K;
        batch.T = T;
        batch.sigma = sigma;
        batch.r = 0.05;
        batch.q = 0.0;
        batch.n = n;

        volos::bsm::greeks_batch(batch, volos::OptionType::Call, out);

        for (std::size_t i = 0; i < n; ++i) {
            volos::OptionInput scalar{};
            scalar.S = S[i];
            scalar.K = K[i];
            scalar.T = T[i];
            scalar.r = batch.r;
            scalar.q = batch.q;
            scalar.sigma = sigma[i];

            const volos::GreeksResult expected = volos::bsm::greeks(scalar, volos::OptionType::Call);

            assert_near(out[i].price, expected.price, 1e-12, "Batch Greeks price matches scalar");
            assert_near(out[i].delta, expected.delta, 1e-12, "Batch Greeks delta matches scalar");
            assert_near(out[i].gamma, expected.gamma, 1e-12, "Batch Greeks gamma matches scalar");
            assert_near(out[i].vega, expected.vega, 1e-12, "Batch Greeks vega matches scalar");
            assert_near(out[i].theta, expected.theta, 1e-12, "Batch Greeks theta matches scalar");
            assert_near(out[i].rho, expected.rho, 1e-12, "Batch Greeks rho matches scalar");
        }
    }

    std::cout << "BSM Greeks tests passed.\n";
}