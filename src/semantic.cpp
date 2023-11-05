#include "semantic.h"
#include "parser.h"

Semantic::Semantic(AST::Node node)
    : node(node)
{
}

AST::Node Semantic::analyze()
{
    AST::Node newNode = node;

    return newNode;
}


