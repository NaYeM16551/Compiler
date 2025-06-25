#include "../header_files/ScopeTable.h"

extern FILE* logout;

ull ScopeTable::sdbm_hash(std::string& str) {
    // sdbm_hash definition
     ull hash = 0, len = str.length();
        for(int i=0; i<len; i++)
            hash = str[i] + (hash<<6) + (hash<<16) - hash;
        return hash;
}

int ScopeTable::hash(std::string& str) {
    // hash definition
     return sdbm_hash(str) % bucket_count;
}

ScopeTable::ScopeTable(int size, int i, ScopeTable* parent) {
    // Constructor definition
    bucket_count = size;
        id = i;
        container = new SymbolInfo*[bucket_count];
        for(int i=0; i<bucket_count; i++){
            container[i] = nullptr;
        }
        parent_scope = parent;
}

ScopeTable::~ScopeTable() {
    // Destructor definition
     for(int i=0; i<bucket_count; i++){
            SymbolInfo* ptr = container[i];
            SymbolInfo* temp;
            while(ptr != nullptr){
                temp = ptr;
                ptr = ptr->getNext();
                delete ptr;
            }
        }
        delete[] container;
}

void ScopeTable::set_parent_scope(ScopeTable* ptr) {
    // set_parent_scope definition
    parent_scope = ptr;
}

ScopeTable* ScopeTable::get_parent_scope() {
    // get_parent_scope definition
    return parent_scope;
}

int ScopeTable::get_bucket_count() {
    // get_bucket_count definition
    return bucket_count;
}

int ScopeTable::get_id() {
    // get_id definition
    return id;
}

bool ScopeTable::insert(std::string& name, std::string& type) {
    // insert definition
     int hash_index = hash(name);
        if(lookup(name) != nullptr) return false;
        SymbolInfo* new_object_ptr = new SymbolInfo(id, hash_index+1, name, type, nullptr);

        if(container[hash_index] == nullptr){
            new_object_ptr->setY(1);
            container[hash_index] = new_object_ptr;
            return true;
        }

        SymbolInfo *ptr = container[hash_index];
        int index = 1;
        while(ptr->getNext() != nullptr){
            ptr = ptr->getNext();
            index++;
        }
        new_object_ptr->setY(index+1);
        ptr->setNext(new_object_ptr);
        return true;
}

bool ScopeTable::insert(SymbolInfo* s) {
    // insert definition
     int hash_index = hash(s->name);
        if(lookup(s->name) != nullptr) return false;
        s->id = this->id;
        s->x = hash_index + 1;
        s->next = nullptr;

        if(container[hash_index] == nullptr){
            s->setY(1);
            container[hash_index] = s;
            return true;
        }

        SymbolInfo *ptr = container[hash_index];
        int index = 1;
        while(ptr->getNext() != nullptr){
            ptr = ptr->getNext();
            index++;
        }
        s->setY(index+1);
        ptr->setNext(s);
        return true;
}

SymbolInfo* ScopeTable::lookup(std::string& name) {
    // lookup definition
    int hash_index = hash(name);
        
        SymbolInfo* ptr = container[hash_index];
        while(ptr != nullptr){
            if(ptr->getName() == name) break;
            ptr = ptr->getNext();
        }

        return ptr;
}

bool ScopeTable::remove(std::string& name) {
    // remove definition
    int hash_index = hash(name);
        if(container[hash_index] == nullptr) return false;

        if(container[hash_index]->getName() == name){
            SymbolInfo* temp = container[hash_index];
            container[hash_index] = container[hash_index]->getNext();
            delete temp;
            return true;
        }

        SymbolInfo* ptr = container[hash_index];
        while(ptr->getNext() != nullptr && (ptr->getNext())->getName() != name)
            ptr = ptr->getNext();
        if(ptr->getNext() == nullptr) return false;

        SymbolInfo* ptr_to_remove = ptr->getNext();
        SymbolInfo* next_ptr = ptr_to_remove->getNext();
        ptr->setNext(next_ptr);
        delete ptr_to_remove;
        return true;
}

void ScopeTable::print() {
    // print definition
    fprintf(logout, "\tScopeTable# %d\n", id);
        for(int i=0; i<bucket_count; i++){
            if(container[i] == nullptr) continue;
			fprintf(logout, "\t%d--> ", i+1);
            SymbolInfo *ptr = container[i];
            while(ptr != nullptr){
				ptr->print(logout);
                ptr = ptr->getNext();
            }
            fprintf(logout, "\n");
        }
}