#include "semantic.h"
#include "parser.h"
#include <iostream>
#include <list>

void Semantic::checkDeclar(AST::Node node, std::list<Variable> &declarList){
    std::list<std::string> nameList;
        std::list<TokenType> typeList;
        std::list<AST::Node> valueList;

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

                    bool inList = false;

                    for (auto declar : declarList)
                    {
                        if (declar.name == grandchild.value.value)
                        {
                            inList = true;
                        }
                    }

                    if (inList)
                    {
                        throw std::runtime_error("Semantic error: redefinition of variable!");
                    }
                }
            }

            if (child.type == NodeType::DECLARATION_RIGHT)
            {
                for (auto grandchild : child.children)
                {
                    if (grandchild.value.type == TokenType::IDENTIFIER)
                    {
                        bool inList = false;

                        for (auto declar : declarList)
                        {
                            if (declar.name == grandchild.value.value)
                            {
                                inList = true;

                                ///////// OPTIMIZATION /////////

                                valueList.push_back(declar.value);
                            }
                        }

                        if (!inList)
                        {
                            throw std::runtime_error("Semantic error: undeclared variable: " + grandchild.value.value + "!");
                        }
                    }

                    else if (grandchild.value.type == TokenType::KEYWORD && grandchild.value.value == "func")
                    {

                        checkFunc(grandchild, declarList);
                        valueList.push_back(grandchild);
                    }

                    else
                    {
                        valueList.push_back(grandchild);
                    }

                    typeList.push_back(grandchild.value.type);
                }
            }
        }

        if (nameList.size() != typeList.size() || nameList.size() != valueList.size())
        {
            throw std::runtime_error("Semantic error: incorrect number of variables!");
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

void Semantic::checkFuncBody(AST::Node node, std::list<Variable> &scopeDeclarList)
{
    ///////// DECLARATION /////////
    if (node.type == NodeType::DECLARATION)
    {
        checkDeclar(node, scopeDeclarList);
    }

    if (node.value.type == TokenType::IDENTIFIER)
    {
        bool inList = false;

        for (auto declar : scopeDeclarList)
        {
            if (declar.name == node.value.value)
            {
                inList = true;
            }
        }

        if (!inList)
        {
            throw std::runtime_error("Semantic error: undeclared variable: " + node.value.value + "!");
        }
    }

    if (node.value.type == TokenType::KEYWORD && node.value.value == "return")
    {
        for (auto child : node.children)
        {
            if (child.type == NodeType::VARS_LIST)
            {
                for (auto grandchild : child.children)
                {
                    if (grandchild.value.type == TokenType::IDENTIFIER)
                    {
                        bool inList = false;

                        for (auto declar : scopeDeclarList)
                        {
                            if (declar.name == grandchild.value.value)
                            {
                                inList = true;
                            }
                        }

                        if (!inList)
                        {
                            throw std::runtime_error("Semantic error: undeclared variable: " + grandchild.value.value + "!");
                        }
                    }
                }
            }
        }
    }
}

void Semantic::checkFunc(AST::Node node, std::list<Variable> declarList)
{
    std::list<std::string> nameList;

    for (auto child : node.children)
    {
        if (child.type == NodeType::VARS_LIST)
        {
            for (auto grandchild : child.children)
            {
                if (grandchild.value.type == TokenType::IDENTIFIER)
                {
                    nameList.push_back(grandchild.value.value);
                }
            }
        }

        if (child.type == NodeType::FUNCTION_BODY)
        {
            ///////// OPTIMIZATION /////////
            ///////// DELETE EVERYTHING AFTER RETURN /////////

            bool afterReturn = false;
            AST::Node newNode = AST::Node(NodeType::FUNCTION_BODY, Token());

            for (auto grandchild : child.children)
            {
                if (!afterReturn)
                {
                    newNode.children.push_back(grandchild);
                }

                if (grandchild.value.type == TokenType::KEYWORD && grandchild.value.value == "return")
                {
                    afterReturn = true;
                }
            }

            child = newNode;

            ///////// FUNCTION DECLARCHECK /////////

            std::list<Variable> scopeDeclarList = declarList;

            for (auto name : nameList)
            {
                scopeDeclarList.push_back(Variable{name, TokenType::UNKNOWNTYPE, AST::Node()});
            }

            for (auto grandchild : child.children)
            {
                checkFuncBody(grandchild, scopeDeclarList);
            }
        }
    }
}

void Semantic::checkIdent(AST::Node node, std::list<Variable> declarList)
{
    bool inList = false;

    for (auto declar : declarList)
    {
        if (declar.name == node.value.value)
        {
            inList = true;
        }
    }

    if (!inList)
    {
        throw std::runtime_error("Semantic error: undeclared variable: " + node.value.value + "!");
    }
}


void Semantic::checkProgram(AST::Node node)
{
    ///////// DECLARATION /////////
    if (node.type == NodeType::DECLARATION)
    {
        checkDeclar(node, declarList);

        return;
    }

    ///////// IDENTIFIER /////////
    if (node.value.type == TokenType::IDENTIFIER)
    {
        checkIdent(node, declarList);

        return;
    }

    ///////// FUNCTION /////////
    if (node.value.type == TokenType::KEYWORD && node.value.value == "func")
    {
        checkFunc(node, declarList);

        return;
    }

    ///////// RETURN /////////
    if (node.value.type == TokenType::KEYWORD && node.value.value == "return")
    {
        throw std::runtime_error("Semantic error: return outside of function!");
    }

    ///////// BREAK /////////
    if (node.value.type == TokenType::KEYWORD && node.value.value == "break")
    {
        throw std::runtime_error("Semantic error: break outside of loop!");
    }

    for (auto child : node.children)
    {
        checkProgram(child);
    }
}
