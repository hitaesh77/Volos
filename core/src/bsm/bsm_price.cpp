#include "bsm/bsm_price.h"
#include "volos_math.h"

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

    // single price calc
    double price(
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
        double d1_cdf = volos::math::norm_cdf(d1);
        double d2_cdf = volos::math::norm_cdf(d2);
        // double d1_cdf_neg = calculate_N_cdf((-1 * intermediate.d1));
        double d1_cdf_neg = 1 - d1_cdf;
        // double d2_cdf_neg = calculate_N_cdf((-1 * intermediate.d2));
        double d2_cdf_neg = 1 - d2_cdf;

        if (type == OptionType::Put) {
            return (input.K * exp_rT * d2_cdf_neg) - (input.S * exp_qT * d1_cdf_neg);
        } else {
            return (input.S * exp_qT * d1_cdf) - (input.K * exp_rT * d2_cdf);
        }
    }

    // batch price calc
    // loop for now, may change to make parallel later
    void price_batch(
        const OptionInputBatch& input,
        OptionType type,
        double* out
    ) {
        for (size_t i = 0; i < input.n; ++i) {
            OptionInput in{
                input.S[i], input.K[i], input.T[i], input.r, input.q, input.sigma[i]
            };
            out[i] = price(in, type);
        }
    }

}