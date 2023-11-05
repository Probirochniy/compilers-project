#pragma once

#include "lexer.h"
#include "parser.h"

class Variable
{
public:
    std::string name;
    TokenType type;

    std::string value;


    Variable(std::string n, TokenType t, std::string v);
};

class Semantic
{
public:
    AST::Node node;
    std::list<Variable> declarList;
    

    Semantic(AST::Node n);

    AST::Node analyze();

    void checkDeclar(AST::Node node);
    AST::Node optimize(AST::Node node);
    void checkTypes(AST::Node node);
};
