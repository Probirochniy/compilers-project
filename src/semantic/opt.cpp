// #include "semantic.h"

// // 1. Simplification of operations with constants. If you have the expression a = 5 + 3, 
// // this can be simplified to a = 8.

// // 2. Removing unused variables.

// // 3. Functions inlining. Substituting the body of functions in the place where they are called. 

// // 4. etc.

// // assignment
// // 

// AST::Node Semantic::optimize(AST::Node node) {
//     if(isOperator(node.value.type)) {
//         if(node.children.size() != 2 ) {
//             throw std::runtime_error("Semantic error: incorrect number of operands");
//         }
//         AST::Node lnode = node.children.front();
//         AST::Node rnode = node.children.back();
        
//         node = operate(lnode, rnode);
//         return;
//     }
//     for (auto ch : node.children)
//         AST::Node updNode = Semantic::optimize(node);

//     return node;
// }

// AST::Node Semantic::simplifyConst(AST::Node node){ // in assinments

// }

// AST::Node Semantic::removeUnused(AST::Node node){ // in body

// }

// AST::Node Semantic::inlineFunc(AST::Node node){ // in assinments

// }

// bool isOperator(TokenType tt) { // +-*/ |&^
//     TokenType operators[4] = {TokenType::PLUS, TokenType::MINUS, TokenType::MULTIPLY, TokenType::DIVIDE};
//     for (int i = 0; i < 4; i ++) {
//         if (tt == operators[i]) return true;
//     }
//     return false;
// }

// AST::Node operate(AST::Node lnode, AST::Node rnode) 
// {
//     TokenType lt = lnode.value.type, rt = rnode.value.type;
//     TokenType numx[2] = {TokenType::INTEGER, TokenType::REAL};
//     TokenType numy[2] = {TokenType::REAL, TokenType::INTEGER};
//     bool ok = false;
//     for (int i = 0; i < 2; i ++) {
//         if(numx[i] == lt && numy[i] == rt) ok = true;
//     }

//     if(lt == rt) {
//         if(lt == TokenType::STRING) {
//             return add(lnode, rnode);
//         } else if (lt == TokenType::LIST) {

//         } else if (lt == TokenType::TUPLE) {

//         }
//     } else {
//         throw std::runtime_error("Semantic error: incompatible operands!");
//     }
    
// }

// AST::Node add(AST::Node lnode, AST::Node rnode) {
    
// }

// AST::Node notAdd(AST::Node lnode, AST::Node rnode) { 
// }