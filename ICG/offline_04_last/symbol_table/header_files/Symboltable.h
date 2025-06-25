#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <string>
#include "ScopeTable.h"
#include "SymbolInfo.h"

class SymbolTable {
public:
    ScopeTable* current_table;
    int table_count;
    int global_bucket_size;

    SymbolTable(int size);
    ~SymbolTable();

    int get_table_count();
    int get_top_id();

    void enter_scope();
    bool exit_scope();
    
    bool insert(std::string& name, std::string& type);
    bool insert(SymbolInfo* s);
    bool remove(std::string& name);
    SymbolInfo* lookup(std::string& name);
    void print_current_table();
    void print_all_tables();
};

#endif
