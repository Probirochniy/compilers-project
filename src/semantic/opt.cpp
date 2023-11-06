#include "semantic.h"

// 1. Simplification of operations with constants. If you have the expression a = 5 + 3, 
// this can be simplified to a = 8.

// 2. Removing unused variables.

// 3. Functions inlining. Substituting the body of functions in the place where they are called. 

// 4. etc.


AST::Node Semantic::optimize(AST::Node node) {
    
}

void Semantic::simplifyConst(){

}

void Semantic::removeUnused(){

}

void Semantic::inlineFunc(){

}