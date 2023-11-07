#include "lexer.h"

Token::Token() {}
Token::Token(TokenType t, std::string v) : type(t), value(v) {}

Lexer::Lexer() {}

Lexer::Lexer(std::string fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        src += line + '\n';
    }
    file.close();
}

std::list<Token> Lexer::analyze()
{
    std::string operators = "+-*/,:=(){}[];.><!\'\"";

    for (int i = 0; i < src.length(); i++)
    {
        std::string token = "";

        if (src[i] == ' ' || src[i] == '\n' || src[i] == '\t')
            continue;

        else if (operators.find(src[i]) != std::string::npos)
        {
            TokenType t;
            if (src[i] == ':')
            {
                if (i + 1 < src.size() && src[i + 1] == '=')
                {
                    token += src[i];
                    token += src[i + 1];
                    i++;
                    t = TokenType::DEFINITION;
                }
                else
                {
                    token += src[i];
                    t = TokenType::COLON;
                }
            }
            else if (src[i] == '/')
            {
                if (i + 1 < src.size() && src[i + 1] == '/')
                {
                    int j = i;
                    while (j < src.size() && src[j] != '\n')
                    {
                        token += src[j];
                        j++;
                    }
                    i = j - 1;
                    t = TokenType::COMMENT;
                }
                else
                {
                    token = "/";
                    t = TokenType::DIVIDE;
                }
            }
            else if (src[i] == '=')
            {
                if (i + 1 < src.size() && src[i + 1] == '=')
                {
                    token += src[i];
                    token += src[i + 1];
                    i++;
                    t = TokenType::EQUALITY;
                }
                else if (i + 1 < src.size() && src[i + 1] == '>')
                {
                    token += src[i];
                    token += src[i + 1];
                    i++;
                    t = TokenType::FOLLOWING;
                }
                else
                {
                    token += src[i];
                    t = TokenType::EQUAL;
                }
            }
            else if (src[i] == '!')
            {
                if (i + 1 < src.size() && src[i + 1] == '=')
                {
                    token += src[i];
                    token += src[i + 1];
                    i++;
                    t = TokenType::UNEQUALITY;
                }
                else
                {
                    token += src[i];
                    t = TokenType::EXCLAMATION;
                }
            }
            else if (src[i] == '>')
            {
                if (i + 1 < src.size() && src[i + 1] == '=')
                {
                    token += src[i];
                    token += src[i + 1];
                    i++;
                    t = TokenType::MOREOREQUAL;
                }
                else
                {
                    token += src[i];
                    t = TokenType::MORE;
                }
            }
            else if (src[i] == '<')
            {
                if (i + 1 < src.size() && src[i + 1] == '=')
                {
                    token += src[i];
                    token += src[i + 1];
                    i++;
                    t = TokenType::LESSOREQUAL;
                }
                else
                {
                    token += src[i];
                    t = TokenType::LESS;
                }
            } else if (src[i] == '\'' || src[i] == '\"'){
                int j = i + 1;
                while(j < src.size() && src[j] != src[i]) {
                    token += src[j];
                    j ++;
                }
                token = src[i] + token + src[i];
                i = j;
                t = TokenType::STRING;
            } else {
                token += src[i];
                t = getTokenType(src[i]);
            }

            tokenCollection.push_back(Token{t, token});
            continue;
        }

        else if (isDigit(src[i]))
        {
            TokenType ttype = TokenType::INTEGER;
            while (i < src.length() && (isDigit(src[i]) || src[i] == '.'))
            {
                token += src[i];
                i ++;
            }

            if (isRange(token))
            {
                ttype = TokenType::RANGE;
            }

            else if (hasMoreThanOneDot(token))
            {
                ttype = TokenType::UNKNOWNTYPE;
            } else if (token.find('.') != std::string::npos) {
                ttype = TokenType::REAL;
            }

            i--;
            tokenCollection.push_back(Token{ttype, token});
            continue;
        }

        else if (isIdentifierOk(src[i]))
        {
            while (i < src.length() && (isIdentifierOk(src[i]) || isDigit(src[i])))
            {
                token += src[i];
                i++;
            }

            tokenCollection.push_back(Token{getKeyWord(token), token});

            i--;
            continue;
        }
        else
        {
            token += src[i];
            tokenCollection.push_back(Token{TokenType::UNKNOWNTYPE, token});
        }
    }
    return tokenCollection;
}

bool Lexer::isRange(const std::string &str)
{
    std::regex rangeRegex("^\\d+\\.\\.\\d+$");
    return std::regex_match(str, rangeRegex);
}

bool Lexer::hasMoreThanOneDot(const std::string &str)
{
    int count = 0;
    for (char ch : str)
    {
        if (ch == '.')
        {
            count++;
            if (count > 1)
            {
                return true;
            }
        }
    }
    return false;
}

TokenType Lexer::getTokenType(char c)
{
    switch (c)
    {
    case '+':
        return TokenType::PLUS;
    case '-':
        return TokenType::MINUS;
    case '/':
        return TokenType::DIVIDE;
    case '*':
        return TokenType::MULTIPLY;
    case ',':
        return TokenType::COMMA;
    case ';':
        return TokenType::DELIMITER;
    case '(':
        return TokenType::OPENBRACKET;
    case ')':
        return TokenType::CLOSEBRACKET;
    case '{':
        return TokenType::OPENFIGUREBRACKET;
    case '}':
        return TokenType::CLOSEFIGUREBRACKET;
    case '[':
        return TokenType::OPENSQUAREBRACKET;
    case ']':
        return TokenType::CLOSESQUAREBRACKET;
    case ':':
        return TokenType::COLON;
    case '=':
        return TokenType::EQUAL;
    default:
        return TokenType::IDENTIFIER;
    }
}

TokenType Lexer::getKeyWord(std::string str)
{
    std::string sarr[18] = {VAR_KEYWORD, IF_KEYWORD, THEN_KEYWORD, ELSE_KEYWORD, FOR_KEYWORD, END_KEYWORD,
                            WHILE_KEYWORD, LOOP_KEYWORD, PRINT_KEYWORD, FUNC_KEYWORD, EMPTY_KEYWORD, TRUE_KEYWORD, 
                            FALSE_KEYWORD, IS_KEYWORD, IN_KEYWORD , RETURN_KEYWORD, BREAK_KEYWORD , RANGE_KEYWORD };
    for (int i = 0; i < 18; i++)
    {
        if (sarr[i] == str)
            return TokenType::KEYWORD;
    }
    return TokenType::IDENTIFIER;
}

bool Lexer::isSpace(char c)
{
    switch (c)
    {
    case ' ':
    case '\t':
    case '\n':
        return true;
    default:
        return false;
    }
}

bool Lexer::isDigit(char c)
{
    return ('0' <= c && c <= '9');
}

bool Lexer::isIdentifierOk(char c)
{
    if ('a' <= c && c <= 'z')
        return true;
    if ('A' <= c && c <= 'Z')
        return true;
    if (c == '_')
        return true;
    return false;
}
