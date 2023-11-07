#include "semantic.h"
#include "parser.h"

Semantic::Semantic(AST::Node node)
    : node(node)
{
}

Variable::Variable(std::string n, TokenType t, AST::Node v)
: name(n), type(t), value(v)
{
}

AST::Node Semantic::analyze()
{
    AST::Node newNode = node;

    checkProgram(newNode);

    return newNode;
}