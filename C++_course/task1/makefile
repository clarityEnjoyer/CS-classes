CXX      := g++
CXXFLAGS := -Wall -Wextra -O2 -std=c++23
TARGET   := medals
SRC      := medals.cpp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(TARGET)
	