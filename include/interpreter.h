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
    void printListCall(AST::Node node);
    void printTupleCall(AST::Node node);
    void printTypeCheck(AST::Node node);
    void runIf(AST::Node node);
    void runFor(AST::Node node);
    void runWhile(AST::Node node);
    AST::Node runFunction(AST::Node node);
    // bool isPrintable(AST::Node node);
    AST::Node evalExpr(AST::Node node); // +-*/
    bool evalBool(AST::Node); // and or <,>,=,<=,>=,
    AST::Node substitude(AST::Node node);
    AST::Node operate(AST::Node oprnd1, AST::Node oprnd2, AST::Node oprtr);

    AST::Node listCall(AST::Node node);
    AST::Node tupleCall(AST::Node node);

};
