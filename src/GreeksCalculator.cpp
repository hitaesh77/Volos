#include "GreeksCalculator.h"
#include <iostream>
#include <cmath>

// constants to make calc O(1)
const double INV_SQRT_2PI = 0.3989422804014327;
const double INV_SQRT_2 = 0.7071067811865476;

// HELPER FUNCTIONS

// calculate intermediate value d1 and d2
d1d2 GreeksCalculator::calculate_d1_d2(const OptionInputs& input){
    double sqrt_T = std::sqrt(input.T);

    double numerator = std::log(input.S / input.K);
    numerator = numerator + ((input.r + ((input.sigma * input.sigma) / 2)) * input.T);

    double denominator = sqrt_T * input.sigma;

    double d1 = numerator / denominator;
    double d2 = d1 - denominator;

    return {d1, d2};
}

// calculate normal cdf derivative of a value
inline double calculate_N_prime(double x) { // use inline to skip adding a function w params to the call stack
    return INV_SQRT_2PI * std::exp(-0.5 * x * x);
}

// calculate normal cdf of a value
inline double calculate_N_cdf(double x) {
    return 0.5 * std::erfc(-x * INV_SQRT_2);
}

GreeksResult GreeksCalculator::calculate_greeks_bsm(const OptionInputs& input){
    std::cout << "hello world" << std::endl;

    // intermediate variables: d1, d2
    d1d2 intermediate = calculate_d1_d2(input);

    // delta calculation
    // (new option price - initial options price) / (new underlying stock price - initial undelying stock price)


    GreeksResult res = {1, 1, 1, 1, 1, 1};
    
    return res;
}