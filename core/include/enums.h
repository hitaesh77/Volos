#pragma once
#include <cstdint>

namespace volos {

    // enum class makes OptionType scope
    // call it by saying OPtionType::Call, instead of Call
    // safer usually
    enum class OptionType : uint8_t {
        Call = 0,
        Put  = 1
    };

}