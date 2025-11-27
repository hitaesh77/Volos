#include <iostream>
#include "GreeksCalculator.h"
#include <chrono>

int main(int argc, char** argv) {
    GreeksCalculator g;

    OptionInputs in;
    in.S = 2500.0;
    in.K = 2500.0;
    in.T = 2.5;
    in.r = 0.02;
    in.sigma = 0.4;
    in.q = 0.05;
    in.isCall = true;

    auto start = std::chrono::high_resolution_clock::now();

    GreeksResult res = g.calculate_greeks_bsm(in);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;

    std::cout << res.price << std::endl;
    std::cout << res.delta << std::endl;
    std::cout << res.gamma << std::endl;
    std::cout << res.vega << std::endl;
    std::cout << res.theta << std::endl;
    std::cout << res.rho << std::endl;
}