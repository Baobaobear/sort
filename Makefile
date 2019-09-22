# Copyright (c) 2019 Baobaobear. All Rights Reserved.

CXX ?= g++
CFLAGS ?= -O3 -Wall -std=c++03 -pedantic -Wno-format

default: clean demo test

.PHONY: default clean

test: benchmark
	./benchmark

clean:
	rm -f demo benchmark

demo: demo.cpp sortlib.hpp sorttest.hpp
	$(CXX) $(CFLAGS) demo.cpp -o demo

benchmark: sorttest.cpp sortlib.hpp sorttest.hpp
	$(CXX) $(CFLAGS) sorttest.cpp -o benchmark
