#include "semantic.h"

// 1. Simplification of operations with constants. If you have the expression a = 5 + 3, 
// this can be simplified to a = 8.

// 2. Removing unused variables.

// 3. Functions inlining. Substituting the body of functions in the place where they are called. 

// 4. etc.

// assignment
// 

AST::Node Semantic::optimize(AST::Node node) {
    if(isOperator(node.value.type)) {
        if(node.children.size() != 2) {
            throw std::runtime_error("Semantic error: incorrect number of operands");
        }
        AST::Node lnode = node.children.front();
        AST::Node rnode = node.children.back();
        
        node = areCompatible(lnode.value.type, rnode.value.type);
        return;
    }
    for (auto ch : node.children)
        AST::Node updNode = Semantic::optimize(node);

    return node;
}

AST::Node Semantic::simplifyConst(AST::Node node){ // in assinments

}

AST::Node Semantic::removeUnused(AST::Node node){ // in body

}

AST::Node Semantic::inlineFunc(AST::Node node){ // in assinments

}

bool isOperator(TokenType tt) { // +-*/ |&^
    TokenType operators[4] = {TokenType::PLUS, TokenType::MINUS, TokenType::MULTIPLY, TokenType::DIVIDE};
    for (int i = 0; i < 4; i ++) {
        if (tt == operators[i]) return true;
    }
    return false;
}

bool operate(TokenType lt, TokenType rt) {
    TokenType numx[2] = {TokenType::INTEGER, TokenType::REAL};
    TokenType numy[2] = {TokenType::REAL, TokenType::INTEGER};
    for (int i = 0; i < 2; i ++) {
        if(numx[i] == lt && numy[i] == rt) return true;
    }
    
}