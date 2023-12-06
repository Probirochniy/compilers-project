#include "semantic.h"
#include "parser.h"
#include "interpreter.h"
#include "printer.h"

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

        // filling in the values
        for (int i = 0; i < var_names.size(); i ++) {
            if(var_values[i].type == NodeType::LISTCALL){
                var_values[i] = listCall(var_values[i]);
            }
            else if (var_values[i].type == NodeType::FUNCTIONCALL){
                var_values[i] = runFunction(var_values[i]);
            }
            else if (var_values[i].type == NodeType::TUPLECALL){
                var_values[i] = tupleCall(var_values[i]);
            }
            else if (var_values[i].type == NodeType::EXPRESSION || var_values[i].type == NodeType::TERM){
                var_values[i] = evalExpr(var_values[i]);
            }



            genDict[var_names[i]] = var_values[i];
            
        }
    }

    if (node.type == NodeType::ASSIGNMENT){
        std::string var_name = "";
        AST::Node var_value;
        for (auto child : node.children){
            if (child.type == NodeType::DECLARATION_LEFT){
                for (auto var : child.children){
                    var_name = var.value.value;
                }
            }
            else if (child.type == NodeType::DECLARATION_RIGHT){
                for (auto var : child.children){
                    var_value = var;
                }
            }
        }

        if (var_value.type == NodeType::EXPRESSION || var_value.type == NodeType::TERM) {
            var_value = evalExpr(var_value);
        }

        if (var_value.type == NodeType::FUNCTIONCALL){
            
            var_value = runFunction(var_value);
        }
        if (var_value.type == NodeType::LISTCALL){

            var_value = listCall(var_value);
        }

        genDict[var_name] = var_value;
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
    if (node.type == NodeType::FORLOOP){
        runFor(node);
    }
    if (node.type == NodeType::WHILELOOP){
        runWhile(node);
    }

    if (node.type == NodeType::FUNCTIONCALL){
        runFunction(node);
    }

}
