#ifndef FUNCTIONCONTAINER_HPP
#define FUNCTIONCONTAINER_HPP

#include <string>
#include <vector>
#include "SymbolInfo.h"

typedef unsigned long long ull;

class SymbolInfo;
struct FunctionContainer {
    std::string retType;
    std::vector<SymbolInfo> params;

    FunctionContainer(std::string r);
    FunctionContainer(std::string r, std::vector<SymbolInfo> p);
};

#endif
