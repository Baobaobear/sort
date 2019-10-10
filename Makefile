# Copyright (c) 2019 Baobaobear. All Rights Reserved.

CXX ?= g++
CFLAGS03 ?= -O3 -std=c++03 -Wall -pedantic -Wno-format
CFLAGS03O1 ?= -O1 -std=c++03 -Wall -pedantic -Wno-format
BENCHMARKFILE ?= sorttest.cpp

default: clean demo1 test

test: benchmark0 benchmark1 benchmark2 benchmark3
	./benchmark0
	./benchmark1
	./benchmark2
	./benchmark3

clean:
	rm -f demo benchmark0 benchmark1 benchmark2 benchmark3

demo1: demo.cpp sortlib.hpp sorttest.hpp
	$(CXX) $(CFLAGS03) demo.cpp -o demo

benchmark0: sorttest.cpp sortlib.hpp sorttest.hpp
	$(CXX) $(CFLAGS03) $(BENCHMARKFILE) -D TEST_TYPE_SIMPLE=0 -o benchmark0

benchmark1: sorttest.cpp sortlib.hpp sorttest.hpp
	$(CXX) $(CFLAGS03) $(BENCHMARKFILE) -D TEST_TYPE_SIMPLE=1 -o benchmark1

benchmark2: sorttest.cpp sortlib.hpp sorttest.hpp
	$(CXX) $(CFLAGS03O1) $(BENCHMARKFILE) -D TEST_TYPE_SIMPLE=0 -o benchmark2

benchmark3: sorttest.cpp sortlib.hpp sorttest.hpp
	$(CXX) $(CFLAGS03O1) $(BENCHMARKFILE) -D TEST_TYPE_SIMPLE=1 -o benchmark3
