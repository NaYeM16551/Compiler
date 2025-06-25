#ifndef PARSETREENODE_H
#define PARSETREENODE_H

#include <iostream>
#include <vector>
using namespace std;
class ParseTreeNode {
private:
    string rule;
    int startLine;
    int endLine;
    bool terminal;
    string label;
   

public:
    vector<ParseTreeNode*> children;
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
    void setLabel(string label);

    //getter
    int getChildrenSize();
    ParseTreeNode* getChildrenAt(int index);
    string getLabel();
     ~ParseTreeNode();
    

};

#endif // PARSETREENODE_H
