
#include "../header_files/SymbolInfo.h"


SymbolInfo::SymbolInfo(int id, int x, std::string name, std::string type, SymbolInfo* next) {
    // Constructor definition
        this->id = id;
        this->x = x;
        this->name = name;
        this->type = type;
        this->next = next;
        isArray = false;
        func = nullptr;
        arrSize = 1;
}

SymbolInfo::SymbolInfo(const char* name, const char* type, int line) {
    // Constructor definition
     this->name = name;
        this->type = type;
        this->line = line;
        next = nullptr;
        isArray = false;
        func = nullptr;
        arrSize = 1;
}

SymbolInfo::SymbolInfo(SymbolInfo* s) {
    // Copy constructor definition
    id = s->id;
        x = s->x;
        y = s->y;
        name = s->name;
        type = s->type;
        next = s->next;
        line = s->line;
        isArray = s->isArray;
        if(s->func == nullptr || s->func == NULL) func = s->func;
        else func = new FunctionContainer(s->func->retType, s->func->params);
        arrSize = s->arrSize;
        stackOffset = s->stackOffset;
}

SymbolInfo::~SymbolInfo() {
    // Destructor definition
    if(func != nullptr) delete func;
}

void SymbolInfo::setId(int _id) {
    // setId definition
    id = _id;
}

void SymbolInfo::setX(int _x) {
    // setX definition
    x = _x;
}

void SymbolInfo::setY(int _y) {
    // setY definition
    y = _y;
}

void SymbolInfo::setName(std::string str) {
    // setName definition
    name=str;
}

void SymbolInfo::setType(std::string str) {
    // setType definition
    type=str;
}

void SymbolInfo::setNext(SymbolInfo* p) {
    // setNext definition
    next = p;
}

int SymbolInfo::getId() {
    // getId definition
    return id;
}

int SymbolInfo::getX() {
    // getX definition
    return x;
}

int SymbolInfo::getY() {
    // getY definition
    return y;
}

std::string SymbolInfo::getName() {
    // getName definition
    return name;
}

std::string SymbolInfo::getType() {
    // getType definition
    return type;
}

SymbolInfo* SymbolInfo::getNext() {
    // getNext definition
    return next;
}

void SymbolInfo::print(FILE* fp) {
    // print definition
     if(func != nullptr){
            fprintf(fp, "<%s, %s, %s> ", name.c_str(), type.c_str(), func->retType.c_str());
            return;
        }
        fprintf(fp, "<%s", name.c_str());
        if(isArray)
            fprintf(fp, ", ARRAY");
        fprintf(fp, ", %s> ", type.c_str());
}