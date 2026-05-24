# Volos

Volos is a low-latency options pricing engine built in C++ with Python bindings and a FastAPI service layer.

The goal of Volos is to build the core pricing path by hand, understand the math behind option pricing deeply, and expose that pricing engine in a way that is usable from Python and web applications. The project focuses on speed, correctness, clean systems design, and measurable performance against standard Python quant libraries.

Volos currently supports Black-Scholes-Merton European option pricing and Greeks, with American option pricing, Python packaging, FastAPI deployment, and benchmarking in progress.

## Why Volos Exists

Volos is a learning focused systems project that implements the pricing path directly in C++, while still exposing a clean Python interface and API layer for real-world use.

The core idea:

> Build the low-latency math engine by hand, then wrap it with modern Python tooling.

## Project Goals

The main goals of Volos are:

1. Build a fast C++ options pricing engine
2. Implement Black-Scholes-Merton pricing and Greeks from scratch
3. Add American option pricing using a binomial tree model
4. Design a batch pricing path that avoids Python loop overhead
5. Expose the C++ engine through Python bindings
6. Package the project as a pip-installable library
7. Wrap the engine in a FastAPI service
8. Benchmark Volos against standard Python quant libraries

## Current Features

### Black-Scholes-Merton Pricing

Volos currently implements European option pricing for calls and puts using the Black-Scholes-Merton model.

Supported inputs:

```txt
S      current underlying price
K      strike price
T      time to expiration in years
r      risk-free interest rate
q      continuous dividend yield
sigma  implied volatility
type   call or put
```

Supported outputs:

```txt
option price
```

### Greeks

Volos currently computes the main first and second order Greeks:

```txt
delta
gamma
vega
theta
rho
```

The implementation supports continuous dividend yield through the `q` input.

### Batch Pricing

Volos includes a batch pricing interface using a structure-of-arrays layout:

```cpp
struct OptionInputBatch {
    const double* S;
    const double* K;
    const double* T;
    double r;
    double q;
    const double* sigma;
    std::size_t n;
};
```

This design keeps input arrays contiguous in memory and prepares the project for faster NumPy-backed batch pricing through Python bindings.

### Correctness Tests

Volos currently includes plain C++ tests.

Current tests cover:

```txt
European call price
European put price
put-call parity
put-call parity with dividend yield
price monotonicity with respect to spot
price monotonicity with respect to volatility
batch price consistency
Greek values
Greek signs and bounds
delta parity
delta parity with dividend yield
batch Greek consistency
```

The test suite is intentionally dependency light so the project can stay easy to build with a simple Makefile.

## In Progress

### American Option Pricing

The next pricing model being added is American option pricing using a Cox-Ross-Rubinstein binomial tree.

This will support early exercise, which makes it different from the European Black-Scholes-Merton model.

Planned American pricing API:

```cpp
volos::american::price_binomial_crr(input, type, steps);
volos::american::price_batch_binomial_crr(batch, type, steps, out);
```

Expected correctness tests:

```txt
American call with no dividends is close to European call
American put is worth at least European put
American option value is at least intrinsic value
American batch output matches scalar output
```

### Python Bindings

Volos will expose the C++ pricing engine to Python through pybind11.

Planned Python API:

```python
import volos

price = volos.price(
    S=100,
    K=100,
    T=1.0,
    r=0.05,
    q=0.0,
    sigma=0.2,
    option_type="call"
)

greeks = volos.greeks(
    S=100,
    K=100,
    T=1.0,
    r=0.05,
    q=0.0,
    sigma=0.2,
    option_type="call"
)
```

Planned batch API:

```python
prices = volos.price_batch(S, K, T, sigma, r=0.05, q=0.0, option_type="call")
```

The important performance goal is to avoid calling C++ once per option from Python. Instead, Python should pass NumPy arrays into C++ once, and the C++ engine should process the full batch.

### FastAPI Service

Volos will include a FastAPI wrapper around the Python bindings.

Planned endpoints:

```txt
GET  /health
POST /price
POST /greeks
POST /price/batch
POST /greeks/batch
```

The API layer is intentionally thin. FastAPI handles request validation and routing, while the pricing logic stays inside the C++ engine.

## Architecture

Volos is organized around a simple layered architecture:

```txt
C++ Core
  Fast pricing and Greeks implementation

Python Bindings
  pybind11 wrapper around the C++ core

Python Package
  pip-installable interface for local use

FastAPI Service
  HTTP API wrapper around the Python package

Benchmarks
  C++ and Python performance comparisons
```

The goal is to keep the pricing path isolated from the API path.

The C++ core should be usable independently of Python, and the Python API should be a thin wrapper over the native implementation.

## C++ Core Design

The C++ core is split into public model APIs and internal math helpers.

Current structure:

```txt
core/
  include/
    bsm/
      bsm_price.h
      bsm_greeks.h
      internal/
        bsm_kernel.h
    american/
      amer_price.h
    enums.h
    types.h
    volos_math.h

  src/
    bsm/
      bsm_price.cpp
      bsm_greeks.cpp
    american/
      amer_price.cpp

  tests/
    test_main.cpp
    test_bsm_price.cpp
    test_bsm_greeks.cpp
    test_american_price.cpp

  benchmarks/
    bench_bsm_greeks.cpp
```

## Build System

Volos uses a simple Makefile.

Basic commands:

```bash
make clean
make test
make bench
```

## Future Steps

### 1. Finish American Pricing

Implement American option pricing using the Cox-Ross-Rubinstein binomial tree.

Priority:

```txt
correct scalar price
batch wrapper
tests
benchmark
```

### 2. Add C++ Benchmarks

Create reproducible C++ benchmarks for BSM pricing and Greeks.

Benchmark sizes:

```txt
10,000 options
100,000 options
1,000,000 options
```

Report:

```txt
total milliseconds
nanoseconds per option
options per second
```

### 3. Add Python Bindings

Use pybind11 to expose the C++ engine to Python.

Initial functions:

```txt
price
greeks
price_batch
greeks_batch
american_price
american_price_batch
```

### 4. Make Volos pip-installable

Add:

```txt
pyproject.toml
setup.py
python/volos/__init__.py
bindings/volos_module.cpp
```

Success condition:

```bash
pip install -e .
python -c "import volos; print(volos.price(100, 100, 1, 0.05, 0, 0.2, 'call'))"
```

### 5. Add FastAPI Service

Create a thin HTTP API layer over the Python package.

Endpoints:

```txt
/health
/price
/greeks
/price/batch
/greeks/batch
```

### 6. Benchmark Against Existing Libraries

Compare Volos against common Python pricing workflows.

Targets:

```txt
pure Python implementation
NumPy/SciPy implementation
QuantLib-Python
py_vollib or py_vollib_vectorized
```

### 7. Write Final Results

Add a benchmark report:

```txt
benchmarks/results.md
```

Include:

```txt
hardware
compiler
compiler flags
Python version
library versions
input sizes
runtime
speedup
notes
```