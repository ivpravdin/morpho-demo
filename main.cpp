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

dyn_var<void(int)> print(builder::as_global("print"));

static void print_wrapper_code(std::ostream &oss) {
	oss << "#include <stdio.h>\n";
	oss << "#include <stdlib.h>\n";
    oss << "#include <stdbool.h>\n";
	oss << "void print(int x) {printf(\"%d\\n\", x);}\n";
}

const uint32_t example_bytecode[] = {
    0x00000002,
    0x0000001e,
    0x00000002,
    0x0001001e,
    0x00010002,
    0x00020102,
    0x0100020a,
    0x0015020f,
    0x00000102,
    0x0000011e,
    0x00000102,
    0x00020202,
    0x0201030a,
    0x0006030f,
    0x0000021d,
    0x01020303,
    0x0000031e,
    0x00010202,
    0x02010103,
    0xfff7ff0d,
    0x0000011d,
    0x0001021d,
    0x0102030a,
    0x0002030f,
    0x0000011d,
    0x0001011e,
    0x00010102,
    0x01000003,
    0xffe8ff0d,
    0x0001001d,
    0x00000022,
    0x00000024,
};

const int n = sizeof(example_bytecode) / sizeof(uint32_t);

static dyn_var<int> morpho_vm(const uint32_t bytecode[], const int n) {
    static_var<uint32_t> consts[] = {0, 1, 30000};

    dyn_var<int32_t[255]> reg = {0};
    dyn_var<int32_t[255]> globals = {0};
    dyn_var<int32_t> left, right;
    
    static_var<int32_t> a, b, c;
    static_var<int32_t> bc;
    static_var<int32_t> pc = 0;

    while (pc < n) {
        bc = bytecode[pc];
        switch (DECODE_OP(bc)) {
            case 0x0: // NOP
                break;
            case 0x1: // MOV
                a=DECODE_A(bc); b=DECODE_B(bc);
                reg[a] = reg[b];
                break;
            case 0x2: // LCT
                a=DECODE_A(bc); b=DECODE_Bx(bc);
                reg[a] = consts[b];
                break;
            case 0x3: // ADD
                a=DECODE_A(bc); b=DECODE_B(bc); c=DECODE_C(bc);
                reg[a] = reg[b] + reg[c];
                break;
            case 0xA: //LT
                a=DECODE_A(bc); b=DECODE_B(bc); c=DECODE_C(bc);
                left = reg[b];
                right = reg[c];

                reg[a] = (left<right);
                break;
            case 0xD: // B
                b=DECODE_sBx(bc);
                pc+=b;
                break;
            case 0xF: // BIFF
                a=DECODE_A(bc);
                left=reg[a];

                if (!left) pc+=DECODE_sBx(bc);
                break;
            case 0x1D: // LGL
                a=DECODE_A(bc);
                b=DECODE_Bx(bc);
                reg[a]=globals[b];
                break;
            case 0x1E: // SGL
                a=DECODE_A(bc);
                b=DECODE_Bx(bc);
                globals[b]=reg[a];
                break;
            case 0x22: // PRINT
                a=DECODE_A(bc);
                left=reg[a];
                print(left);
                break;
            case 0x24: // END
                return 0;
            default:
                return 1;
        }

        pc++;
    }
    return 1;
}

int main(int argc, char* argv[]) {
	builder::builder_context context;
	auto ast = context.extract_function_ast(morpho_vm, "main", example_bytecode, n);
    print_wrapper_code(std::cout);
	block::c_code_generator::generate_code(ast, std::cout, 0);
	return 0;
}

