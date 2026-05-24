#include "bsm/bsm_greeks.h"
#include "internal/bsm_kernel.h"
#include "volos_math.h"

namespace volos::bsm {

    // single greeks calc
    GreeksResult greeks(
        const OptionInput& input,
        OptionType type
    ) {

        internal::BSMKernel kernel = internal::compute_kernel(input);

        // greek calculations
        GreeksResult res{};
        
        res.gamma = kernel.discount_q * kernel.d1_pdf / (input.S * input.sigma * kernel.sqrt_T);
        res.vega = input.S * kernel.discount_q * kernel.sqrt_T * kernel.d1_pdf;

        if (type == OptionType::Put) { // PUT OPTION
            res.delta = kernel.discount_q * (kernel.d1_cdf  - 1);

            res.price =   (input.K * kernel.discount_r * kernel.d2_cdf_neg) 
                        - (input.S * kernel.discount_q * kernel.d1_cdf_neg);

            res.theta = -((input.S * kernel.discount_q * input.sigma * kernel.d1_pdf) / (2 * kernel.sqrt_T)) 
                        + (input.r * input.K * kernel.discount_r * kernel.d2_cdf_neg) 
                        - (input.q * input.S * kernel.discount_q * kernel.d1_cdf_neg);

            res.rho = -(input.K * input.T * kernel.discount_r * kernel.d2_cdf_neg);

        } else { // CALL OPTION
            res.delta = kernel.discount_q * kernel.d1_cdf;

            res.price =   (input.S * kernel.discount_q * kernel.d1_cdf) 
                        - (input.K * kernel.discount_r * kernel.d2_cdf);

            res.theta = -((input.S * kernel.discount_q * input.sigma * kernel.d1_pdf) / (2 * kernel.sqrt_T)) 
                        - (input.r * input.K * kernel.discount_r * kernel.d2_cdf) 
                        + (input.q * input.S * kernel.discount_q * kernel.d1_cdf);

            res.rho = input.K * input.T * kernel.discount_r * kernel.d2_cdf;
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