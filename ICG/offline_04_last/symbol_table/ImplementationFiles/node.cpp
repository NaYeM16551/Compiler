#include "../header_files/node.h"

Node::Node(bool leaf, string label, string lexeme, int startLine) {
    this->leaf = leaf;
    this->label = label;
    this->lexeme = lexeme;
    this->startLine = startLine;
    endLine = startLine;
}

Node::~Node() {
    for (int i = 0; i < children.size(); i++)
        delete children[i];
}

void Node::print_derivation(int offset, FILE* out) {
    update_line();
    for (int i = 0; i < offset; i++)
        fprintf(out, " ");
    fprintf(out, "%s : ", label.c_str());
    for (int i = 0; i < children.size(); i++)
        fprintf(out, "%s ", children[i]->label.c_str());
    fprintf(out, "\t<Line: %d-%d>\n", startLine, endLine);
}

void Node::print_leaf(int offset, FILE* out) {
    for (int i = 0; i < offset; i++)
        fprintf(out, " ");
    fprintf(out, "%s : %s\t<Line: %d>\n", label.c_str(), lexeme.c_str(), startLine);
}

void Node::update_line() {
    if (children.empty()) return;
    startLine = children[0]->startLine;
    endLine = children[children.size() - 1]->endLine;
}
