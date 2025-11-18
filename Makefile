BUILDIT_PATH=./buildit

CXX=g++
CC=clang

CFLAGS=$(shell make --no-print-directory -C $(BUILDIT_PATH) compile-flags) -O3
LDFLAGS=$(shell make --no-print-directory -C $(BUILDIT_PATH) linker-flags)

all: main

main: main.cpp
		$(CXX) $< $(CFLAGS) $(LDFLAGS) -o $@
