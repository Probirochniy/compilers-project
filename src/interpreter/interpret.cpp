#include "semantic.h"
#include "parser.h"
#include "interpreter.h"

void Interpreter::interpret(AST::Node node)
{
    if (node.type == NodeType::PROGRAM)
    {
        for (auto child : node.children)
        {
            interpret(child);
        }
    }

    if (node.type == NodeType::DECLARATION){
        std::vector<std::string> var_names;
        std::vector<AST::Node> var_values;
        
        for (auto child : node.children) {
            if (child.type == NodeType::DECLARATION_LEFT){
                for (auto var : child.children){
                    var_names.push_back(var.value.value);
                }
            }
            else if (child.type == NodeType::DECLARATION_RIGHT){
                for (auto var : child.children){
                    var_values.push_back(var);
                }
            }
        }

        // filled
        for (int i = 0; i < var_names.size(); i ++) {
            genDict[var_names[i]] = var_values[i];
        }
    }

    if (node.type == NodeType::PRINT)
    {
        for (auto child : node.children)
        {
            if (child.type == NodeType::VARS_LIST)
            {
                for (auto var : child.children)
                {
                    printer(var);
                }
            }
            std::cout << std::endl;
        }
    }

    if (node.type == NodeType::IF_STATEMENT){
        runIf(node);
    }

}
