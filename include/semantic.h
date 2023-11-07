#pragma once

#include "lexer.h"
#include "parser.h"

class Variable
{
public:
    std::string name;
    TokenType type;
    AST::Node value;

    Variable(std::string n, TokenType t, AST::Node v);
};

class Semantic
{
public:
    AST::Node node;
    std::list<Variable> declarList;
    

    Semantic(AST::Node n);

    AST::Node analyze();

    void checkProgram(AST::Node node);
    void checkDeclar(AST::Node node, std::list<Variable> &localDeclarList);
    void checkFunc(AST::Node node, std::list<Variable> localDeclarList);
    void checkFuncBody(AST::Node node, std::list<Variable> &localDeclarList);
    void checkIdent(AST::Node node, std::list<Variable> localDeclarList);


    void checkTypes(AST::Node node);


    
    AST::Node optimize(AST::Node node);
    AST::Node simplifyConst(AST::Node node);
    AST::Node removeUnused(AST::Node node);
    AST::Node inlineFunc(AST::Node node);
};
