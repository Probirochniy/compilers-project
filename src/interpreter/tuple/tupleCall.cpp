#include "interpreter.h"
#include "printer.h"

AST::Node Interpreter::tupleCall(AST::Node callNode)
{
    std::string tupleName = "";
    AST::Node indexNode = AST::Node();

    for (auto child : callNode.children)
    {
        if (child.type == NodeType::IDENTIFIER)
        {
            tupleName = child.value.value;
        }
        else if (child.type == NodeType::FACTOR)
        {
            if (child.value.type == TokenType::INTEGER)
            {
                indexNode = child;
            }
            else if (child.value.type == TokenType::IDENTIFIER)
            {
                indexNode = child;
            }
            else
            {
                throw std::runtime_error("Interpreter error: Invalid index type");
            }
        }
    }

    AST::Node tupleValue = genDict[tupleName];

    if (indexNode.value.type == TokenType::INTEGER)
    {
        int index = std::stoi(indexNode.value.value);
        if (index >= tupleValue.children.size() + 1)
        {
            throw std::runtime_error("Interpreter error: Index out of range");
        }

        int i = 1;
        for (auto child : tupleValue.children)
        {
            if (i == index)
            {
                printer(child);
            }
            i++;
        }
    }
    else {
        std::string indexName = indexNode.value.value;

        for (auto child : tupleValue.children)
        {
            if (child.type == NodeType::DECLARATION)
            {
                bool flag = false;
                for (auto declVar : child.children)
                {

                    if (declVar.type == NodeType::DECLARATION_LEFT)
                    {
                        std::string varName = "";
                        for (auto var : declVar.children)
                        {
                            if (var.type == NodeType::IDENTIFIER)
                            {
                                varName = var.value.value;
                            }
                        }

                        if (varName == indexName)
                        {
                            flag = true;
                        }
                    }

                    if (declVar.type == NodeType::DECLARATION_RIGHT && flag)
                    {
                        for (auto var : declVar.children)
                        {
                            return var;
                        }
                    }
                    
                }
            }
        }
    }

    return AST::Node();
}
