#ifndef ICG_HPP
#define ICG_HPP
#include<bits/stdc++.h>
#include "../symbol_table/header_files/ScopeTable.h"
#include "../symbol_table/header_files/functionContainer.h"
#include "../symbol_table/header_files/SymbolInfo.h"
#include "../symbol_table/header_files/Symboltable.h"
#include "../symbol_table/header_files/node.h"
#include "varCount.hpp"
using std::vector, std::string, std::isnan, std::to_string;
#define ENDL "\n";

typedef void (*fptr_expr_void)(const Node* node);

std::ofstream code;

// Used in return statements to jump to the label before RET
string return_label;

int labelCount = 0;
int currStack = 0;

string get_label() {
    return "L" + to_string(labelCount++);
}

vector<SymbolInfo*> globalVarList;
int global_symbol_count = 0;

// Has only the global variables and functions inserted at start
extern SymbolTable table;

// Functions corresponding to (almost )each non-terminal
void start(const Node* node);
void branching_expression_statement(const Node* node, const string& body_label, const string& end_label);
void branching_expression(const Node* node, const string& body_label, const string& end_label);
void branching_logic_expression(const Node* node, const string& body_label, const string& end_label);


void compound_statement(const Node* node, vector<SymbolInfo>& paramList);
void statements(const Node* node);
void statement(const Node* node);
void var_declaration(const Node* node);
void expression_statement(const Node* node);

void variable(const Node* node);
void expression(const Node* node);
void logic_expression(const Node* node);
void rel_expression(const Node* node);
void simple_expression(const Node* node);
void term(const Node* node);
void unary_expression(const Node* node);
void factor(const Node* node);
void argument_list(const Node* node);


void branching_rel_expression(const Node* node, const string& body_label, const string& end_label);
void branching_simple_expression(const Node* node, const string& body_label, const string& end_label);
void branching_term(const Node* node, const string& body_label, const string& end_label);
void branching_unary_expression(const Node* node, const string& body_label, const string& end_label);
void branching_factor(const Node* node, const string& body_label, const string& end_label);

fptr_expr_void get_expr_void_funct(string label) {
    if(label == "expression_statement") return expression_statement;
    if(label == "expression") return expression;
    if(label == "logic_expression") return logic_expression;
    if(label == "rel_expression") return rel_expression;
    if(label == "simple_expression") return simple_expression;
    if(label == "term") return term;
    if(label == "unary_expression") return unary_expression;
    if(label == "factor") return factor;
}



void start(const Node* node) {
    code.open("code.asm", std::ofstream::out);
    code << ".MODEL SMALL" << ENDL;
    code << ".STACK 1000H" << ENDL;
    code << ".DATA" << ENDL;
    code << "\tnumber DB \"00000$\"" << ENDL;

    const Node* program = node->children[0];
    vector<Node*> unitList;
    int cnt=0;
    while(program->label == "program") {
        if(program->children.size() == 2) unitList.push_back(program->children[1]->children[0]);
        else unitList.push_back(program->children[0]->children[0]);
        program = program->children[0];
        //cnt++;
    }
    //cout<<"cnt :"<<cnt<<endl;

    for(int i=0; i<unitList.size(); i++) {
        if(unitList[i]->label != "var_declaration") continue;
        var_declaration(unitList[i]);
    }

    for(int i=0; i<globalVarList.size(); i++) {
        if(globalVarList[i]->isArray)
            code << "\t" << globalVarList[i]->asmName << " DW " << globalVarList[i]->arrSize << " DUP (0000H)" << "\n";
        else
            code << "\t" << globalVarList[i]->asmName << " DW 0" << "\n";
        delete globalVarList[i];
    }
    globalVarList.clear();
    code << ".CODE" << ENDL;
    // init_main();

    Node* currFunc;
    SymbolInfo* info;
    for(int i=unitList.size()-1; i>=0; i--) {
        if(unitList[i]->label != "func_definition") continue;
        currFunc = unitList[i];
        info = table.lookup(currFunc->children[1]->lexeme);
        info->asmName = info->name + "_" + std::to_string(global_symbol_count++);
        cout<<info->name + "_" + std::to_string(global_symbol_count++)<<endl;
        if(info->name != "main") {
            code << "\n" << info->asmName << " PROC" << ENDL;
        }
        else
            code << "\nmain PROC" << ENDL;

        if(info->name == "main") {
            code << "\tMOV AX , @DATA" << ENDL;
            code << "\tMOV DS , AX" << ENDL;
        }

        code << "\tPUSH BP" << ENDL;
        code << "\tMOV BP , SP" << ENDL;

        int varCount = compound_statement_varcount(currFunc->children[currFunc->children.size()-1]);
        code << "\tSUB SP , " << varCount << ENDL;

        return_label = get_label();
        currStack = 0;
        compound_statement(currFunc->children[currFunc->children.size()-1], info->func->params);

        code << return_label << ":" << ENDL;
        code << "\tADD SP , " << varCount << ENDL;
        code << "\tPOP BP" << ENDL;
        if(info->name == "main") {
            code << "\tMOV AH , 4CH" << ENDL;
            code << "\tINT 21H" << ENDL;
        }
        else {
            code << "\tRET";
            if(info->func != nullptr) code << " " << 2 * info->func->params.size();
            code << ENDL;
        }
        if(info->name != "main") {
            code << "\n" << info->asmName << " ENDP" << ENDL;
        }
        else
            code << "\nmain ENDP" << ENDL;
    }

    std::ifstream printlnFile("println.asm");
    string str;
    while(getline(printlnFile, str)) 
        code << str << ENDL;
    code << "\nEND MAIN" << ENDL;
    code.close();
    printlnFile.close();
}



void compound_statement(const Node* node, vector<SymbolInfo>& paramList) {
    if(node->children.size() == 2) return;
    table.enter_scope();

    int stackPos = 4;
    for(int i=0; i<paramList.size(); i++) {
        paramList[i].stackOffset = stackPos;
        stackPos += 2;
        table.insert(&paramList[i]);
    }

    code << "\n\t; Compund statement starting at line " << node->startLine << ENDL;
    statements(node->children[1]);
    code << "\t; Compund statement ending at line " << node->endLine << ENDL;
   table.exit_scope();
}

void statements(const Node* node) {
    // statements : statement
    if(node->children.size() == 1) {
        return statement(node->children[0]);
    }
    
    vector<Node*> statementList;
    const Node* stmt = node;
    while(stmt->label == "statements") {
        if(stmt->children.size() == 1) 
            statementList.push_back(stmt->children[0]);
        else statementList.push_back(stmt->children[1]);
        stmt = stmt->children[0];
    }

    for(int i=statementList.size()-1; i>=0; i--)
        statement(statementList[i]);
}

void statement(const Node* node) {
    vector<SymbolInfo> param;
    if(node->children[0]->label == "var_declaration")
        return var_declaration(node->children[0]);
    if(node->children[0]->label == "expression_statement")
        return expression_statement(node->children[0]);
    if(node->children[0]->label == "compound_statement")
        return compound_statement(node->children[0],param);
    
    // IF LPAREN expression RPAREN statement
    if(node->children[0]->label == "IF" && node->children.size() == 5) {
        code << "\n\t; if statement starting at line " << node->startLine << ENDL;
        string next_label = get_label(), end_label = get_label();

        branching_expression(node->children[2], next_label, end_label);

        code << next_label << ":" << ENDL;
        statement(node->children[4]);
        code << end_label << ":" << ENDL;
        code << "\t; if statement ending at line " << node->endLine << ENDL;
        return;
    }

    // IF LPAREN expression RPAREN statement ELSE statement
    if(node->children[0]->label == "IF") {
        code << "\n\t; if-else statement starting at line " << node->startLine << ENDL;

        string if_label = get_label(), else_label = get_label(), end_label = get_label();
        branching_expression(node->children[2], if_label, else_label);

        code << if_label << ":" << ENDL;
        statement(node->children[4]);
        code << "\tJMP " << end_label << ENDL;

        code << else_label << ":" << ENDL;
        statement(node->children[6]);
        code << end_label << ":" << ENDL;

        code << "\t; if-else statement ending at line " << node->endLine << ENDL;
        return;
    }

    // WHILE LPAREN expression RPAREN statement
    if(node->children[0]->label == "WHILE") {
        string start_label = get_label(), body_label = get_label(), end_label = get_label();
        
        code << start_label << ":\t; while loop starting at line " << node->startLine << ENDL;
        branching_expression(node->children[2], body_label, end_label);

        code << body_label << ":" << ENDL;
        statement(node->children[4]);
        code << "\tJMP " << start_label << ENDL;

        code << end_label << ":\t; while loop ending at line " << node->endLine << ENDL;
        return;
    }

    // FOR LPAREN expression_statement expression_statement expression RPAREN statement
    if(node->children[0]->label == "FOR") {
        code << "\n\t; for loop starting at line " << node->startLine;
        expression_statement(node->children[2]);
        string start_label = get_label(), body_label = get_label(), end_label = get_label();

        code << start_label << ":" << ENDL;
        branching_expression_statement(node->children[3], body_label, end_label);

        code << body_label << ":" << ENDL;
        statement(node->children[6]);
        expression(node->children[4]);
        code << "\tPOP AX" << ENDL;
        code << "\tJMP " << start_label << ENDL;

        code << end_label << ":\t; for loop ending at line " << node->endLine << ENDL;
        return;
    }

    // RETURN expression SEMICOLON
    if(node->children[0]->label == "RETURN" && node->children.size() == 3) {
        code << "\t; Return statement at line " << node->startLine << ENDL;
        expression(node->children[1]);
        code << "\tPOP AX" << ENDL;
        code << "\tJMP " << return_label << ENDL;
        return;
    }

    // RETURN SEMICOLON
    if(node->children[0]->label == "RETURN") {
        code << "\t; Return statement at line " << node->startLine << ENDL;
        code << "\tJMP " << return_label << ENDL;
        return;
    }

    // PRINTLN LPAREN expression RPAREN SEMICOLON
    expression(node->children[2]);
    code << "\tPOP AX" << ENDL;
    code << "\tCALL println" << " ; At line " << node->children[0]->startLine << ENDL;
}

void var_declaration(const Node* node) {
    string typeSpecifier = node->children[0]->children[0]->label;
    cout<<typeSpecifier<<endl;
    vector<SymbolInfo*> varList;
    Node* decl = node->children[1]; // declaration_list

    string name;
    int line;
   // cout<<line<<endl;
    while(decl->label == "declaration_list") {
        if(decl->children.size() == 1) { // ID
            name = decl->children[0]->lexeme;
            line = decl->children[0]->startLine;
            if(table.get_top_id() == 1) {
                globalVarList.push_back(new SymbolInfo(name.c_str(), typeSpecifier.c_str(), line));
                globalVarList[globalVarList.size()-1]->asmName = name + "_" + std::to_string(global_symbol_count++);
                cout<< name + "_" + std::to_string(global_symbol_count++)<<endl;
                //SymbolInfo* info = table.lookup(name);
               // info->asmName = globalVarList[globalVarList.size()-1]->asmName;
            }
            else
                varList.push_back(new SymbolInfo(name.c_str(), typeSpecifier.c_str(), line));
        }
        else if(decl->children.size() == 4) { // ID LTHIRD CONST_INT RTHIRD
            name = decl->children[0]->lexeme;
            line = decl->children[0]->startLine;
            if(table.get_top_id() == 1) {
                globalVarList.push_back(new SymbolInfo(name.c_str(), typeSpecifier.c_str(), line));
                globalVarList[globalVarList.size()-1]->isArray = true;
                globalVarList[globalVarList.size()-1]->arrSize = stoi(decl->children[2]->lexeme);
                globalVarList[globalVarList.size()-1]->asmName = name + "_" + std::to_string(global_symbol_count++);
               // SymbolInfo* info = table.lookup(name);
               // info->asmName = globalVarList[globalVarList.size()-1]->asmName;
            }
            else {
                varList.push_back(new SymbolInfo(name.c_str(), typeSpecifier.c_str(), line));
                varList[varList.size()-1]->isArray = true;
                varList[varList.size()-1]->arrSize = stoi(decl->children[2]->lexeme);
            }
        }
        else if(decl->children.size() == 3) { // declaration_list COMMA ID
            name = decl->children[2]->lexeme;
            line = decl->children[2]->startLine;
            if(table.get_top_id() == 1) {
                globalVarList.push_back(new SymbolInfo(name.c_str(), typeSpecifier.c_str(), line));
                globalVarList[globalVarList.size()-1]->asmName = name + "_" + std::to_string(global_symbol_count++);
               // SymbolInfo* info = table.lookup(name);
               // info->asmName = globalVarList[globalVarList.size()-1]->asmName;
            }
            else 
                varList.push_back(new SymbolInfo(name.c_str(), typeSpecifier.c_str(), line));
        }
        else { // declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
            name = decl->children[2]->lexeme;
            line = decl->children[2]->startLine;
            if(table.get_top_id() == 1) {
                globalVarList.push_back(new SymbolInfo(name.c_str(), typeSpecifier.c_str(), line));
                globalVarList[globalVarList.size()-1]->isArray = true;
                globalVarList[globalVarList.size()-1]->arrSize = stoi(decl->children[4]->lexeme);
                globalVarList[globalVarList.size()-1]->asmName = name + "_" + std::to_string(global_symbol_count++);
               // SymbolInfo* info = table.lookup(name);
               // info->asmName = globalVarList[globalVarList.size()-1]->asmName;
            }
            else {
                varList.push_back(new SymbolInfo(name.c_str(), typeSpecifier.c_str(), line));
                varList[varList.size()-1]->isArray = true;
                varList[varList.size()-1]->arrSize = stoi(decl->children[4]->lexeme);
            }
            
        }
        decl = decl->children[0];
    }
    
    for(int i=varList.size()-1; i>=0; i--) {
        if(!varList[i]->isArray) {
            currStack -= 2;
            varList[i]->arrSize = 1;
        }
        else currStack -= 2 * varList[i]->arrSize;
        varList[i]->stackOffset = currStack;
        table.insert(varList[i]);
        // code << "\tSUB SP , " << 2*varList[i]->arrSize << ENDL;
    }
}

void expression_statement(const Node* node) { 
    if(node->children.size() == 1) return;
    code << "\n\t; Expression statement starting at line " << node->startLine << ENDL;
    expression(node->children[0]);
    code << "\tPOP AX" << ENDL;
    code << "\t; Expression statement ending at line " << node->endLine << ENDL;
}

void variable(const Node* node) {
    SymbolInfo* info = table.lookup(node->children[0]->lexeme);

    if(node->children.size() == 1 && info->id == 1) { // global non-array variable
        code << "\tPUSH " << info->asmName << ENDL;
        return;
    }

    if(node->children.size() == 1) { // local non-array variable
        code << "\tPUSH BP[" << info->stackOffset << "]" << ENDL;
        return;
    }

    if(info->id == 1) { // global array
        expression(node->children[2]);
        code << "\tPOP SI" << ENDL;
        code << "\tSHL SI , 1" << ENDL;
        code << "\tPUSH " << info->asmName << "[SI]" << ENDL;
        return;
    }

    // local array
    expression(node->children[2]);
    code << "\tPOP SI" << ENDL;
    code << "\tSHL SI , 1" << ENDL;
    code << "\tADD SI , " << info->stackOffset << ENDL;
    code << "\tPUSH BP[SI]" << ENDL;
}

void expression(const Node* node) {
    if(node->children.size() == 1) {
        const Node* currNode = node;
        while(currNode->children.size() == 1 && currNode->label != "factor") currNode = currNode->children[0];
        fptr_expr_void func = get_expr_void_funct(currNode->label);
        return func(currNode);
    }

    // Starting variable assignment
    Node* variable = node->children[0];
    SymbolInfo* info = table.lookup(variable->children[0]->lexeme);
    
    if(variable->children.size() == 1 && info->id == 1) { // global non-array variable
        logic_expression(node->children[2]);
        code << "\tPOP AX" << ENDL;
        code << "\tMOV " << info->asmName << " , AX" << ENDL;
        code << "\tPUSH " << info->asmName << ENDL;
        return;
    }

    if(variable->children.size() == 1) { // local non-array variable
        logic_expression(node->children[2]);
        code << "\tPOP AX" << ENDL;
        code << "\tMOV BP[" << info->stackOffset << "] , AX" << ENDL;
        code << "\tPUSH BP[" << info->stackOffset << "]" << ENDL;
        return;
    }

    if(info->id == 1) { // global array
        logic_expression(node->children[2]);
        expression(variable->children[2]);
        code << "\tPOP SI" << ENDL;
        code << "\tPOP AX" << ENDL;
        code << "\tSHL SI , 1" << ENDL;
        code << "\tMOV " << info->asmName << "[SI] , AX" << ENDL;
        code << "\tPUSH " << info->asmName << "[SI]" << ENDL;
        return;
    }

    // local array
    logic_expression(node->children[2]);
    expression(variable->children[2]);
    code << "\tPOP SI" << ENDL;
    code << "\tPOP AX" << ENDL;
    code << "\tSHL SI , 1" << ENDL;
    code << "\tADD SI , " << info->stackOffset << ENDL;
    code << "\tMOV BP[SI] , AX" << ENDL;
    code << "\tPUSH BP[SI]" << ENDL;
}

void logic_expression(const Node* node) {
    if(node->children.size() == 1) {
        const Node* currNode = node;
        while(currNode->children.size() == 1 && currNode->label != "factor") currNode = currNode->children[0];
        fptr_expr_void func = get_expr_void_funct(currNode->label);
        return func(currNode);
    }
    
    if(node->children[1]->lexeme == "||") {
        string one_label = get_label(), end_label = get_label();
        rel_expression(node->children[0]);
        code << "\tPOP AX" << ENDL;
        code << "\tCMP AX , 0" << ENDL;
        code << "\tJNE " << one_label << ENDL;

        rel_expression(node->children[2]);
        code << "\tPOP AX" << ENDL;
        code << "\tCMP AX , 0" << ENDL;
        code << "\tJNE " << one_label << ENDL;

        code << "\tMOV AX , 0" << ENDL;
        code << "\tJMP " << end_label << ENDL;
        code << one_label << ":" << ENDL;
        code << "\tMOV AX , 1" << ENDL;
        code << end_label << ":" << ENDL;
        code << "\tPUSH AX" << ENDL;
        return;
    }

    string zero_label = get_label(), end_label = get_label(); 
    rel_expression(node->children[0]);
    code << "\tPOP AX" << ENDL;
    code << "\tCMP AX , 0" << ENDL;
    code << "\tJE " << zero_label << ENDL;

    rel_expression(node->children[2]);
    code << "\tPOP AX" << ENDL;
    code << "\tCMP AX , 0" << ENDL;
    code << "\tJE " << zero_label << ENDL;

    code << "\tMOV AX , 1" << ENDL;
    code << "\tJMP " << end_label << ENDL;
    code << zero_label << ":" << ENDL;
    code << "\tMOV AX , 0" << ENDL;
    code << end_label << ":" << ENDL;
    code << "\tPUSH AX" << ENDL;
}

void rel_expression(const Node* node) {
    if(node->children.size() == 1) {
        const Node* currNode = node;
        while(currNode->children.size() == 1 && currNode->label != "factor") currNode = currNode->children[0];
        fptr_expr_void func = get_expr_void_funct(currNode->label);
        return func(currNode);
    }

    simple_expression(node->children[0]);
    simple_expression(node->children[2]);
    code << "\tPOP CX" << ENDL;
    code << "\tPOP AX" << ENDL;
    string one_label = get_label(), end_label = get_label();
    string op = node->children[1]->lexeme;

    code << "\tCMP AX , CX" << ENDL;

    if(op == "<") code << "\tJL " << one_label << ENDL;
    if(op == "<=") code << "\tJLE " << one_label << ENDL;
    if(op == ">") code << "\tJG " << one_label << ENDL;
    if(op == ">=") code << "\tJGE " << one_label << ENDL;
    if(op == "==") code << "\tJE " << one_label << ENDL;
    if(op == "!=") code << "\tJNE " << one_label << ENDL;

    code << "\tMOV AX , 0" << ENDL;
    code << "\tJMP " << end_label << ENDL;
    code << one_label << ":" << ENDL;
    code << "\tMOV AX , 1" << ENDL;
    code << end_label << ":" << ENDL;
    code << "\tPUSH AX" << ENDL;
}

void simple_expression(const Node* node) {
    if(node->children.size() == 1) {
        const Node* currNode = node;
        while(currNode->children.size() == 1 && currNode->label != "factor") currNode = currNode->children[0];
        fptr_expr_void func = get_expr_void_funct(currNode->label);
        return func(currNode);
    }

    simple_expression(node->children[0]);
    term(node->children[2]);
    code << "\tPOP CX" << ENDL;
    code << "\tPOP AX" << ENDL;
    
    if(node->children[1]->lexeme == "+") {
        code << "\tADD AX , CX" << ENDL;
    }
    else {
        code << "\tSUB AX , CX" << ENDL;
    }
    code << "\tPUSH AX" << ENDL;
}
void factor(const Node* node) {
    string label = node->children[0]->label;
    if(label == "CONST_INT" || label == "CONST_FLOAT") {
        code << "\tMOV AX , " << stoi(node->children[0]->lexeme) << ENDL;
        code << "\tPUSH AX"<< ENDL;
        return;
    }

    if(label == "LPAREN")
        return expression(node->children[1]);
    if(label == "variable" && node->children.size() == 1)
        return variable(node->children[0]);
    
    if(label == "variable") {
        variable(node->children[0]);
        Node* variable = node->children[0];
        SymbolInfo* info = table.lookup(variable->children[0]->lexeme);
        
        if(variable->children.size() == 1 && info->id == 1) { // global non-array variable
            if(node->children[1]->label == "INCOP") {
                code << "\tADD WORD PTR " << info->asmName << " , 1" << ENDL;
            }
            else code << "\tSUB WORD PTR " << info->asmName << " , 1" << ENDL;
            return;
        }

        if(variable->children.size() == 1) { // local non-array variable
            if(node->children[1]->label == "INCOP") {
                code << "\tADD WORD PTR BP[" << info->stackOffset << "] , 1" << ENDL;
            }
            else code << "\tSUB WORD PTR BP[" << info->stackOffset << "] , 1" << ENDL;
            return;
        }

        if(info->id == 1) { // global array
            if(node->children[1]->label == "INCOP") {
                code << "\tADD WORD PTR " << info->asmName << "[SI] , 1" << ENDL;
            }
            else code << "\tSUB WORD PTR " << info->asmName << "[SI] , 1" << ENDL;
            return;
        }

        // local array
        if(node->children[1]->label == "INCOP") {
            code << "\tADD WORD PTR " << "BP[SI] , 1" << ENDL;
        }
        else code << "\tSUB WORD PTR " << "BP[SI] , 1" << ENDL;
        return;
    }

    if(label == "ID") {
        argument_list(node->children[2]);
        SymbolInfo* info = table.lookup(node->children[0]->lexeme);
        code << "\tCALL " << info->asmName << " ; At line " << node->children[0]->startLine << ENDL;
        code << "\tPUSH AX" << ENDL; // Return value was stored in AX
    }
}


void term(const Node* node) {
    if(node->children.size() == 1) {
        const Node* currNode = node;
        while(currNode->children.size() == 1 && currNode->label != "factor") currNode = currNode->children[0];
        fptr_expr_void func = get_expr_void_funct(currNode->label);
        return func(currNode);
    }

    term(node->children[0]);
    unary_expression(node->children[2]);
    code << "\tPOP CX" << ENDL;
    code << "\tPOP AX" << ENDL;
    
    if(node->children[1]->lexeme == "*") {
        code << "\tIMUL CX" << ENDL;
        code << "\tPUSH AX" << ENDL;
    }
    else if(node->children[1]->lexeme == "/") {
        code << "\tCWD" << ENDL;
        code << "\tIDIV CX" << ENDL;
        code << "\tPUSH AX" << ENDL;
    }
    else {
        code << "\tCWD" << ENDL;
        code << "\tIDIV CX" << ENDL;
        code << "\tPUSH DX" << ENDL;
    }
}



void argument_list(const Node* node) {
    if(node->children.size() == 0) return;
    Node* arguments = node->children[0];
    while(arguments->label == "arguments") {
        if(arguments->children.size() == 1) logic_expression(arguments->children[0]);
        else logic_expression(arguments->children[2]);
        arguments = arguments->children[0];
    }
}

void branching_expression_statement(const Node* node, const string& body_label, const string& end_label) {
    if(node->children.size() == 1) {
        code << "\tJMP " << body_label << ENDL;
        return;
    }
    branching_expression(node->children[0], body_label, end_label);
}

void branching_expression(const Node* node, const string& body_label, const string& end_label) {
    if(node->children.size() == 1)
        return branching_logic_expression(node->children[0], body_label, end_label);

    expression(node);
    code << "\tPOP AX" << ENDL;
    code << "\tCMP AX , 0" << ENDL;
    code << "\tJNE " << body_label << ENDL;
    code << "\tJMP " << end_label << ENDL;
}

void branching_logic_expression(const Node* node, const string& body_label, const string& end_label) {
    if(node->children.size() == 1)
        return branching_rel_expression(node->children[0], body_label, end_label);
    
    if(node->children[1]->lexeme == "||") {
        string pass_label = get_label(), mid_label = get_label();
        branching_rel_expression(node->children[0], pass_label, mid_label);

        code << pass_label << ":" << ENDL;
        code << "\tJMP " << body_label << ENDL;

        code << mid_label << ":" << ENDL;
        branching_rel_expression(node->children[2], body_label, end_label);
        return;
    }

    string mid_label = get_label(); 
    branching_rel_expression(node->children[0], mid_label, end_label);

    code << mid_label << ":" << ENDL;
    branching_rel_expression(node->children[2], body_label, end_label);
}

void branching_rel_expression(const Node* node, const string& body_label, const string& end_label) {
    if(node->children.size() == 1) return branching_simple_expression(node->children[0], body_label, end_label);

    simple_expression(node->children[0]);
    simple_expression(node->children[2]);
    code << "\tPOP CX" << ENDL;
    code << "\tPOP AX" << ENDL;
    string op = node->children[1]->lexeme;

    code << "\tCMP AX , CX" << ENDL;

    if(op == "<") code << "\tJL " << body_label << ENDL;
    if(op == "<=") code << "\tJLE " << body_label << ENDL;
    if(op == ">") code << "\tJG " << body_label << ENDL;
    if(op == ">=") code << "\tJGE " << body_label << ENDL;
    if(op == "==") code << "\tJE " << body_label << ENDL;
    if(op == "!=") code << "\tJNE " << body_label << ENDL;

    code << "\tJMP " << end_label << ENDL;
}

void branching_simple_expression(const Node* node, const string& body_label, const string& end_label) {
    if(node->children.size() == 1) return branching_term(node->children[0], body_label, end_label);

    simple_expression(node);
    code << "\tPOP AX" << ENDL;
    code << "\tCMP AX , 0" << ENDL;
    code << "\tJNE " << body_label << ENDL;
    code << "\tJMP " << end_label << ENDL;
}

void branching_term(const Node* node, const string& body_label, const string& end_label) {
    if(node->children.size() == 1) return branching_unary_expression(node->children[0], body_label, end_label);

    term(node);
    code << "\tPOP AX" << ENDL;
    code << "\tCMP AX , 0" << ENDL;
    code << "\tJNE " << body_label << ENDL;
    code << "\tJMP " << end_label << ENDL;
}

void branching_unary_expression(const Node* node, const string& body_label, const string& end_label) {
    if(node->children.size() == 1) return branching_factor(node->children[0], body_label, end_label);

    string op = node->children[0]->lexeme;
    if(op=="+" || op=="-") return branching_unary_expression(node->children[1], body_label, end_label);

    string fail_label = get_label();
    branching_unary_expression(node->children[1], fail_label, body_label);
    code << fail_label << ":" << ENDL;
    code << "\tJMP " << end_label << ENDL;
}

void branching_factor(const Node* node, const string& body_label, const string& end_label) {
    string label = node->children[0]->label;
    if(label == "CONST_INT" || label == "CONST_FLOAT") {
        if(stoi(node->children[0]->lexeme)){ 
            code << "\tJMP " << body_label << ENDL; 
        }
        else code << "\tJMP " << end_label << ENDL;
        return;
    }

    if(label == "LPAREN")
        return branching_expression(node->children[1], body_label, end_label);
    
    factor(node);
    code << "\tPOP AX" << ENDL;
    code << "\tCMP AX , 0" << ENDL;
    code << "\tJNE " << body_label << ENDL;
    code << "\tJMP " << end_label << ENDL;
}
void unary_expression(const Node* node) {
    if(node->children.size() == 1) 
        return factor(node->children[0]);

    unary_expression(node->children[1]);
    string op = node->children[0]->lexeme;
    if(op == "+") return;
    if(op == "-") {
        code << "\tPOP AX" << ENDL;
        code << "\tNEG AX" << ENDL;
        code << "\tPUSH AX" << ENDL;
        return;
    }
    // op == "!"
    string one_label = get_label(), end_label = get_label();
    code << "\tPOP AX" << ENDL;
    code << "\tCMP AX , 0" << ENDL;
    code << "\tJE " << one_label << ENDL;
    code << "\tMOV AX , 0" << ENDL;
    code << "\tJMP " << end_label << ENDL;
    code << one_label << ":" << ENDL;
    code << "\tMOV AX , 1" << ENDL;
    code << end_label << ":" << ENDL;
    code << "\tPUSH AX" << ENDL;
}


#endif