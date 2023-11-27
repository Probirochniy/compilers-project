// #include "semantic.h"
// #include "parser.h"
// #include "printer.h"

// Semantic::Semantic(AST::Node node)
//     : node(node)
// {
// }

// Variable::Variable(){}

// Variable::Variable(std::string n, NodeType nt, Token v)
// : name(n), nodeType(nt), value(v)
// {
// }

// AST::Node Semantic::analyze()
// {
//     Semantic::dfs(node, MAP_VAR(), 0);
//     return node;
// }


// void Semantic::dfs(AST::Node& node, MAP_VAR varMap, int scope) {
//     // mapping decl
//     std::cout << scope << '\n';
//     for (auto &ch : node.children) {
//         if(ch.type == NodeType::DECLARATION) {
//             insertVarMap(ch, varMap, scope); // insert into map& during declaration
//             continue;
//         }
//         if(ch.type == NodeType::ASSIGNMENT) {
//             updVarMap(ch, varMap, scope);  // update the map& during assignment
//             continue;
//         }
//         dfs(ch, varMap, scope + 1);
//     }
// }

// // AST::Node Semantic::removeUnused(AST::Node node){ // in body
// // }

// // AST::Node Semantic::inlineFunc(AST::Node node){ // in assinments
// // }

// void Semantic::insertVarMap(AST::Node& declNode, MAP_VAR &varMap, int scope) {
//     std::list<AST::Node>& leftVals = declNode.children.front().children; // lvalues
//     std::list<AST::Node>& rightVals = declNode.children.back().children; // rvalues
//     if(leftVals.size() != rightVals.size()) {
//         throw std::runtime_error("Semantic error: the number of lvalue and rvalue are incompatible");
//     }

//     int sz = leftVals.size();
//     while(sz --) {
//         AST::Node& lnode = leftVals.front(); 
//         AST::Node& rnode = rightVals.front();
//         rnode = simplify(rnode, varMap, scope);
//         if(varMap.find({lnode.token.value, scope}) != varMap.end()) { // if exists error
//             throw std::runtime_error("Semantic error: redeclaration");
//         }
//         varMap[{lnode.token.value, scope}] = Variable{lnode.token.value, rnode.type, rnode.token};
//     }
//     std::cout << "insert varMap.size(): " << scope << ' ' << varMap.size() << '\n';
// }

// void Semantic::updVarMap(AST::Node& assNode, MAP_VAR &varMap, int scope) {
//     std::list<AST::Node>& leftVals = assNode.children.front().children; // lvalues
//     std::list<AST::Node>& rightVals = assNode.children.back().children; // rvalues
//     if(leftVals.size() != rightVals.size()) {
//         throw std::runtime_error("Semantic error: the number of lvalue and rvalue are incompatible");
//     }

//     std::cout << "update varMap.size(): " << scope << ' ' << varMap.size() << '\n';
//     int sz = leftVals.size();
//     while(sz --) {
//         AST::Node& lnode = leftVals.front(); 
//         AST::Node& rnode = rightVals.front(); 
//         rnode = simplify(rnode, varMap, scope);
//         if(varMap.find({lnode.token.value, scope}) == varMap.end()) { // if does not exist
//             std::cout << "varMap.size(): " << lnode.token.value << ' ' << 
//             scope << ' ' << varMap.size() << '\n';
//             throw std::runtime_error("Semantic error: undeclared variable use");
//         }
//         varMap[{lnode.token.value, scope}] = Variable{lnode.token.value, rnode.type, rnode.token};
//     }
// }


// AST::Node Semantic::simplify(AST::Node node, MAP_VAR& varMap, int scope) {
//     if(!isOperator(node.type)) return node;

//     if(node.children.size() != 2) {
//         std::cout << '\n'<< node.token.value << '\n';
//         throw std::runtime_error("Semantic error: incorrect number of operands");
//     }
//     AST::Node lnode = node.children.front();
//     AST::Node rnode = node.children.back();

//     lnode = substitude(lnode, varMap, scope);
//     rnode = substitude(rnode, varMap, scope);

//     // move semantic

//     return operate(simplify(lnode, varMap, scope).token, rnode.token, node.token);
// }

// bool Semantic::isOperator(NodeType nt) { // +-*/ |&^
//     return nt == NodeType::EXPRESSION || nt == NodeType::TERM;
// }

// AST::Node Semantic::substitude(AST::Node idntfNode, MAP_VAR& varMap, int scope) {
//     if (idntfNode.token.type != TokenType::IDENTIFIER) return idntfNode;
//     std::cout << "substitude: " << idntfNode.token.value;
//     return AST::Node{varMap[{idntfNode.token.value, scope}].nodeType, varMap[{idntfNode.token.value, scope}].value};
// }

// bool Semantic::areTypesCompatible(TokenType lt, TokenType rt) {
//     TokenType numx[6] = {TokenType::INTEGER, TokenType::REAL, TokenType::INTEGER, TokenType::REAL, TokenType::STRING, TokenType::BOOL};
//     TokenType numy[6] = {TokenType::REAL, TokenType::INTEGER, TokenType::INTEGER, TokenType::REAL, TokenType::STRING, TokenType::BOOL};
//     for (int i = 0; i < 6; i ++) {
//         if(numx[i] == lt && numy[i] == rt) return true;
//     }
//     return false;
// }

// bool Semantic::areTypesNumber(TokenType lt, TokenType rt) {
//     TokenType numx[4] = {TokenType::INTEGER, TokenType::REAL, TokenType::INTEGER, TokenType::REAL};
//     TokenType numy[4] = {TokenType::REAL, TokenType::INTEGER, TokenType::INTEGER, TokenType::REAL};
//     for (int i = 0; i < 4; i ++) {
//         if(numx[i] == lt && numy[i] == rt) return true;
//     }
//     return false;
// }

// AST::Node Semantic::operate(Token oprnd1, Token oprnd2, Token oprtr) {
//     Token retToken;
//     if(!areTypesNumber(oprnd1.type, oprnd2.type)) {
//         std::cout << "non-numbers: " << Printer::getTokenTypeName(oprnd1.type) << ' ' << Printer::getTokenTypeName(oprnd2.type) << '\n';
//         throw std::runtime_error("Semantic error: non-number operands");
//     }

//     switch (oprtr.type) {
//     case TokenType::PLUS:
//         retToken = (oprnd1.type == TokenType::INTEGER && oprnd2.type == TokenType::INTEGER) 
//             ? Token{TokenType::INTEGER, std::to_string(stoi(oprnd1.value) + stoi(oprnd2.value))}
//             : Token{TokenType::REAL, std::to_string(stod(oprnd1.value) + stod(oprnd2.value))};
//         break;
//     case TokenType::MINUS:
//         retToken = (oprnd1.type == TokenType::INTEGER && oprnd2.type == TokenType::INTEGER) 
//             ? Token{TokenType::INTEGER, std::to_string(stoi(oprnd1.value) - stoi(oprnd2.value))}
//             : Token{TokenType::REAL, std::to_string(stod(oprnd1.value) - stod(oprnd2.value))};
//         break;
//     case TokenType::MULTIPLY:
//         retToken = (oprnd1.type == TokenType::INTEGER && oprnd2.type == TokenType::INTEGER) 
//             ? Token{TokenType::INTEGER, std::to_string(stoi(oprnd1.value) * stoi(oprnd2.value))}
//             : Token{TokenType::REAL, std::to_string(stod(oprnd1.value) * stod(oprnd2.value))};
//         break;
//     case TokenType::DIVIDE:
//         retToken = (oprnd1.type == TokenType::INTEGER && oprnd2.type == TokenType::INTEGER) 
//             ? Token{TokenType::INTEGER, std::to_string(stoi(oprnd1.value) / stoi(oprnd2.value))}
//             : Token{TokenType::REAL, std::to_string(stod(oprnd1.value) / stod(oprnd2.value))};
//         break;
//     default: // WARNING!!!
//         break;
//     }

//     return AST::Node{NodeType::FACTOR, retToken};
// }