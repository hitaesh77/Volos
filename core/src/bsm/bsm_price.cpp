#include "bsm/bsm_price.h"
#include "internal/bsm_kernel.h"
#include "volos_math.h"

namespace volos::bsm {

    // single price calc
    double price(
        const OptionInput& input,
        OptionType type
    ) {

        internal::BSMKernel kernel = internal::compute_kernel(input);

        if (type == OptionType::Put) {
            return (input.K * kernel.discount_r * kernel.d2_cdf_neg) - (input.S * kernel.discount_q * kernel.d1_cdf_neg);
        } else {
            return (input.S * kernel.discount_q * kernel.d1_cdf) - (input.K * kernel.discount_r * kernel.d2_cdf);
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