#include "interpreter.h"
#include "printer.h"

void Interpreter::printList(AST::Node listNode)
{
    std::cout << "[";
    for (auto decs : listNode.children)
    {
        printer(decs);
        std::cout << ", ";
    }
    std::cout << "\b\b]" << std::endl;
}