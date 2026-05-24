#pragma once

#include <cmath>
#include "types.h"
#include "volos_math.h"

namespace volos::bsm::internal {
    struct BSMKernel {
        double sqrt_T;

        double discount_r;
        double discount_q;

        double d1;
        double d2;

        double d1_pdf;

        double d1_cdf;
        double d2_cdf;
        double d1_cdf_neg;
        double d2_cdf_neg;
    };

    inline BSMKernel compute_kernel(const OptionInput& input) {
        BSMKernel k{};

        k.sqrt_T = std::sqrt(input.T);
        k.discount_r = std::exp((-1 * input.r * input.T));
        k.discount_q = std::exp((-1 * input.q * input.T));

        double numerator = std::log(input.S / input.K) + ((input.r - input.q + ((input.sigma * input.sigma) * 0.5)) * input.T);
        double denominator = k.sqrt_T * input.sigma;
        k.d1 = numerator / denominator;
        k.d2 = k.d1 - denominator; // used denominator since calculation is same (minus sigma * sqrt_t)

        k.d1_pdf = volos::math::norm_pdf(k.d1);
        k.d1_cdf = volos::math::norm_cdf(k.d1);
        k.d2_cdf = volos::math::norm_cdf(k.d2);
        k.d1_cdf_neg = 1 - k.d1_cdf;
        k.d2_cdf_neg = 1 - k.d2_cdf;

        return k;
    }
}