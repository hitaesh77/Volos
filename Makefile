CXX := g++
CXXFLAGS := -std=c++17 -O3 -march=native -Wall -Wextra -Icore/include

BUILD_DIR := build

SRC := \
	core/src/bsm/bsm_price.cpp \
	core/src/bsm/bsm_greeks.cpp \
# 	core/src/american/amer_price.cpp

TEST_SRC := \
	core/tests/test_volos.cpp \
	core/tests/test_bsm_price.cpp \
	core/tests/test_bsm_greeks.cpp

BENCH_SRC := \
	core/benchmarks/bench_bsm_greeks.cpp

TEST_BIN := $(BUILD_DIR)/volos_tests
BENCH_BIN := $(BUILD_DIR)/volos_bench

all: test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

test: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRC) $(TEST_SRC) -o $(TEST_BIN)
	./$(TEST_BIN)

bench: $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRC) $(BENCH_SRC) -o $(BENCH_BIN)
	./$(BENCH_BIN)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all test bench clean