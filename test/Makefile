BOOST_PATH ?= /usr/local/boost_1_68_0

CXX ?= g++
CXXFLAGS ?= -g -pg -pthread -std=c++17 -I$(BOOST_PATH) -O0 -Wall -Wextra -pedantic -Werror

.PHONY: all bin debug clean run

all: bin clean
	$(CXX) $(CXXFLAGS) list_seq.cpp -o bin/list_seq
	$(CXX) $(CXXFLAGS) list_par.cpp -o bin/list_par

bin:
	mkdir -p bin

debug:
	CXXFLAGS="-g $(CXXFLAGS)" make

clean:
	rm -f bin/*

run: all
	bin/list_seq; true
	bin/list_par; true
