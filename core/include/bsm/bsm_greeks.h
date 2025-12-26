#pragma once

#include "include/types.h"
#include "include/enums.h"
// #include <cstddef>

namespace volos::bsm {

    GreeksResult greeks(
        const OptionInput& input,
        OptionType type
    );

    // batch version
    void greeks_batch(
        const double* S,
        const double* K,
        const double* T,
        const double* sigma,
        std::size_t n,
        double r,
        double q,
        OptionType type,
        GreeksResult* out
    );

}
