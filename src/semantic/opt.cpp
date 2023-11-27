#include "parser.h"
#include "semantic.h"
#include "printer.h"

bool Semantic::isOperator(NodeType nt)
{ // +-*/ |&^
    return nt == NodeType::EXPRESSION || nt == NodeType::TERM;
}

AST::Node Semantic::optimize(AST::Node node)
{
    ///// Takes some node
    ///// Pattern matches and simplifies it
    ///// Returns the simplified node

    AST::Node newNode = node;

    if (isOperator(node.type))
    {
        newNode = simplifyExpr(node);
    }

    if (!isOperator(node.type))
    {
        newNode = substitude(node);
    }

    return newNode;
}

AST::Node Semantic::simplifyExpr(AST::Node node)
{
    ///// Takes an expression node
    ///// Simplifies it
    ///// Returns the simplified node

    if (node.children.size() != 2 && (node.type == NodeType::EXPRESSION || node.type == NodeType::TERM || node.type == NodeType::FACTOR))
    {
        throw std::runtime_error("Semantic error: invalid expression");
    }

    AST::Node child1 = node.children.front();
    AST::Node child2 = node.children.back();

    if (isOperator(child1.type))
    {
        child1 = simplifyExpr(child1);
    }

    if (isOperator(child2.type))
    {
        child2 = simplifyExpr(child2);
    }

    Printer printer;

    if (child1.value.type == TokenType::IDENTIFIER)
    {
        child1 = substitude(child1);
    }

    if (child2.value.type == TokenType::IDENTIFIER)
    {
        child2 = substitude(child2);
    }

    Printer printing;

    std::cout << "child1: ";
    std::cout << std::endl;
    printing.printAST(child1);
    std::cout << std::endl;
    std::cout << "child2: ";
    printing.printAST(child2);
    std::cout << std::endl;

    if (areTypesCompatible(child1.value.type, child2.value.type))
    {
        AST::Node newNode = operate(child1, child2, node);
        return newNode;
    }

    return node;
}

AST::Node Semantic::substitude(AST::Node node)
{
    ///// Takes a node
    ///// Substitudes variables in it
    ///// Returns the substituted node

    AST::Node newNode = node;

    if (node.value.type == TokenType::IDENTIFIER)
    {
        for (auto var : declarList)
        {
            if (var.name == node.value.value)
            {
                newNode = var.value;
                return newNode;
            }
        }
        throw std::runtime_error("Semantic error: undeclared variable use");
    }

    return node;
}

bool Semantic::areTypesCompatible(TokenType lt, TokenType rt)
{
    TokenType numx[7] = {TokenType::INTEGER, TokenType::REAL, TokenType::INTEGER, TokenType::REAL, TokenType::STRING, TokenType::BOOL, TokenType::LIST};
    TokenType numy[7] = {TokenType::REAL, TokenType::INTEGER, TokenType::INTEGER, TokenType::REAL, TokenType::STRING, TokenType::BOOL, TokenType::LIST};
    for (int i = 0; i < 7; i++)
    {
        if (numx[i] == lt && numy[i] == rt)
            return true;
    }

    throw std::runtime_error("Semantic error: incompatible types");
}

bool isNum(TokenType t)
{
    return t == TokenType::INTEGER || t == TokenType::REAL;
}

std::string boolToString(bool b)
{
    return b ? "true" : "false";
}

AST::Node Semantic::operate(AST::Node oprnd1, AST::Node oprnd2, AST::Node oprtr)
{

    Token retToken;
    if (!areTypesCompatible(oprnd1.value.type, oprnd2.value.type))
    {
        throw std::runtime_error("Semantic error: operands are not compatible!");
    }

    if (isNum(oprnd1.value.type) && isNum(oprnd2.value.type))
    {
        if (oprtr.value.type == TokenType::PLUS)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::REAL, std::to_string(stoi(oprnd1.value.value) + stoi(oprnd2.value.value))}};
        }

        if (oprtr.value.type == TokenType::MINUS)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::REAL, std::to_string(stoi(oprnd1.value.value) - stoi(oprnd2.value.value))}};
        }

        if (oprtr.value.type == TokenType::MULTIPLY)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::REAL, std::to_string(stoi(oprnd1.value.value) * stoi(oprnd2.value.value))}};
        }

        if (oprtr.value.type == TokenType::DIVIDE)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::REAL, std::to_string(stoi(oprnd1.value.value) / stoi(oprnd2.value.value))}};
        }


        if (oprtr.value.type == TokenType::MOREOREQUAL){
            return AST::Node{NodeType::FACTOR, Token{TokenType::BOOL, boolToString((stoi(oprnd1.value.value) >= stoi(oprnd2.value.value)))}};
        }
        if (oprtr.value.type == TokenType::LESSOREQUAL){
            return AST::Node{NodeType::FACTOR, Token{TokenType::BOOL, boolToString((stoi(oprnd1.value.value) <= stoi(oprnd2.value.value)))}};
        }
        if (oprtr.value.type == TokenType::MORE){
            return AST::Node{NodeType::FACTOR, Token{TokenType::BOOL, boolToString((stoi(oprnd1.value.value) > stoi(oprnd2.value.value)))}};
        }
        if (oprtr.value.type == TokenType::LESS){
            return AST::Node{NodeType::FACTOR, Token{TokenType::BOOL, boolToString((stoi(oprnd1.value.value) < stoi(oprnd2.value.value)))}};
        }
        if (oprtr.value.type == TokenType::UNEQUALITY){
            return AST::Node{NodeType::FACTOR, Token{TokenType::BOOL, boolToString((stoi(oprnd1.value.value) != stoi(oprnd2.value.value)))}};
        }
    }

    if (oprnd1.value.type == TokenType::LIST && oprnd2.value.type == TokenType::LIST)
    {
        if (oprtr.value.type == TokenType::PLUS)
        {
            AST::Node newList = AST::Node{NodeType::LIST, Token{TokenType::LIST, ""}};
            for (auto el : oprnd1.children)
            {
                newList.children.push_back(el);
            }
            for (auto el : oprnd2.children)
            {
                newList.children.push_back(el);
            }
            return newList;
        }
    }

    if (oprnd1.value.type == TokenType::STRING && oprnd2.value.type == TokenType::STRING)
    {
        if (oprtr.value.type == TokenType::PLUS)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::STRING, oprnd1.value.value + oprnd2.value.value}};
        }
    }

    if (oprnd1.value.type == TokenType::BOOL && oprnd2.value.type == TokenType::BOOL)
    {
        if (oprtr.value.type == TokenType::KEYWORD && oprtr.value.value == "or")
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::BOOL, oprnd1.value.value == "true" || oprnd2.value.value == "true" ? "true" : "false"}};
        }

        if (oprtr.value.type == TokenType::KEYWORD && oprtr.value.value == "and")
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::BOOL, oprnd1.value.value == "true" && oprnd2.value.value == "true" ? "true" : "false"}};
        }
    }

    if ((isNum(oprnd1.value.type) && isNum(oprnd2.value.type)) || (oprnd1.value.type == TokenType::STRING && oprnd2.value.type == TokenType::STRING) || (oprnd1.value.type == TokenType::BOOL && oprnd2.value.type == TokenType::BOOL))
    {
        if (oprtr.value.type == TokenType::EQUALITY)
        {
            return AST::Node{NodeType::FACTOR, Token{TokenType::BOOL, oprnd1.value.value == oprnd2.value.value ? "true" : "false"}};
        }
    }

    return AST::Node{NodeType::FACTOR, oprnd1.value};
}