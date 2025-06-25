#ifndef NODE_HPP
#define NODE_HPP

#include <bits/stdc++.h>
using namespace std;

struct Node {
    bool leaf;
    string label;
    vector<Node*> children;
    int startLine;
    int endLine;
    string lexeme;
    double eval;

    Node(bool leaf, string label, string lexeme="", int startLine=-1);
    ~Node();
    void print_derivation(int offset, FILE* out);
    void print_leaf(int offset, FILE* out);
    void update_line();
};

#endif
