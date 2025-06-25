#ifndef var_COUNT 
#define var_COUNT

#include<bits/stdc++.h>
#include "../symbol_table/header_files/ScopeTable.h"
#include "../symbol_table/header_files/functionContainer.h"
#include "../symbol_table/header_files/SymbolInfo.h"
#include "../symbol_table/header_files/Symboltable.h"
#include "../symbol_table/header_files/node.h"
using std::vector, std::string, std::isnan, std::to_string;
#define ENDL "\n";

// To know the amount of stack space to set aside for local variables
int compound_statement_varcount(const Node* node);
int statements_varcount(const Node* node);
int statement_varcount(const Node* node);
int var_declaration_varcount(const Node* node);

int compound_statement_varcount(const Node* node) {
    if(node->children.size() == 2) return 0; // LCURL RCURL
    return statements_varcount(node->children[1]);
}

int statements_varcount(const Node* node) {
    // statements : statement
    if(node->children.size() == 1) {
        return statement_varcount(node->children[0]);
    }
    
    vector<Node*> statementList;
    const Node* stmt = node;
    while(stmt->label == "statements") {
        if(stmt->children.size() == 1) 
            statementList.push_back(stmt->children[0]);
        else statementList.push_back(stmt->children[1]);
        stmt = stmt->children[0];
    }

    int counter = 0;
    for(int i=statementList.size()-1; i>=0; i--)
        counter += statement_varcount(statementList[i]);
    return counter;
}

int statement_varcount(const Node* node) {
    if(node->children[0]->label == "var_declaration")
        return var_declaration_varcount(node->children[0]);
    if(node->children[0]->label == "compound_statement")
        return compound_statement_varcount(node->children[0]);

    // IF LPAREN expression RPAREN statement
    if(node->children[0]->label == "IF" && node->children.size() == 5) 
        return statement_varcount(node->children[4]);

    // IF LPAREN expression RPAREN statement ELSE statement
    if(node->children[0]->label == "IF") 
        return statement_varcount(node->children[4]) + statement_varcount(node->children[6]);

    // WHILE LPAREN expression RPAREN statement
    if(node->children[0]->label == "WHILE") 
        return statement_varcount(node->children[4]);

    // FOR LPAREN expression_statement expression_statement expression RPAREN statement
    if(node->children[0]->label == "FOR") 
        return statement_varcount(node->children[6]);
    
    return 0;
}

int var_declaration_varcount(const Node* node) {
    Node* decl = node->children[1]; // declaration_list
    int counter = 0;
    while(decl->label == "declaration_list") {
        if(decl->children.size() == 1) { // ID
            counter += 2;
        }
        else if(decl->children.size() == 4) { // ID LTHIRD CONST_INT RTHIRD
            counter += 2 * stoi(decl->children[2]->lexeme);
        }
        else if(decl->children.size() == 3) { // declaration_list COMMA ID
            counter += 2;
        }
        else { // declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
            counter += 2 * stoi(decl->children[4]->lexeme);
        }
        decl = decl->children[0];
    }
    return counter;   
}
#endif