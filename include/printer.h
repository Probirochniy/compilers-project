#pragma once

#include "semantic.h"

namespace printer{
    std::string getNodeTypeName(NodeType nodetype);

    std::string getTokenTypeName(TokenType tokenType);

    void printTokens(std::list<Token> tokens);

    void printAST(AST::Node node, std::string tab = "  ");   
}