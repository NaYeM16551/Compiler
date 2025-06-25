#include "header_files/symbol_table.h"
#include "header_files/scope_table.h"
#include "header_files/symbol_stack.h"
#include <iostream>

SymbolTable::SymbolTable(int bucketSize,std::ofstream& fout) {
    ScopeTable* sc = new ScopeTable(bucketSize);
    sc->setUniqueId("1");
    s.push(sc);
    fout << "       ScopeTable# 1 created" << std::endl;
}

void SymbolTable::createScope(int bucketSize,ofstream& fout) {
    ScopeTable* sc = new ScopeTable(bucketSize);
    auto parentScope = s.top();
    parentScope->incrementChildCount();

    sc->setUniqueId(parentScope->getUniqueId() + "." + std::to_string(parentScope->getChildCount()));
    s.push(sc);
    fout<<"         ScopeTable# "<<s.top()->getUniqueId()<<" created"<<std::endl;
}

bool SymbolTable::removeScopeHelper(std::ofstream& fout) {

    fout << "      ScopeTable# " << s.top()->getUniqueId() << " deleted" << std::endl;
    delete s.top(); // Delete the popped ScopeTable object
    s.pop();
    
    return true;
}

bool SymbolTable::removeScope(std::ofstream& fout) {
    if (s.size() <= 1) {
        fout << "        ScopeTable# 1 cannot be deleted" << std::endl;
        return false;
    }
    
    return removeScopeHelper(fout);
    

}

bool SymbolTable::removeAll(std::ofstream& fout)
{
    while(!s.empty())
    {
        removeScopeHelper(fout);
    }
    return true;
}

bool SymbolTable::insert(const std::string symbolName, const std::string symbolType,ofstream& fout)
{
    auto scopeTable = s.top();
    fout<< scopeTable->insert(symbolName, symbolType)<<endl;
    return true;
}

string SymbolTable::remove(const std::string symbolName)
{
    auto scopeTable = s.top();
    return scopeTable->deleteNode(symbolName);
}

void SymbolTable::printCurrentScope(ofstream& fout)
{
    auto scopeTable = s.top();
    scopeTable->print(fout);
}

bool SymbolTable::lookup(const std::string symbolName,ofstream& fout)
{
    auto temp=s;
    //std::cout<<temp.top().getUniqueId()<<std::endl;
    std::string sf;
    while(sf.empty() && !temp.empty())
    {
        sf=temp.top()->lookup(symbolName);
        temp.pop();
    }
    if(!sf.empty())
    {
        fout<<sf<<endl;
        return true;
    }

      
    return false;  
}

void SymbolTable::printAllScope(ofstream& fout)
{
    auto temp = s;
    while (!temp.empty())
    {
        auto sc = temp.top();
        sc->print(fout);
        temp.pop();
    }
}
