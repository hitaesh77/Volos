CC = g++
CFLAGS = -I include
SOURCES = src/GreeksCalculator.cpp tests/TestGreekCalculator.cpp
TARGET = greeks

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean