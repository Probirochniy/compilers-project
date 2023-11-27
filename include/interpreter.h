#pragma once

#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include <map>


class Interpreter {
    std::map<std::string, AST::Node> genDict;
public:

    Interpreter();
    
    void runProgram(AST::Node node);

    void interpret(AST::Node node);

    void printer(AST::Node node);  // genDict[var_name] list/tuple/func.call-> (int, real, string, list, tuple) 
    void printIRS(AST::Node node);
    void printList(AST::Node node);
    void printTuple(AST::Node node);
    void runIf(AST::Node node);
    // bool isPrintable(AST::Node node);
    
    AST::Node evalExpr(AST::Node); // +-*/
    bool evalBool(AST::Node); // and or <,>,=,<=,>=,

};