#include "../header_files/functionContainer.h"

FunctionContainer::FunctionContainer(std::string r) {
    // Constructor definition
    retType = r;
}

FunctionContainer::FunctionContainer(std::string r, std::vector<SymbolInfo> p) {
    // Constructor definition
      retType = r;
      params = p;
}