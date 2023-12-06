#include "semantic.h"
#include "parser.h"
#include <iostream>
#include <list>
#include "printer.h"

AST::Node Semantic::checkDeclar(AST::Node node, std::list<Variable> &declarList)
{
    std::list<std::string> nameList;
    std::list<TokenType> typeList;
    std::list<AST::Node> valueList;

    AST::Node outputNode = AST::Node(NodeType::DECLARATION, Token());

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

            outputNode.children.push_back(child);
        }

        if (child.type == NodeType::DECLARATION_RIGHT)
        {
            AST::Node rightNode = AST::Node(NodeType::DECLARATION_RIGHT, Token());

            for (auto grandchild : child.children)
            {
                if (grandchild.value.type == TokenType::IDENTIFIER)
                {
                    rightNode.children.push_back(optimize(checkIdent(grandchild, declarList)));
                    valueList.push_back(optimize(grandchild));
                }

                else if (grandchild.type == NodeType::FUNCTION)
                {
                    rightNode.children.push_back(checkFunc(grandchild, declarList));
                    valueList.push_back(grandchild);
                }

                else if (grandchild.type == NodeType::EXPRESSION)
                {
                    rightNode.children.push_back(optimize(checkExpr(grandchild, declarList)));
                    valueList.push_back(optimize(grandchild));
                }

                else if (grandchild.type == NodeType::LIST)
                {
                    rightNode.children.push_back(checkList(grandchild, declarList));
                    valueList.push_back(grandchild);
                }

                else
                {
                    rightNode.children.push_back(grandchild);
                    valueList.push_back(grandchild);
                }

                typeList.push_back(optimize(grandchild).value.type);
            }

            outputNode.children.push_back(rightNode);
        }
    }

    if (nameList.size() != typeList.size() || nameList.size() != valueList.size())
    {
        throw std::runtime_error("Semantic error: incorrect number of variables!");
    }

    int size = nameList.size();

    for (int i = 0; i < size; i++)
    {
        declarList.push_back(Variable{nameList.front(), typeList.front(), valueList.front()});
        nameList.pop_front();
        typeList.pop_front();
        valueList.pop_front();
    }

    return outputNode;
}

AST::Node Semantic::checkList(AST::Node node, std::list<Variable> declarList)
{
    AST::Node newNode = AST::Node(NodeType::LIST, Token());
    for (auto child : node.children)
    {
        if (child.type == NodeType::EXPRESSION)
        {
            checkExpr(child, declarList);
        }
        if (child.type == NodeType::IDENTIFIER)
        {
            checkIdent(child, declarList);
        }
        newNode.children.push_back(optimize(child));
    }
    return newNode;
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

AST::Node Semantic::checkFunc(AST::Node node, std::list<Variable> declarList)
{
    std::list<std::string> nameList;

    AST::Node outputNode = AST::Node(NodeType::FUNCTION, Token());

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

            outputNode.children.push_back(child);
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

            ///////// FUNCTION DECLARCHECK /////////

            std::list<Variable> scopeDeclarList = declarList;

            for (auto name : nameList)
            {
                scopeDeclarList.push_back(Variable{name, TokenType::UNKNOWNTYPE, AST::Node()});
            }

            for (auto grandchild : newNode.children)
            {
                checkFuncBody(grandchild, scopeDeclarList);
            }

            outputNode.children.push_back(newNode);
        }
    }

    return outputNode;
}

AST::Node Semantic::checkIdent(AST::Node node, std::list<Variable> declarList)
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

    return node;
}

AST::Node Semantic::checkFor(AST::Node node, std::list<Variable> declarList)
{
    AST::Node outputNode = AST::Node(NodeType::FORLOOP, Token());

    std::list<std::string> nameList;

    for (auto child : node.children)
    {
        if (child.type == NodeType::IDENTIFIER)
        {
            nameList.push_back(child.value.value);
            outputNode.children.push_back(child);
        }

        if (child.type == NodeType::RANGE)
        {
            outputNode.children.push_back(child);
            continue;
        }

        if (child.type == NodeType::FORLOOP_BODY)
        {
            std::list<Variable> scopeDeclarList = declarList;

            for (auto name : nameList)
            {
                scopeDeclarList.push_back(Variable{name, TokenType::UNKNOWNTYPE, AST::Node()});
            }

            ///////// OPTIMIZATION /////////
            ///////// DELETE EVERYTHING AFTER BREAK /////////

            bool afterBreak = false;
            AST::Node newNode = AST::Node(NodeType::FORLOOP_BODY, Token());

            for (auto grandchild : child.children)
            {
                if (!afterBreak)
                {
                    newNode.children.push_back(grandchild);
                }

                if (grandchild.value.type == TokenType::KEYWORD && grandchild.value.value == "break")
                {
                    afterBreak = true;
                }
            }

            ///////// CHECK FOR BODY /////////
            for (auto grandchild : newNode.children)
            {
                checkForBody(grandchild, scopeDeclarList);
            }

            outputNode.children.push_back(newNode);
        }
    }

    return outputNode;
}

void Semantic::checkForBody(AST::Node node, std::list<Variable> &scopeDeclarList)
{
    ///////// DECLARATION /////////
    if (node.type == NodeType::DECLARATION)
    {
        checkDeclar(node, scopeDeclarList);
    }

    ////////// IDENTIFIER /////////
    if (node.value.type == TokenType::IDENTIFIER)
    {
        checkIdent(node, scopeDeclarList);
    }

    if (node.type == NodeType::PRINT)
    {
        for (auto child : node.children)
        {
            if (child.type == NodeType::VARS_LIST)
            {
                for (auto grandchild : child.children)
                {
                    if (grandchild.value.type == TokenType::IDENTIFIER)
                    {
                        checkIdent(grandchild, scopeDeclarList);
                    }
                }
            }
        }
    }
}

AST::Node Semantic::checkExpr(AST::Node node, std::list<Variable> declarList)
{
    AST::Node outputNode = AST::Node(NodeType::EXPRESSION, node.value);

    for (auto child : node.children)
    {
        if (child.type == NodeType::TERM)
        {
            outputNode.children.push_back(checkTerm(child, declarList));
        }

        else if (child.type == NodeType::EXPRESSION)
        {
            outputNode.children.push_back(checkExpr(child, declarList));
        }

        else if (child.value.type == TokenType::IDENTIFIER)
        {
            outputNode.children.push_back(checkIdent(child, declarList));
        }
        else
        {
            outputNode.children.push_back(child);
        }
    }

    return optimize(outputNode);
}

AST::Node Semantic::checkTerm(AST::Node node, std::list<Variable> declarList)
{
    AST::Node outputNode = AST::Node(NodeType::TERM, node.value);

    for (auto child : node.children)
    {
        if (child.value.type == TokenType::IDENTIFIER)
        {
            outputNode.children.push_back(checkIdent(child, declarList));
            continue;
        }

        if (child.type == NodeType::EXPRESSION)
        {
            outputNode.children.push_back(checkExpr(child, declarList));
        }

        if (child.type == NodeType::FACTOR)
        {
            outputNode.children.push_back(child);
        }

        if (child.type == NodeType::TERM)
        {
            outputNode.children.push_back(checkTerm(child, declarList));
        }
    }

    return outputNode;
}

AST::Node Semantic::checkIf(AST::Node node, std::list<Variable> &declarList)
{
    AST::Node outputNode = AST::Node(NodeType::IF_STATEMENT, node.value);
    for (auto child : node.children)
    {
        ////////////IF COND////////////
        if (child.type == NodeType::IF_COND)
        {
            AST::Node newNode = AST::Node(NodeType::IF_COND, Token());
            for (auto grandchild : child.children)
            {
                if (grandchild.type == NodeType::EXPRESSION)
                {
                    newNode.children.push_back(optimize(checkExpr(grandchild, declarList)));
                }

                if (grandchild.type == NodeType::TERM)
                {
                    newNode.children.push_back(optimize(checkTerm(grandchild, declarList)));
                }

                if (grandchild.value.type == TokenType::IDENTIFIER)
                {
                    newNode.children.push_back(optimize(checkIdent(grandchild, declarList)));
                }
            }
            outputNode.children.push_back(newNode);
        }

        ////////////IF BODY////////////
        if (child.type == NodeType::IF_BODY)
        {
            AST::Node newNode = AST::Node(NodeType::IF_BODY, Token());
            for (auto grandchild : child.children)
            {
                newNode.children.push_back(checkProgram(grandchild));
                checkProgram(grandchild);
            }
            outputNode.children.push_back(newNode);
        }

        if (child.type == NodeType::ELSE_BODY)
        {
            AST::Node newNode = AST::Node(NodeType::ELSE_BODY, Token());
            for (auto grandchild : child.children)
            {
                newNode.children.push_back(checkProgram(grandchild));
                checkProgram(grandchild);
            }
            outputNode.children.push_back(newNode);
        }
    }
    return outputNode;
}

AST::Node Semantic::checkWhile(AST::Node node, std::list<Variable> declarList)
{
    AST::Node outputNode = AST::Node(NodeType::WHILELOOP, Token());

    for (auto child : node.children)
    {
        if (child.type == NodeType::IF_COND)
        {
            for (auto grandchild : child.children)
            {
                if (grandchild.type == NodeType::EXPRESSION)
                {
                    checkExpr(grandchild, declarList);
                }

                if (grandchild.type == NodeType::TERM)
                {
                    checkTerm(grandchild, declarList);
                }

                if (grandchild.value.type == TokenType::IDENTIFIER)
                {
                    checkIdent(grandchild, declarList);
                }
            }

            outputNode.children.push_back(child);
        }

        if (child.type == NodeType::WHILELOOP_BODY)
        {

            std::list<Variable> scopeDeclarList = declarList;

            ///////// OPTIMIZATION /////////
            ///////// DELETE EVERYTHING AFTER BREAK /////////

            bool afterBreak = false;
            AST::Node newNode = AST::Node(NodeType::FORLOOP_BODY, Token());

            for (auto grandchild : child.children)
            {
                if (!afterBreak)
                {
                    newNode.children.push_back(grandchild);
                }

                if (grandchild.value.type == TokenType::KEYWORD && grandchild.value.value == "break")
                {
                    afterBreak = true;
                }
            }

            ///////// CHECK WHILE BODY /////////
            for (auto grandchild : newNode.children)
            {
                checkForBody(grandchild, scopeDeclarList);
            }

            outputNode.children.push_back(newNode);
        }
    }

    return outputNode;
}

AST::Node Semantic::checkAssignment(AST::Node node, std::list<Variable> localDeclarList)
{
    AST::Node outputNode = AST::Node(NodeType::ASSIGNMENT, Token());

    Token newValue = Token();
    std::string name = "";

    for (auto child : node.children)
    {
        if (child.type == NodeType::DECLARATION_LEFT)
        {

            AST::Node newNode = AST::Node(NodeType::DECLARATION_LEFT, Token());

            for (auto grandchild : child.children)
            {
                if (grandchild.value.type == TokenType::IDENTIFIER)
                {
                    name = grandchild.value.value;
                    checkIdent(grandchild, localDeclarList);

                    newNode.children.push_back(grandchild);
                }
                else
                {
                    throw std::runtime_error("Semantic error: unexpected value at assignment!");
                }
            }

            outputNode.children.push_back(newNode);
        }

        if (child.type == NodeType::DECLARATION_RIGHT)
        {
            AST::Node newNode = AST::Node(NodeType::DECLARATION_RIGHT, Token());
            for (auto grandchild : child.children)
            {
                if (grandchild.value.type == TokenType::IDENTIFIER)
                {
                    checkIdent(grandchild, localDeclarList);
                }

                else if (grandchild.type == NodeType::FUNCTION)
                {
                    checkFunc(grandchild, localDeclarList);
                }

                else if (grandchild.type == NodeType::EXPRESSION)
                {
                    checkExpr(grandchild, localDeclarList);
                }

                else
                {
                }

                newValue = optimize(grandchild).value;
                newNode.children.push_back(optimize(grandchild));
            }

            outputNode.children.push_back(newNode);
        }
    }

    for (auto &var : declarList)
    {
        std::cout << var.name << std::endl;
        if (var.name == name)
        {
            var.value = AST::Node(NodeType::EXPRESSION, newValue);
            std::cout << "Value of " << name << " changed to " << newValue.value << std::endl;
        }
    }

    return outputNode;
}

AST::Node Semantic::checkListCall(AST::Node node, std::list<Variable> localDeclarList)
{
    AST::Node newNode = AST::Node(NodeType::LISTCALL, Token());
    std::string name = "";
    int index = 0;
    for (auto child : node.children)
    {

        if (child.type == NodeType::IDENTIFIER)
        {
            checkIdent(child, localDeclarList);
            name = child.value.value;
        }
        if (child.type == NodeType::FACTOR)
        {
            newNode.children.push_back(child);
            index = std::stoi(child.value.value);
        }
    }

    for (auto var : declarList)
    {
        if (var.name == name)
        {
            if (var.value.children.size() < index)
            {
                throw std::runtime_error("Semantic error: index out of range!");
            }

            else
            {
                int i = 0;
                for (auto element : var.value.children)
                {
                    if (i == index)
                    {
                        newNode = element;
                    }
                    i++;
                }
            }

            break;
        }
    }

    return newNode;
}

AST::Node Semantic::checkProgram(AST::Node node)
{
    ///////// DECLARATION /////////
    if (node.type == NodeType::DECLARATION)
    {
        AST::Node newNode = checkDeclar(node, declarList);

        return newNode;
    }

    ///////// IDENTIFIER /////////
    if (node.value.type == TokenType::IDENTIFIER)
    {
        checkIdent(node, declarList);

        return node;
    }

    ///////// FUNCTION /////////
    if (node.type == NodeType::FUNCTION)
    {

        AST::Node newNode = checkFunc(node, declarList);

        return newNode;
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

    if (node.type == NodeType::FORLOOP)
    {
        AST::Node newNode = checkFor(node, declarList);

        return newNode;
    }

    if (node.type == NodeType::IF_STATEMENT)
    {
        AST::Node newNode = checkIf(node, declarList);

        return newNode;
    }

    if (node.type == NodeType::PRINT)
    {
        for (auto child : node.children)
        {
            if (child.type == NodeType::VARS_LIST)
            {
                for (auto grandchild : child.children)
                {
                    if (grandchild.value.type == TokenType::IDENTIFIER)
                    {
                        checkIdent(grandchild, declarList);
                    }
                }
            }
        }
        return node;
    }

    if (node.type == NodeType::PROGRAM)
    {
        AST::Node newNode = AST::Node(NodeType::PROGRAM, Token());
        for (auto child : node.children)
        {
            newNode.children.push_back(checkProgram(child));
        }
        return newNode;
    }

    if (node.type == NodeType::EXPRESSION)
    {
        AST::Node newNode = checkExpr(node, declarList);

        newNode = optimize(newNode);
        return newNode;
    }

    if (node.type == NodeType::TERM)
    {
        AST::Node newNode = checkTerm(node, declarList);
        return newNode;
    }

    if (node.type == NodeType::WHILELOOP)
    {
        AST::Node newNode = checkWhile(node, declarList);
        return newNode;
    }

    if (node.type == NodeType::ASSIGNMENT)
    {
        AST::Node newNode = checkAssignment(node, declarList);
        return newNode;
    }

    if (node.type == NodeType::LIST)
    {
        return checkList(node, declarList);
    }

    if (node.type == NodeType::LISTCALL)
    {
        return checkListCall(node, declarList);
    }

    return node;
}
