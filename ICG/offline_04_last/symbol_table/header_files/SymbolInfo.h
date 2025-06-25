#ifndef SYMBOLINFO_HPP
#define SYMBOLINFO_HPP

#include <string>
#include <cstdio>
#include "functionContainer.h"

class FunctionContainer; // Forward declaration

class SymbolInfo {
public:
    int id;
    int x, y;

    std::string name;
    std::string type;
    std::string asmName;
    SymbolInfo* next;
    int line;
    bool isArray;
    int arrSize;
    int stackOffset;
    FunctionContainer* func;

    SymbolInfo(int id = -1, int x = -1, std::string name = "", std::string type = "", SymbolInfo* next = nullptr);
    SymbolInfo(const char* name, const char* type, int line);
    SymbolInfo(SymbolInfo* s);
    ~SymbolInfo();

    void setId(int _id);
    void setX(int _x);
    void setY(int _y);
    void setName(std::string str);
    void setType(std::string str);
    void setNext(SymbolInfo* p);

    int getId();
    int getX();
    int getY();
    std::string getName();
    std::string getType();
    SymbolInfo* getNext();

    void print(FILE* fp);
};

#endif
