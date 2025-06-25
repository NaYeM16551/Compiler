#ifndef SCOPETABLE_HPP
#define SCOPETABLE_HPP

#include <string>
#include "SymbolInfo.h"

class ScopeTable {
public:
    SymbolInfo **container;
    int bucket_count;
    int id;
    ScopeTable* parent_scope;

    static unsigned long long sdbm_hash(std::string& str);
    int hash(std::string& str);

    ScopeTable(int size, int i, ScopeTable* parent = nullptr);
    ~ScopeTable();

    void set_parent_scope(ScopeTable* ptr);
    ScopeTable* get_parent_scope();
    int get_bucket_count();
    int get_id();

    bool insert(std::string& name, std::string& type);
    bool insert(SymbolInfo* s);
    SymbolInfo* lookup(std::string& name);
    bool remove(std::string& name);
    void print();
};

#endif
