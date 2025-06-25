#ifndef PARSETREENODE_H
#define PARSETREENODE_H

#include <iostream>
#include <vector>
using namespace std;
/* enum func_status {
    NONE,
    FUNCTION,
    // Add other function statuses as needed
};

struct Param {
    std::string name;
    std::string data_type;
    bool is_array;
}; */



class ParseTreeNode {
private:
    string rule;
    int startLine;
    int endLine;
    bool terminal;
    string type;
    vector<ParseTreeNode*> children;

public:
    // Constructor
    ParseTreeNode(string rule, int startLine, int endLine, bool terminal = false);

    // Getter methods
    string getRule() const;
    int getStartLine() const;
    int getEndLine() const;
    bool isTerminal() const;

    // Setter methods
    void setRule(const string& newRule);
    void setStartLine(int newStartLine);
    void setEndLine(int newEndLine);
    void setTerminal(bool newTerminal);
    void addChild(vector<ParseTreeNode*> child);
    void print_tree_node(ostream &out, int level = 0);
    void  setType(string type);
    string getType();
     ~ParseTreeNode();
    

};

#endif // PARSETREENODE_H
