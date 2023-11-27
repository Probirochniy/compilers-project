#include "interpreter.h"

void Interpreter::printTuple(AST::Node tupleNode){
    std::cout << "{";
    for (auto decs : tupleNode.children){
        printer(decs);
        std::cout << ", ";
    }
    std::cout << "\b\b}" << std::endl;
}   