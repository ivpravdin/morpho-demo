BUILDIT_PATH=./buildit
MORPHO_HEADER_PATH=./include/morpho
MORPHO_LIB_PATH=./lib

CFLAGS=$(shell make --no-print-directory -C $(BUILDIT_PATH) compile-flags) -O3 -I$(MORPHO_HEADER_PATH) -Wl,-rpath,$(MORPHO_LIB_PATH)
LDFLAGS=$(shell make --no-print-directory -C $(BUILDIT_PATH) linker-flags) -lmorpho -L$(MORPHO_LIB_PATH)  

CXX=g++
CC=clang

.PHONY: buildit all

all: buildit main

buildit:
	$(MAKE) -C $(BUILDIT_PATH)

main: main.cpp buildit
	$(CXX) $< $(CFLAGS) $(LDFLAGS) -o $@

clean: 
	$(MAKE) -C $(BUILDIT_PATH) clean
	rm -rf *.o main
