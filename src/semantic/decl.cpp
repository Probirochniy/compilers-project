#include "semantic.h"
#include "parser.h"

void Semantic::checkDeclar(AST::Node node)
{

    if (node.type == NodeType::DECLARATION)
    {

        std::list<std::string> nameList;
        std::list<TokenType> typeList;
        std::list<std::string> valueList;

        for (auto child : node.children)
        {
            if (child.type == NodeType::DECLARATION_LEFT)
            {
                for (auto grandchild : child.children)
                {
                    if (grandchild.value.type == TokenType::IDENTIFIER)
                    {
                        nameList.push_back(grandchild.value.value);
                    }
                    else
                    {
                        throw std::runtime_error("Semantic error: unexpected value at definition!");
                    }
                }
            }

            if (child.type == NodeType::DECLARATION_RIGHT)
            {
                for (auto grandchild : child.children)
                {
                    if (grandchild.type == NodeType::IDENTIFIER)
                    {
                        bool inList = false;

                        for (auto declar : declarList)
                        {
                            if (declar.name == grandchild.value.value)
                            {
                                inList = true;
                            }
                        }

                        if (!inList){
                            throw std::runtime_error("Semantic error: undeclared variable!");
                        }
                    }
                    typeList.push_back(grandchild.value.type);
                    valueList.push_back(grandchild.value.value);
                }
            }
        }

        for (int i = 0; i < nameList.size(); i++)
        {
            declarList.push_back(Variable{nameList.front(), typeList.front(), valueList.front()});
            nameList.pop_front();
            typeList.pop_front();
            valueList.pop_front();
        }
        return;
    }

    if (node.value.type == TokenType::IDENTIFIER)
    {
        bool inList = false;

        for (auto declar : declarList)
        {
            if (declar.name == node.value.value)
            {
                inList = true;
            }
        }

        if (!inList){
            throw std::runtime_error("Semantic error: undeclared variable!");
        }
    }

    for (auto child : node.children)
    {
        checkDeclar(child);
    }
}