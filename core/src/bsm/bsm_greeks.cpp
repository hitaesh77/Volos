#include "bsm/bsm_greeks.h"
#include "volos_math.h"

// NEED TO REFACTOR TO REDUCE REDUNDANT PRICE CALCS

namespace {

    // HELPER FUNCTIONS

    // calculate intermediate value d1 and d2
    void calculate_d1_d2(const volos::OptionInput& input, double sqrt_T, double& d1, double& d2){
        double numerator = std::log(input.S / input.K) + ((input.r - input.q + ((input.sigma * input.sigma) * 0.5)) * input.T);
        double denominator = sqrt_T * input.sigma;

        d1 = numerator / denominator;
        d2 = d1 - denominator; // used denominator since calculation is same (minus sigma * sqrt_t)
    }

} // anonymous namespace

namespace volos::bsm {

    // single greeks calc
    GreeksResult greeks(
        const OptionInput& input,
        OptionType type
    ) {
        // cache recurring vars
        double sqrt_T = std::sqrt(input.T);
        double exp_rT = std::exp((-1 * input.r * input.T));
        double exp_qT = std::exp((-1 * input.q * input.T));

        // intermediate variables: d1, d2
        double d1, d2;
        calculate_d1_d2(input, sqrt_T, d1, d2);

        // cache recurring vars that require d1, d2
        double d1_pdf = volos::math::norm_pdf(d1);
        double d1_cdf = volos::math::norm_cdf(d1);
        double d2_cdf = volos::math::norm_cdf(d2);
        // double d1_cdf_neg = calculate_N_cdf((-1 * intermediate.d1));
        double d1_cdf_neg = 1 - d1_cdf;
        // double d2_cdf_neg = calculate_N_cdf((-1 * intermediate.d2));
        double d2_cdf_neg = 1 - d2_cdf;

        // greek calculations
        GreeksResult res{};
        
        res.gamma = exp_qT * d1_pdf / (input.S * input.sigma * sqrt_T);
        res.vega = input.S * exp_qT * sqrt_T * d1_pdf;

        if (type == OptionType::Put) { // PUT OPTION
            res.delta = exp_qT * (d1_cdf  - 1);
            res.price = (input.K * exp_rT * d2_cdf_neg) - (input.S * exp_qT * d1_cdf_neg);
            res.theta = -((input.S * exp_qT * input.sigma * d1_pdf) / (2 * sqrt_T)) + (input.r * input.K * exp_rT * d2_cdf_neg) - (input.q * input.S * exp_qT * d1_cdf_neg);
            res.rho = -(input.K * input.T * exp_rT * d2_cdf_neg);
        } else { // CALL OPTION
            res.delta = exp_qT * d1_cdf;
            res.price = (input.S * exp_qT * d1_cdf) - (input.K * exp_rT * d2_cdf);
            res.theta = -((input.S * exp_qT * input.sigma * d1_pdf) / (2 * sqrt_T)) - (input.r * input.K * exp_rT * d2_cdf) + (input.q * input.S * exp_qT * d1_cdf);
            res.rho = input.K * input.T * exp_rT * d2_cdf;
        }
        
        return res;
    }

    // batch greeks calc
    // loop for now, may change to make parallel later
    void greeks_batch(
        const OptionInputBatch& input,
        OptionType type,
        GreeksResult* out
    ) {
        for (size_t i = 0; i < input.n; ++i) {
            OptionInput in{
                input.S[i], input.K[i], input.T[i], input.r, input.q, input.sigma[i]
            };
            out[i] = greeks(in, type);
        }
    }

}