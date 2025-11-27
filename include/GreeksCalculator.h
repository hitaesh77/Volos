#include "OptionsData.h"

class GreeksCalculator {
    private:
        d1d2 calculate_d1_d2(const OptionInputs& input, double sqrt_T);
        inline double calculate_N_cdf(double x);
        inline double calculate_N_prime(double x);
    
    public:
        // send reference for low memory input, but make it const to ensure nothing changes
        GreeksResult calculate_greeks_bsm(const OptionInputs& input); // black scholes
};