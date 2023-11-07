#include "lexer.h"
#include "parser.h"
// #include "semantic.h"
#include "printer.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "ERROR: ";
        return 1;
    }

    Printer printer(argv[1]);
    Lexer lexer(argv[1]);

    std::list<Token> tokens = lexer.analyze();
    printer.printTokens(tokens);

    Parser parser(tokens);
    AST::Node rootNode = parser.analyze();
    printer.printAST(rootNode);

    Semantic semantic(rootNode);
    AST::Node updNode = semantic.analyze();

    printer.printAST(updNode);
    
    return 0;
}
