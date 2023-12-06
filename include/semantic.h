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

    AST::Node checkProgram(AST::Node node);
    AST::Node checkFor(AST::Node node, std::list<Variable> localDeclarList);
    void checkForBody(AST::Node, std::list<Variable> &localDeclarList);
    AST::Node checkDeclar(AST::Node node, std::list<Variable> &localDeclarList);
    AST::Node checkFunc(AST::Node node, std::list<Variable> localDeclarList);
    AST::Node checkExpr(AST::Node node, std::list<Variable> localDeclarList);
    AST::Node checkTerm(AST::Node node, std::list<Variable> localDeclarList);   
    void checkFuncBody(AST::Node node, std::list<Variable> &localDeclarList);
    AST::Node checkIdent(AST::Node node, std::list<Variable> localDeclarList);
    AST::Node checkAssignment(AST::Node node, std::list<Variable> localDeclarList);
    AST::Node checkList(AST::Node node, std::list<Variable> localDeclarList);
    AST::Node checkListCall(AST::Node node, std::list<Variable> localDeclarList);

    AST::Node checkIf(AST::Node node, std::list<Variable> &localDeclarList);

    AST::Node checkWhile(AST::Node node, std::list<Variable> localDeclarList);


    void checkTypes(AST::Node node);

    bool isOperator(NodeType nt);
    
    AST::Node optimize(AST::Node node);
    AST::Node simplifyExpr(AST::Node node);
    AST::Node substitude(AST::Node node);
    AST::Node removeUnused(AST::Node node);
    AST::Node inlineFunc(AST::Node node);
    bool areTypesCompatible(TokenType lt, TokenType rt);

    AST::Node operate(AST::Node oprnd1, AST::Node oprnd2, AST::Node oprtr);
    bool areTypesNumber(TokenType lt, TokenType rt);
};
