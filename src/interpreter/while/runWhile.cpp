#include "interpreter.h"
#include "printer.h"

void Interpreter::runWhile(AST::Node node)
{
    AST::Node condNode;
    AST::Node bodyNode;
    
    for (auto child : node.children)
    {
        if (child.type == NodeType:: IF_COND)
        {
            condNode = child;
        }
        else if (child.type == NodeType::FORLOOP_BODY)
        {
            bodyNode = child;
        }
    }


    while (evalBool(condNode.children.front()))
    {
        for (auto child : bodyNode.children)
        {
            if (child.type == NodeType::BREAK)
            {
                return;
            }
            interpret(child);
        }
    }
}
