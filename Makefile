# Copyright (c) 2019 Baobaobear. All Rights Reserved.

CXX ?= g++
CFLAGS03 ?= -O3 -std=c++03 -Wall -pedantic -Wno-format
CFLAGS03NOWALL ?= -O3 -std=c++03 -pedantic -Wno-format
CFLAGS11 ?= -O3 -std=c++11 -Wall -pedantic -Wno-format

default: clean demo1 demo2 test

.PHONY: default clean

test: benchmark0 benchmark1
	./benchmark0
	./benchmark1

clean:
	rm -f demo benchmark0 benchmark1

demo1: demo.cpp sortlib.hpp sorttest.hpp
	$(CXX) $(CFLAGS03) demo.cpp -o demo

demo2: demo.cpp sortlib.hpp sorttest.hpp
	$(CXX) $(CFLAGS11) demo.cpp -o demo

benchmark0: sorttest.cpp sortlib.hpp sorttest.hpp
	$(CXX) $(CFLAGS03NOWALL) sorttest.cpp -D TEST_TYPE_SIMPLE=0 -o benchmark0

benchmark1: sorttest.cpp sortlib.hpp sorttest.hpp
	$(CXX) $(CFLAGS03) sorttest.cpp -D TEST_TYPE_SIMPLE=1 -o benchmark1
