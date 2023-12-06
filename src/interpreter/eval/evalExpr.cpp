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

            for (auto child : newNode.children)
            {
                if (child.type == NodeType::IDENTIFIER)
                {
                    listName = child.value.value;
                }
                else if (child.type == NodeType::FACTOR)
                {
                    if (child.value.type == TokenType::INTEGER)
                    {
                        index = std::stoi(child.value.value);
                    }
                    else if (child.value.type == TokenType::IDENTIFIER)
                    {
                        index = std::stoi(genDict[child.value.value].value.value);
                    }
                    else
                    {
                        throw std::runtime_error("Interpreter error: Invalid index type");
                    }
                }
            }
            AST::Node listValue = genDict[listName];

            if (index >= listValue.children.size())
            {
                throw std::runtime_error("Interpreter error: Index out of range");
            }

            int i = 0;
            for (auto child : listValue.children)
            {
                if (i == index)
                {
                    return child;
                }
                i++;
            }
        }

        if(newNode.value.type == TokenType::IDENTIFIER){
            return substitude(newNode);
        }

        return newNode;
    }

    throw std::runtime_error("Interpreter error: Invalid type for substitude");
}

AST::Node Interpreter::evalExpr(AST::Node node)
{
    if (isOperator(node))
    {
        AST::Node child1 = node.children.front();
        AST::Node child2 = node.children.back();

        if (isOperator(child1))
        {
            child1 = evalExpr(child1);
        }

        if (isOperator(child2))
        {
            child2 = evalExpr(child2);
        }

        if (child1.value.type == TokenType::IDENTIFIER)
        {
            child1 = substitude(child1);
        }

        if (child2.value.type == TokenType::IDENTIFIER)
        {
            child2 = substitude(child2);
        }

        return operate(child1, child2, node);
    }

    throw std::runtime_error("Interpreter error: Invalid type for evalExpr");
}

AST::Node Interpreter::operate(AST::Node oprnd1, AST::Node oprnd2, AST::Node oprtr)
{
    if (isNumber(oprnd1) && isNumber(oprnd2))
    {
        if(oprnd1.value.type == TokenType::INTEGER && oprnd2.value.type == TokenType::INTEGER){
            if (oprtr.value.type == TokenType::PLUS)
            {
                return AST::Node{NodeType::FACTOR, Token{TokenType::INTEGER, std::to_string(stoi(oprnd1.value.value) + stoi(oprnd2.value.value))}};
            }
            if (oprtr.value.type == TokenType::MINUS)
            {
                return AST::Node{NodeType::FACTOR, Token{TokenType::INTEGER, std::to_string(stoi(oprnd1.value.value) - stoi(oprnd2.value.value))}};
            }
            if (oprtr.value.type == TokenType::MULTIPLY)
            {
                return AST::Node{NodeType::FACTOR, Token{TokenType::INTEGER, std::to_string(stoi(oprnd1.value.value) * stoi(oprnd2.value.value))}};
            }
            if (oprtr.value.type == TokenType::DIVIDE)
            {
                return AST::Node{NodeType::FACTOR, Token{TokenType::REAL, std::to_string(stod(oprnd1.value.value) / stod(oprnd2.value.value))}};
            }
        }
        else {
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
        }

        if (oprtr.value.type == TokenType::LESSOREQUAL)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::BOOL, (std::to_string(stod(oprnd1.value.value) <= stod(oprnd2.value.value)) == "1") ? "true" : "false"}}; // WARNING!!!
        }

        if (oprtr.value.type == TokenType::MOREOREQUAL)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::BOOL, (std::to_string(stod(oprnd1.value.value) >= stod(oprnd2.value.value)) == "1") ? "true" : "false"}}; // WARNING!!!
        }
        if (oprtr.value.type == TokenType::LESS)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::BOOL, (std::to_string(stod(oprnd1.value.value) < stod(oprnd2.value.value)) == "1") ? "true" : "false"}}; // WARNING!!!
        }
        if (oprtr.value.type == TokenType::MORE)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::BOOL, (std::to_string(stod(oprnd1.value.value) > stod(oprnd2.value.value)) == "1") ? "true" : "false"}}; // WARNING!!!
        }
    }

    std::cout << "oprnd1: " << oprnd1.value.value << '\n';
    std::cout << "oprnd2: " << oprnd2.value.value << '\n';
    std::cout << "oprtr: " << oprtr.value.value << '\n';

    throw std::runtime_error("Interpreter error: Invalid type for operate");
}