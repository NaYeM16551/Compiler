%{
#include<bits/stdc++.h>
#include "symbol_table/header_files/scope_table.h"
#include "symbol_table/header_files/symbol_table.h"
#include "symbol_table/header_files/parse_tree_node.h"
//#define YYSTYPE yylval

using namespace std;

int yyparse(void);
int yylex(void);
extern FILE *yyin;
extern SymbolTable st;
extern int line_count;
extern int error_count;
extern bool lcurl;
extern string last_ID;

string return_or_variable_type="";
vector<ParseTreeNode*> variable_vector;
vector<string> current_func_param_list;

//SymbolTable *table;

ofstream flog("log.txt");
ofstream treeout("tree.txt");
ofstream errorout("error.txt");

//raw file
ofstream decout("check_dec.txt");

class FunctionRecord
{

private:
    int parameter_number;
    string return_type;
    string function_name;
    bool isDelar;
	vector<string> parameter;
    bool is_identical(FunctionRecord &fc)
    {
        if (fc.getFunctionName().compare(this->function_name) == 0 && 
		fc.getParameterNumber() == this->parameter_number && fc.getReturnType().compare(this->return_type) == 0)
            return true;
        return false;
    }

public:
    FunctionRecord(string function_name, int parameter_number, string return_type, bool isDelar)
    {
        this->function_name = function_name;
        this->parameter_number = parameter_number;
        this->return_type = return_type;
        this->isDelar = isDelar;
    }

	FunctionRecord()
	{

	}

    string getFunctionName()
    {
        return this->function_name;
    }

    string getReturnType()
    {
        return this->return_type;
    }

    int getParameterNumber()
    {
        return this->parameter_number;
    }
    bool getIsDeclar()
    {
        return this->isDelar;
    }

    pair<bool, string> is_error(FunctionRecord &fc)
    {
        if (fc.getIsDeclar() == isDelar)
        {
            bool ans = is_identical(fc);
            if (ans)
                return {true, " Redefinition of function '" + function_name+"'"};
        }

        if (fc.getFunctionName().compare(this->function_name) == 0 && 
        fc.getParameterNumber() == this->parameter_number && fc.getReturnType().compare(this->return_type) != 0)
        {
            return {true,"Conflicting types for '"+ function_name+"'"};
        }
		return {false,""};
    }
	bool is_exist(string name)
	{
		if(this->function_name.compare(name)==0)
		   return true;
		return false;   
	}

	void add_parameter(string type)
	{
		parameter.push_back(type);
	}

	std::pair<bool, std::string> is_parameter_equivalent(const std::vector<std::string>& param) {
        if (param.size() > parameter.size())
            return { false, "Too many arguments to function '" + function_name + "'" };
        else if (param.size() < parameter.size())
            return { false, "Too few arguments to function '" + function_name + "'" };
        else {
            for (auto i = 0; i < param.size(); ++i) {
                if (param[i] != parameter[i])
                    return { false, "Type mismatch for argument " + std::to_string(i + 1) + " '" + function_name + "'" };
            }

            return { true, "" };
        }
    }
};

vector<FunctionRecord> function_record;
int param_count=0;

void yyerror(string error)
{
	//write your code
	cout<<error<<endl;
}

ParseTreeNode* getNode(string rule,int startLine,int endLine)
{
	return new ParseTreeNode(rule,startLine,endLine);
}

std::vector<std::string> tokenize(const std::string &str, const char delim) {
    // Construct a stream from the string
    std::stringstream ss(str);
    std::vector<std::string> out;

    std::string s;
    while (std::getline(ss, s, delim)) {
        if (!s.empty())
		{
			while(s[s.size() - 1] == ' ')
			  s.pop_back();
			while(s[0]==' ')
			  s.erase(0,1);  
			out.push_back(s);
		}
            
    }
    return out;
}

void final_outputt()
{
	//st.printAllScope(flog);
	flog<<"Total lines: "<<line_count<<endl;
	flog<<"Total errors: "<<error_count<<endl;
	//flog<<"Total warnings: "<<warning_count<<endl;
	return;

}
string getIDName(ParseTreeNode* p)
{
	auto ans=tokenize(p->getRule(),':');
	return ans[1];
}


%}

%union {
	ParseTreeNode* parseTreeNode;
	
}

%token <parseTreeNode> IF ELSE FOR WHILE CHAR DO DOUBLE BITOP LSQUARE RSQUARE  RETURN  PRINTLN ADDOP INCOP DECOP RELOP ASSIGNOP LOGICOP NOT LPAREN RPAREN LCURL RCURL  COMMA SEMICOLON INT FLOAT VOID
%token <parseTreeNode> CONST_INT CONST_FLOAT ID MULOP
%type <parseTreeNode> start program unit var_declaration func_declaration func_definition arguments type_specifier parameter_list compound_statement statements declaration_list statement expression expression_statement logic_expression rel_expression simple_expression term unary_expression factor variable argument_list
%token <parseTreeNode> LTHIRD RTHIRD 

%%

start : program
    {
        string parent = "start";
        string children = "program";
        string rule = parent + " : " + children;
        flog << rule << endl;
        int startLine = $1->getStartLine();
        int endLine = $1->getEndLine();
        $$ = getNode(rule, startLine, endLine);
        $$->addChild({$1});
		final_outputt();
		$$->print_tree_node(treeout);
    }
    ;

program : program unit
    {
        string parent = "program";
        string children = "program unit";
        string rule = parent + " : " + children;
        flog << rule << endl;
        int startLine = $1->getStartLine();
        int endLine = $2->getEndLine();
        $$ = getNode(rule, startLine, endLine);
        $$->addChild({$1, $2});
    }
    | unit
    {
        string parent = "program";
        string children = "unit";
        string rule = parent + " : " + children;
 		flog << rule << endl;
        int startLine = $1->getStartLine();
        int endLine = $1->getEndLine();
        $$ = getNode(rule, startLine, endLine);
        $$->addChild({$1});
    }
    ;

unit : var_declaration
     {
        string parent = "unit";
        string children = "var_declaration";
        string rule = parent + " : " + children;
        flog << rule << endl;
        int startLine = $1->getStartLine();
        int endLine = $1->getEndLine();
        $$ = getNode(rule, startLine, endLine);
        $$->addChild({$1});
     }
     | func_declaration
     {
        string parent = "unit";
        string children = "func_declaration";
        string rule = parent + " : " + children;
        flog << rule << endl;
		decout<<rule<<endl;
        int startLine = $1->getStartLine();
        int endLine = $1->getEndLine();
        $$ = getNode(rule, startLine, endLine);
        $$->addChild({$1});
     }
     | func_definition
     {
        string parent = "unit";
        string children = "func_definition";
        string rule = parent + " : " + children;
        flog << rule << endl;
        int startLine = $1->getStartLine();
        int endLine = $1->getEndLine();
        $$ = getNode(rule, startLine, endLine);
        $$->addChild({$1});
     }
	 
     
     ;

func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
		{
			string parent = "func_declaration";
			string children = "type_specifier ID LPAREN parameter_list RPAREN SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
			decout<<rule<<endl;
            int startLine = $1->getStartLine();
            int endLine = $6->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3,$4,$5,$6});
			auto ans=tokenize($1->getRule(),':');
			auto ans2=tokenize($2->getRule(),':');
			FunctionRecord fc(ans2[1],param_count,ans[1],true);
			bool flag=true;
			for(auto& f : function_record)
			{
				if(flag && f.is_error(fc).first==true)
				{
					//errorout<<"hello"<<endl;
					errorout<<"Line# "<<$2->getStartLine()<<" : "<<f.is_error(fc).second<<endl;
					flag=false;
					error_count++;
					break;
				}
				  
			}
			if(flag)
			   function_record.push_back(fc);
			param_count=0;
			st.insertIntoGlobal(ans2[1],"FUNCTION, "+ans[1],flog);
			variable_vector.clear();

			
		}
		| type_specifier ID LPAREN RPAREN SEMICOLON
		{
			string parent = "func_declaration";
			string children = "type_specifier ID LPAREN RPAREN SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $5->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3,$4,$5});
			auto ans=tokenize($1->getRule(),':');
			auto ans2=tokenize($2->getRule(),':');
			FunctionRecord fc(ans2[1],0,ans[1],true);
			bool flag=true;
			for(auto& f : function_record)
			{
				if(flag && f.is_error(fc).first==true)
				{
					//errorout<<"hello no param"<<endl;
					errorout<<"Line# "<<$2->getStartLine()<<" : "<<f.is_error(fc).second<<endl;
					flag=false;
					error_count++;
					break;
				}
				  
			}
			if(flag)
			   function_record.push_back(fc);
			param_count=0;
			st.insertIntoGlobal(ans2[1],"FUNCTION, "+ans[1],flog);
		}
		;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement
		{
			auto ans=tokenize($1->getRule(),':');
			auto ans2=tokenize($2->getRule(),':');
			FunctionRecord fc(ans2[1],param_count,ans[1],false);
			bool flag=true;
			for(auto& f : function_record)
			{
				if(flag && f.is_error(fc).first==true)
				{
					//errorout<<"hello no param"<<endl;
					errorout<<"Line# "<<$2->getStartLine()<<" : "<<f.is_error(fc).second<<endl;
					flag=false;
					error_count++;
					break;
				}
				  
			}
			if(!(st.lookup(ans2[1],flog).empty()))
			{
				flag=false;
				errorout<<"Line# "<<$2->getStartLine()<<" : "<<"'"+ans2[1]+"'"+"redeclared as different kind of symbol"<<endl;
				error_count++;
			}
			if(flag)
			{
				 function_record.push_back(fc);
				 st.insertIntoGlobal(ans2[1],"FUNCTION, "+ans[1],flog);

			}
            st.printAllScope(flog);
            st.removeScope(flog);
			string parent = "func_definition";
			string children = "type_specifier ID LPAREN parameter_list RPAREN compound_statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
			
            int startLine = $1->getStartLine();
            int endLine = $6->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1, $2, $3, $4,$5,$6});
			
			  
			param_count=0;
			for(auto& c : variable_vector)
			{
				//errorout<<c->getType()<<" ";
				fc.add_parameter(c->getType());
			}
			variable_vector.clear();
			
			/* for(auto& node : variable_vector)
			{
				auto ans=tokenize(node->getRule(),':');
				st.insert(ans[1],node->getType(),flog);

			}
			variable_vector.clear(); */
			
			
			
		}
		| type_specifier ID LPAREN RPAREN compound_statement
		{
			auto ans=tokenize($1->getRule(),':');
			auto ans2=tokenize($2->getRule(),':');
			FunctionRecord fc(ans2[1],0,ans[1],false);
			bool flag=true;
			for(auto& f : function_record)
			{
				if(flag && f.is_error(fc).first==true)
				{
					//errorout<<"hello no param"<<endl;
					errorout<<"Line# "<<$2->getStartLine()<<" : "<<f.is_error(fc).second<<endl;
					flag=false;
					error_count++;
					break;
				}
				  
			}
			if(!(st.lookup(ans2[1],flog).empty()))
			{
				flag=false;
				errorout<<"Line# "<<$2->getStartLine()<<" : "<<"'"+ans2[1]+"'"+"redeclared as different kind of symbol"<<endl;
				error_count++;
			}
			//errorout<<ans2[1]<<endl;
			if(flag)
			{
				 function_record.push_back(fc);
				 st.insertIntoGlobal(ans2[1],"FUNCTION, "+ans[1],flog);
			}
			st.printAllScope(flog);
            st.removeScope(flog);
			string parent = "func_definition";
			string children = "type_specifier ID LPAREN RPAREN compound_statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $5->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1, $2, $3, $4,$5});
			
			  
			param_count=0;
			for(auto& v : variable_vector)
			{
				fc.add_parameter(getIDName(v));
			}
			variable_vector.clear();
			
		}
 		;				

parameter_list  : parameter_list COMMA type_specifier ID
		{
			string parent = "parameter_list";
			string children = "parameter_list COMMA type_specifier ID";
			string rule = parent + " : " + children;
 			flog << rule << endl;
			decout<<rule<<endl;
            int startLine = $1->getStartLine();
            int endLine = $4->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1, $2,$3, $4});
			param_count++;
			//current_func_param_list.push_back(getIDName($4));
			
			auto ans=tokenize($3->getRule(),':');
			$4->setType(ans[1]);
			variable_vector.push_back($4);
		}
		| parameter_list COMMA type_specifier
		{
			string parent = "parameter_list";
			string children = "parameter_list COMMA type_specifier";
			string rule = parent + " : " + children;
 			flog << rule << endl;
			decout<<rule<<endl;
            int startLine = $1->getStartLine();
            int endLine = $3->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3});
			param_count++;
			//current_func_param_list.push_back(getIDName($4));
		}
		| type_specifier ID
		{
			string parent = "parameter_list";
			string children = "type_specifier ID";
			string rule = parent + " : " + children;
 			flog << rule << endl;
			decout<<rule<<endl;
            int startLine = $1->getStartLine();
            int endLine = $2->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1, $2});
			param_count++;
			//current_func_param_list.push_back(getIDName($2));
			auto ans=tokenize($1->getRule(),':');
			$2->setType(ans[1]);
			variable_vector.push_back($2);
		}
		| type_specifier
		{
			string parent = "parameter_list";
			string children = "type_specifier";
			string rule = parent + " : " + children;
 			flog << rule << endl;
			decout<<rule<<endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
			param_count++;
			//current_func_param_list.push_back(getIDName($4));
		}
		;

compound_statement : LCURL statements RCURL
		{
			string parent = "compound_statement";
			string children = "LCURL statements RCURL";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $3->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3});
			//st.printAllScope(flog);
			for(auto& node : variable_vector)
			{
				auto ans=tokenize(node->getRule(),':');
				st.insert(ans[1],node->getType(),flog);

			}
			variable_vector.clear();
			
		}
		| LCURL RCURL
		{
			string parent = "compound_statement";
			string children = "LCURL RCURL";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $2->getEndLine();
            $$ = getNode(rule, startLine, endLine);
			$$->addChild({$1,$2});
			
		}
		;
	
    
var_declaration : type_specifier declaration_list SEMICOLON
 		{
			string parent = "var_declaration";
			string children = "type_specifier declaration_list SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $3->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3});
			auto ans3=tokenize($1->getRule(),':');
			bool void_flag=false;
			for(auto& node: variable_vector)
			{
				auto ans=tokenize(node->getRule(),':');
				auto s1=st.lookupLocal(ans[1],flog);
				if(s1.find(",")!= string::npos)
				{
					auto type_na=tokenize(s1,',');
					s1=type_na[1];
				}
				if(!s1.empty())
				{
					string s2=node->getType();
					//errorout<<s2<<" "<<s1<<endl;
					if(s2.compare(s1)!=0)
					  errorout<<"Line# "+ to_string(node->getStartLine())  +"  : Conflicting types for '" + ans[1] +"'"<<endl;
					else 
					  errorout<<"Line# "+ to_string(node->getStartLine())  +"  : Redefinition of variable '" + ans[1] +"'"<<endl;
					  
				}
				else if(ans3[1].compare("VOID")==0)
				{
					if(!void_flag)
					  errorout<<"Line# "+to_string($$->getStartLine())+" : Variable or field ";
					void_flag=true;
					
					//for(auto& x : variable_vector)
					   errorout<<"'"+ans[1]+"' ";
					   

				}
				else
				   st.insert(ans[1],node->getType(),flog);
			}
			if(void_flag)
			{
				errorout<<"declared void"<<endl;
				error_count++;
			}
			  
			variable_vector.clear();
		}
 		;
 		 
type_specifier	: INT
		{
			string parent = "type_specifier";
			string children = "INT";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
			return_or_variable_type="INT";
		}
 		| FLOAT
		{
			string parent = "type_specifier";
			string children = "FLOAT";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
             $$->addChild({$1});
			 return_or_variable_type="FLOAT";
		}
 		| VOID
		{
			string parent = "type_specifier";
			string children = "VOID";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
             $$->addChild({$1});
			 return_or_variable_type="VOID";
			
		}
 		;
 		
declaration_list : declaration_list COMMA ID
		{
			string parent = "declaration_list";
			string children = "declaration_list COMMA ID";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $3->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2, $3});
			$3->setType(return_or_variable_type);
			variable_vector.push_back($3);
		}
		| declaration_list COMMA ID LSQUARE CONST_INT RSQUARE
		{
			string parent = "declaration_list";
			string children = "declaration_list COMMA ID LSQUARE CONST_INT RSQUARE";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $6->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3,$4,$5,$6});
			$3->setType("ARRAY, "+return_or_variable_type);
			//$3->setType("ARRAY");
			variable_vector.push_back($3);
		}
		| ID
		{
			string parent = "declaration_list";
			string children = "ID";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
			$1->setType(return_or_variable_type);
			variable_vector.push_back($1);
		}
		| ID LSQUARE CONST_INT RSQUARE
		{
			string parent = "declaration_list";
			string children = "ID LSQUARE CONST_INT RSQUARE";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $4->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3,$4});
			//$1->setType("ARRAY, "+return_or_variable_type);
			$1->setType("ARRAY");
			variable_vector.push_back($1);
		}
		;
		
statements : statement
		{
			string parent = "statements";
			string children = "statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
		}
		| statements statement
		{
			string parent = "statements";
			string children = "statements statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $2->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1, $2});
		}
		;

statement : var_declaration
		{
			string parent = "statement";
			string children = "var_declaration";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
		}
		| expression_statement
		{
			string parent = "statement";
			string children = "expression_statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
		}
		| compound_statement
		{
			string parent = "statement";
			string children = "compound_statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
		}
		| FOR LPAREN expression_statement expression_statement expression RPAREN statement
		{
			string parent = "statement";
			string children = "FOR LPAREN expression_statement expression_statement expression RPAREN statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $7->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1, $2, $3,$4,$5,$6,$7});
		}
		| IF LPAREN expression RPAREN statement
		{
			string parent = "statement";
			string children = "IF LPAREN expression RPAREN statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $5->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3,$4,$5});
		}
		| IF LPAREN expression RPAREN statement ELSE statement
		{
			string parent = "statement";
			string children = "IF LPAREN expression RPAREN statement ELSE statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $7->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3,$4,$5,$6,$7});
		}
		| WHILE LPAREN expression RPAREN statement
		{
			string parent = "statement";
			string children = "WHILE LPAREN expression RPAREN statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $5->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3,$4,$5});
		}
		| PRINTLN LPAREN ID RPAREN SEMICOLON
		{
			string parent = "statement";
			string children = "PRINTLN LPAREN ID RPAREN SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $5->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3,$4,$5});
		}
		| RETURN expression SEMICOLON
		{
			string parent = "statement";
			string children = "RETURN expression SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $3->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3});
		}
		;
	  
expression_statement : SEMICOLON			
		{
			string parent = "expression_statement";
			string children = "SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
			 $$->addChild({$1});
		}
		| expression SEMICOLON 
		{
			string parent = "expression_statement";
			string children = "expression SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $2->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2});
		}
		;
	  
variable : ID 		
		{
			string parent = "variable";
			string children = "ID";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
             $$->addChild({$1});
			 errorout<<getIDName($1)<<endl;
		
		  if(st.lookup(getIDName($1),flog).empty())
		{
			errorout<<"Line# "+ to_string($$->getStartLine())+": Undeclared variable '"<<getIDName($1)<<"'"<<endl;
			error_count++;
		} 
		current_func_param_list.push_back("INT");
		}
 		| ID LSQUARE expression RSQUARE 
		{
			string parent = "variable";
			string children = "ID LSQUARE expression RSQUARE";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $4->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3,$4});
		}
 		;
	 
expression : logic_expression
	   {
			string parent = "expression";
			string children = "logic_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
		}
	   | variable ASSIGNOP logic_expression
	   {
			string parent = "expression";
			string children = "variable ASSIGNOP logic_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $3->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2, $3});
		}
	   ;

logic_expression : rel_expression
		 {
			string parent = "logic_expression";
			string children = "rel_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
		 }
		 | rel_expression LOGICOP rel_expression
		 {
			string parent = "logic_expression";
			string children = "rel_expression LOGICOP rel_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $3->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2, $3});
		 }
		 ;

rel_expression : simple_expression
		{
			string parent = "rel_expression";
			string children = "simple_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
		}
		| simple_expression RELOP simple_expression
		{
			string parent = "rel_expression";
			string children = "simple_expression RELOP simple_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $3->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2, $3});
		}
		;

simple_expression : term
		  {
			string parent = "simple_expression";
			string children = "term";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
		  }
		  | simple_expression ADDOP term
		  {
			string parent = "simple_expression";
			string children = "simple_expression ADDOP term";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $3->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2, $3});
		  }
		  ;

term : unary_expression
     {
		string parent = "term";
		string children = "unary_expression";
		string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
	 }
     | term MULOP unary_expression
     {
		string parent = "term";
		string children = "term MULOP unary_expression";
		string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $3->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2, $3});
	 }
     ;

unary_expression : ADDOP unary_expression
		 {
			string parent = "unary_expression";
			string children = "ADDOP unary_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $2->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1, $2});
		 }
		 | NOT unary_expression
		 {
			string parent = "unary_expression";
			string children = "NOT unary_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $2->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1, $2});
		 }
		 | factor
		 {
			string parent = "unary_expression";
			string children = "factor";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
		 }
		 ;
factor : variable
	{
		string parent = "factor";
		string children = "variable";
		string rule = parent + " : " + children;
 		flog << rule << endl;
        int startLine = $1->getStartLine();
        int endLine = $1->getEndLine();
        $$ = getNode(rule, startLine, endLine);
        $$->addChild({$1});
		


	}
	| ID LPAREN argument_list RPAREN
	{
		string parent = "factor";
		string children = "ID LPAREN argument_list RPAREN";
		string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $4->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2,$3,$4});
			bool flag=false;
			FunctionRecord pt;
			for(auto& c : function_record)
			{
				if(c.is_exist(getIDName($1)))
				  {
					flag=true;
					pt=c;
					break;
					
				  }
			}
			if(!flag)
			{
				errorout<<"Line# "+ to_string($$->getStartLine())+": Undeclared function '"+getIDName($1)+"'"<<endl;
				error_count++;
			}
			if(flag)
			{
				//errorout<<current_func_param_list.size()<<endl;
				if(!(pt.is_parameter_equivalent(current_func_param_list).first))
				{
					errorout<<"Line# "+ to_string($$->getStartLine())+": "+pt.is_parameter_equivalent(current_func_param_list).second<<endl;
					error_count++;
				}

			}
			current_func_param_list.clear();
	}
	| LPAREN expression RPAREN
	{
		string parent = "factor";
		string children = "LPAREN expression RPAREN";
		string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $3->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1,$2, $3});
		
	}
	| CONST_INT
	{
		string parent = "factor";
		string children = "CONST_INT";
		string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
            $$->addChild({$1});
			current_func_param_list.push_back("INT");
		
	}
	| CONST_FLOAT
	{
		string parent = "factor";
		string children = "CONST_FLOAT";
		string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = $1->getStartLine();
            int endLine = $1->getEndLine();
            $$ = getNode(rule, startLine, endLine);
             $$->addChild({$1});
			 current_func_param_list.push_back("FLOAT");
		
	}
	| variable INCOP
	{
		string parent = "factor";
		string children = "variable INCOP";
		string rule = parent + " : " + children;
		flog << rule << endl;
        int startLine = $1->getStartLine();
        int endLine = $2->getEndLine();
        $$ = getNode(rule, startLine, endLine);
        $$->addChild({$1,$2});
		
	}
	| variable DECOP
	{
		string parent = "factor";
		string children = "variable DECOP";
		string rule = parent + " : " + children;
		flog << rule << endl;
        int startLine = $1->getStartLine();
        int endLine = $2->getEndLine();
        $$ = getNode(rule, startLine, endLine);
        $$->addChild({$1,$2});
		
	}
	;

argument_list : arguments
	{
		string parent = "argument_list";
		string children = "arguments";
		string rule = parent + " : " + children;
		flog << rule << endl;
        int startLine = $1->getStartLine();
        int endLine = $1->getEndLine();
        $$ = getNode(rule, startLine, endLine);
        $$->addChild({$1});
	}
	|
	{
		string parent = "argument_list";
		string children = "";
		string rule = parent + " : " + children;
		flog << rule << endl;
        int startLine =line_count; // Adjust accordingly based on your implementation
        int endLine = line_count; // Adjust accordingly based on your implementation
        $$ = getNode(rule, startLine, endLine);
	}
	;

arguments : arguments COMMA logic_expression
	{
		string parent = "arguments";
		string children = "arguments COMMA logic_expression";
		string rule = parent + " : " + children;
		flog << rule << endl;
        int startLine = $1->getStartLine();
        int endLine = $3->getEndLine();
        $$ = getNode(rule, startLine, endLine);
        $$->addChild({$1,$2, $3});
		
	}
	| logic_expression
	{
		string parent = "arguments";
		string children = "logic_expression";
		string rule = parent + " : " + children;
		flog << rule << endl;
        int startLine = $1->getStartLine();
        int endLine = $1->getEndLine();
        $$ = getNode(rule, startLine, endLine);
        $$->addChild({$1});
		
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
	

	yyin= fin;
	
	yyparse();
	
	
	fclose(yyin);
	return 0;
}

