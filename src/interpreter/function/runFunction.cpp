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

        if (child.type == NodeType::DECLARATION){
            for (auto gchild : child.children){
                if (gchild.type == NodeType::DECLARATION_LEFT){
                    for (auto var : gchild.children){
                        funcArgs.push_back(var);
                    }
                }
            }
        }

        interpret(child);
        if (child.type == NodeType::BREAK && child.value.value == "return")
        {

            for (auto arg : funcArgs){
                genDict.erase(arg.value.value);
            }

            // returning the first out of the return var list
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
