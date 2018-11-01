CXX ?= g++
CXXFLAGS += -std=c++17 -O0 -Wall -Wextra -Wpedantic -Werror

.PHONY: all debug clean run

all: clean
	$(CXX) $(CXXFLAGS) src/main.cpp src/list.hpp -o build/main

debug:
	CXXFLAGS=-g make

clean:
	rm -f build/*

run: all
	build/main
