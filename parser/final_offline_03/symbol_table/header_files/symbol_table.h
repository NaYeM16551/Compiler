#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "scope_table.h"
#include "symbol_stack.h"
#include<bits/stdc++.h>
using namespace std;

class SymbolTable {
private:
    //stack<ScopeTable*> s;
     stack<ScopeTable*> s;
    // ScopeStack<ScopeTable*> s;
   
     ScopeTable* globalScopeTable;
     bool removeScopeHelper(std::ofstream& fout);
    
public:
    SymbolTable(int bucketSize,std::ofstream& fout);

    void createScope(int bucketSize,ofstream& fout);

    bool removeScope(std::ofstream& fout);
     bool removeAll(std::ofstream& fout);

    bool insert(const std::string symbolName, const std::string symbolType,ofstream& fout);

    string remove(const std::string symbolName);

    void printCurrentScope(ofstream& fout);

    void printAllScope(ofstream& fout);
    string lookup(const std::string symbolName,ofstream& fout);
    bool insertIntoGlobal(const std::string symbolName, const std::string symbolType,ofstream& fout);
    string lookupLocal(const std::string symbolName,ofstream& fout);
};

#endif /* SYMBOL_TABLE_H */
