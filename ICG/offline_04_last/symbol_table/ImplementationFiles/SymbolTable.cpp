#include "../header_files/Symboltable.h"

SymbolTable::SymbolTable(int size) {
    // Constructor definition
     global_bucket_size = size;
        current_table = new ScopeTable(global_bucket_size, 1);
        table_count = 1;
}

SymbolTable::~SymbolTable() {
    // Destructor definition
     ScopeTable *curr = current_table;
        while(curr != nullptr){
            ScopeTable *temp = curr;
            curr = curr->get_parent_scope();
            delete temp;
        }
}

int SymbolTable::get_table_count() {
    // get_table_count definition
    return table_count;
}

int SymbolTable::get_top_id() {
    // get_top_id definition
    return current_table->get_id();
}

void SymbolTable::enter_scope() {
    // enter_scope definition
        table_count++;
        ScopeTable* new_table = new ScopeTable(global_bucket_size, table_count, current_table);
        current_table = new_table;
}

bool SymbolTable::exit_scope() {
    // exit_scope definition
     if(get_top_id() == 1) return false;
        ScopeTable* temp = current_table;
        current_table = current_table->get_parent_scope();
        delete temp;
        return true;
}

bool SymbolTable::insert(std::string& name, std::string& type) {
    // insert definition
    return current_table->insert(name, type);
}

bool SymbolTable::insert(SymbolInfo* s) {
    // insert definition
    return current_table->insert(s);
    
        
        
}

bool SymbolTable::remove(std::string& name) {
    // remove definition
    return current_table->remove(name);
}

SymbolInfo* SymbolTable::lookup(std::string& name) {
    // lookup definition
     ScopeTable *curr = current_table;
        SymbolInfo *result = nullptr;
        while(curr != nullptr){
            result = curr->lookup(name);
            if(result != nullptr) break;
            curr = curr->get_parent_scope();
        }
        return result;
}

void SymbolTable::print_current_table() {
    // print_current_table definition
     current_table->print();
}

void SymbolTable::print_all_tables() {
    // print_all_tables definition
    ScopeTable *curr_ptr = current_table;
        while(curr_ptr != nullptr){
            curr_ptr->print();
            curr_ptr = curr_ptr->get_parent_scope();
        }
}