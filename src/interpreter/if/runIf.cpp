#include "interpreter.h"

void Interpreter::runIf(AST::Node node)
{
    bool condResult = false;
    for (auto child : node.children)
    {
        if (child.type == NodeType::IF_COND)
        {
            condResult = evalBool(child.children.front());
        }
        else if (child.type == NodeType::IF_BODY)
        {
            if (condResult)
            {
                for (auto grandchild : child.children){
                    interpret(grandchild);
                }
            }
        }
        else if (child.type == NodeType::ELSE_BODY)
        {
            if (!condResult)
            {
                for (auto grandchild : child.children){
                    interpret(grandchild);
                }
                
            }
        }
    }
}