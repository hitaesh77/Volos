# include "OptionsData.h"

class GreeksCalculator {
    private:
    
    public:
        // send reference for low memory input, but make it const to ensure nothing changes
        GreeksResult calculate_greeks_bs(const OptionInputs& input); // black scholes
};