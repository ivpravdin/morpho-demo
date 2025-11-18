// Include the headers
#include "blocks/c_code_generator.h"
#include "builder/static_var.h"
#include "builder/dyn_var.h"
#include <iostream>

// Include the BuildIt types
using builder::dyn_var;
using builder::static_var;

static dyn_var<int> morpho_vm(const char* morpho_bytecode) {
    switch (morpho_bytecode[0]) {
        case '1':
            int x, y;
            sscanf(morpho_bytecode + 1, "%1d", &x);
            sscanf(morpho_bytecode + 2, "%1d", &y);
            return x + y;
            break;
        default:
            break;
    }
    return 0;
}

int main(int argc, char* argv[]) {
	builder::builder_context context;
    // 1 for add, 0 for return 0
    const char *morpho_bytecode = "123";
	auto ast = context.extract_function_ast(morpho_vm, "morpho_vm", morpho_bytecode);
	block::c_code_generator::generate_code(ast, std::cout, 0);
	return 0;
}

