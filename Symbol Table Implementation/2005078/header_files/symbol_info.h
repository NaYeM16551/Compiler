#ifndef SYMBOL_INFO_H
#define SYMBOL_INFO_H

#include <string>

class SymbolInfo {
private:
    std::string symbolName;
    std::string symbolType;

public:
    SymbolInfo *next;

    SymbolInfo(std::string symbolName, std::string symbolType);

    std::string getSymbolName();
    std::string getSymbolType();

    ~SymbolInfo();
};

#endif // SYMBOL_INFO_H
