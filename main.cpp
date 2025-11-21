// Include the headers
#include "blocks/c_code_generator.h"
#include "builder/static_var.h"
#include "builder/dyn_var.h"
#include <iostream>

// Include the BuildIt types
using builder::dyn_var;
using builder::static_var;

using namespace std;

/** Decode the opcode */
#define DECODE_OP(x) (x & 0xff)

/** Decode operand A */
#define DECODE_A(x) ((x>>8) & (0xff))
/** Decode operand B */
#define DECODE_B(x) ((x>>16) & (0xff))
/** Decode operand C */
#define DECODE_C(x) ((x>>24) & (0xff))

/** Decode long operand Bx */
#define DECODE_Bx(x) ((x>>16) & (0xffff))

/** Decode signed long operand Bx */
#define DECODE_sBx(x) ((short) ((x>>16) & (0xffff)))

const uint32_t example_bytecode[3] = {
    0xA0103,                              // Load constant 10 to register 1
    0xF0203,                              // Load constant 15 to register 2
    0x1020004,                            // Add register 1 and 2 and store in register 0
};

static uint32_t reg[255] = {0};

static dyn_var<int> morpho_vm(const uint32_t bytecode[], int n) {
    uint32_t x, a, b, c;
    for (int i = 0; i < n; i++) {
        x = bytecode[i];
        switch (DECODE_OP(x)) {
            case 0x3:
                a=DECODE_A(x); b=DECODE_Bx(x);
                reg[a] = b;
                break;
            case 0x4:
                a=DECODE_A(x); b=DECODE_B(x); c=DECODE_C(x);
                reg[a] = reg[b] + reg[c];
                break;
            default:
                cout << "Not implemented" << endl;
                exit(0);
        }
    }
    return reg[0];
}

int main(int argc, char* argv[]) {
	builder::builder_context context;
	auto ast = context.extract_function_ast(morpho_vm, "main", example_bytecode, 3);
	block::c_code_generator::generate_code(ast, std::cout, 0);
	return 0;
}

