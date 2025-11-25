struct OptionInputs {
    double S;       // current underlying price
    double K;       // options strike price
    double T;       // time to expiration
    double r;       // risk free interest rate r
    double o;       // sigma: implied volatility
    double q;       // dividend yield
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