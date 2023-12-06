#include "interpreter.h"
#include "printer.h"

void Interpreter::printTypeCheck(AST::Node callNode)
{
    AST::Node typeNode = AST::Node();
    AST::Node valueNode = AST::Node();

    for (auto child : callNode.children)
    {
        if (child.type == NodeType::IDENTIFIER)
        {
            valueNode = genDict[child.value.value];
        }
        else if (child.type == NodeType::FACTOR)
        {
            typeNode = child;
        }
    }

    if (valueNode.type == NodeType::IDENTIFIER)
    {
        valueNode = substitude(valueNode);
    }

    if (valueNode.type == NodeType::LISTCALL)
    {
        valueNode = substitude(valueNode);
    }

    if (valueNode.type == NodeType::EXPRESSION || valueNode.type == NodeType::TERM)
    {
        valueNode = evalExpr(valueNode);
    }



    if (valueNode.value.type == TokenType::INTEGER)
    {
        if (typeNode.value.value == "int")
        {
            std::cout << "true";
        }
        else
        {
            std::cout << "false";
        }
    }

    else if (valueNode.value.type == TokenType::REAL)
    {
        if (typeNode.value.value == "real")
        {
            std::cout << "true";
        }
        else
        {
            std::cout << "false";
        }
    }

    else if (valueNode.value.type == TokenType::STRING)
    {
        if (typeNode.value.value == "string")
        {
            std::cout << "true";
        }
        else
        {
            std::cout << "false";
        }
    }

    else if (valueNode.value.type == TokenType::BOOL)
    {
        if (typeNode.value.value == "bool")
        {
            std::cout << "true";
        }
        else
        {
            std::cout << "false";
        }
    }

    else if (valueNode.type == NodeType::TUPLE)
    {
        if (typeNode.value.value == "tuple")
        {
            std::cout << "true";
        }
        else
        {
            std::cout << "false";
        }
    }

    else if (valueNode.type == NodeType::LIST)
    {
        if (typeNode.value.value == "list")
        {
            std::cout << "true";
        }
        else
        {
            std::cout << "false";
        }
    }
    else if (valueNode.type == NodeType::FUNCTION){
        if (typeNode.value.value == "func")
        {
            std::cout << "true";
        }
        else
        {
            std::cout << "false";
        }
    }
    else
    {
        throw std::runtime_error("Interpreter error: Invalid type");
    }
}