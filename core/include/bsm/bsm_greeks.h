#pragma once

#include "types.h"
#include "enums.h"
// #include <cstddef>

namespace volos::bsm {

    GreeksResult greeks(
        const OptionInput& input,
        OptionType type
    );

    // batch version
    // not using std::vector to keep mem contiguous -> simpel array for now, may change
    void greeks_batch(
        const OptionInputBatch& input,
        OptionType type,
        GreeksResult* out
    );

}
