struct OptionInputs {
    double S;       // current underlying price
    double K;       // options strike price
    double T;       // time to expiration
    double r;       // risk free interest rate r
    double q;       // dividend yield
    double sigma;   // sigma: implied volatility
    bool isCall;    // true if call, false if put
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