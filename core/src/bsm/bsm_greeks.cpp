#include "core/include/bsm/bsm_greeks.h"
#include "core/include/math.h"

namespace {

    // HELPER FUNCTIONS

    // calculate intermediate value d1 and d2
    void calculate_d1_d2(const volos::OptionInput& input, double sqrt_T, double& d1, double& d2){
        double numerator = std::log(input.S / input.K) + ((input.r + ((input.sigma * input.sigma) * 0.5)) * input.T);
        double denominator = sqrt_T * input.sigma;

        d1 = numerator / denominator;
        d2 = d1 - denominator; // used denominator since calculation is same (minus sigma * sqrt_t)
    }

} // anonymous namespace

namespace volos::bsm {

    GreeksResult greeks(
        const OptionInput& input,
        OptionType type
    ) {
        // cache recurring vars
        double sqrt_T = std::sqrt(input.T);
        double exp_rt = std::exp((-1 * input.r * input.T));

        // intermediate variables: d1, d2
        double d1, d2;
        calculate_d1_d2(input, sqrt_T, d1, d2);

        // cache recurring vars that require d1, d2
        double d1_n_prime = volos::math::norm_pdf(d1);
        double d1_cdf = volos::math::norm_cdf(d1);
        double d2_cdf = volos::math::norm_cdf(d2);
        // double d1_cdf_neg = calculate_N_cdf((-1 * intermediate.d1));
        double d1_cdf_neg = 1 - d1_cdf;
        // double d2_cdf_neg = calculate_N_cdf((-1 * intermediate.d2));
        double d2_cdf_neg = 1 - d2_cdf;

        // greek calculations
        GreeksResult res;
        
        res.delta = d1_cdf;
        res.gamma =d1_n_prime / (input.S * input.sigma * sqrt_T);
        res.vega = input.S * sqrt_T * d1_n_prime;

        if (type == OptionType::Put) { // PUT OPTION
            res.delta = res.delta - 1;
            res.price = (input.K * exp_rt * d2_cdf_neg) - (input.S * d1_cdf_neg);
            res.theta = -((input.S * input.sigma * d1_n_prime) / (2 * sqrt_T)) + (input.r * input.K * exp_rt * d2_cdf_neg);
            res.rho = -(input.K * input.T * exp_rt * d2_cdf_neg);
        } else { // CALL OPTION
            res.price = (input.S * d1_cdf) - (input.K * exp_rt * d2_cdf);
            res.theta = -((input.S * input.sigma * d1_n_prime) / (2 * sqrt_T)) - (input.r * input.K * exp_rt * d2_cdf);
            res.rho = input.K * input.T * exp_rt * d2_cdf;
        }
        
        return res;
    }

}