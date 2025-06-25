#include <bits/stdc++.h>
#include "header_files/scope_table.h"
#include "header_files/hash_function.h" // Include your hash function header here
using namespace std;

#define watch(x) cerr<<#x<<" = "<<x<<"\n"

ScopeTable::ScopeTable(int bucketSize)
{
    this->bucketSize = bucketSize;
    arr = new SymbolInfo *[bucketSize](); // Initialize array elements to nullptr
    this->childCount = 0;
}
ScopeTable::ScopeTable()
{
    this->childCount = 0;
}

SymbolInfo *ScopeTable::getTailNode(SymbolInfo *head,int &i)
{
    auto temp = head;
    i++;
    while (temp->next != nullptr)
    {
        i++;
        temp = temp->next;
    }
        
    return temp;
}

void ScopeTable::printLL(SymbolInfo *head,ofstream &fout) 
{
    fout<<" --> ";
    while (head)
    {
        fout << "<" << head->getSymbolName() << "," << head->getSymbolType() << ">";
        head = head->next;
    }
    fout << endl;
}

bool ScopeTable::lookup_check(const std::string symbolName)
{
    int bucketIndex = sdbm_hash(symbolName.c_str()) % bucketSize;
    auto head = arr[bucketIndex];
    int i = 1;
    while (head)
    {
        if (head->getSymbolName() == symbolName)
        {
            return true;
        }
        i++;

        head = head->next;
    }
    return false;
}

void ScopeTable::setUniqueId(std::string uniqueId)
{
    this->unique_id = uniqueId;
}
std::string ScopeTable::getUniqueId()
{
    return this->unique_id;
}

int ScopeTable::getChildCount()
{
    return this->childCount;
}

void ScopeTable::incrementChildCount()
{
    this->childCount++;
    
}

bool ScopeTable::insert(const std::string symbolName, const std::string symbolType)
{
    //watch(symbolName);
    
    if (lookup_check(symbolName))
    {
        std::string ans= "     '" + symbolName + "' already exists in the current ScopeTable";
    
        return false;
    }

    SymbolInfo *SymInfo = new SymbolInfo(symbolName, symbolType);
    auto bucketIndex = sdbm_hash(symbolName.c_str()) % bucketSize;
    int i=1;

    if (arr[bucketIndex] == nullptr)
    {
        arr[bucketIndex] = SymInfo;
    }
    else
    {
        auto tail = getTailNode(arr[bucketIndex],i);
        tail->next = SymInfo;
    }
    std::string ans="        Inserted  at position <"+to_string(bucketIndex+1)+", "+to_string(i)+"> of ScopeTable# "+getUniqueId();
    
    return true;
}

std::string ScopeTable::lookup(const std::string symbolName)
{
    int bucketIndex = sdbm_hash(symbolName.c_str()) % bucketSize;
    auto head = arr[bucketIndex];
    int i = 1;
    std::string str;
    while (head)
    {
        if (head->getSymbolName() == symbolName)
        {
            std::string ans= "       '" + symbolName + "'  found at position <" +to_string(bucketIndex + 1) + 
            ", " +to_string(i) + "> of ScopeTable# " + getUniqueId();
            return head->getSymbolType();
        }
        i++;

        head = head->next;
    }
    return str;
}


std::string ScopeTable::deleteNode(const std::string symbolName)
{
    int bucketIndex = sdbm_hash(symbolName.c_str()) % bucketSize;
    SymbolInfo *head = arr[bucketIndex];
    SymbolInfo *prev = nullptr;
    int i = 1;

    while (head != nullptr)
    {
        if (head->getSymbolName() == symbolName)
        {
            if (prev != nullptr)
            {
                prev->next = head->next;
            }
            else
            {
                arr[bucketIndex] = head->next;
            }

            delete head;
            string ans= "       Deleted '" + symbolName + "' from position <" + to_string(bucketIndex + 1) + "," +to_string(i) + "> of ScopeTable# "
             + getUniqueId();
            return ans;
        }

        i++;
        prev = head;
        head = head->next;
    }

    string ans= "       Not found in the current ScopeTable# " +getUniqueId();
    return ans;
}

void ScopeTable::print(ofstream& fout)
{
    fout<<"    ScopeTable# "<< getUniqueId() << endl;
    for (int i = 0; i < bucketSize; ++i)
    {
        if(arr[i])
        {
            fout << "    "<<i + 1;
            printLL(arr[i],fout);
        }
        
    }
}
ScopeTable::~ScopeTable() {
    for (int i = 0; i < bucketSize; ++i) {
        SymbolInfo* current = arr[i];
        while (current != nullptr) {
            SymbolInfo* temp = current;
            current = current->next;
            temp->next=nullptr;
            delete temp; // Free memory for each SymbolInfo node
        }
    }
    delete[] arr; // Free memory for the array of pointers

    
}

