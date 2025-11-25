#include <iostream>
#include "../include/GreeksCalculator.h"

int main(int argc, char** argv) {
    GreeksCalculator g;

    OptionInputs in = {1, 1, 1, 1, 1, 1, true};

    g.calculate_greeks_bs(in);
}