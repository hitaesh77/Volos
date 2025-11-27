#include "GreeksCalculator.h"

int main() {
    GreeksCalculator g;
    OptionInputs in = {2500.0, 2500.0, 2.5, 0.02, 0.0, 0.4, true};
    g.calculate_greeks_bsm(in);
    return 0;
}