#include "interpreter.h"

void Interpreter:: printIRS(AST::Node node)
{
    std::cout << node.value.value;
}