#include "semantic.h"
#include "parser.h"


/*
Type-checks: A compiler should report an error if an operator is applied to an incompatible operand. For example, adding an array variable and a function variable together would generate an error.

Flow of control checks: Statements that cause the flow of control to leave a construct must have someplace to which to transfer the flow of control. For example, a break statement in C++ causes control to leave the smallest enclosing while, for, or switch statement. An error occurs if such an enclosing statement does not exist.

Uniqueness checks: There are situations in which an object must be defined only once. For example, in C++, an identifier must be declared uniquely.

Name-related checks: Sometimes the same name may appear two or more times. The compiler must check that the same name is used at both places.

Return type checks: The compiler checks the return types of functions to ensure they match their declarations. For example, returning a string from a function that's declared to return an int would generate an error.
*/
void Semantic::checkTypes(AST::Node node)
{
    if (node.type == NodeType:: EXPRESSION)
    {
        if(node.value.type == TokenType:: PLUS)
        {
        for (auto child : node.children)
            {
                if (child.type == NodeType::FACTOR)
                {
                    typeList.push_back(child.value);
                }
            }
        }
        if(node.value.type == TokenType:: MINUS)
        {
        for (auto child : node.children)
            {
                if (child.type == NodeType::FACTOR) (child.value.type != TokenType:: INTEGER && child.value.type !=  TokenType:: REAL))
                {
                    throw std::runtime_error("Semantic error");
                }
            }
        }
        typeList.push_back(node.value);
    }
    if (node.type == NodeType::TERM)
    {
        for (auto child : node.children)
            {
                if (child.type == NodeType::FACTOR && (child.value.type != TokenType:: INTEGER && child.value.type !=  TokenType:: REAL))
                {
                    throw std::runtime_error("Semantic error");
                }
            }
    }
    for (auto child : node.children)
    {
        checkTypes(child);
    }
}