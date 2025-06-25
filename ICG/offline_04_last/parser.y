%{
#include<bits/stdc++.h>
#include "symbol_table/header_files/node.h"
#include "symbol_table/header_files/SymbolInfo.h"
#include "symbol_table/header_files/Symboltable.h"
#include "symbol_table/header_files/ScopeTable.h"
#include "symbol_table/header_files/functionContainer.h"
#include "ICG/icg.hpp"
using std::vector, std::string, std::isnan, std::toupper;

int yyparse(void);
int yylex(void);

// I/O files
extern FILE *yyin;
FILE *logout;
FILE *errout;
FILE *parseout;

// counters
extern int line_count;
extern int error_count;

SymbolTable table(11);

// temporary containers
string currType;
vector<SymbolInfo*> currVars;
vector<SymbolInfo*> currParams;
vector<string> argList;
// vector<Node*> unitList;

// function helpers
bool inFunction = false;
string returnType;

int errLine;
Node* root;

void yyerror(string  s){
	cout<<s<<endl;
} 

void del_s(SymbolInfo* a){
	if(a!=NULL && a!=nullptr) delete a;
}



void print_tree(FILE* fp, int offset, Node* node){
    if(node->leaf) {
        node->print_leaf(offset, fp);
        return;
    }
    node->print_derivation(offset, fp);
    for(int i=0; i<node->children.size(); i++){
        print_tree(fp, offset+1, node->children[i]);
    }
}
string _toupper(string s) {
	string str = s;
	for(int i=0; i<str.length(); i++) str[i] = toupper(str[i]);
	return str;
}
void log_print(const char* text){fprintf(logout, "%s\n", text);}
void insert_function(string type, SymbolInfo* s){
	inFunction = true;
	s->type = "FUNCTION";
	FunctionContainer* container = new FunctionContainer(type);
	for(int i=0; i<currParams.size(); i++)
		container->params.push_back(*currParams[i]);
	s->func = container;
	returnType = type;

	if(table.insert(s)) // No previous declaration, new function inserted
		return;

	

	

	table.remove(s->name);
	table.insert(s);
}



%}
//%define parse.error verbose
%union{
	SymbolInfo* symbolInfo;
	int line;
	Node* node;
}
%token<line> LPAREN RPAREN LCURL RCURL LSQUARE RSQUARE
%token<line> COMMA SEMICOLON
%token<symbolInfo> ID CONST_INT CONST_FLOAT
%token<line> PRINTLN
%token<line> IF ELSE FOR WHILE RETURN
%token<line> INT FLOAT VOID
%token<symbolInfo> ADDOP MULOP INCOP DECOP RELOP ASSIGNOP LOGICOP NOT


%type<node> start program unit func_declaration func_definition parameter_list compound_statement
%type<node> var_declaration type_specifier declaration_list statements statement expression_statement
%type<node> variable expression logic_expression rel_expression simple_expression term unary_expression
%type<node> factor argument_list arguments
%type start_scope  




%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

/* %destructor	{delete_symbolinfo($$);} <symbolInfo> */

%%

start : program 
	{
		log_print("start : program");
		$$ = new Node(false, "start");
		$$->children.push_back($1);
		$$->update_line();
		root = $$;
		//check_def_exists();
	}
	;

program : program unit { 
		log_print("program : program unit");
		$$ = new Node(false, "program");
		$$->children.push_back($1);
		$$->children.push_back($2);
		$$->update_line();
	}
	| unit{
		log_print("program : unit");
		$$ = new Node(false, "program");
		$$->children.push_back($1);
		$$->update_line();
	}
	;
	
unit : var_declaration{ 
		log_print("unit : var_declaration");
		$$ = new Node(false, "unit");
		$$->children.push_back($1);
		$$->update_line();
		// unitList.push_back($1);
	}
    | func_declaration{
		log_print("unit : func_declaration");
		$$ = new Node(false, "unit");
		$$->children.push_back($1);
		$$->update_line();
		// unitList.push_back($1);
	}
    | func_definition{
		log_print("unit : func_definition");
		$$ = new Node(false, "unit");
		$$->children.push_back($1);
		$$->update_line();
		// unitList.push_back($1);
	}
	
    ;
     
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON { 
			log_print("func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
			FunctionContainer* container = new FunctionContainer($1->children[0]->label);
			for(int i=0; i<currParams.size(); i++){
				container->params.push_back(*currParams[i]);
				delete currParams[i];
			}
			currParams.clear();

			SymbolInfo* info = new SymbolInfo($2->name.c_str(), "FUNC_DEC", $2->line);
			info->func = container;
			
			if(!table.insert(info)){
				/* fprintf(errout, "Line# %d: Re-declaration of function %s\n", info->line, info->name.c_str());
				fprintf(logout, "Line# %d: Re-declaration of function %s\n", info->line, info->name.c_str());
				error_count++;
				del_s(info); */
			}

			$$ = new Node(false, "func_declaration");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "ID", $2->name.c_str(), $2->line));
			$$->children.push_back(new Node(true, "LPAREN", "(", $3));
			$$->children.push_back($4);
			$$->children.push_back(new Node(true, "RPAREN", ")", $5));
			$$->children.push_back(new Node(true, "SEMICOLON", ";", $6));
			$$->update_line();
			
			del_s($2);
		}
		| type_specifier ID LPAREN RPAREN SEMICOLON{
			log_print("func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
			FunctionContainer* container = new FunctionContainer($1->children[0]->label);
			currParams.clear();

			SymbolInfo* info = new SymbolInfo($2->name.c_str(), "FUNC_DEC", $2->line);
			info->func = container;
			
			if(!table.insert(info)){
				/* fprintf(errout, "Line# %d: Re-declaration of function %s\n", info->line, info->name.c_str());
				fprintf(logout, "Line# %d: Re-declaration of function %s\n", info->line, info->name.c_str());
				error_count++; */
				del_s(info);
			}

			$$ = new Node(false, "func_declaration");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "ID", $2->name.c_str(), $2->line));
			$$->children.push_back(new Node(true, "LPAREN", "(", $3));
			$$->children.push_back(new Node(true, "RPAREN", ")", $4));
			$$->children.push_back(new Node(true, "SEMICOLON", ";", $5));
			$$->update_line();

			del_s($2);
		}
		
		
		
		;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN {insert_function($1->children[0]->label, $2);} compound_statement { 
			log_print("func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement");
			inFunction = false;
			for(int i=0; i<currParams.size(); i++) del_s(currParams[i]);
			currParams.clear();

			$$ = new Node(false, "func_definition");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "ID", $2->name.c_str(), $2->line));
			$$->children.push_back(new Node(true, "LPAREN", "(", $3));
			$$->children.push_back($4);
			$$->children.push_back(new Node(true, "RPAREN", ")", $5));
			$$->children.push_back($7);
			$$->update_line();
		}
		| type_specifier ID LPAREN RPAREN {insert_function($1->children[0]->label, $2);} compound_statement{
			log_print("func_definition : type_specifier ID LPAREN RPAREN compound_statement");
			inFunction = false;
			for(int i=0; i<currParams.size(); i++) del_s(currParams[i]);
			currParams.clear();

			$$ = new Node(false, "func_definition");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "ID", $2->name.c_str(), $2->line));
			$$->children.push_back(new Node(true, "LPAREN", "(", $3));
			$$->children.push_back(new Node(true, "RPAREN", ")", $4));
			$$->children.push_back($6);
			$$->update_line();
			
		}
 		;				


parameter_list : parameter_list COMMA type_specifier ID { 
			log_print("parameter_list : parameter_list COMMA type_specifier ID");
			string type = $3->children[0]->label;
			if(type == "VOID") 
			{

			}
				//error_void_parameter($3->startLine);
			else{
				$4->type = type;
				currParams.push_back($4);
			}

			$$ = new Node(false, "parameter_list");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "COMMA", ",", $2));
			$$->children.push_back($3);
			$$->children.push_back(new Node(true, "ID", $4->name, $4->line));
			$$->update_line();
			if(type=="VOID") del_s($4);
		}
		| parameter_list COMMA type_specifier {
			log_print("parameter_list : parameter_list COMMA type_specifier");
			string type = $3->children[0]->label;
			if(type == "VOID") 
			{

			}
				//error_void_parameter($3->startLine);
			else{
				SymbolInfo* info = new SymbolInfo("", type.c_str(), $3->children[0]->startLine);
				currParams.push_back(info);
			}

			$$ = new Node(false, "parameter_list");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "COMMA", ",", $2));
			$$->children.push_back($3);
			$$->update_line();
		}
 		| type_specifier ID {
			log_print("parameter_list : type_specifier ID");
			string type = $1->children[0]->label;
			if(type == "VOID") 
			{

			}
				//error_void_parameter($1->startLine);
			else{
				$2->type = type;
				currParams.push_back($2);
			}

			$$ = new Node(false, "parameter_list");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "ID", $2->name, $2->line));
			$$->update_line();
			if(type=="VOID") del_s($2);
		}
		| type_specifier {
			log_print("parameter_list : type_specifier");
			string type = $1->children[0]->label;
			if(type == "VOID") 
			{

			}
				//error_void_parameter($1->startLine);
			else{
				SymbolInfo* info = new SymbolInfo("", type.c_str(), $1->children[0]->startLine);
				currParams.push_back(info);
			}

			$$ = new Node(false, "parameter_list");
			$$->children.push_back($1);
			$$->update_line();
		}
		
 		;

 		
compound_statement : LCURL start_scope statements RCURL { 
				log_print("compound_statement : LCURL statements RCURL");
				table.print_all_tables();
				table.exit_scope();

				$$ = new Node(false, "compound_statement");
				$$->children.push_back(new Node(true, "LCURL", "{", $1));
				$$->children.push_back($3);
				$$->children.push_back(new Node(true, "RCURL", "}", $4));
				$$->update_line();
			}
 		    | LCURL start_scope RCURL {
				log_print("compound_statement : LCURL RCURL");
				table.print_all_tables();
				table.exit_scope();

				$$ = new Node(false, "compound_statement");
				$$->children.push_back(new Node(true, "LCURL", "{", $1));
				$$->children.push_back(new Node(true, "RCURL", "}", $3));
				$$->update_line();
			}
 		    ;

start_scope : { // parsetree not needed
			table.enter_scope();
			if(inFunction) {
				for(int i=0; i<currParams.size(); i++) {
					if(currParams[i]->name == ""){
						/* fprintf(errout, "Line# %d: Parameter name missing in function definition\n", currParams[i]->line);
						fprintf(logout, "Line# %d: Parameter name missing in function definition\n", currParams[i]->line);
						error_count++; */
						continue;
					}
					if(!table.insert(currParams[i])) {
						/* fprintf(errout, "Line# %d: Multiple declaration of parameter %s\n", currParams[i]->line, currParams[i]->name.c_str());
						fprintf(logout, "Line# %d: Multiple declaration of parameter %s\n", currParams[i]->line, currParams[i]->name.c_str()); */
						//error_count++;
					}
				}
				inFunction = false;
			}
		}
 		    
var_declaration : type_specifier declaration_list SEMICOLON { 
			log_print("var_declaration : type_specifier declaration_list SEMICOLON");
			 {
				for(int i=0; i<currVars.size(); i++){
					currVars[i]->type = currType;
					if(!table.insert(currVars[i])){
						/* fprintf(errout, "Line# %d: Identifier %s already in use\n", currVars[i]->line, currVars[i]->name.c_str());
						fprintf(logout, "Line# %d: Identifier %s already in use\n", currVars[i]->line, currVars[i]->name.c_str());
						error_count++; */
					}
					// if(currVars[i]->id == 1) globalVarList.push_back(new SymbolInfo(currVars[i]));
				}
			}
			currVars.clear();
			currType = "";

			$$ = new Node(false, "var_declaration");
			$$->children.push_back($1);
			$$->children.push_back($2);
			$$->children.push_back(new Node(true, "SEMICOLON", ";", $3));
			$$->update_line();
		}
		
 		;
 		 
type_specifier	: INT { 
			log_print("type_specifier : INT");
			currType = "INT";
			$$ = new Node(false, "type_specifier");
			$$->children.push_back(new Node(true, currType, "int", $1));
			$$->update_line();
		}
 		| FLOAT {
			log_print("type_specifier : FLOAT");
			currType = "FLOAT";
			$$ = new Node(false, "type_specifier");
			$$->children.push_back(new Node(true, currType, "float", $1));
			$$->update_line();
		}
 		| VOID {
			log_print("type_specifier : VOID");
			currType = "VOID";
			$$ = new Node(false, "type_specifier");
			$$->children.push_back(new Node(true, currType, "void", $1));
			$$->update_line();
		}
 		;
 		
declaration_list : declaration_list COMMA ID { 
			log_print("declaration_list : declaration_list COMMA ID");
			SymbolInfo* info = $3;
			currVars.push_back(info);

			$$ = new Node(false, "declaration_list");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "COMMA", ",", $2));
			$$->children.push_back(new Node(true, "ID", $3->name, $3->line));
			$$->update_line();
		}
 		| declaration_list COMMA ID LSQUARE CONST_INT RSQUARE {
			log_print("declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
			SymbolInfo* info = $3;
			info->isArray = true;
			info->arrSize = stoi($5->name);
			currVars.push_back(info);

			$$ = new Node(false, "declaration_list");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "COMMA", ",", $2));
			$$->children.push_back(new Node(true, "ID", $3->name, $3->line));
			$$->children.push_back(new Node(true, "LSQUARE", "[", $4));
			$$->children.push_back(new Node(true, "CONST_INT", $5->name, $5->line));
			$$->children.push_back(new Node(true, "RSQUARE", "]", $6));
			$$->update_line();
		}
		| declaration_list COMMA ID LSQUARE CONST_FLOAT RSQUARE {
			log_print("declaration_list : declaration_list COMMA ID LSQUARE CONST_FLOAT RSQUARE");
			SymbolInfo* info = $3;
			info->isArray = true;
			currVars.push_back(info);
			//error_invalid_index($3->line, $3->name);

			$$ = new Node(false, "declaration_list");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "COMMA", ",", $2));
			$$->children.push_back(new Node(true, "ID", $3->name, $3->line));
			$$->children.push_back(new Node(true, "LSQUARE", "[", $4));
			$$->children.push_back(new Node(true, "CONST_INT", $5->name, $5->line));
			$$->children.push_back(new Node(true, "RSQUARE", "]", $6));
			$$->update_line();
		}
 		| ID {
			log_print("declaration_list : ID");
			SymbolInfo* info = $1;
			currVars.push_back(info);

			$$ = new Node(false, "declaration_list");
			$$->children.push_back(new Node(true, "ID", $1->name, $1->line));
			$$->update_line();
		}
 		| ID LSQUARE CONST_INT RSQUARE {
			log_print("declaration_list : ID LSQUARE CONST_INT RSQUARE");
			SymbolInfo* info = $1;
			info->isArray = true;
			info->arrSize = stoi($3->name);
			currVars.push_back(info);

			$$ = new Node(false, "declaration_list");
			$$->children.push_back(new Node(true, "ID", $1->name, $1->line));
			$$->children.push_back(new Node(true, "LSQUARE", "[", $2));
			$$->children.push_back(new Node(true, "CONST_INT", $3->name, $3->line));
			$$->children.push_back(new Node(true, "RSQUARE", "]", $4));
			$$->update_line();
		}
		| ID LSQUARE CONST_FLOAT RSQUARE { // Array declared with floating point index
			log_print("declaration_list : ID LSQUARE CONST_FLOAT RSQUARE");
			SymbolInfo* info = $1;
			info->isArray = true;
			currVars.push_back(info);
			//error_invalid_index($1->line, $1->name);

			$$ = new Node(false, "declaration_list");
			$$->children.push_back(new Node(true, "ID", $1->name, $1->line));
			$$->children.push_back(new Node(true, "LSQUARE", "[", $2));
			$$->children.push_back(new Node(true, "CONST_INT", $3->name, $3->line));
			$$->children.push_back(new Node(true, "RSQUARE", "]", $4));
			$$->update_line();
		}
 		;
 		  
statements : statement { 
			log_print("statements : statement");
			$$ = new Node(false, "statements");
			$$->children.push_back($1);
			$$->update_line();
		}
		| statements statement {
			log_print("statements : statements statement");
			$$ = new Node(false, "statements");
			$$->children.push_back($1);
			$$->children.push_back($2);
			$$->update_line();
		}
		;
	   
statement : var_declaration { 
			log_print("statement : var_declaration");
			$$ = new Node(false, "statement");
			$$->children.push_back($1);
			$$->update_line();
		}
		| expression_statement {
			log_print("statement : expression_statement");
			$$ = new Node(false, "statement");
			$$->children.push_back($1);
			$$->update_line();
		}
		| compound_statement {
			log_print("statement: compound_statement");
			$$ = new Node(false, "statement");
			$$->children.push_back($1);
			$$->update_line();
		}
		| FOR LPAREN expression_statement expression_statement expression RPAREN statement{
			log_print("statement: FOR LPAREN expression_statement expression_statement expression RPAREN statement");
			$$ = new Node(false, "statement");
			$$->children.push_back(new Node(true, "FOR", "for", $1));
			$$->children.push_back(new Node(true, "LPAREN", "(", $2));
			$$->children.push_back($3);
			$$->children.push_back($4);
			$$->children.push_back($5);
			$$->children.push_back(new Node(true, "RPAREN", ")", $6));
			$$->children.push_back($7);
			$$->update_line();
		}
		| IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE	{
			log_print("statement: IF LPAREN expression RPAREN statement");
			$$ = new Node(false, "statement");
			$$->children.push_back(new Node(true, "IF", "if", $1));
			$$->children.push_back(new Node(true, "LPAREN", "(", $2));
			$$->children.push_back($3);
			$$->children.push_back(new Node(true, "RPAREN", ")", $4));
			$$->children.push_back($5);
			$$->update_line();
		}
		| IF LPAREN expression RPAREN statement ELSE statement {
			log_print("statement: IF LPAREN expression RPAREN statement ELSE statement");
			$$ = new Node(false, "statement");
			$$->children.push_back(new Node(true, "IF", "if", $1));
			$$->children.push_back(new Node(true, "LPAREN", "(", $2));
			$$->children.push_back($3);
			$$->children.push_back(new Node(true, "RPAREN", ")", $4));
			$$->children.push_back($5);
			$$->children.push_back(new Node(true, "ELSE", "else", $6));
			$$->children.push_back($7);
			$$->update_line();
		}
		| WHILE LPAREN expression RPAREN statement {
			log_print("statement: WHILE LPAREN expression RPAREN statement");
			$$ = new Node(false, "statement");
			$$->children.push_back(new Node(true, "WHILE", "while", $1));
			$$->children.push_back(new Node(true, "LPAREN", "(", $2));
			$$->children.push_back($3);
			$$->children.push_back(new Node(true, "RPAREN", ")", $4));
			$$->children.push_back($5);
			$$->update_line();
		}
		| PRINTLN LPAREN expression RPAREN SEMICOLON {
			log_print("statement: PRINTLN LPAREN expression RPAREN SEMICOLON");
			$$ = new Node(false, "statement");
			$$->children.push_back(new Node(true, "PRINTLN", "println", $1));
			$$->children.push_back(new Node(true, "LPAREN", "(", $2));
			$$->children.push_back($3);
			$$->children.push_back(new Node(true, "RPAREN", ")", $4));
			$$->children.push_back(new Node(true, "SEMICOLON", ";", $5));
			$$->update_line();
		}
		| RETURN expression SEMICOLON {
			log_print("statement: RETURN expression SEMICOLON");
			$$ = new Node(false, "statement");
			$$->children.push_back(new Node(true, "RETURN", "return", $1));
			$$->children.push_back($2);
			$$->children.push_back(new Node(true, "SEMICOLON", ";", $3));
			$$->update_line();
		}
		| RETURN SEMICOLON {
			log_print("statement: RETURN SEMICOLON");
			$$ = new Node(false, "statement");
			$$->children.push_back(new Node(true, "RETURN", "return", $1));
			$$->children.push_back(new Node(true, "SEMICOLON", ";", $2));
			$$->update_line();
		}
		;
	  
expression_statement : SEMICOLON { 
				log_print("expression_statement : SEMICOLON");
				$$ = new Node(false, "expression_statement");
				$$->children.push_back(new Node(true, "SEMICOLON", ";", $1));
				$$->update_line();
			}
			| expression SEMICOLON {
				log_print("expression_statement : expression SEMICOLON");
				$$ = new Node(false, "expression_statement");
				$$->children.push_back($1);
				$$->children.push_back(new Node(true, "SEMICOLON", ";", $2));
				$$->update_line();
				// expression_statement($$);
			}
			
			;
	  
variable : ID { 
		log_print("variable : ID");
		$$ = new Node(false, "variable");
		$$->children.push_back(new Node(true, "ID", $1->name, $1->line));
		$$->update_line();

		SymbolInfo* info = table.lookup($1->name);
		$$->lexeme = "ERROR";
		$$->lexeme = info->type;
		$$->eval = NAN;
		del_s($1);
	}		
	| ID LSQUARE expression RSQUARE {
		log_print("variable : ID LSQUARE expression RSQUARE");
		$$ = new Node(false, "variable");
		$$->children.push_back(new Node(true, "ID", $1->name, $1->line));
		$$->children.push_back(new Node(true, "LSQUARE", "[", $2));
		$$->children.push_back($3);
		$$->children.push_back(new Node(true, "RSQUARE", "]", $4));
		$$->update_line();

		SymbolInfo* info = table.lookup($1->name);
		$$->lexeme = "ERROR";
		$$->lexeme = info->type;
		$$->eval = NAN;
		del_s($1);
	}
	;
	 
expression : logic_expression { 
			log_print("expression : logic_expression");
			$$ = new Node(false, "expression");
			$$->children.push_back($1);
			$$->update_line();
			$$->lexeme = $1->lexeme;
			$$->eval = $1->eval;
 		}
		| variable ASSIGNOP logic_expression {
			log_print("expression : variable ASSIGNOP logic_expression");
			$$ = new Node(false, "expression");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "ASSIGNOP", $2->name, $2->line));
			$$->children.push_back($3);
			$$->update_line();
			$$->lexeme = $1->lexeme;
			$$->eval = NAN;
			del_s($2);
		}
		
		;
			
logic_expression : rel_expression { 
			log_print("logic_expression : rel_expression");
			$$ = new Node(false, "logic_expression");
			$$->children.push_back($1);
			$$->update_line();
			$$->lexeme = $1->lexeme;
			$$->eval = $1->eval;
		}
		| rel_expression LOGICOP rel_expression {
			log_print("logic_expression : rel_expression LOGICOP rel_expression");
			$$ = new Node(false, "logic_expression");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "LOGICOP", $2->name, $2->line));
			$$->children.push_back($3);
			$$->update_line();
			$$->lexeme = "INT";
			$$->eval = NAN;
			del_s($2);
		}
		;
			
rel_expression	: simple_expression { 
			log_print("rel_expression	: simple_expression");
			$$ = new Node(false, "rel_expression");
			$$->children.push_back($1);
			$$->update_line();
			$$->lexeme = $1->lexeme;
			$$->eval = $1->eval;
		}
		| simple_expression RELOP simple_expression	{
			log_print("rel_expression	: simple_expression RELOP simple_expression");
			$$ = new Node(false, "rel_expression");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "RELOP", $2->name, $2->line));
			$$->children.push_back($3);
			$$->update_line();

			$$->lexeme = "INT";
			$$->eval = NAN;
			del_s($2);
		}
		;
				
simple_expression : term { 
			log_print("simple_expression : term");
			$$ = new Node(false, "simple_expression");
			$$->children.push_back($1);
			$$->update_line();
			$$->lexeme = $1->lexeme;
			$$->eval = $1->eval;
		}
		| simple_expression ADDOP term {
			log_print("simple_expression : simple_expression ADDOP term");
			$$ = new Node(false, "simple_expression");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "ADDOP", $2->name, $2->line));
			$$->children.push_back($3);
			$$->update_line();

			$$->lexeme = "INT";
		     if($1->lexeme=="FLOAT" || $3->lexeme=="FLOAT") $$->lexeme = "FLOAT";
			if($2->name == "+") $$->eval = $1->eval + $3->eval;
			else if($2->name == "-") $$->eval = $1->eval - $3->eval;
			del_s($2);
		}
		;
					
term : unary_expression { 
		log_print("term : unary_expression");
		$$ = new Node(false, "term");
		$$->children.push_back($1);
		$$->update_line();
		$$->lexeme = $1->lexeme;
		$$->eval = $1->eval;
	}
    | term MULOP unary_expression {
		log_print("term : term MULOP unary_expression");
		$$ = new Node(false, "term");
		$$->children.push_back($1);
		$$->children.push_back(new Node(true, "MULOP", $2->name, $2->line));
		$$->children.push_back($3);
		$$->update_line();

		$$->lexeme = "INT";
		if($2->name == "%") $$->eval = NAN;
		else if($2->name == "*") $$->eval = $1->eval * $3->eval;
		else if($2->name == "/") $$->eval = $1->eval / $3->eval;
		del_s($2);
	}
    ;

unary_expression : ADDOP unary_expression { 
			log_print("unary_expression : ADDOP unary_expression");
			$$ = new Node(false, "unary_expression");
			$$->children.push_back(new Node(true, "ADDOP", $1->name, $1->line));
			$$->children.push_back($2);
			$$->update_line();

			$$->lexeme = $2->lexeme;
			if(isnan($2->eval)) $2->eval = $2->eval;
			else if($1->name == "+") $$->eval = $2->eval;
			else if($1->name == "-") $$->eval = -($2->eval);
			del_s($1);
		}
		| NOT unary_expression {
			log_print("unary_expression : NOT unary_expression");
			$$ = new Node(false, "unary_expression");
			$$->children.push_back(new Node(true, "NOT", $1->name, $1->line));
			$$->children.push_back($2);
			$$->update_line();

			$$->lexeme = $2->lexeme;
			if(isnan($2->eval)) $$->eval = $2->eval;
			else $$->eval = !($2->eval);
			del_s($1);
		}
		| factor {
			log_print("unary_expression : factor");
			$$ = new Node(false, "unary_expression");
			$$->children.push_back($1);
			$$->update_line();
			$$->lexeme = $1->lexeme;
			$$->eval = $1->eval;
		}
		;
	
factor : variable { 
		log_print("factor : variable");
		$$ = new Node(false, "factor");
		$$->children.push_back($1);
		$$->update_line();
		$$->lexeme = $1->lexeme;
		$$->eval = NAN;
	}
	| ID LPAREN argument_list RPAREN {
		log_print("ID LPAREN argument_list RPAREN");
		$$ = new Node(false, "factor");
		$$->children.push_back(new Node(true, "ID", $1->name, $1->line));
		$$->children.push_back(new Node(true, "LPAREN", "(", $2));
		$$->children.push_back($3);
		$$->children.push_back(new Node(true, "RPAREN", ")", $4));
		$$->update_line();

		SymbolInfo* info = table.lookup($1->name);
		// std::cout<<info->name<<" "<<info->type<<" "<<info->func->retType<<"\n";
		$$->lexeme = "ERROR";
		argList.clear();
		$$->eval = NAN;
		del_s($1);
	}
	| LPAREN expression RPAREN {
		log_print("factor : LPAREN expression RPAREN");
		$$ = new Node(false, "factor");
		$$->children.push_back(new Node(true, "LPAREN", "(", $1));
		$$->children.push_back($2);
		$$->children.push_back(new Node(true, "RPAREN", ")", $3));
		$$->update_line();
		$$->lexeme = $2->lexeme;
		$$->eval = $2->eval;
	}
	| CONST_INT {
		log_print("factor : CONST_INT");
		$$ = new Node(false, "factor");
		$$->children.push_back(new Node(true, "CONST_INT", $1->name, $1->line));
		$$->update_line();
		$$->lexeme = "INT";
		$$->eval = stod($1->name);
		del_s($1);
	}
	| CONST_FLOAT {
		log_print("factor : CONST_FLOAT");
		$$ = new Node(false, "factor");
		$$->children.push_back(new Node(true, "CONST_FLOAT", $1->name, $1->line));
		$$->update_line();
		$$->lexeme = "FLOAT";
		$$->eval = stod($1->name);
		del_s($1);
	}
	| variable INCOP {
		log_print("factor : variable INCOP");
		$$ = new Node(false, "factor");
		$$->children.push_back($1);
		$$->children.push_back(new Node(true, "INCOP", $2->name, $2->line));
		$$->update_line();
		$$->lexeme = $1->lexeme;
		$$->eval = NAN;
		del_s($2);
	}
	| variable DECOP {
		log_print("factor : variable DECOP");
		$$ = new Node(false, "factor");
		$$->children.push_back($1);
		$$->children.push_back(new Node(true, "DECOP", $2->name, $2->line));
		$$->update_line();
		$$->lexeme = $1->lexeme;
		$$->eval = NAN;
		del_s($2);
	}
	;
	
argument_list : arguments { 
				log_print("argument_list : arguments");
				$$ = new Node(false, "argument_list");
				$$->children.push_back($1);
				$$->update_line();
			}
			| {
				log_print("argument_list : ");
				$$ = new Node(true, "argument_list", "", line_count);
			}
			;
	
arguments : arguments COMMA logic_expression { 
			log_print("arguments : arguments COMMA logic_expression");
			$$ = new Node(false, "arguments");
			$$->children.push_back($1);
			$$->children.push_back(new Node(true, "COMMA", ",", $2));
			$$->children.push_back($3);
			$$->update_line();
			argList.push_back($3->lexeme);
		}
	    | logic_expression {
			log_print("arguments : logic_expression");
			$$ = new Node(false, "arguments");
			$$->children.push_back($1);
			$$->update_line();
			argList.push_back($1->lexeme);
		}

	    ;
 

%%

int main(int argc,char *argv[])
{
	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}
	
	logout= fopen("log.txt","w");
	errout = fopen("error.txt", "w");
	parseout = fopen("parsetree.txt", "w");

	yyin=fin;
	yyparse();

	//table.print_all_tables();
	//fprintf(logout, "Total Lines: %d\n", line_count);
	//fprintf(logout, "Total Errors: %d\n", error_count);
	//print_tree(parseout, 0, root);

	if(error_count)
	{
      std::cout<<"The program contains errors, code cannot be generated\n";
	  return 0;
	}
		
 {
		start(root);
		
	}	

	fclose(fin);
	fclose(logout);
	fclose(errout);
	fclose(parseout);
	delete root;
	
	return 0;
}

