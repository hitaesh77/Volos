#include "OptionsData.h"

class GreeksCalculator {
    private:
        d1d2 calculate_d1_d2(const OptionInputs& input);
        inline double calculate_N_cdf(double x);
        inline double calculate_N_prime(double x);
        double calculate_price();
        double calculate_delta();
        double calculate_gamme();
        double calculate_vega();
        double calculate_theta();
        double calculate_rho();

    
    public:
        // send reference for low memory input, but make it const to ensure nothing changes
        GreeksResult calculate_greeks_bsm(const OptionInputs& input); // black scholes
};