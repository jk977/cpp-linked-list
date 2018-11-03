CXX ?= g++
CXXFLAGS += -g -std=c++17 -I/usr/local/boost_1_68_0 -O0 -Wall -Wextra -pedantic -Werror

.PHONY: all debug clean run

all: clean
	$(CXX) $(CXXFLAGS) list.cpp -o bin/list
	$(CXX) $(CXXFLAGS) list_iterator.cpp -o bin/list_iterator

debug:
	CXXFLAGS=-g make

clean:
	rm -f bin/*

run: all
	bin/list; true
	bin/list_iterator; true
