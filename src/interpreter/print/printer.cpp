#include "semantic.h"
#include "parser.h"
#include "interpreter.h"
#include "printer.h"

void Interpreter::printer(AST::Node node)
{

    if (node.type == NodeType::FACTOR)
    {
        if (node.value.type == TokenType::INTEGER || node.value.type == TokenType::REAL || node.value.type == TokenType::STRING || node.value.type == TokenType::BOOL)
        {
            printIRS(node);
        }
        else if (node.value.type == TokenType::IDENTIFIER)
        {
            printer(genDict[node.value.value]);
        }
    }
    else if (node.type == NodeType::LIST)
    {
        printList(node);
    }
    else if (node.type == NodeType::TUPLE)
    {
        // std::cout << "Tuple";
        printTuple(node);
    }
    else if (node.type == NodeType::DECLARATION)
    {

        for (auto child : node.children)
        {

            if (child.type == NodeType::DECLARATION_RIGHT)
            {
                for (auto var : child.children)
                {
                    printer(var);
                }
            }
        }
    }
    else if (node.type == NodeType::IDENTIFIER)
    {
        printer(genDict[node.value.value]);
    }

    else if (node.type == NodeType::LISTCALL)
    {
        printListCall(node);
    }

    else if (node.type == NodeType::EXPRESSION){
        
        std::cout << evalExpr(node).value.value;
    }

    else
    {
        std::runtime_error("Interpreter error: Invalid type for print statement");
    }
}