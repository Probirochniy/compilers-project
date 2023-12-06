#include "interpreter.h"
#include "printer.h"

AST::Node Interpreter::listCall(AST::Node callNode){
    std::string listName = "";
    int index = 0;

    for (auto child : callNode.children){
        if (child.type == NodeType::IDENTIFIER){
            listName = child.value.value;
        }
        else if (child.type == NodeType::FACTOR){
            if (child.value.type == TokenType::INTEGER){
                index = std::stoi(child.value.value);
            }
            else if (child.value.type == TokenType::IDENTIFIER){
                index = std::stoi(genDict[child.value.value].value.value);
            }
            else {
                throw std::runtime_error("Interpreter error: Invalid index type");
            }
            
        }
    }
    AST::Node listValue = genDict[listName];

    if (index >= listValue.children.size()){
        throw std::runtime_error("Interpreter error: Index out of range");
    }

    int i = 0;
    for (auto child : listValue.children){
        if (i == index){
            return child;
        }
        i++;
    }

    return AST::Node();
}   
