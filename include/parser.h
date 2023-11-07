#pragma once

#include "lexer.h"
#include <algorithm>

enum class NodeType
{
    PROGRAM,
    EXPRESSION,
    TERM,
    FACTOR,
    IDENTIFIER,
    VARIABLE,
    CONSTANT,
    DECLARATION,
    DECLARATION_LEFT,
    DECLARATION_RIGHT,
    IF_STATEMENT,
    IF_BODY,
    IF_COND,
    FUNCTION,
    FUNCTION_VARIABLES,
    FUNCTION_BODY,
    FORLOOP,
    WHILELOOP,
    RANGE,
    BODY,
    VARS_LIST,
    COMMENT,
    LIST,
    LIST_ELEMENT,
    TUPLE,
    TUPLE_ELEMENT,
    FORLOOP_BODY,
    PRINT,
    WHILELOOP_BODY,
    ELSE_BODY,
    RETURN,
    BREAK
};

class AST
{
public:
    class Node
    {
    public:
        NodeType type;
        std::list<Node> children;
        Token value;

        Node();

        Node(NodeType t);

        Node(NodeType t, Token v);

        // Node& operator=(Node);
    };

    Node node;

    AST();
};

class Parser : public Lexer
{
    std::list<Token> tokens;
    AST tree;
    std::list<Token> curTokensList;
    Token curToken;
public:
    Parser(std::list<Token> tokenCollection);

    Token getToken();

    bool getCurToken();

    AST::Node parseFunction();

    AST::Node parseExprList();
    AST::Node parseVarsList();
    AST::Node parseForLoop();
    AST::Node parseIf();
    AST::Node parseWhileLoop();
    AST::Node parseCond();
    AST::Node parseAssignment();
    AST::Node parseTuple();
    AST::Node parseList();
    // AST::Node parseList();
    std::list<Token> extractList();
    std::list<Token> extractFunction();
    AST::Node getNode();
    std::list<AST::Node> parseBody();
    AST::Node analyze();
    AST::Node makeTree(NodeType nodetype, Token value, std::list<AST::Node> children);
    AST::Node parseExpr();
    AST::Node parseTerm();
    AST::Node parseFactor();
};
