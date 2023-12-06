#include "lexer.h"
#include "parser.h"
// #include "semantic.h"
#include "printer.h"
#include "interpreter.h"

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

    std::cout << "\n\n\n";

    printer.printTokens(tokens);

    Parser parser(tokens);
    AST::Node rootNode = parser.analyze();
    printer.printAST(rootNode);

    Semantic semantic(rootNode);
    AST::Node updNode = semantic.analyze();

    std::cout << "\n\n\n";

    printer.printAST(updNode);

    std::cout << "\n\n\n";

    std::cout << "stdout:" << std::endl;

    Interpreter interpreter;
    interpreter.interpret(updNode);

    return 0;
}
