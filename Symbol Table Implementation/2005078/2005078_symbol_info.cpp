#include "header_files/symbol_info.h"

SymbolInfo::SymbolInfo(std::string symbolName, std::string symbolType)
    : symbolName(symbolName), symbolType(symbolType), next(nullptr) {
     
}

std::string SymbolInfo::getSymbolName() {
    return symbolName;
}

std::string SymbolInfo::getSymbolType() {
    return symbolType;
}

SymbolInfo::~SymbolInfo() {
    // Delete the 'next' pointer 
    if (next != nullptr) {
        delete next;
        next = nullptr;
    }
    // No explicit cleanup needed for std::string as they manage their own memory.
}
