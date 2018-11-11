CXX ?= g++
CXXFLAGS ?= -std=c++17 -O0 -Wall -Wextra -pedantic -Werror

.PHONY: all debug clean run

all: clean
	$(CXX) $(CXXFLAGS) src/main.cpp src/list.hpp -o bin/main

debug:
	CXXFLAGS="-g $(CXXFLAGS)" make

clean:
	rm -f bin/*

run: all
	bin/main
