#include "semantic.h"
#include "parser.h"
#include "interpreter.h"
#include "printer.h"

bool isOperator(AST::Node nt)
{ // +-*/ |&^
    return nt.type == NodeType::EXPRESSION || nt.type == NodeType::TERM;
}

bool isNumber(AST::Node nt)
{ // int, real
    return nt.value.type == TokenType::INTEGER || nt.value.type == TokenType::REAL;
}

AST::Node Interpreter::substitude(AST::Node node)
{
    ///// Takes a node
    ///// Substitudes variables in it
    ///// Returns the substituted node

    AST::Node newNode = node;

    if (node.value.type == TokenType::IDENTIFIER)
    {
        newNode = genDict[node.value.value];

        if (newNode.type == NodeType::LISTCALL)
        {
            std::string listName = "";
            int index = 0;

            for (auto child : newNode.children){
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
        }

        return newNode;
    }

    throw std::runtime_error("Interpreter error: Invalid type for substitude");
}

AST::Node Interpreter::evalExpr(AST::Node node) {
    if (isOperator(node)){
        AST::Node child1 = node.children.front();
        AST::Node child2 = node.children.back();

        if (isOperator(child1)){
            child1 = evalExpr(child1);
        }

        if (isOperator(child2)){
            child2 = evalExpr(child2);
        }

        if (child1.value.type == TokenType::IDENTIFIER){
            child1 = substitude(child1);
        }

        if (child2.value.type == TokenType::IDENTIFIER){
            child2 = substitude(child2);
        }

        return operate(child1, child2, node);

    }

    throw std::runtime_error("Interpreter error: Invalid type for evalExpr");
}

AST::Node Interpreter::operate(AST::Node oprnd1, AST::Node oprnd2, AST::Node oprtr){
    if (isNumber(oprnd1) && isNumber(oprnd2)){
        if (oprtr.value.type == TokenType::PLUS)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::REAL, std::to_string(stod(oprnd1.value.value) + stod(oprnd2.value.value))}};
        }

        if (oprtr.value.type == TokenType::MINUS)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::REAL, std::to_string(stod(oprnd1.value.value) - stod(oprnd2.value.value))}};
        }

        if (oprtr.value.type == TokenType::MULTIPLY)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::REAL, std::to_string(stod(oprnd1.value.value) * stod(oprnd2.value.value))}};
        }

        if (oprtr.value.type == TokenType::DIVIDE)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::REAL, std::to_string(stod(oprnd1.value.value) / stod(oprnd2.value.value))}};
        }

        if (oprtr.value.type == TokenType::LESSOREQUAL){
            return AST::Node{NodeType::FACTOR, Token{TokenType::BOOL, std::to_string(stod(oprnd1.value.value) <= stod(oprnd2.value.value))}};
        }
    }

    std::cout << "oprnd1: " << oprnd1.value.value << '\n';
    std::cout << "oprnd2: " << oprnd2.value.value << '\n';
    std::cout << "oprtr: " << oprtr.value.value << '\n';

    throw std::runtime_error("Interpreter error: Invalid type for operate");
}

// AST::Node Interpreter::operate(Token oprnd1, Token oprnd2, Token oprtr) {
//     Token retToken;
//     if(!areTypesNumber(oprnd1.type, oprnd2.type)) {
//         std::cout << "non-numbers: " << Printer::getTokenTypeName(oprnd1.type) << ' ' << Printer::getTokenTypeName(oprnd2.type) << '\n';
//         throw std::runtime_error("Semantic error: non-number operands");
//     }

//     switch (oprtr.type) {
//     case TokenType::PLUS:
//         retToken = (oprnd1.type == TokenType::INTEGER && oprnd2.type == TokenType::INTEGER) 
//             ? Token{TokenType::INTEGER, std::to_string(stoi(oprnd1.value) + stoi(oprnd2.value))}
//             : Token{TokenType::REAL, std::to_string(stod(oprnd1.value) + stod(oprnd2.value))};
//         break;
//     case TokenType::MINUS:
//         retToken = (oprnd1.type == TokenType::INTEGER && oprnd2.type == TokenType::INTEGER) 
//             ? Token{TokenType::INTEGER, std::to_string(stoi(oprnd1.value) - stoi(oprnd2.value))}
//             : Token{TokenType::REAL, std::to_string(stod(oprnd1.value) - stod(oprnd2.value))};
//         break;
//     case TokenType::MULTIPLY:
//         retToken = (oprnd1.type == TokenType::INTEGER && oprnd2.type == TokenType::INTEGER) 
//             ? Token{TokenType::INTEGER, std::to_string(stoi(oprnd1.value) * stoi(oprnd2.value))}
//             : Token{TokenType::REAL, std::to_string(stod(oprnd1.value) * stod(oprnd2.value))};
//         break;
//     case TokenType::DIVIDE:
//         retToken = (oprnd1.type == TokenType::INTEGER && oprnd2.type == TokenType::INTEGER) 
//             ? Token{TokenType::INTEGER, std::to_string(stoi(oprnd1.value) / stoi(oprnd2.value))}
//             : Token{TokenType::REAL, std::to_string(stod(oprnd1.value) / stod(oprnd2.value))};
//         break;
//     default: // WARNING!!!
//         break;
//     }

//     return AST::Node{NodeType::FACTOR, retToken};
// }