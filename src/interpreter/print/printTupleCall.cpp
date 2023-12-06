#include "interpreter.h"
#include "printer.h"

void Interpreter::printTupleCall(AST::Node callNode){
    AST::Node value = tupleCall(callNode);
    printer(value);
}   
