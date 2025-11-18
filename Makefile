BUILDIT_PATH=./buildit

CXX=g++
CC=clang

CFLAGS=$(shell make --no-print-directory -C $(BUILDIT_PATH) compile-flags) -O3
LDFLAGS=$(shell make --no-print-directory -C $(BUILDIT_PATH) linker-flags)

.PHONY: buildit all

all: buildit main

buildit:
	$(MAKE) -C $(BUILDIT_PATH)

main: main.cpp
	$(CXX) $< $(CFLAGS) $(LDFLAGS) -o $@

clean: 
	$(MAKE) -C $(BUILDIT_PATH) clean
	rm -rf *.o main