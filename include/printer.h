#pragma once

#include "semantic.h"
#include <fstream>

class Printer{
    std::ofstream fout;
public:

    Printer();
    Printer(std::string p);
    ~Printer();

    std::string getNodeTypeName(NodeType nodetype);

    std::string getTokenTypeName(TokenType tokenType);

    void printTokens(std::list<Token> tokens);

    void printAST(AST::Node node, std::string tab = "  ");   
};