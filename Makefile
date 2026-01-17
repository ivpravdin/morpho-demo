BUILDIT_PATH=./buildit
MORPHO_HEADER_PATH=./include/morpho
MORPHO_LIB_PATH=./lib

# WARNING_FLAGS=-Wall -Wextra -Werror
CFLAGS=$(shell make --no-print-directory -C $(BUILDIT_PATH) compile-flags) -O3 -I$(MORPHO_HEADER_PATH) -Wl,-rpath,$(MORPHO_LIB_PATH) $(WARNING_FLAGS)
LDFLAGS=$(shell make --no-print-directory -C $(BUILDIT_PATH) linker-flags) -lmorpho -L$(MORPHO_LIB_PATH)  $(WARNING_FLAGS)

CXX=g++
CC=clang

.PHONY: buildit all

all: buildit main

buildit:
	$(MAKE) -C $(BUILDIT_PATH)

main: main.cpp buildit
	$(CXX) $< $(CFLAGS) $(LDFLAGS) -o $@

# morpho:
# 	cd ./morpho/build
# 	cmake -DCMAKE_BUILD_TYPE=Release ./morpho
# 	cp ./morpho/build/**.h include/morpho

clean: 
	$(MAKE) -C $(BUILDIT_PATH) clean
	rm -rf *.o main
