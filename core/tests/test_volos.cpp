#include <iostream>

void run_bsm_price_tests();
void run_bsm_greeks_tests();

int main() {
    run_bsm_price_tests();
    run_bsm_greeks_tests();

    std::cout << "\nAll Volos tests passed.\n";
    return 0;
}