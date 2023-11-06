#include "printer.h"

Printer::Printer(){}
Printer::Printer(std::string p) {
    for (int i = p.size() - 1; i >= 0; i --) {
        if(p[i] == '/') {
            p = p.substr(0, i + 1);
            break;
        }
    }
    fout.open(p + "out.adi");
    std::cout << p + "out.adi";
}
Printer::~Printer(){
    fout.close();
}

std::string Printer::getNodeTypeName(NodeType nodetype)
{
    std::string typeName;

    switch (nodetype)
    {
    case NodeType::PROGRAM:
        typeName = "PROGRAM";
        break;

    case NodeType::DECLARATION:
        typeName = "DECLARATION";
        break;

    case NodeType::FACTOR:
        typeName = "FACTOR";
        break;

    case NodeType::CONSTANT:
        typeName = "CONSTANT";
        break;

    case NodeType::TERM:
        typeName = "TERM";
        break;

    case NodeType::VARIABLE:
        typeName = "VARIABLE";
        break;

    case NodeType::DECLARATION_LEFT:
        typeName = "DECLARATION_LEFT";
        break;

    case NodeType::DECLARATION_RIGHT:
        typeName = "DECLARATION_RIGHT";
        break;

    case NodeType::IF_BODY:
        typeName = "IF_BODY";
        break;

    case NodeType::IF_COND:
        typeName = "IF_COND";
        break;

    case NodeType::IF_STATEMENT:
        typeName = "IF_STATEMENT";
        break;

    case NodeType::EXPRESSION:
        typeName = "EXPRESSION";
        break;

    case NodeType::FUNCTION:
        typeName = "FUNCTION";
        break;

    case NodeType::FUNCTION_BODY:
        typeName = "FUNCTION_BODY";
        break;

    case NodeType::FUNCTION_VARIABLES:
        typeName = "FUNCTION_VARIABLES";
        break;

    case NodeType::FORLOOP:
        typeName = "FORLOOP";
        break;

    case NodeType::WHILELOOP:
        typeName = "WHILELOOP";
        break;

    case NodeType::RANGE:
        typeName = "RANGE";
        break;

    case NodeType::VARS_LIST:
        typeName = "VARS_LIST";
        break;

    case NodeType::LIST:
        typeName = "LIST";
        break;

    case NodeType::LIST_ELEMENT:
        typeName = "LIST_ELEMENT";
        break;

    case NodeType::TUPLE:
        typeName = "TUPLE";
        break;

    case NodeType::TUPLE_ELEMENT:
        typeName = "TUPLE_ELEMENT";
        break;

    case NodeType::FORLOOP_BODY:
        typeName = "FORLOOP_BODY";
        break;

    case NodeType::PRINT:
        typeName = "PRINT";
        break;

    case NodeType::WHILELOOP_BODY:
        typeName = "WHILELOOP_BODY";
        break;

    case NodeType::ELSE_BODY:
        typeName = "ELSE_BODY";
        break;

    case NodeType::IDENTIFIER:
        typeName = "IDENTIFIER";
        break;
    case NodeType::BODY:
        typeName = "BODY";
        break;
    case NodeType::COMMENT:
        typeName = "COMMENT";
        break;
    }
    return typeName;
}

std::string Printer::getTokenTypeName(TokenType tokenType)
{
    std::string typeName;

    switch (tokenType)
    {
    case TokenType::IDENTIFIER:
        typeName = "IDENTIFIER";
        break;
    case TokenType::COMMA:
        typeName = "COMMA";
        break;
    case TokenType::KEYWORD:
        typeName = "KEYWORD";
        break;
    case TokenType::DELIMITER:
        typeName = "DELIMITER";
        break;
    case TokenType::PLUS:
        typeName = "PLUS";
        break;
    case TokenType::MINUS:
        typeName = "MINUS";
        break;
    case TokenType::MULTIPLY:
        typeName = "MULTIPLY";
        break;
    case TokenType::DIVIDE:
        typeName = "DIVIDE";
        break;
    case TokenType::END_OF_FILE:
        typeName = "END_OF_FILE";
        break;
    case TokenType::COMMENT:
        typeName = "COMMENT";
        break;
    case TokenType::DEFINITION:
        typeName = "DEFINITION";
        break;
    case TokenType::OPENBRACKET:
        typeName = "OPENBRACKET";
        break;
    case TokenType::CLOSEBRACKET:
        typeName = "CLOSEBRACKET";
        break;
    case TokenType::OPENSQUAREBRACKET:
        typeName = "OPENSQUAREBRACKET";
        break;
    case TokenType::CLOSESQUAREBRACKET:
        typeName = "CLOSESQUAREBRACKET";
        break;
    case TokenType::OPENFIGUREBRACKET:
        typeName = "OPENFIGUREBRACKET";
        break;
    case TokenType::CLOSEFIGUREBRACKET:
        typeName = "CLOSEFIGUREBRACKET";
        break;
    case TokenType::RANGE:
        typeName = "RANGE";
        break;
    case TokenType::UNKNOWNTYPE:
        typeName = "UNKNOWNTYPE";
        break;
    case TokenType::DOT:
        typeName = "DOT";
        break;
    case TokenType::COLON:
        typeName = "COLON";
        break;
    case TokenType::EQUAL:
        typeName = "EQUAL";
        break;
    case TokenType::EQUALITY:
        typeName = "EQUALITY";
        break;
    case TokenType::UNEQUALITY:
        typeName = "UNEQUALITY";
        break;
    case TokenType::MORE:
        typeName = "MORE";
        break;
    case TokenType::LESS:
        typeName = "LESS";
        break;
    case TokenType::MOREOREQUAL:
        typeName = "MOREOREQUAL";
        break;
    case TokenType::LESSOREQUAL:
        typeName = "LESSOREQUAL";
        break;
    case TokenType::FOLLOWING:
        typeName = "FOLLOWING";
        break;
    case TokenType::EXCLAMATION:
        typeName = "EXCLAMATION";
        break;
    case TokenType::STRING:
        typeName = "STRING";
        break;
    case TokenType::INTEGER:
        typeName = "INTEGER";
        break;
    case TokenType::REAL:
        typeName = "REAL";
        break;
    }

    return typeName;
}


void Printer::printTokens(std::list<Token> tokens)
{
    for (auto token : tokens)
    {
        fout << Printer::getTokenTypeName(token.type) << " |" << token.value << "|\n";
    }
}

void Printer::printAST(AST::Node node, std::string tab)
{
    std::string TAB = "   ";
    fout << tab << "{\n";
    std::string t = tab;
    tab += TAB;
    fout << tab << "\"Token\": {\"type\": \"" << Printer::getTokenTypeName(node.value.type) << "\", \"value\": \"" << node.value.value << "\"}," << '\n';
    fout << tab << "\"Type\": \"" << Printer::getNodeTypeName(node.type) << "\",\n";
    if (node.children.empty())
    {
        fout << tab << "\"children\": []\n";
    }
    else
    {
        fout << tab << "\"children\": [\n";
        for (auto n : node.children)
        {
            printAST(n, tab + TAB);
        }
        fout << tab << "]\n";
    }
    fout << t << "},\n";
}