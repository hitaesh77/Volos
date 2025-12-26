#pragma once // replaces include guards (ifndef, def, endif)
#include <cmath>

// constants to make calc O(1)
// constexpr moves constant to compile time -> should be faster
// inline tells linker that there should only be one copy of this
inline constexpr double INV_SQRT_2PI   = 0.3989422804014327;
inline constexpr double INV_SQRT_2     = 0.7071067811865476;


namespace volos::math {
    // calculate normal cdf derivative (pdf) of a value
    inline double norm_pdf(double x) {
        return INV_SQRT_2PI * std::exp(-0.5 * x * x);
    }

    // calculate normal cdf of a value
    inline double norm_cdf(double x) {
        return 0.5 * std::erfc((-x * INV_SQRT_2));
    }
}