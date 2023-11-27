#include "semantic.h"
#include "parser.h"
#include "interpreter.h"

Interpreter::Interpreter() {}

void Interpreter::runProgram(AST::Node node) {
    interpret(node);
}
