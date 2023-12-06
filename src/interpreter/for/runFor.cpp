#include "interpreter.h"
#include "printer.h"

void Interpreter::runFor(AST::Node node)
{
    std::string indexName = "";
    std::list<std::string> indexValues;
    AST::Node forBody;
    int fromIdx = 0, toIdx = 0;

    for (auto child : node.children)
    {
        if (child.type == NodeType::IDENTIFIER)
        {
            indexName = child.value.value;
        }

        else if (child.type == NodeType::RANGE)
        {
            std::string rangeStr = child.value.value; // "1..5"
            
            std::string delimiter = "..";
            
            size_t pos = 0;

            int i = 0;
            while (rangeStr[i] != '.') {
                fromIdx *= 10;
                fromIdx += (rangeStr[i]-'0');
                i ++;
            }
            i += 2; // ignoring ..
            while (i < rangeStr.size()) {
                toIdx *= 10;
                toIdx += (rangeStr[i]-'0');
                i ++;
            }
            
        } 
        
        else if (child.type == NodeType::FORLOOP_BODY) 
        {
            forBody = child;
        }
    }
    
    for(int i=fromIdx; i<=toIdx; i++)
    {
        genDict[indexName] = AST::Node(NodeType::FACTOR, Token(TokenType::INTEGER, std::to_string(i)));
        
        for(auto child : forBody.children)
        {
            if (child.type == NodeType::BREAK){
                genDict.erase(indexName);
                return;
            }
            interpret(child);
        }

        // if(interpret(forBody)) {
        //     return 
        // }
        genDict.erase(indexName);
    }
    
}