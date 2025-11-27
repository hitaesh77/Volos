#include "GreeksCalculator.h"
#include <iostream>
#include <cmath>

// constants to make calc O(1)
const double INV_SQRT_2PI = 0.3989422804014327;
const double INV_SQRT_2 = 0.7071067811865476;

// HELPER FUNCTIONS

// calculate intermediate value d1 and d2
d1d2 GreeksCalculator::calculate_d1_d2(const OptionInputs& input, double sqrt_T){
    double numerator = std::log(input.S / input.K) + ((input.r + ((input.sigma * input.sigma) * 0.5)) * input.T);
    double denominator = sqrt_T * input.sigma;

    double d1 = numerator / denominator;
    double d2 = d1 - denominator; // used denominator since calculation is same (minus sigma * sqrt_t)

    return {d1, d2};
}

// calculate normal cdf derivative of a value
double GreeksCalculator::calculate_N_prime(double x) {
    return INV_SQRT_2PI * std::exp((-0.5 * x * x));
}

// calculate normal cdf of a value
double GreeksCalculator::calculate_N_cdf(double x) {
    return 0.5 * std::erfc((-x * INV_SQRT_2));
}

GreeksResult GreeksCalculator::calculate_greeks_bsm(const OptionInputs& input){
    // cache recurring vars
    double sqrt_T = std::sqrt(input.T);
    double exp_rt = std::exp((-1 * input.r * input.T));

    // intermediate variables: d1, d2
    d1d2 intermediate = calculate_d1_d2(input, sqrt_T);

    // cache recurring vars that require d1, d2
    double d1_n_prime = calculate_N_prime(intermediate.d1);
    double d1_cdf = calculate_N_cdf(intermediate.d1);
    double d2_cdf = calculate_N_cdf(intermediate.d2);
    // double d1_cdf_neg = calculate_N_cdf((-1 * intermediate.d1));
    double d1_cdf_neg = 1 - d1_cdf;
    // double d2_cdf_neg = calculate_N_cdf((-1 * intermediate.d2));
    double d2_cdf_neg = 1 - d2_cdf;

    // greek calculations
    GreeksResult res;
    
    res.delta = d1_cdf;
    res.gamma =d1_n_prime / (input.S * input.sigma * sqrt_T);
    res.vega = input.S * sqrt_T * d1_n_prime;

    if (!input.isCall) { // PUT OPTION
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