#include "semantic.h"
#include "parser.h"
#include "interpreter.h"

bool Interpreter::evalBool(AST::Node node) {
    if (node.type == NodeType::FACTOR && node.value.type == TokenType::BOOL){
        if (node.value.value == "true") {
            return true;
        }
        else if (node.value.value == "false") {
            return false;
        }
        else {
            std::runtime_error("Interpreter error: Invalid boolean value");
        }
    }
    else if (node.type == NodeType::TERM) {
        if (evalExpr(node).value.value == "1") {
            return true;
        }
        else if (evalExpr(node).value.value == "0") {
            return false;
        }
    }
    else {
        std::runtime_error("Interpreter error: Invalid type for boolean expression");
    }

    return false;
}