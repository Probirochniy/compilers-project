#include "parser.h"
#include <algorithm>
#include "printer.h"

AST::Node::Node()
{
}

AST::Node::Node(NodeType t) : type(t)
{
}

AST::Node::Node(NodeType t, Token v) : type(t), value(v)
{
}

AST::AST()
{
    node.type = NodeType::PROGRAM;
    node.value = Token();
}

Parser::Parser(std::list<Token> tokenCollection)
{
    tokens = tokenCollection;
}

Token Parser::getToken()
{
    Token t = tokens.front();
    tokens.pop_front();
    return t;
}

bool Parser::getCurToken()
{
    if (curTokensList.empty())
        return false;
    Token t = curTokensList.front();
    curTokensList.pop_front();
    curToken = t;
    return true;
}

AST::Node Parser::parseFunction()
{

    Token funcToken = Token(TokenType::KEYWORD, FUNC_KEYWORD);

    AST::Node functionNode(NodeType::FUNCTION, funcToken);

    functionNode.value.value = FUNC_KEYWORD;
    functionNode.value.type = TokenType::KEYWORD;

    std::list<Token> variables;

    getCurToken();
    if (curToken.type != TokenType::OPENBRACKET)
    {
        throw std::runtime_error("No variables definition in the function!");
    }

    while (curToken.type != TokenType::CLOSEBRACKET)
    {
        variables.push_back(curToken);
        if (curTokensList.size() == 0)
        {
            throw std::runtime_error("Not closed bracket!");
        }

        getCurToken();
    }

    std::list<Token> tokensBackupOne = curTokensList;

    curTokensList = variables;
    getCurToken();

    // vars

    functionNode.children.push_back(parseVarsList());

    curTokensList = tokensBackupOne;

    std::list<Token> body;

    getCurToken();

    if (curToken.type != TokenType::KEYWORD && curToken.value != IS_KEYWORD)
    {
        throw std::runtime_error("Function body was not opened!");
    }

    while (!curTokensList.empty())
    {
        getCurToken();
        body.push_back(curToken);
    }

    AST::Node bodyNode;

    std::list<Token> tokensBackup;
    tokensBackup = tokens;

    tokens = body;

    bodyNode.type = NodeType::FUNCTION_BODY;
    bodyNode.children = parseBody();
    functionNode.children.push_back(bodyNode);

    tokens = tokensBackup;

    return functionNode;
}

AST::Node Parser::parseExprList()
{
    AST::Node exprListNode;
    while (curTokensList.size() != 0)
    {
        getCurToken();
        AST::Node newNode;
        newNode.type = NodeType::FACTOR;
        newNode.value = curToken;
        exprListNode.children.push_back(newNode);

        if (curTokensList.size() == 0)
        {
            break;
        }

        getCurToken();

        if (curToken.type != TokenType::COMMA)
        {
            throw std::runtime_error("not found ',' between expressions!");
        }
    }

    exprListNode.type = NodeType::DECLARATION_LEFT;
    exprListNode.value = Token();
    return exprListNode;
}

AST::Node Parser::parseVarsList()
{
    AST::Node varsListNode;

    while (curTokensList.size() != 0)
    {
        getCurToken();
        AST::Node newNode;

        if (curToken.type == TokenType::IDENTIFIER)
        {
            if (curTokensList.front().type == TokenType::OPENSQUAREBRACKET)
            {
                newNode.type = NodeType::LISTCALL;
                newNode.value = curToken;

                newNode.children.push_back(AST::Node(NodeType::IDENTIFIER, curToken));

                getCurToken();
                getCurToken();

                std::list<Token> localList;
                while (curToken.type != TokenType::CLOSESQUAREBRACKET)
                {
                    localList.push_back(curToken);
                    getCurToken();
                }

                curTokensList = localList;

                getCurToken();

                AST::Node indexNode = parseExpr();

                newNode.children.push_back(indexNode);
            }
            
            else if (curTokensList.front().type == TokenType::IDENTIFIER && curTokensList.front().value == "."){
                newNode.type = NodeType::TUPLECALL;
                newNode.value = curToken;

                newNode.children.push_back(AST::Node(NodeType::IDENTIFIER, curToken));

                getCurToken();
                getCurToken();

                AST::Node indexNode = AST::Node(NodeType::FACTOR, curToken);

                newNode.children.push_back(indexNode);
            }

            else if (curTokensList.front().type == TokenType::KEYWORD && curTokensList.front().value == IS_KEYWORD)
            {
                newNode.type = NodeType::TYPECHECK;
                newNode.value = curToken;

                newNode.children.push_back(AST::Node(NodeType::IDENTIFIER, curToken));

                getCurToken();
                getCurToken();

                AST::Node typeNode = AST::Node(NodeType::FACTOR, curToken);

                newNode.children.push_back(typeNode);
            }

            else if (curTokensList.front().type == TokenType::OPENBRACKET)
            {
                newNode = AST::Node(NodeType::FUNCTIONCALL, Token());

                std::list<Token> localList;

                Token tk;
                Token name = curToken;

                getCurToken();

                AST::Node vars = AST::Node(NodeType::VARS_LIST, Token());

                while (tk.type != TokenType::CLOSEBRACKET)
                {
                    getCurToken();
                    tk = curToken;
                    localList.push_back(tk);

                    if (curTokensList.front().type == TokenType::COMMA)
                    {
                        std::list<Token> backup = curTokensList;

                        curTokensList = localList;

                        getCurToken();

                        vars.children.push_back(parseExpr());

                        localList.clear();

                        curTokensList = backup;

                        getCurToken();
                    }

                    if (curTokensList.front().type == TokenType::CLOSEBRACKET)
                    {
                        std::list<Token> backup = curTokensList;

                        curTokensList = localList;

                        getCurToken();

                        vars.children.push_back(parseExpr());

                        localList.clear();

                        curTokensList = backup;

                        getCurToken();

                        break;
                    }
                }

                newNode.children.push_back(AST::Node(NodeType::IDENTIFIER, name));
                newNode.children.push_back(vars);
            }

            else
            {
                newNode.type = NodeType::IDENTIFIER;
                newNode.value = curToken;
            }
        }

        else if (curToken.type == TokenType::STRING || curToken.type == TokenType::REAL || curToken.type == TokenType::INTEGER)
        {
            newNode.type = NodeType::FACTOR;
            newNode.value = curToken;
        }

        varsListNode.children.push_back(newNode);

        getCurToken();

        if (curTokensList.size() == 0)
        {
            break;
        }

        if (curToken.type != TokenType::COMMA)
        {
            throw std::runtime_error("not found ',' between variables!");
        }
    }

    varsListNode.type = NodeType::VARS_LIST;
    varsListNode.value = Token();
    return varsListNode;
}

AST::Node Parser::parseForLoop()
{
    AST::Node forNode; // before Loop
    forNode.type = NodeType::FORLOOP;
    forNode.value = Token();

    Token token = getToken();
    std::vector<Token> left;
    while (token.value != LOOP_KEYWORD)
    {
        left.push_back(token);
        token = getToken();
    }
    token = getToken(); // skip loop keyword

    if (left.size() != 3 || left[0].type != TokenType::IDENTIFIER || left[1].value != IN_KEYWORD || left[2].type != TokenType::RANGE)
    {
        // std::cout << getTokenTypeName(left[2].type);
        throw std::runtime_error("for condition is not correct!");
    }

    forNode.children.push_back({NodeType::IDENTIFIER, left[0]});
    forNode.children.push_back({NodeType::RANGE, left[2]});

    AST::Node bodyNode;
    bodyNode.type = NodeType::FORLOOP_BODY;

    std::list<Token> right;

    while (!(token.type == TokenType::KEYWORD && token.value == END_KEYWORD))
    {
        right.push_back(token);
        token = getToken();
    }

    if (right.empty())
    {
        throw std::runtime_error("for body is empty!");
    }

    std::list<Token> tokensBackup;
    tokensBackup = tokens;

    tokens = right;

    bodyNode.children = parseBody();
    forNode.children.push_back(bodyNode);

    tokens = tokensBackup;

    return forNode;
}

AST::Node Parser::parseIf()
{
    AST::Node ifNode(NodeType::IF_STATEMENT, Token());

    Token token = getToken();
    std::list<Token> left;

    if (token.type != TokenType::OPENBRACKET)
    {
        throw std::runtime_error("if condition is not correct!");
    }

    while (token.type != TokenType::CLOSEBRACKET)
    {
        left.push_back(token);
        token = getToken();
    }

    token = getToken(); // skip then keyword

    if (left.empty())
    {
        throw std::runtime_error("if condition is empty!");
    }

    curTokensList = left;

    AST::Node condNode = parseCond();

    //////////////////////////////////

    ifNode.children.push_back(condNode);

    AST::Node bodyNode;
    bodyNode.type = NodeType::IF_BODY;

    std::list<Token> right;
    std::list<Token> elseBody;

    bool elseFound = false;

    while (token.value != END_KEYWORD && token.value != ELSE_KEYWORD)
    {
        token = getToken();
        if (token.value == ELSE_KEYWORD)
            elseFound = true;
        if (token.value != END_KEYWORD && token.value != ELSE_KEYWORD)
        {
            right.push_back(token);
        }
    }

    if (right.empty())
    {
        throw std::runtime_error("if body is empty!");
    }

    std::list<Token> tokensBackup;
    tokensBackup = tokens;

    tokens = right;

    bodyNode.children = parseBody();
    ifNode.children.push_back(bodyNode);

    tokens = tokensBackup;

    if (elseFound)
    {
        token = getToken();
        while (token.value != END_KEYWORD)
        {
            elseBody.push_back(token);
            token = getToken();
        }

        if (elseBody.empty())
        {
            throw std::runtime_error("else body is empty!");
        }

        std::list<Token> tokensBackup;
        tokensBackup = tokens;

        tokens = elseBody;

        AST::Node elseBodyNode;
        elseBodyNode.type = NodeType::ELSE_BODY;
        elseBodyNode.children = parseBody();
        ifNode.children.push_back(elseBodyNode);

        tokens = tokensBackup;
    }

    //////////////////////////////////

    return ifNode;
}

AST::Node Parser::parseWhileLoop()
{
    AST::Node whileNode;
    whileNode.type = NodeType::WHILELOOP;

    Token token = getToken();
    std::list<Token> left;
    while (token.value != LOOP_KEYWORD)
    {
        left.push_back(token);
        token = getToken();
    }

    token = getToken(); // skip loop keyword

    if (left.empty())
    {
        throw std::runtime_error("while condition is empty!");
    }

    curTokensList = left;

    AST::Node parseNode = parseCond();

    whileNode.children.push_back(parseNode);

    AST::Node bodyNode;
    bodyNode.type = NodeType::WHILELOOP_BODY;

    std::list<Token> right;

    while (token.value != END_KEYWORD)
    {
        right.push_back(token);
        token = getToken();
    }

    if (right.empty())
    {
        throw std::runtime_error("while body is empty!");
    }

    std::list<Token> tokensBackup;
    tokensBackup = tokens;

    tokens = right;

    bodyNode.children = parseBody();
    whileNode.children.push_back(bodyNode);

    tokens = tokensBackup;

    return whileNode;
}

AST::Node Parser::parseCond()
{
    AST::Node condNode;
    condNode.type = NodeType::IF_COND;

    if (curTokensList.front().type == TokenType::OPENBRACKET)
    {
        curTokensList.pop_front();
    }

    if (curTokensList.back().type == TokenType::CLOSEBRACKET)
    {
        curTokensList.pop_back();
    }

    getCurToken();
    condNode.children.push_back(parseCondExpr());

    return condNode;
}

AST::Node Parser::parseCondExpr()
{
    AST::Node left = parseCondTerm();
    while ((curToken.type == TokenType::KEYWORD && curToken.value == OR_KEYWORD) || (curToken.type == TokenType::KEYWORD && curToken.value == AND_KEYWORD))
    {
        Token tk = curToken;
        getCurToken();
        left = makeTree(NodeType::EXPRESSION, tk, {left, parseCondTerm()});
    }

    return left;
}

AST::Node Parser::parseCondTerm()
{
    AST::Node left = parseCondFactor();
    while (curToken.type == TokenType::LESS || curToken.type == TokenType::MORE || curToken.type == TokenType::EQUALITY || curToken.type == TokenType::UNEQUALITY || curToken.type == TokenType::LESSOREQUAL || curToken.type == TokenType::MOREOREQUAL)
    {
        Token tk = curToken;
        getCurToken();
        left = makeTree(NodeType::TERM, tk, {left, parseCondFactor()});
    }

    return left;
}

AST::Node Parser::parseCondFactor()
{
    AST::Node res;
    Token tk = curToken;
    if (tk.type == TokenType::OPENBRACKET)
    {
        getCurToken(); // skip '('
        res = parseCondExpr();
        getCurToken(); // skip ')'
    }
    else
    {
        getCurToken();
        res = makeCondTree(NodeType::FACTOR, tk, {});
    }
    return res;
}

AST::Node Parser::makeCondTree(NodeType nodetype, Token value, std::list<AST::Node> children)
{
    AST::Node newnode;
    newnode.type = nodetype;
    newnode.value = value;
    swap(newnode.children, children);
    return newnode;
}

AST::Node Parser::parseAssignment(NodeType type)
{
    AST::Node assignmentNode;
    assignmentNode.type = type;

    assignmentNode.value.value = ":=";
    assignmentNode.value.type = TokenType::DEFINITION;

    Token token = getToken();
    std::list<Token> left;

    while (token.type != TokenType::DEFINITION)
    {
        left.push_back(token);
        token = getToken();

        if (tokens.empty())
        {
            throw std::runtime_error("no declaration after var!");
        }
    }

    if (left.empty())
    {
        throw std::runtime_error("var left part is empty!");
    }

    curTokensList = left;

    AST::Node assignmentLeft = parseVarsList();
    assignmentLeft.type = NodeType::DECLARATION_LEFT;
    assignmentLeft.value = Token();
    assignmentNode.children.push_back(assignmentLeft);

    token = getToken();

    std::list<Token> right;
    std::list<Token> empty;

    AST::Node assignmentRight;
    assignmentRight.type = NodeType::DECLARATION_RIGHT;
    assignmentRight.value = Token();

    bool allClosed = false;

    while (token.type != TokenType::DELIMITER)
    {

        if (token.type == TokenType::COMMA)
        {
            curTokensList = right;

            getCurToken();

            if (curToken.type == TokenType::STRING || curToken.type == TokenType::REAL || curToken.type == TokenType::INTEGER || curToken.type == TokenType::IDENTIFIER)
            {
                assignmentRight.children.push_back(parseExpr());
            }

            else if (curToken.type == TokenType::BOOL && (curToken.value == TRUE_KEYWORD || curToken.value == FALSE_KEYWORD))
            {
                assignmentRight.children.push_back(parseCondExpr());
            }

            else if (tokens.front().type == TokenType::OPENSQUAREBRACKET)
            {
                assignmentRight.children.push_back(parseListCall(token));
                allClosed = true;
            }
            else if (tokens.front().type == TokenType::OPENBRACKET)
            {
                std::cout << "HERE"
                          << "\n";
            }

            else
            {
                throw std::runtime_error("Unknown assignment type!");
            }

            right = empty;
            allClosed = false;
        }

        else if (token.value == FUNC_KEYWORD)
        {
            curTokensList = extractFunction();

            assignmentRight.children.push_back(parseFunction());
            allClosed = true;
        }

        else if (token.type == TokenType::OPENSQUAREBRACKET)
        {
            // curTokensList = extractList();

            assignmentRight.children.push_back(parseList());
            allClosed = true;
        }

        else if (token.type == TokenType::OPENFIGUREBRACKET)
        {
            assignmentRight.children.push_back(parseTuple());
            allClosed = true;
        }

        else if (tokens.front().type == TokenType::OPENSQUAREBRACKET)
        {
            assignmentRight.children.push_back(parseListCall(token));
            allClosed = true;
        }

        else if (tokens.front().type  == TokenType::IDENTIFIER && tokens.front().value == "."){
            assignmentRight.children.push_back(parseTupleCall(token));
            allClosed = true;
        }

        else if (tokens.front().type == TokenType::OPENBRACKET)
        {
            assignmentRight.children.push_back(parseFunctionCall(token));
            allClosed = true;
        }


        else
        {
            right.push_back(token);
            allClosed = false;
        }

        if (tokens.empty())
        {
            throw std::runtime_error("Delimiter not found!");
        }

        token = getToken();
    }

    if (!allClosed)
    {

        if (right.empty())
        {
            throw std::runtime_error("var right part is not complete!");
        }

        curTokensList = right;
        Token firstToken = curTokensList.front();
        getCurToken();

        if (firstToken.type == TokenType::STRING || firstToken.type == TokenType::REAL || firstToken.type == TokenType::INTEGER || firstToken.type == TokenType::IDENTIFIER)
        {
            assignmentRight.children.push_back(parseExpr());
        }

        else if (curToken.type == TokenType::BOOL && (curToken.value == TRUE_KEYWORD || curToken.value == FALSE_KEYWORD))
        {
            assignmentRight.children.push_back(parseCondExpr());
        }


        else
        {
            throw std::runtime_error("Unknown assignment type!");
        }
    }

    assignmentNode.children.push_back(assignmentRight);

    return assignmentNode;
}

AST::Node Parser::parseTuple()
{
    Token tk = getToken();

    AST::Node tupleNode(NodeType::TUPLE);
    tupleNode.value = Token(TokenType::TUPLE, "tuple");

    std::list<Token> curelement;
    std::list<Token> empty;

    while (!tokens.empty())
    {
        // std::cout << tk.value << " " << getTokenTypeName(tk.type) << "\n";

        if (tk.type == TokenType::CLOSEFIGUREBRACKET)
        {
            curTokensList = curelement;

                if (!curTokensList.empty()){
                    getCurToken();
    
                bool isAssignment = false;
    
                for (auto token : curTokensList)
                {
                    if (token.type == TokenType::DEFINITION)
                    {
                        isAssignment = true;
                    }
                }
    
                if (isAssignment)
                {
                    tokens.push_front(Token(TokenType::DELIMITER, ";"));
    
                    std::reverse(curelement.begin(), curelement.end());
    
                    for (auto token : curelement)
                    {
                        tokens.push_front(token);
                    }
    
                    tupleNode.children.push_back(parseAssignment(NodeType::DECLARATION));
                }
    
                else
                {
                    tupleNode.children.push_back(parseExpr());
                }
            }
            

            return tupleNode;
        }
        else if (tk.type == TokenType::OPENSQUAREBRACKET)
        {
            curTokensList = curelement;
            getCurToken();

            tupleNode.children.push_back(parseList());
            curelement = empty;

            // skip comma
            if (tokens.front().type == TokenType::COMMA)
            {
                tk = getToken();
            }

        }
        else if (tk.type == TokenType::OPENFIGUREBRACKET)
        {
            curTokensList = curelement;
            getCurToken();

            tupleNode.children.push_back(parseTuple());
            curelement = empty;

            //skip comma
           if (tokens.front().type == TokenType::COMMA)
            {
                tk = getToken();
            }
        
        }
        else if (tk.type == TokenType::COMMA)
        {
            curTokensList = curelement;
            getCurToken();

            bool isAssignment = false;

            for (auto token : curTokensList)
            {
                if (token.type == TokenType::DEFINITION)
                {
                    isAssignment = true;
                }
            }

            if (isAssignment)
            {
                tokens.push_front(Token(TokenType::DELIMITER, ";"));

                std::reverse(curelement.begin(), curelement.end());

                for (auto token : curelement)
                {
                    tokens.push_front(token);
                }

                tupleNode.children.push_back(parseAssignment(NodeType::DECLARATION));
            }

            else
            {
                tupleNode.children.push_back(parseExpr());
            }

            curelement = empty;
        }

        else
        {
            curelement.push_back(tk);
        }

        tk = getToken();
    }

    throw std::runtime_error("tuple was not closed!");
}

AST::Node Parser::parseList()
{

    Token tk = getToken();

    AST::Node listNode(NodeType::LIST);
    listNode.value = Token(TokenType::LIST, "list");

    std::list<Token> curelement;
    std::list<Token> empty;

    while (!tokens.empty())
    {

        // std::cout << tk.value << " " << getTokenTypeName(tk.type) << "\n";

        if (tk.type == TokenType::CLOSESQUAREBRACKET)
        {

            curTokensList = curelement;
            if (!curTokensList.empty()){
                getCurToken();
                listNode.children.push_back(parseExpr());
            }
            

            return listNode;
        }
        else if (tk.type == TokenType::OPENSQUAREBRACKET)
        {
            curTokensList = curelement;
            getCurToken();

            listNode.children.push_back(parseList());
            curelement = empty;

            // skip comma
            if (tokens.front().type == TokenType::COMMA)
            {
                tk = getToken();
            }

        }
        else if (tk.type == TokenType::OPENFIGUREBRACKET)
        {
            curTokensList = curelement;
            getCurToken();

            listNode.children.push_back(parseTuple());
            curelement = empty;

            //skip comma
           if (tokens.front().type == TokenType::COMMA)
            {
                tk = getToken();
            }
        
        }
        else if (tk.type == TokenType::COMMA)
        {
            curTokensList = curelement;
            getCurToken();

            listNode.children.push_back(parseExpr());
            curelement = empty;
        }

        else
        {
            curelement.push_back(tk);
        }

        tk = getToken();
    }

    throw std::runtime_error("list was not closed!");
}

std::list<Token> Parser::extractList()
{

    Token tk = getToken();

    std::list<Token> result;

    while (!tokens.empty())
    {

        if (tk.type == TokenType::CLOSESQUAREBRACKET)
        {
            return result;
        }
        else if (tk.type == TokenType::COMMA)
        {
        }

        else
        {
            result.push_back(tk);
        }

        tk = getToken();
    }

    return result;
}

std::list<Token> Parser::extractFunction()
{
    int endNum = 0;
    std::list<Token> functionList;
    while (!tokens.empty())
    {
        Token tk = getToken();

        if (tk.value == IS_KEYWORD || tk.value == THEN_KEYWORD)
        {
            endNum++;
        }
        if (tk.value == END_KEYWORD)
        {
            endNum--;

            if (endNum == 0)
            {
                return functionList;
            }
        }
        functionList.push_back(tk);
    }

    throw std::runtime_error("Not finished function!");
}

AST::Node Parser::parseListCall(Token tk)
{
    std::list<Token> localList;

    Token name = tk;

    getToken();

    while (tk.type != TokenType::CLOSESQUAREBRACKET)
    {
        localList.push_back(tk);
        tk = getToken();
    }

    curTokensList = localList;

    AST::Node result = AST::Node(NodeType::LISTCALL, Token());

    result.children.push_back(AST::Node(NodeType::IDENTIFIER, name));

    getCurToken();
    getCurToken();

    AST::Node indexNode = parseExpr();

    result.children.push_back(indexNode);

    return result;
}

AST::Node Parser::parseTupleCall(Token tk){
    Token name = tk;

    AST::Node result = AST::Node(NodeType::TUPLECALL, Token());
    result.children.push_back(AST::Node(NodeType::IDENTIFIER, name));

    getToken(); // skip '.'

    Token index = getToken();

    result.children.push_back(AST::Node(NodeType::FACTOR, index));

    return result;
}

AST::Node Parser::parseFunctionCall(Token tk)
{
    std::list<Token> localList;

    Token name = tk;

    getToken();

    AST::Node vars = AST::Node(NodeType::VARS_LIST, Token());

    while (tk.type != TokenType::CLOSEBRACKET)
    {
        tk = getToken();
        localList.push_back(tk);

        for (auto l : localList)
        {
            std::cout << l.value << " ";
        }

        std::cout << "\n";

        if (tokens.front().type == TokenType::COMMA)
        {
            curTokensList = localList;
            getCurToken();
            vars.children.push_back(parseExpr());

            localList.clear();

            getToken();
        }

        if (tokens.front().type == TokenType::CLOSEBRACKET)
        {
            curTokensList = localList;
            getCurToken();
            vars.children.push_back(parseExpr());

            localList.clear();

            getToken();

            break;
        }
    }

    AST::Node result = AST::Node(NodeType::FUNCTIONCALL, Token());

    result.children.push_back(AST::Node(NodeType::IDENTIFIER, name));
    result.children.push_back(vars);

    return result;
}

AST::Node Parser::getNode()
{

    Token tk = getToken();

    // Comment - skip it in parseBody
    if (tk.type == TokenType::COMMENT)
    {
        return AST::Node(NodeType::COMMENT, tk);
    }

    // single delimiter
    if (tk.type == TokenType::DELIMITER)
    {
        return AST::Node(NodeType::COMMENT, tk);
    }

    // List
    if (tk.type == TokenType::OPENSQUAREBRACKET)
    {
        curTokensList = extractList();
        return parseList();
    }

    // Tuple
    if (tk.type == TokenType::OPENFIGUREBRACKET)
    {
        return parseTuple();
    }

    // Expression
    if (tk.type == TokenType::STRING || tk.type == TokenType::REAL || tk.type == TokenType::INTEGER || tk.type == TokenType::IDENTIFIER || tk.type == TokenType::OPENBRACKET)
    {
        if (tokens.front().type == TokenType::DEFINITION)
        {
            tokens.push_front(tk);
            return parseAssignment(NodeType::ASSIGNMENT);
        }

        if (tokens.front().type == TokenType::OPENSQUAREBRACKET)
        {
            return parseListCall(tk);
        }

        if (tokens.front().type == TokenType::IDENTIFIER && tokens.front().value == ".")
        {
            return parseTupleCall(tk);
        }

        if (tokens.front().type == TokenType::OPENBRACKET)
        {
            return parseFunctionCall(tk);
        }

        while (tk.type != TokenType::DELIMITER)
        {
            curTokensList.push_back(tk);

            if (tokens.empty())
            {
                throw std::runtime_error("Delimiter not found!");
            }
            tk = getToken();
        }

        getCurToken();
        return parseExpr();
    }

    // Assignment
    else if (tk.type == TokenType::KEYWORD && tk.value == VAR_KEYWORD)
    {
        return parseAssignment(NodeType::DECLARATION);
    }

    // Function
    else if (tk.type == TokenType::KEYWORD && tk.value == FUNC_KEYWORD)
    {
        curTokensList = extractFunction();
        return parseFunction();
    }

    // for loop
    else if (tk.type == TokenType::KEYWORD && tk.value == FOR_KEYWORD)
    {
        return parseForLoop();
    }

    // while loop
    else if (tk.value == WHILE_KEYWORD)
    {
        return parseWhileLoop();
    }

    // if statement
    else if (tk.value == IF_KEYWORD)
    {
        return parseIf();
    }

    // print function
    else if (tk.type == TokenType::KEYWORD && tk.value == PRINT_KEYWORD)
    {
        AST::Node printNode(NodeType::PRINT, tk);
        while (tk.type != TokenType::DELIMITER)
        {
            curTokensList.push_back(tk);
            if (tokens.empty())
            {
                throw std::runtime_error("Delimiter not found!");
            }
            tk = getToken();
        }

        getCurToken();

        printNode.children.push_back(parseVarsList());

        return printNode;
    }

    else if (tk.type == TokenType::KEYWORD && tk.value == BREAK_KEYWORD)
    {
        AST::Node breakNode(NodeType::BREAK, tk);
        return breakNode;
    }

    else if (tk.type == TokenType::KEYWORD && tk.value == RETURN_KEYWORD)
    {
        AST::Node printNode(NodeType::BREAK, tk);
        while (tk.type != TokenType::DELIMITER)
        {
            curTokensList.push_back(tk);
            if (tokens.empty())
            {
                throw std::runtime_error("Delimiter not found!");
            }
            tk = getToken();
        }

        getCurToken();

        printNode.children.push_back(parseVarsList());

        return printNode;
    }

    // nothing matched
    else
    {
        throw std::runtime_error("Statement did not match any pattern!");
    }
    return AST::Node();
}

std::list<AST::Node> Parser::parseBody()
{
    std::list<AST::Node> children;
    while (!tokens.empty())
    {
        AST::Node newNode = getNode();
        if (newNode.type != NodeType::COMMENT)
            children.push_back(newNode);
    }
    return children;
}

AST::Node Parser::analyze()
{
    tree.node.children = parseBody();

    return tree.node;
}
AST::Node Parser::makeTree(NodeType nodetype, Token value, std::list<AST::Node> children)
{
    AST::Node newnode;
    newnode.type = nodetype;
    newnode.value = value;
    swap(newnode.children, children);
    return newnode;
}
AST::Node Parser::parseExpr()
{
    AST::Node left = parseTerm();
    while (curToken.type == TokenType::PLUS || curToken.type == TokenType::MINUS )
    {
        Token tk = curToken;
        getCurToken();
        left = makeTree(NodeType::EXPRESSION, tk, {left, parseTerm()});
    }

    return left;
}

AST::Node Parser::parseTerm()
{
    AST::Node left = parseFactor();
    while (curToken.type == TokenType::MULTIPLY || curToken.type == TokenType::DIVIDE
           || curToken.type == TokenType::MORE || curToken.type == TokenType::LESS
           || curToken.type == TokenType::EQUALITY || curToken.type == TokenType::UNEQUALITY
           || curToken.type == TokenType::LESSOREQUAL || curToken.type == TokenType::MOREOREQUAL)
    {
        Token tk = curToken;
        getCurToken();
        left = makeTree(NodeType::TERM, tk, {left, parseFactor()});
    }

    return left;
}

AST::Node Parser::parseFactor()
{
    AST::Node res;
    Token tk = curToken;
    if (tk.type == TokenType::OPENBRACKET)
    {
        getCurToken(); // skip '('
        res = parseExpr();
        getCurToken(); // skip ')'
    }
    else
    {
        getCurToken();
        res = makeTree(NodeType::FACTOR, tk, {});
    }
    return res;
}
