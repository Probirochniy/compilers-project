#include "interpreter.h"
#include "printer.h"

void Interpreter::printListCall(AST::Node callNode){
    AST::Node value = listCall(callNode);
    printer(value);
}   
