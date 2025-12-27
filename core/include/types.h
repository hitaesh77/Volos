#pragma once
#include <cstddef>

namespace volos {

    struct OptionInput {
        double S;       // current underlying price
        double K;       // options strike price
        double T;       // time to expiration
        double r;       // risk free interest rate r
        double q;       // dividend yield
        double sigma;   // sigma: implied volatility
    };

    struct OptionInputBatch {
        const double* S;
        const double* K;
        const double* T;
        const double* sigma;
        size_t n;
        double r;
        double q;
    };

    struct GreeksResult {
        double price;
        double delta;
        double gamma;
        double vega;
        double theta;
        double rho;
    };

    struct d1d2 {
        double d1;
        double d2;
    };

}