// Constructor
#include "../headerFiles/parse_tree_node.h"

ParseTreeNode::ParseTreeNode(string rule, int startLine, int endLine, bool terminal)
    : rule(rule), startLine(startLine), endLine(endLine), terminal(terminal) {}

// Getter methods
string ParseTreeNode::getRule() const {
    return rule;
}

int ParseTreeNode::getStartLine() const {
    return startLine;
}

int ParseTreeNode::getEndLine() const {
    return endLine;
}

bool ParseTreeNode::isTerminal() const {
    return terminal;
}

// Setter methods
void ParseTreeNode::setRule(const string& newRule) {
    rule = newRule;
}

void ParseTreeNode::setStartLine(int newStartLine) {
    startLine = newStartLine;
}

void ParseTreeNode::setEndLine(int newEndLine) {
    endLine = newEndLine;
}

void ParseTreeNode::setTerminal(bool newTerminal) {
    terminal = newTerminal;
}

void ParseTreeNode::addChild(vector<ParseTreeNode*> child)
    {
        for(auto&c : child)
         children.push_back(c);
    }

void ParseTreeNode::print_tree_node(ostream &out, int level) {
        for (int j = 0; j < level; j++) out << " ";
        out << rule << (terminal ? "" : " ") << "\t";
        out << "<Line: ";
        if (terminal) out << startLine;
        else out << startLine << "-" << endLine;
        out << ">" << endl;
        for (auto& c : children) {
            c->print_tree_node(
                out,
                level + 1);  // lcurls -> LCURL has been handled, so no need to
                             // check that separately
        }
    } 
   int ParseTreeNode::getChildrenSize()
   {
    return this->children.size();
   }

   ParseTreeNode* ParseTreeNode::getChildrenAt(int index)
   {
    return this->children[index];
   }

   void ParseTreeNode::setLabel(string label)
   {
    this->label=label;
   }

   string ParseTreeNode::getLabel()
   {
    return this->label;
   }

ParseTreeNode::~ParseTreeNode() {
    for (auto child : children) {
        delete child;
    }
}
       