BOOST_PATH ?= /usr/local/boost_1_68_0

CXX ?= g++
CXXFLAGS ?= -g -std=c++17 -I$(BOOST_PATH) -O0 -Wall -Wextra -pedantic -Werror

.PHONY: all bin debug clean run

all: bin clean
	$(CXX) $(CXXFLAGS) list.cpp -o bin/list
	$(CXX) $(CXXFLAGS) list_iterator.cpp -o bin/list_iterator

bin:
	mkdir -p bin

debug:
	CXXFLAGS="-g $(CXXFLAGS)" make

clean:
	rm -f bin/*

run: all
	bin/list; true
	bin/list_iterator; true
