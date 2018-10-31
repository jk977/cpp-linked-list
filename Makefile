CXX := g++
CXXFLAGS := -std=c++17 -O0 -Wall -Wextra -Wpedantic -Werror

.PHONY: all

all:
	$(CXX) $(CXXFLAGS) -c list.hpp -o build/list.o
