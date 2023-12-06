#include "interpreter.h"
#include "printer.h"

AST::Node Interpreter::runFunction(AST::Node node)
{
    std::string funcName = "";
    std::vector<AST::Node> funcArgs;
    AST::Node func;
    AST::Node funcBody;

    for (auto child : node.children)
    {
        if (child.type == NodeType::IDENTIFIER)
        {
            funcName = child.value.value;
        }
        else if (child.type == NodeType::VARS_LIST)
        {
            for (auto grandchild : child.children)
            {
                funcArgs.push_back(grandchild);
            }
        }
    }

    func = genDict[funcName];

    for (auto child : func.children)
    {

        if (child.type == NodeType::VARS_LIST)
        {
            int index = 0;
            for (auto var : child.children)
            {
                if (funcArgs[index].type == NodeType::EXPRESSION || funcArgs[index].type == NodeType::TERM){
                    genDict[var.value.value] = evalExpr(funcArgs[index]);
                    index++;
                }
                else {
                    genDict[var.value.value] = funcArgs[index];
                    index++;
                }
                
            }
        }
        else if (child.type == NodeType::FUNCTION_BODY){
            funcBody = child;
        }
    }
    

    for (auto child : funcBody.children)
    {
        interpret(child);
        if (child.type == NodeType::BREAK && child.value.value == "return")
        {

            // returning the first out of the return var list. temp.
            for (auto returnNode : child.children)
            {
                for (auto var : returnNode.children)
                {
                    if (var.type == NodeType::FACTOR)
                    {
                        return var;
                    }
                    if (var.type == NodeType::IDENTIFIER)
                    {
                        return genDict[var.value.value];
                    }
                }
                
            }
        }
    }

    return AST::Node();
}