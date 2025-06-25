#ifndef SCOPETABLE_H
#define SCOPETABLE_H

#include <string>
#include <fstream> // Include <fstream> for file stream operations
#include "symbol_info.h" // Include your SymbolInfo class header here
class ScopeTable {
private:
    std::string unique_id;
    SymbolInfo **arr;
    int bucketSize;
    int childCount;

private:
    SymbolInfo* getTailNode(SymbolInfo* head, int& i);
    void printLL(SymbolInfo *head,std::ofstream& fout);
    bool lookup_check(const std::string symbolName);

public:
    explicit ScopeTable(int bucketSize);
    ScopeTable();
    void setUniqueId(std::string unique_id);
    std::string getUniqueId();
    bool insert(const std::string symbolName, const std::string symbolType);
    std::string lookup(const std::string symbolName);
    std::string deleteNode(const std::string symbolName);
    void print(std::ofstream& fout);
    void incrementChildCount();
    int getChildCount();

    ~ScopeTable();
};

#endif // SCOPETABLE_H
