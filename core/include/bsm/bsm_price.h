#pragma once

#include "types.h"
#include "enums.h"

namespace volos::bsm {

    double price(
        const OptionInput& input,
        OptionType type
    );

    // batch pricing
    void price_batch(
        const OptionInputBatch& input,
        OptionType type,
        double* out
    );

}
