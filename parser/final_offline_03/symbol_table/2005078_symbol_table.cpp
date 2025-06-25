#include "header_files/symbol_table.h"
#include "header_files/scope_table.h"
#include "header_files/symbol_stack.h"
#include <iostream>

int getScopeTableCount()
{
    static int cnt=0;
    cnt++;
    return cnt;
}

SymbolTable::SymbolTable(int bucketSize,std::ofstream& fout) {
    globalScopeTable = new ScopeTable(bucketSize);
    globalScopeTable->setUniqueId(to_string(getScopeTableCount()));
    s.push(globalScopeTable);
    cout << "       ScopeTable# 1 created" << std::endl;
}

void SymbolTable::createScope(int bucketSize,ofstream& fout) {
    ScopeTable* sc = new ScopeTable(bucketSize);
    //auto parentScope = s.top();
    //parentScope->incrementChildCount();

    sc->setUniqueId(to_string(getScopeTableCount()));
    s.push(sc);
    cout<<"         ScopeTable# "<<s.top()->getUniqueId()<<" created"<<std::endl;
}

bool SymbolTable::removeScopeHelper(std::ofstream& fout) {

    cout << "      ScopeTable# " << s.top()->getUniqueId() << " deleted" << std::endl;
    delete s.top(); // Delete the popped ScopeTable object
    s.pop();
    
    return true;
}

bool SymbolTable::removeScope(std::ofstream& fout) {
    if (s.size() <= 1) {
        cout << "        ScopeTable# 1 cannot be deleted" << std::endl;
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
    cout<<"inserting size"<<s.size()<<endl;
    auto scopeTable = s.top();
    cout<<scopeTable->getUniqueId()<<" in insert"<<endl;
    cout<< scopeTable->insert(symbolName, symbolType)<<endl;
    return true;
}

bool SymbolTable::insertIntoGlobal(const std::string symbolName, const std::string symbolType,ofstream& fout)
{
    cout<<"inserting size"<<s.size()<<endl;
    cout<< globalScopeTable->insert(symbolName, symbolType)<<endl;
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

string SymbolTable::lookup(const std::string symbolName,ofstream& fout)
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
        cout<<sf<<endl;
        return sf;
    }

      
    return sf;  
}

string SymbolTable::lookupLocal(const std::string symbolName,ofstream& fout)
{
    //auto temp=s;
    //std::cout<<temp.top().getUniqueId()<<std::endl;
    std::string sf;
   // while(sf.empty() && !temp.empty())
    {
        sf=s.top()->lookup(symbolName);
        //temp.pop();
    }
    

      
    return sf;  
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
