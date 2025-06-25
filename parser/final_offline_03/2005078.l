%option noyywrap

%x STRING_STATE
%x SINGLE_LINE_COMMENT_STATE
%x MULTI_LINE_COMMENT_STATE

%{
#include<bits/stdc++.h>
#include "symbol_table/header_files/scope_table.h"
#include "symbol_table/header_files/symbol_table.h"
#include "symbol_table/header_files/parse_tree_node.h"
#include "y.tab.h"
#define bucketSize 11
extern YYSTYPE yylval;
using namespace std;



//variable declaration
int warning_count=0;
int line_count=1;
int error_count=0;
int init_cnt=0;
bool multiLine=false;
int zero=0,one=1;
string estring="";
string dummy;
string string_input="";
string log_string="";
string last_ID="";
std::map<std::string, std::string> escapeMap = {
        {"\\n", "\n"},
        {"\\t", "\t"},
        {"\\r", "\r"},
		{"\\a", "\a"}
       
    };

ParseTreeNode* getToken(string lexeme,string tokenName)
{
	string rule=tokenName+ " : "+lexeme;
	auto node=new ParseTreeNode(rule,line_count,line_count);
	node->setTerminal(true);
	return node;

}	

bool lcurl=false;
int space_count=0;
bool isNewline=false;
bool type=false;















ofstream tout("2005078_token.txt");
extern ofstream flog;
ofstream check("check.txt");
//extern SymbolTable st;
SymbolTable st(bucketSize,check);
extern ofstream errorout;

string to_upper_case(string s)
{
	for(auto &ch:s) ch = toupper(ch);
	return s;
}

void warningCheck(string ytext,string regex="")
{
	if(isNewline && lcurl && !regex.empty())
			   {
				if(space_count!=4)
				   warning_count++;
				 if(space_count<4)
				   flog<<"Line# "<<line_count<<": warning, 1 of tabs needed but got 0 tabs."<<endl;
				 if(space_count>4 && space_count%4==0)
				   flog<<"Line# "<<line_count<<": warning, 1 of tabs needed but got "<<space_count/4 <<" tabs."<<endl;
				 else if(space_count>4)
				    flog<<"Line# "<<line_count<<": Warning, tab requrired but got space."<<endl;
				 space_count=0;	
				        
			   }
}

void final_output()
{
	//st.printAllScope(flog);
	flog<<"Total lines: "<<line_count<<endl;
	flog<<"Total errors: "<<error_count<<endl;
	flog<<"Total warnings: "<<warning_count<<endl;
	log_string="";
	return ;

}

ParseTreeNode* fix(string tokenName)
{
	string str=estring;
	str+=yytext;
	return getToken(str,tokenName);
}

%}

LETTER [a-zA-Z_]
DIGIT [0-9]
NEWLINE [\n]
SLASH \\
AlphaNumeric [a-z_A-Z0-9]
SQ \'





%%

[ ] {
	if(isNewline && lcurl)
	  space_count+=1;
	
}

[\t] {
	if(isNewline && lcurl)
	  space_count+=4;
	
}

[\r] {
	
}

[\f] {
	
}

[\v] {

}

{NEWLINE} {
isNewline=true;
space_count=0;
type=false;
line_count++;}

"if"					{
							isNewline=false;
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;yylval.parseTreeNode=fix("IF");
							return IF;
						}
"else"					{
							isNewline=false;
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;yylval.parseTreeNode=fix("ELSE");
							
							return ELSE;
						}
"for"					{
							isNewline=false;
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;yylval.parseTreeNode=fix("FOR");
							return FOR;
						}
"while"					{
							isNewline=false;
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;yylval.parseTreeNode=fix("WHILE");
							return WHILE;
						}
"do"					{
							isNewline=false;
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;yylval.parseTreeNode=fix("DO");
							
							return DO;
						}
"switch"				{
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;
							//return SWITCH;
						}
"case"					{
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;
							//return CASE;
						}
"default"				{
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;
							//return DEFAULT;
						}
"continue"				{
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;
							//return CONTINUE;
						}
"println"				{
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;
							//return PRINTLN;
						}						
"break"					{
							isNewline=false;
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;
							
						}
"int"					{
							isNewline=false;
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;yylval.parseTreeNode=fix("INT");
							return INT;
						}
"char"					{
							isNewline=false;
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;yylval.parseTreeNode=fix("CHAR");
							return CHAR;
						}
"float"					{
							isNewline=false;
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;yylval.parseTreeNode=fix("FLOAT");
							return FLOAT;
						}
"double"				{
							isNewline=false;
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;yylval.parseTreeNode=fix("DOUBLE");
							return DOUBLE;
						}
"void"					{
							isNewline=false;
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;yylval.parseTreeNode=fix("VOID");
							
							return VOID;
						}
"return"				{
							isNewline=false;
							flog<<"Line# "<<line_count<<": Token <"<<to_upper_case(yytext)<<"> Lexeme "<<yytext<<" found"<<endl;yylval.parseTreeNode=fix("RETURN");
							
							return RETURN;
						}


{DIGIT}+	{
	            //warningCheck(yytext,"{DIGIT}+");
				
			    isNewline=false;
			   	//tout<<"<CONST_INT, "<<yytext<<">"<<endl;
				flog<<"Line# "<<line_count<<": Token <CONST_INT> Lexeme "<<yytext<<" found"<<endl;
				yylval.parseTreeNode=fix("CONST_INT");
				type=true;
				return CONST_INT;

			}		

({DIGIT}+)?(\.{DIGIT}+)?([E][+-]?{DIGIT}+)?  {
	             //warningCheck(yytext,"({DIGIT}+)?({DIGIT}+)?([E][+-]?{DIGIT}+)?");
				
				isNewline=false;
				//tout<<"<CONST_FLOAT, "<<yytext<<">"<<endl;	
				flog<<"Line# "<<line_count<<": Token <CONST_FLOAT> Lexeme "<<yytext<<" found"<<endl;
				yylval.parseTreeNode=fix("CONST_FLOAT");
				type=true;
				return CONST_FLOAT;
			}			

{DIGIT}*(\.{DIGIT}+)\.[0-9\.Ee+-]*(\.)* {
	//warningCheck(yytext,"{DIGIT}*({DIGIT}+)[0-9Ee+-]*()*");
	
	isNewline=false;
	error_count++;
	flog<<"Error at line# "<< line_count <<": TOO_MANY_DECIMAL_POINTS "<<yytext<<endl;
}
([0-9]*(\.[0-9]*)?[E][+-]?[0-9]*[0-9\.E+-]*(\.)*)|([0-9]+\.) {
	//warningCheck(yytext,"([0-9]*)");
	
	isNewline=false;
	error_count++;
	flog<<"Error at line# "<< line_count <<": ILLFORMED_NUMBER "<<yytext<<endl;
}

{SQ}[^\'\n\\]{SQ}  {
	            //warningCheck(yytext,"{SQ}[^\'\n\\]{SQ}");
                isNewline=false;
	            //tout<<"<CONST_CHAR, "<<yytext[1]<<">"<<endl;
				flog<<"Line# "<<line_count<<": Token <CONST_CHAR> Lexeme "<<yytext[1]<<" found"<<endl;

			}
{SQ}{SLASH}{AlphaNumeric}{SQ}	 {
	            //warningCheck(yytext,"{SQ}{SLASH}{AlphaNumeric}{SQ}");
				isNewline=false;
				dummy=yytext;
				dummy=dummy.substr(1,2);
				//tout<<"<CONST_CHAR, "<<escapeMap[dummy]<<">"<<endl;
				flog<<"Line# "<<line_count<<": Token <CONST_CHAR> Lexeme "<<escapeMap[dummy]<<" found"<<endl;
			}
{SQ}{SLASH}{SQ} {
	    //warningCheck(yytext,"{SQ}{SLASH}{SQ}");
		isNewline=false;
	     error_count++;
		 flog<<"Error at line# "<< line_count <<": UNFINISHED_CONST_CHAR "<<yytext<<endl;

}			
{SQ}{SQ} {
	 //warningCheck(yytext,"{SQ}{SQ}");
	 isNewline=false;
	 error_count++;
	 flog<<"Error at line# "<< line_count <<": EMPTY_CONST_CHAR "<<yytext<<endl;

}			
{SQ}[^\'\n]*{SQ} {
	    //warningCheck(yytext,"{SQ}[^\'\n]*{SQ}");
	    isNewline=false;
	     error_count++;
		 flog<<"Error at line# "<< line_count <<": MULTICHAR_CONST_CHAR "<<yytext<<endl;

}	
{SQ}[^\'\n]* {
	     //warningCheck(yytext,"{SQ}[^\'\n]*");
	     isNewline=false;
	     error_count++;
		 flog<<"Error at line# "<< line_count <<": UNFINISHED_CONST_CHAR "<<yytext<<endl;

}			
"+"|"-" {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <ADDOP> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("ADDOP");
    return ADDOP;
}

"*"|"/"|"%" {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <MULOP> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("MULOP");
    return MULOP;
}

"++"|"--" {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <INCOP> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("INCOP");
    return INCOP;
}

"<"|"<="|">"|">="|"=="|"!=" {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <RELOP> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("RELOP");
    return RELOP;
}

"=" {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <ASSIGNOP> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("ASSIGNOP");
    return ASSIGNOP;
}

"&&"|"||" {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <LOGICOP> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("LOGICOP");
    return LOGICOP;
}

"&"|"|"|"^"|"<<"|">>" {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <BITOP> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("BITOP");
    return BITOP;
}

"!" {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <NOT> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("NOT");
    return NOT;
}

"(" {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <LPAREN> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("LPAREN");
    return LPAREN;
}

")" {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <RPAREN> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("RPAREN");
    return RPAREN;
}

"{" {
    st.createScope(bucketSize,flog);
    isNewline = false;
    lcurl = true;
    flog << "Line# " << line_count << ": Token <LCURL> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("LCURL");
    return LCURL;
}

"}" {
	
    if (isNewline) {
        if (space_count != 0)
            cout << "Line# " << line_count << ": Warning, no space required but got some" << endl;
    }
    isNewline = false;
    lcurl = false;
    flog << "Line# " << line_count << ": Token <RCURL> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("RCURL");
	
    return RCURL;
}

"[" {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <LSQUARE> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("LSQUARE");
    return LSQUARE;
}

"]" {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <RSQUARE> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("RSQUARE");
    return RSQUARE;
}

"," {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <COMMA> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("COMMA");
	type=true;
    return COMMA;
}

";" {
    isNewline = false;
    flog << "Line# " << line_count << ": Token <SEMICOLON> Lexeme " << yytext << " found" << endl;
    yylval.parseTreeNode = fix("SEMICOLON");
	type=false;
    return SEMICOLON;
}



{LETTER}{AlphaNumeric}*	{
		//st.insert(yytext,"ID",flog);
		//warningCheck(yytext,"{LETTER}{AlphaNumeric}*");
		isNewline=false;
		flog<<"Line# "<<line_count<<": Token <ID> Lexeme "<<yytext<<" found"<<endl;
		last_ID=yytext;
		// if(!ans)
		//  flog<<"    " << yytext << " already exists in the current ScopeTable"<<endl;
		// else
		//   st.printAllScope(flog); 
		//tout<<"<ID, "<<yytext<<">"<<endl;
		yylval.parseTreeNode = fix("ID");
		return ID;
		
}

{DIGIT}+{LETTER}{AlphaNumeric}* {
	error_count++;
	
	//warningCheck(yytext,"{DIGIT}+{LETTER}{AlphaNumeric}*");
	isNewline=false;
	flog<<"Error at line# "<< line_count <<": INVALID_ID_SUFFIX_NUM_PREFIX "<<yytext<<endl;

}

"\"" {
	//warningCheck(yytext,"string start");
	isNewline=false;
	init_cnt=line_count;
	log_string+=yytext;
	type=false;
	BEGIN STRING_STATE;}

<STRING_STATE>[\t]+	{
	 
	  string_input+="  ";
	  log_string += "  ";
	  //warningCheck(yytext);
	  isNewline=false;
}

<STRING_STATE>{SLASH}{AlphaNumeric}	 {
				dummy=yytext;
				string_input+=escapeMap[dummy];
				log_string+=yytext;
				//warningCheck(yytext);
				isNewline=false;
				
			}
<STRING_STATE>{SLASH}{NEWLINE}  {
	multiLine=true;
	string_input+="  ";
	log_string+="\\\n";
	line_count++;
	//warningCheck(yytext);
	isNewline=true;
	space_count=0;
	
}

<STRING_STATE>{NEWLINE}  {
	error_count++;
	flog<<"Error at line# "<< line_count <<": UNFINISHED_STRING "<<log_string<<endl;
	line_count++;
	log_string="";
	string_input="";
	////warningCheck(yytext);
	isNewline=true;
	space_count=0;
	BEGIN INITIAL;
}

<STRING_STATE>[^\\\"\n]* {
    string_input += yytext;
    log_string += yytext;
    //warningCheck(yytext);
    isNewline = false;
}
<STRING_STATE>"\"" {
	//warningCheck(yytext);
	isNewline=false;
	log_string+="\"";
	if(multiLine)
	{
		 flog<<"Line# "<<init_cnt<<": Token <MULTI LINE STRING> Lexeme "<<log_string<<" found"<<endl;
		 //tout<<"<MULTI LINE STRING, "<<string_input<<">"<<endl;
	}
	
	else
	{
		flog<<"Line# "<<line_count<<": Token <SINGLE LINE STRING> Lexeme "<<log_string<<" found"<<endl;
		//tout<<"<SINGLE LINE STRING, "<<string_input<<">"<<endl;
	}
	 
	string_input="";
	multiLine=false;
	log_string="";
	BEGIN INITIAL;
	}
<STRING_STATE><<EOF>>  {
	error_count++;
	//warningCheck(yytext,"<STRING_STATE><<EOF>>");
	isNewline=false;
	flog<<"Error at line# "<< line_count <<": UNFINISHED_STRING "<<log_string<<endl;
	final_output();
	return 0;
}

"//"  {
	 init_cnt=line_count;
	 isNewline=false;
	 BEGIN SINGLE_LINE_COMMENT_STATE;
	 log_string="//";

}

<SINGLE_LINE_COMMENT_STATE>[^\n]*[\\][\n]	{
	    log_string+=yytext;
		line_count++;
		isNewline=true;
		space_count=0;
}
<SINGLE_LINE_COMMENT_STATE>[^\n]*	{
		log_string+=yytext;
		isNewline=false;
}
<SINGLE_LINE_COMMENT_STATE>[\n]  {
	
	flog<<"Line# "<<init_cnt<<": Token <SINGLE LINE COMMENT> Lexeme "<<log_string<<" found"<<endl;
	line_count++;
	log_string="";
	isNewline=true;
	space_count=0;
	BEGIN INITIAL;
}

"/*" {
	log_string+=yytext;
	init_cnt=line_count;
	isNewline=false;
	BEGIN MULTI_LINE_COMMENT_STATE;
}
<MULTI_LINE_COMMENT_STATE>[^\n]*[*][/][\n]	{
	log_string+=yytext;
	log_string.pop_back();
	//log_string.pop_back();
	flog<<"Line# "<<init_cnt<<": Token <MULTI LINE COMMENT> Lexeme "<<log_string<<" found"<<endl;
	line_count++;
	log_string="";
	isNewline=true;
	space_count=0;
	BEGIN INITIAL;
}
<MULTI_LINE_COMMENT_STATE>[^\n]*[*][/]	{
	log_string+=yytext;
	flog<<"Line# "<<init_cnt<<": Token <MULTI LINE COMMENT> Lexeme "<<log_string<<" found"<<endl;
	log_string="";
	isNewline=false;
	BEGIN INITIAL;
}
	
<MULTI_LINE_COMMENT_STATE>(.)+[\n] {
	log_string+=yytext;
    line_count++;
	isNewline=true;
	space_count=0;
}
<MULTI_LINE_COMMENT_STATE>(.)+[^\n] {
	log_string+=yytext;
	isNewline=false;
}

<MULTI_LINE_COMMENT_STATE>[\n] {
	log_string+=yytext;
    line_count++;
	isNewline=true;
	space_count=0;
}
<MULTI_LINE_COMMENT_STATE><<EOF>> {
	error_count++;
	isNewline=false;
	flog<<"Error at line# "<< line_count <<": UNFINISHED_COMMENT "<<log_string<<endl;
	final_output();
	return 0;
}
[^ \r\f\v\t] {
	error_count++;
	//warningCheck(yytext,"unrecog");
	cout<<"in digit"<<endl;
	isNewline=false;
	flog<<"Error at line# "<< line_count <<": UNRECOGNIZED_CHAR "<<yytext<<endl;
	cout<<"Error at line# "<< line_count <<": UNRECOGNIZED_CHAR "<<yytext<<endl;

}
<<EOF>> {
	//warningCheck(yytext,"<<EOF>>");
	isNewline=false;
	//final_output();
	return 0;
}













%%
/* int main(int argc,char *argv[]){
	
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
	yylex();
	fclose(yyin);
	return 0;
}
 */