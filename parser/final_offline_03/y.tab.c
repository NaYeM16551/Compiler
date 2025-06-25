/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 2
#define YYMINOR 0
#define YYPATCH 20220114

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#undef YYBTYACC
#define YYBTYACC 0
#define YYDEBUGSTR YYPREFIX "debug"
#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "parser.y"
#include<bits/stdc++.h>
#include "symbol_table/header_files/scope_table.h"
#include "symbol_table/header_files/symbol_table.h"
#include "symbol_table/header_files/parse_tree_node.h"
/*#define YYSTYPE yylval*/

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

/*SymbolTable *table;*/

ofstream flog("log.txt");
ofstream treeout("tree.txt");
ofstream errorout("error.txt");

/*raw file*/
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
	/*write your code*/
	cout<<error<<endl;
}

ParseTreeNode* getNode(string rule,int startLine,int endLine)
{
	return new ParseTreeNode(rule,startLine,endLine);
}

std::vector<std::string> tokenize(const std::string &str, const char delim) {
    /* Construct a stream from the string*/
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
	/*st.printAllScope(flog);*/
	flog<<"Total lines: "<<line_count<<endl;
	flog<<"Total errors: "<<error_count<<endl;
	/*flog<<"Total warnings: "<<warning_count<<endl;*/
	return;

}
string getIDName(ParseTreeNode* p)
{
	auto ans=tokenize(p->getRule(),':');
	return ans[1];
}


#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 178 "parser.y"
typedef union YYSTYPE {
	ParseTreeNode* parseTreeNode;
	
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 211 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

#if !(defined(yylex) || defined(YYSTATE))
int YYLEX_DECL();
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define IF 257
#define ELSE 258
#define FOR 259
#define WHILE 260
#define CHAR 261
#define DO 262
#define DOUBLE 263
#define BITOP 264
#define LSQUARE 265
#define RSQUARE 266
#define RETURN 267
#define PRINTLN 268
#define ADDOP 269
#define INCOP 270
#define DECOP 271
#define RELOP 272
#define ASSIGNOP 273
#define LOGICOP 274
#define NOT 275
#define LPAREN 276
#define RPAREN 277
#define LCURL 278
#define RCURL 279
#define COMMA 280
#define SEMICOLON 281
#define INT 282
#define FLOAT 283
#define VOID 284
#define CONST_INT 285
#define CONST_FLOAT 286
#define ID 287
#define MULOP 288
#define LTHIRD 289
#define RTHIRD 290
#define YYERRCODE 256
typedef int YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    1,    1,    2,    2,    2,    4,    4,    5,    5,
    8,    8,    8,    8,    9,    9,    3,    7,    7,    7,
   11,   11,   11,   11,   10,   10,   12,   12,   12,   12,
   12,   12,   12,   12,   12,   14,   14,   21,   21,   13,
   13,   15,   15,   16,   16,   17,   17,   18,   18,   19,
   19,   19,   20,   20,   20,   20,   20,   20,   20,   22,
   22,    6,    6,
};
static const YYINT yylen[] = {                            2,
    1,    2,    1,    1,    1,    1,    6,    5,    6,    5,
    4,    3,    2,    1,    3,    2,    3,    1,    1,    1,
    3,    6,    1,    4,    1,    2,    1,    1,    1,    7,
    5,    7,    5,    5,    3,    1,    2,    1,    4,    1,
    3,    1,    3,    1,    3,    1,    3,    1,    3,    2,
    2,    1,    1,    4,    3,    1,    1,    2,    2,    1,
    0,    3,    1,
};
static const YYINT yydefred[] = {                         0,
   18,   19,   20,    0,    0,    3,    4,    5,    6,    0,
    2,    0,    0,    0,    0,    0,   17,    0,    0,    0,
    0,    0,   24,    0,    8,   10,   13,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   16,   36,
   56,   57,    0,   27,    0,   29,    0,   25,    0,   28,
   40,    0,    0,    0,   48,   52,    0,    7,    9,    0,
    0,    0,    0,    0,    0,    0,   50,    0,   51,    0,
    0,    0,    0,   15,   26,   37,    0,    0,    0,    0,
   58,   59,    0,   11,   22,    0,    0,    0,   35,    0,
   55,    0,    0,   63,    0,   43,    0,    0,   49,   41,
    0,    0,    0,    0,   39,    0,   54,    0,    0,   33,
   34,   62,    0,    0,   32,   30,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  282,  283,  284,  292,  293,  294,  295,  296,  297,  299,
  294,  287,  303,  265,  276,  280,  281,  285,  277,  299,
  300,  287,  266,  278,  281,  301,  287,  277,  280,  265,
  257,  259,  260,  267,  268,  269,  275,  276,  279,  281,
  285,  286,  287,  295,  299,  301,  302,  304,  305,  306,
  307,  308,  309,  310,  311,  312,  313,  281,  301,  299,
  285,  276,  276,  276,  305,  276,  311,  313,  311,  305,
  265,  276,  287,  279,  304,  281,  274,  269,  272,  288,
  270,  271,  273,  287,  266,  305,  306,  305,  281,  287,
  277,  305,  298,  307,  314,  308,  310,  309,  311,  307,
  277,  306,  277,  277,  266,  280,  277,  304,  305,  304,
  281,  307,  258,  277,  304,  304,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          4,
    5,    6,   44,    8,    9,   93,   45,   21,   46,   47,
   13,   48,   49,   50,   51,   52,   53,   54,   55,   56,
   57,   95,
};
static const YYINT yysindex[] = {                      -145,
    0,    0,    0,    0, -145,    0,    0,    0,    0, -275,
    0, -248, -150, -271, -231, -267,    0, -232, -199, -237,
 -178, -210,    0, -195,    0,    0,    0, -172, -145, -228,
 -217, -215, -213, -209, -201, -209, -209, -209,    0,    0,
    0,    0, -247,    0, -202,    0, -164,    0, -187,    0,
    0, -166, -162, -175,    0,    0, -173,    0,    0, -186,
 -122, -209, -260, -209, -165, -154,    0, -130,    0, -114,
 -209, -209, -118,    0,    0,    0, -209, -209, -209, -209,
    0,    0, -209,    0,    0, -113, -260, -109,    0, -108,
    0, -110, -105,    0, -107,    0, -175,  -98,    0,    0,
 -102, -209, -102, -104,    0, -209,    0,  -86,  -99,    0,
    0,    0, -102, -102,    0,    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,    0,  187,    0,    0,    0,    0,    0,
    0, -121,    0,    0,    0,    0,    0,    0,    0, -152,
    0, -119,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -80,    0,    0,    0,    0,    0,    0,    0,
    0,  -28, -242,  -50,    0,    0,  -60,    0,    0, -148,
    0,    0,    0,    0,    0,    0,    0,  -70,    0,    0,
    0,  -89, -121,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -82,    0,    0,    0,  -40,  -30,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -133,    0,    0,
    0,    0,    0,    0,    0,    0,
};
#if YYBTYACC
static const YYINT yycindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,
};
#endif
static const YYINT yygindex[] = {                         0,
    0,  193,   49,    0,    0,    0,    8,    0,   -9,    0,
    0,  -45,  -31,  -57,  -61,  126,  134,  127,  -32,    0,
  -36,    0,
};
#define YYTABLESIZE 253
static const YYINT yytable[] = {                         68,
   68,   75,   65,   67,   69,   87,   70,   10,   36,   26,
   94,   12,   10,   18,   37,   38,   14,   71,   59,   22,
   40,  100,   20,   44,   41,   42,   43,   15,   72,  102,
   86,   44,   88,   23,   44,   68,   60,   44,   44,   92,
   68,   68,   68,   68,  112,   19,   68,   99,    7,   27,
    1,    2,    3,    7,   30,  108,   61,  110,   62,   36,
   63,   31,   64,   32,   33,   37,   38,  115,  116,   68,
  109,   34,   35,   36,   66,   41,   42,   43,   24,   37,
   38,   25,   24,   39,   73,   40,    1,    2,    3,   41,
   42,   43,   31,   76,   32,   33,   81,   82,   28,   83,
   84,   29,   34,   35,   36,   24,   78,   77,   58,   79,
   37,   38,   80,   24,   74,   89,   40,    1,    2,    3,
   41,   42,   43,   31,   14,   31,   31,   14,   12,   16,
   17,   12,   90,   31,   31,   31,    1,    2,    3,   81,
   82,   31,   31,   85,   31,   31,   14,   31,   31,   31,
   31,   31,   31,   31,   31,  105,   32,   33,   23,   23,
   21,   21,   91,  101,   34,   35,   36,  103,  104,  107,
   78,  113,   37,   38,  106,   24,  111,  114,   40,    1,
    2,    3,   41,   42,   43,   38,    1,   61,   38,   38,
   38,   38,   38,   38,   60,   53,   38,   11,   53,   38,
   38,   53,   96,   53,   97,   53,   53,   38,   53,   53,
   53,   53,   98,   53,    0,   46,   53,   53,   46,    0,
   53,   46,    0,   46,    0,   47,   46,   53,   47,   46,
   46,   47,    0,   47,    0,   45,   47,   42,    0,   47,
   47,    0,    0,   45,    0,    0,   45,    0,   42,   45,
   45,   42,   42,
};
static const YYINT yycheck[] = {                         36,
   37,   47,   34,   36,   37,   63,   38,    0,  269,   19,
   72,  287,    5,  285,  275,  276,  265,  265,   28,  287,
  281,   83,   15,  266,  285,  286,  287,  276,  276,   87,
   62,  274,   64,  266,  277,   72,   29,  280,  281,   71,
   77,   78,   79,   80,  106,  277,   83,   80,    0,  287,
  282,  283,  284,    5,  265,  101,  285,  103,  276,  269,
  276,  257,  276,  259,  260,  275,  276,  113,  114,  106,
  102,  267,  268,  269,  276,  285,  286,  287,  278,  275,
  276,  281,  278,  279,  287,  281,  282,  283,  284,  285,
  286,  287,  257,  281,  259,  260,  270,  271,  277,  273,
  287,  280,  267,  268,  269,  278,  269,  274,  281,  272,
  275,  276,  288,  278,  279,  281,  281,  282,  283,  284,
  285,  286,  287,  257,  277,  259,  260,  280,  277,  280,
  281,  280,  287,  267,  268,  269,  282,  283,  284,  270,
  271,  275,  276,  266,  278,  279,  265,  281,  282,  283,
  284,  285,  286,  287,  257,  266,  259,  260,  280,  281,
  280,  281,  277,  277,  267,  268,  269,  277,  277,  277,
  269,  258,  275,  276,  280,  278,  281,  277,  281,  282,
  283,  284,  285,  286,  287,  266,    0,  277,  269,  270,
  271,  272,  273,  274,  277,  266,  277,    5,  269,  280,
  281,  272,   77,  274,   78,  266,  277,  288,  269,  280,
  281,  272,   79,  274,   -1,  266,  277,  288,  269,   -1,
  281,  272,   -1,  274,   -1,  266,  277,  288,  269,  280,
  281,  272,   -1,  274,   -1,  266,  277,  266,   -1,  280,
  281,   -1,   -1,  274,   -1,   -1,  277,   -1,  277,  280,
  281,  280,  281,
};
#if YYBTYACC
static const YYINT yyctable[] = {                        -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,
};
#endif
#define YYFINAL 4
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 290
#define YYUNDFTOKEN 315
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"$end",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"error","IF","ELSE","FOR","WHILE","CHAR",
"DO","DOUBLE","BITOP","LSQUARE","RSQUARE","RETURN","PRINTLN","ADDOP","INCOP",
"DECOP","RELOP","ASSIGNOP","LOGICOP","NOT","LPAREN","RPAREN","LCURL","RCURL",
"COMMA","SEMICOLON","INT","FLOAT","VOID","CONST_INT","CONST_FLOAT","ID","MULOP",
"LTHIRD","RTHIRD","$accept","start","program","unit","var_declaration",
"func_declaration","func_definition","arguments","type_specifier",
"parameter_list","compound_statement","statements","declaration_list",
"statement","expression","expression_statement","logic_expression",
"rel_expression","simple_expression","term","unary_expression","factor",
"variable","argument_list","illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : start",
"start : program",
"program : program unit",
"program : unit",
"unit : var_declaration",
"unit : func_declaration",
"unit : func_definition",
"func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON",
"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON",
"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement",
"func_definition : type_specifier ID LPAREN RPAREN compound_statement",
"parameter_list : parameter_list COMMA type_specifier ID",
"parameter_list : parameter_list COMMA type_specifier",
"parameter_list : type_specifier ID",
"parameter_list : type_specifier",
"compound_statement : LCURL statements RCURL",
"compound_statement : LCURL RCURL",
"var_declaration : type_specifier declaration_list SEMICOLON",
"type_specifier : INT",
"type_specifier : FLOAT",
"type_specifier : VOID",
"declaration_list : declaration_list COMMA ID",
"declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE",
"declaration_list : ID",
"declaration_list : ID LSQUARE CONST_INT RSQUARE",
"statements : statement",
"statements : statements statement",
"statement : var_declaration",
"statement : expression_statement",
"statement : compound_statement",
"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement",
"statement : IF LPAREN expression RPAREN statement",
"statement : IF LPAREN expression RPAREN statement ELSE statement",
"statement : WHILE LPAREN expression RPAREN statement",
"statement : PRINTLN LPAREN ID RPAREN SEMICOLON",
"statement : RETURN expression SEMICOLON",
"expression_statement : SEMICOLON",
"expression_statement : expression SEMICOLON",
"variable : ID",
"variable : ID LSQUARE expression RSQUARE",
"expression : logic_expression",
"expression : variable ASSIGNOP logic_expression",
"logic_expression : rel_expression",
"logic_expression : rel_expression LOGICOP rel_expression",
"rel_expression : simple_expression",
"rel_expression : simple_expression RELOP simple_expression",
"simple_expression : term",
"simple_expression : simple_expression ADDOP term",
"term : unary_expression",
"term : term MULOP unary_expression",
"unary_expression : ADDOP unary_expression",
"unary_expression : NOT unary_expression",
"unary_expression : factor",
"factor : variable",
"factor : ID LPAREN argument_list RPAREN",
"factor : LPAREN expression RPAREN",
"factor : CONST_INT",
"factor : CONST_FLOAT",
"factor : variable INCOP",
"factor : variable DECOP",
"argument_list : arguments",
"argument_list :",
"arguments : arguments COMMA logic_expression",
"arguments : logic_expression",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;
int      yynerrs;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
YYLTYPE  yyloc; /* position returned by actions */
YYLTYPE  yylloc; /* position from the lexer */
#endif

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(loc, rhs, n) \
do \
{ \
    if (n == 0) \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 0).last_line; \
        (loc).first_column = YYRHSLOC(rhs, 0).last_column; \
        (loc).last_line    = YYRHSLOC(rhs, 0).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, 0).last_column; \
    } \
    else \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 1).first_line; \
        (loc).first_column = YYRHSLOC(rhs, 1).first_column; \
        (loc).last_line    = YYRHSLOC(rhs, n).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, n).last_column; \
    } \
} while (0)
#endif /* YYLLOC_DEFAULT */
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#if YYBTYACC

#ifndef YYLVQUEUEGROWTH
#define YYLVQUEUEGROWTH 32
#endif
#endif /* YYBTYACC */

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#ifndef YYINITSTACKSIZE
#define YYINITSTACKSIZE 200
#endif

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  *p_base;
    YYLTYPE  *p_mark;
#endif
} YYSTACKDATA;
#if YYBTYACC

struct YYParseState_s
{
    struct YYParseState_s *save;    /* Previously saved parser state */
    YYSTACKDATA            yystack; /* saved parser stack */
    int                    state;   /* saved parser state */
    int                    errflag; /* saved error recovery status */
    int                    lexeme;  /* saved index of the conflict lexeme in the lexical queue */
    YYINT                  ctry;    /* saved index in yyctable[] for this conflict */
};
typedef struct YYParseState_s YYParseState;
#endif /* YYBTYACC */
/* variables for the parser stack */
static YYSTACKDATA yystack;
#if YYBTYACC

/* Current parser state */
static YYParseState *yyps = 0;

/* yypath != NULL: do the full parse, starting at *yypath parser state. */
static YYParseState *yypath = 0;

/* Base of the lexical value queue */
static YYSTYPE *yylvals = 0;

/* Current position at lexical value queue */
static YYSTYPE *yylvp = 0;

/* End position of lexical value queue */
static YYSTYPE *yylve = 0;

/* The last allocated position at the lexical value queue */
static YYSTYPE *yylvlim = 0;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
/* Base of the lexical position queue */
static YYLTYPE *yylpsns = 0;

/* Current position at lexical position queue */
static YYLTYPE *yylpp = 0;

/* End position of lexical position queue */
static YYLTYPE *yylpe = 0;

/* The last allocated position at the lexical position queue */
static YYLTYPE *yylplim = 0;
#endif

/* Current position at lexical token queue */
static YYINT  *yylexp = 0;

static YYINT  *yylexemes = 0;
#endif /* YYBTYACC */
#line 1201 "parser.y"
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

#line 721 "y.tab.c"

/* For use in generated program */
#define yydepth (int)(yystack.s_mark - yystack.s_base)
#if YYBTYACC
#define yytrial (yyps->save)
#endif /* YYBTYACC */

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE *newps;
#endif

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    newps = (YYLTYPE *)realloc(data->p_base, newsize * sizeof(*newps));
    if (newps == 0)
        return YYENOMEM;

    data->p_base = newps;
    data->p_mark = newps + i;
#endif

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;

#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%sdebug: stack size increased to %d\n", YYPREFIX, newsize);
#endif
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    free(data->p_base);
#endif
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif /* YYPURE || defined(YY_NO_LEAKS) */
#if YYBTYACC

static YYParseState *
yyNewState(unsigned size)
{
    YYParseState *p = (YYParseState *) malloc(sizeof(YYParseState));
    if (p == NULL) return NULL;

    p->yystack.stacksize = size;
    if (size == 0)
    {
        p->yystack.s_base = NULL;
        p->yystack.l_base = NULL;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        p->yystack.p_base = NULL;
#endif
        return p;
    }
    p->yystack.s_base    = (YYINT *) malloc(size * sizeof(YYINT));
    if (p->yystack.s_base == NULL) return NULL;
    p->yystack.l_base    = (YYSTYPE *) malloc(size * sizeof(YYSTYPE));
    if (p->yystack.l_base == NULL) return NULL;
    memset(p->yystack.l_base, 0, size * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    p->yystack.p_base    = (YYLTYPE *) malloc(size * sizeof(YYLTYPE));
    if (p->yystack.p_base == NULL) return NULL;
    memset(p->yystack.p_base, 0, size * sizeof(YYLTYPE));
#endif

    return p;
}

static void
yyFreeState(YYParseState *p)
{
    yyfreestack(&p->yystack);
    free(p);
}
#endif /* YYBTYACC */

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab
#if YYBTYACC
#define YYVALID        do { if (yyps->save)            goto yyvalid; } while(0)
#define YYVALID_NESTED do { if (yyps->save && \
                                yyps->save->save == 0) goto yyvalid; } while(0)
#endif /* YYBTYACC */

int
YYPARSE_DECL()
{
    int yym, yyn, yystate, yyresult;
#if YYBTYACC
    int yynewerrflag;
    YYParseState *yyerrctx = NULL;
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyerror_loc_range[3]; /* position of error start/end (0 unused) */
#endif
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
    if (yydebug)
        fprintf(stderr, "%sdebug[<# of symbols on state stack>]\n", YYPREFIX);
#endif
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(yyerror_loc_range, 0, sizeof(yyerror_loc_range));
#endif

#if YYBTYACC
    yyps = yyNewState(0); if (yyps == 0) goto yyenomem;
    yyps->save = 0;
#endif /* YYBTYACC */
    yym = 0;
    /* yyn is set below */
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base;
#endif
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
#if YYBTYACC
        do {
        if (yylvp < yylve)
        {
            /* we're currently re-reading tokens */
            yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc = *yylpp++;
#endif
            yychar = *yylexp++;
            break;
        }
        if (yyps->save)
        {
            /* in trial mode; save scanner results for future parse attempts */
            if (yylvp == yylvlim)
            {   /* Enlarge lexical value queue */
                size_t p = (size_t) (yylvp - yylvals);
                size_t s = (size_t) (yylvlim - yylvals);

                s += YYLVQUEUEGROWTH;
                if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL) goto yyenomem;
                if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL) goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL) goto yyenomem;
#endif
                yylvp   = yylve = yylvals + p;
                yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp   = yylpe = yylpsns + p;
                yylplim = yylpsns + s;
#endif
                yylexp  = yylexemes + p;
            }
            *yylexp = (YYINT) YYLEX;
            *yylvp++ = yylval;
            yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *yylpp++ = yylloc;
            yylpe++;
#endif
            yychar = *yylexp++;
            break;
        }
        /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
        yychar = YYLEX;
#if YYBTYACC
        } while (0);
#endif /* YYBTYACC */
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, " <%s>", YYSTYPE_TOSTRING(yychar, yylval));
#endif
            fputc('\n', stderr);
        }
#endif
    }
#if YYBTYACC

    /* Do we have a conflict? */
    if (((yyn = yycindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
        yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        YYINT ctry;

        if (yypath)
        {
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: CONFLICT in state %d: following successful trial parse\n",
                                YYDEBUGSTR, yydepth, yystate);
#endif
            /* Switch to the next conflict context */
            save = yypath;
            yypath = save->save;
            save->save = NULL;
            ctry = save->ctry;
            if (save->state != yystate) YYABORT;
            yyFreeState(save);

        }
        else
        {

            /* Unresolved conflict - start/continue trial parse */
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
            {
                fprintf(stderr, "%s[%d]: CONFLICT in state %d. ", YYDEBUGSTR, yydepth, yystate);
                if (yyps->save)
                    fputs("ALREADY in conflict, continuing trial parse.\n", stderr);
                else
                    fputs("Starting trial parse.\n", stderr);
            }
#endif
            save                  = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (save == NULL) goto yyenomem;
            save->save            = yyps->save;
            save->state           = yystate;
            save->errflag         = yyerrflag;
            save->yystack.s_mark  = save->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (save->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            save->yystack.l_mark  = save->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (save->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            save->yystack.p_mark  = save->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (save->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            ctry                  = yytable[yyn];
            if (yyctable[ctry] == -1)
            {
#if YYDEBUG
                if (yydebug && yychar >= YYEOF)
                    fprintf(stderr, "%s[%d]: backtracking 1 token\n", YYDEBUGSTR, yydepth);
#endif
                ctry++;
            }
            save->ctry = ctry;
            if (yyps->save == NULL)
            {
                /* If this is a first conflict in the stack, start saving lexemes */
                if (!yylexemes)
                {
                    yylexemes = (YYINT *) malloc((YYLVQUEUEGROWTH) * sizeof(YYINT));
                    if (yylexemes == NULL) goto yyenomem;
                    yylvals   = (YYSTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYSTYPE));
                    if (yylvals == NULL) goto yyenomem;
                    yylvlim   = yylvals + YYLVQUEUEGROWTH;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpsns   = (YYLTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYLTYPE));
                    if (yylpsns == NULL) goto yyenomem;
                    yylplim   = yylpsns + YYLVQUEUEGROWTH;
#endif
                }
                if (yylvp == yylve)
                {
                    yylvp  = yylve = yylvals;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp  = yylpe = yylpsns;
#endif
                    yylexp = yylexemes;
                    if (yychar >= YYEOF)
                    {
                        *yylve++ = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                        *yylpe++ = yylloc;
#endif
                        *yylexp  = (YYINT) yychar;
                        yychar   = YYEMPTY;
                    }
                }
            }
            if (yychar >= YYEOF)
            {
                yylvp--;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp--;
#endif
                yylexp--;
                yychar = YYEMPTY;
            }
            save->lexeme = (int) (yylvp - yylvals);
            yyps->save   = save;
        }
        if (yytable[yyn] == ctry)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                                YYDEBUGSTR, yydepth, yystate, yyctable[ctry]);
#endif
            if (yychar < 0)
            {
                yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp++;
#endif
                yylexp++;
            }
            if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                goto yyoverflow;
            yystate = yyctable[ctry];
            *++yystack.s_mark = (YYINT) yystate;
            *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *++yystack.p_mark = yylloc;
#endif
            yychar  = YYEMPTY;
            if (yyerrflag > 0) --yyerrflag;
            goto yyloop;
        }
        else
        {
            yyn = yyctable[ctry];
            goto yyreduce;
        }
    } /* End of code dealing with conflicts */
#endif /* YYBTYACC */
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                            YYDEBUGSTR, yydepth, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yylloc;
#endif
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;
#if YYBTYACC

    yynewerrflag = 1;
    goto yyerrhandler;
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */

yyerrlab:
    /* explicit YYERROR from an action -- pop the rhs of the rule reduced
     * before looking for error recovery */
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif

    yynewerrflag = 0;
yyerrhandler:
    while (yyps->save)
    {
        int ctry;
        YYParseState *save = yyps->save;
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: ERROR in state %d, CONFLICT BACKTRACKING to state %d, %d tokens\n",
                            YYDEBUGSTR, yydepth, yystate, yyps->save->state,
                    (int)(yylvp - yylvals - yyps->save->lexeme));
#endif
        /* Memorize most forward-looking error state in case it's really an error. */
        if (yyerrctx == NULL || yyerrctx->lexeme < yylvp - yylvals)
        {
            /* Free old saved error context state */
            if (yyerrctx) yyFreeState(yyerrctx);
            /* Create and fill out new saved error context state */
            yyerrctx                 = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (yyerrctx == NULL) goto yyenomem;
            yyerrctx->save           = yyps->save;
            yyerrctx->state          = yystate;
            yyerrctx->errflag        = yyerrflag;
            yyerrctx->yystack.s_mark = yyerrctx->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (yyerrctx->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yyerrctx->yystack.l_mark = yyerrctx->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (yyerrctx->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yyerrctx->yystack.p_mark = yyerrctx->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (yyerrctx->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yyerrctx->lexeme         = (int) (yylvp - yylvals);
        }
        yylvp          = yylvals   + save->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yylpp          = yylpsns   + save->lexeme;
#endif
        yylexp         = yylexemes + save->lexeme;
        yychar         = YYEMPTY;
        yystack.s_mark = yystack.s_base + (save->yystack.s_mark - save->yystack.s_base);
        memcpy (yystack.s_base, save->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
        yystack.l_mark = yystack.l_base + (save->yystack.l_mark - save->yystack.l_base);
        memcpy (yystack.l_base, save->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yystack.p_mark = yystack.p_base + (save->yystack.p_mark - save->yystack.p_base);
        memcpy (yystack.p_base, save->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
        ctry           = ++save->ctry;
        yystate        = save->state;
        /* We tried shift, try reduce now */
        if ((yyn = yyctable[ctry]) >= 0) goto yyreduce;
        yyps->save     = save->save;
        save->save     = NULL;
        yyFreeState(save);

        /* Nothing left on the stack -- error */
        if (!yyps->save)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%sdebug[%d,trial]: trial parse FAILED, entering ERROR mode\n",
                                YYPREFIX, yydepth);
#endif
            /* Restore state as it was in the most forward-advanced error */
            yylvp          = yylvals   + yyerrctx->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylpp          = yylpsns   + yyerrctx->lexeme;
#endif
            yylexp         = yylexemes + yyerrctx->lexeme;
            yychar         = yylexp[-1];
            yylval         = yylvp[-1];
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc         = yylpp[-1];
#endif
            yystack.s_mark = yystack.s_base + (yyerrctx->yystack.s_mark - yyerrctx->yystack.s_base);
            memcpy (yystack.s_base, yyerrctx->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yystack.l_mark = yystack.l_base + (yyerrctx->yystack.l_mark - yyerrctx->yystack.l_base);
            memcpy (yystack.l_base, yyerrctx->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yystack.p_mark = yystack.p_base + (yyerrctx->yystack.p_mark - yyerrctx->yystack.p_base);
            memcpy (yystack.p_base, yyerrctx->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yystate        = yyerrctx->state;
            yyFreeState(yyerrctx);
            yyerrctx       = NULL;
        }
        yynewerrflag = 1;
    }
    if (yynewerrflag == 0) goto yyinrecovery;
#endif /* YYBTYACC */

    YYERROR_CALL("syntax error");
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yyerror_loc_range[1] = yylloc; /* lookahead position is error start position */
#endif

#if !YYBTYACC
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
#endif
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: state %d, error recovery shifting to state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* lookahead position is error end position */
                yyerror_loc_range[2] = yylloc;
                YYLLOC_DEFAULT(yyloc, yyerror_loc_range, 2); /* position of error span */
                *++yystack.p_mark = yyloc;
#endif
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: error recovery discarding state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* the current TOS position is the error start position */
                yyerror_loc_range[1] = *yystack.p_mark;
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
                if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark, yystack.p_mark);
#else
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
                --yystack.s_mark;
                --yystack.l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                --yystack.p_mark;
#endif
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, error recovery discarding token %d (%s)\n",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
        }
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval, &yylloc);
#else
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
    yym = yylen[yyn];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: state %d, reducing by rule %d (%s)",
                        YYDEBUGSTR, yydepth, yystate, yyn, yyrule[yyn]);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            if (yym > 0)
            {
                int i;
                fputc('<', stderr);
                for (i = yym; i > 0; i--)
                {
                    if (i != yym) fputs(", ", stderr);
                    fputs(YYSTYPE_TOSTRING(yystos[yystack.s_mark[1-i]],
                                           yystack.l_mark[1-i]), stderr);
                }
                fputc('>', stderr);
            }
#endif
        fputc('\n', stderr);
    }
#endif
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)

    /* Perform position reduction */
    memset(&yyloc, 0, sizeof(yyloc));
#if YYBTYACC
    if (!yytrial)
#endif /* YYBTYACC */
    {
        YYLLOC_DEFAULT(yyloc, &yystack.p_mark[-yym], yym);
        /* just in case YYERROR is invoked within the action, save
           the start of the rhs as the error start position */
        yyerror_loc_range[1] = yystack.p_mark[1-yym];
    }
#endif

    switch (yyn)
    {
case 1:
#line 191 "parser.y"
	{
        string parent = "start";
        string children = "program";
        string rule = parent + " : " + children;
        flog << rule << endl;
        int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
        int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
        yyval.parseTreeNode = getNode(rule, startLine, endLine);
        yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
		final_outputt();
		yyval.parseTreeNode->print_tree_node(treeout);
    }
#line 1405 "y.tab.c"
break;
case 2:
#line 206 "parser.y"
	{
        string parent = "program";
        string children = "program unit";
        string rule = parent + " : " + children;
        flog << rule << endl;
        int startLine = yystack.l_mark[-1].parseTreeNode->getStartLine();
        int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
        yyval.parseTreeNode = getNode(rule, startLine, endLine);
        yyval.parseTreeNode->addChild({yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
    }
#line 1419 "y.tab.c"
break;
case 3:
#line 217 "parser.y"
	{
        string parent = "program";
        string children = "unit";
        string rule = parent + " : " + children;
 		flog << rule << endl;
        int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
        int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
        yyval.parseTreeNode = getNode(rule, startLine, endLine);
        yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
    }
#line 1433 "y.tab.c"
break;
case 4:
#line 230 "parser.y"
	{
        string parent = "unit";
        string children = "var_declaration";
        string rule = parent + " : " + children;
        flog << rule << endl;
        int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
        int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
        yyval.parseTreeNode = getNode(rule, startLine, endLine);
        yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
     }
#line 1447 "y.tab.c"
break;
case 5:
#line 241 "parser.y"
	{
        string parent = "unit";
        string children = "func_declaration";
        string rule = parent + " : " + children;
        flog << rule << endl;
		decout<<rule<<endl;
        int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
        int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
        yyval.parseTreeNode = getNode(rule, startLine, endLine);
        yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
     }
#line 1462 "y.tab.c"
break;
case 6:
#line 253 "parser.y"
	{
        string parent = "unit";
        string children = "func_definition";
        string rule = parent + " : " + children;
        flog << rule << endl;
        int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
        int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
        yyval.parseTreeNode = getNode(rule, startLine, endLine);
        yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
     }
#line 1476 "y.tab.c"
break;
case 7:
#line 268 "parser.y"
	{
			string parent = "func_declaration";
			string children = "type_specifier ID LPAREN parameter_list RPAREN SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
			decout<<rule<<endl;
            int startLine = yystack.l_mark[-5].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-5].parseTreeNode,yystack.l_mark[-4].parseTreeNode,yystack.l_mark[-3].parseTreeNode,yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
			auto ans=tokenize(yystack.l_mark[-5].parseTreeNode->getRule(),':');
			auto ans2=tokenize(yystack.l_mark[-4].parseTreeNode->getRule(),':');
			FunctionRecord fc(ans2[1],param_count,ans[1],true);
			bool flag=true;
			for(auto& f : function_record)
			{
				if(flag && f.is_error(fc).first==true)
				{
					/*errorout<<"hello"<<endl;*/
					errorout<<"Line# "<<yystack.l_mark[-4].parseTreeNode->getStartLine()<<" : "<<f.is_error(fc).second<<endl;
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
#line 1514 "y.tab.c"
break;
case 8:
#line 303 "parser.y"
	{
			string parent = "func_declaration";
			string children = "type_specifier ID LPAREN RPAREN SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-4].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-4].parseTreeNode,yystack.l_mark[-3].parseTreeNode,yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
			auto ans=tokenize(yystack.l_mark[-4].parseTreeNode->getRule(),':');
			auto ans2=tokenize(yystack.l_mark[-3].parseTreeNode->getRule(),':');
			FunctionRecord fc(ans2[1],0,ans[1],true);
			bool flag=true;
			for(auto& f : function_record)
			{
				if(flag && f.is_error(fc).first==true)
				{
					/*errorout<<"hello no param"<<endl;*/
					errorout<<"Line# "<<yystack.l_mark[-3].parseTreeNode->getStartLine()<<" : "<<f.is_error(fc).second<<endl;
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
#line 1548 "y.tab.c"
break;
case 9:
#line 336 "parser.y"
	{
			auto ans=tokenize(yystack.l_mark[-5].parseTreeNode->getRule(),':');
			auto ans2=tokenize(yystack.l_mark[-4].parseTreeNode->getRule(),':');
			FunctionRecord fc(ans2[1],param_count,ans[1],false);
			bool flag=true;
			for(auto& f : function_record)
			{
				if(flag && f.is_error(fc).first==true)
				{
					/*errorout<<"hello no param"<<endl;*/
					errorout<<"Line# "<<yystack.l_mark[-4].parseTreeNode->getStartLine()<<" : "<<f.is_error(fc).second<<endl;
					flag=false;
					error_count++;
					break;
				}
				  
			}
			if(!(st.lookup(ans2[1],flog).empty()))
			{
				flag=false;
				errorout<<"Line# "<<yystack.l_mark[-4].parseTreeNode->getStartLine()<<" : "<<"'"+ans2[1]+"'"+"redeclared as different kind of symbol"<<endl;
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
			
            int startLine = yystack.l_mark[-5].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-5].parseTreeNode, yystack.l_mark[-4].parseTreeNode, yystack.l_mark[-3].parseTreeNode, yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
			
			  
			param_count=0;
			for(auto& c : variable_vector)
			{
				/*errorout<<c->getType()<<" ";*/
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
#line 1613 "y.tab.c"
break;
case 10:
#line 398 "parser.y"
	{
			auto ans=tokenize(yystack.l_mark[-4].parseTreeNode->getRule(),':');
			auto ans2=tokenize(yystack.l_mark[-3].parseTreeNode->getRule(),':');
			FunctionRecord fc(ans2[1],0,ans[1],false);
			bool flag=true;
			for(auto& f : function_record)
			{
				if(flag && f.is_error(fc).first==true)
				{
					/*errorout<<"hello no param"<<endl;*/
					errorout<<"Line# "<<yystack.l_mark[-3].parseTreeNode->getStartLine()<<" : "<<f.is_error(fc).second<<endl;
					flag=false;
					error_count++;
					break;
				}
				  
			}
			if(!(st.lookup(ans2[1],flog).empty()))
			{
				flag=false;
				errorout<<"Line# "<<yystack.l_mark[-3].parseTreeNode->getStartLine()<<" : "<<"'"+ans2[1]+"'"+"redeclared as different kind of symbol"<<endl;
				error_count++;
			}
			/*errorout<<ans2[1]<<endl;*/
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
            int startLine = yystack.l_mark[-4].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-4].parseTreeNode, yystack.l_mark[-3].parseTreeNode, yystack.l_mark[-2].parseTreeNode, yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
			
			  
			param_count=0;
			for(auto& v : variable_vector)
			{
				fc.add_parameter(getIDName(v));
			}
			variable_vector.clear();
			
		}
#line 1666 "y.tab.c"
break;
case 11:
#line 450 "parser.y"
	{
			string parent = "parameter_list";
			string children = "parameter_list COMMA type_specifier ID";
			string rule = parent + " : " + children;
 			flog << rule << endl;
			decout<<rule<<endl;
            int startLine = yystack.l_mark[-3].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-3].parseTreeNode, yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
			param_count++;
			/*current_func_param_list.push_back(getIDName($4));*/
			
			auto ans=tokenize(yystack.l_mark[-1].parseTreeNode->getRule(),':');
			yystack.l_mark[0].parseTreeNode->setType(ans[1]);
			variable_vector.push_back(yystack.l_mark[0].parseTreeNode);
		}
#line 1687 "y.tab.c"
break;
case 12:
#line 468 "parser.y"
	{
			string parent = "parameter_list";
			string children = "parameter_list COMMA type_specifier";
			string rule = parent + " : " + children;
 			flog << rule << endl;
			decout<<rule<<endl;
            int startLine = yystack.l_mark[-2].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
			param_count++;
			/*current_func_param_list.push_back(getIDName($4));*/
		}
#line 1704 "y.tab.c"
break;
case 13:
#line 482 "parser.y"
	{
			string parent = "parameter_list";
			string children = "type_specifier ID";
			string rule = parent + " : " + children;
 			flog << rule << endl;
			decout<<rule<<endl;
            int startLine = yystack.l_mark[-1].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
			param_count++;
			/*current_func_param_list.push_back(getIDName($2));*/
			auto ans=tokenize(yystack.l_mark[-1].parseTreeNode->getRule(),':');
			yystack.l_mark[0].parseTreeNode->setType(ans[1]);
			variable_vector.push_back(yystack.l_mark[0].parseTreeNode);
		}
#line 1724 "y.tab.c"
break;
case 14:
#line 499 "parser.y"
	{
			string parent = "parameter_list";
			string children = "type_specifier";
			string rule = parent + " : " + children;
 			flog << rule << endl;
			decout<<rule<<endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
			param_count++;
			/*current_func_param_list.push_back(getIDName($4));*/
		}
#line 1741 "y.tab.c"
break;
case 15:
#line 515 "parser.y"
	{
			string parent = "compound_statement";
			string children = "LCURL statements RCURL";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-2].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
			/*st.printAllScope(flog);*/
			for(auto& node : variable_vector)
			{
				auto ans=tokenize(node->getRule(),':');
				st.insert(ans[1],node->getType(),flog);

			}
			variable_vector.clear();
			
		}
#line 1764 "y.tab.c"
break;
case 16:
#line 535 "parser.y"
	{
			string parent = "compound_statement";
			string children = "LCURL RCURL";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-1].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
			yyval.parseTreeNode->addChild({yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
			
		}
#line 1779 "y.tab.c"
break;
case 17:
#line 550 "parser.y"
	{
			string parent = "var_declaration";
			string children = "type_specifier declaration_list SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-2].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
			auto ans3=tokenize(yystack.l_mark[-2].parseTreeNode->getRule(),':');
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
					/*errorout<<s2<<" "<<s1<<endl;*/
					if(s2.compare(s1)!=0)
					  errorout<<"Line# "+ to_string(node->getStartLine())  +"  : Conflicting types for '" + ans[1] +"'"<<endl;
					else 
					  errorout<<"Line# "+ to_string(node->getStartLine())  +"  : Redefinition of variable '" + ans[1] +"'"<<endl;
					  
				}
				else if(ans3[1].compare("VOID")==0)
				{
					if(!void_flag)
					  errorout<<"Line# "+to_string(yyval.parseTreeNode->getStartLine())+" : Variable or field ";
					void_flag=true;
					
					/*for(auto& x : variable_vector)*/
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
#line 1835 "y.tab.c"
break;
case 18:
#line 605 "parser.y"
	{
			string parent = "type_specifier";
			string children = "INT";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
			return_or_variable_type="INT";
		}
#line 1850 "y.tab.c"
break;
case 19:
#line 617 "parser.y"
	{
			string parent = "type_specifier";
			string children = "FLOAT";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
             yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
			 return_or_variable_type="FLOAT";
		}
#line 1865 "y.tab.c"
break;
case 20:
#line 629 "parser.y"
	{
			string parent = "type_specifier";
			string children = "VOID";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
             yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
			 return_or_variable_type="VOID";
			
		}
#line 1881 "y.tab.c"
break;
case 21:
#line 644 "parser.y"
	{
			string parent = "declaration_list";
			string children = "declaration_list COMMA ID";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-2].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
			yystack.l_mark[0].parseTreeNode->setType(return_or_variable_type);
			variable_vector.push_back(yystack.l_mark[0].parseTreeNode);
		}
#line 1897 "y.tab.c"
break;
case 22:
#line 657 "parser.y"
	{
			string parent = "declaration_list";
			string children = "declaration_list COMMA ID LSQUARE CONST_INT RSQUARE";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-5].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-5].parseTreeNode,yystack.l_mark[-4].parseTreeNode,yystack.l_mark[-3].parseTreeNode,yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
			yystack.l_mark[-3].parseTreeNode->setType("ARRAY, "+return_or_variable_type);
			/*$3->setType("ARRAY");*/
			variable_vector.push_back(yystack.l_mark[-3].parseTreeNode);
		}
#line 1914 "y.tab.c"
break;
case 23:
#line 671 "parser.y"
	{
			string parent = "declaration_list";
			string children = "ID";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
			yystack.l_mark[0].parseTreeNode->setType(return_or_variable_type);
			variable_vector.push_back(yystack.l_mark[0].parseTreeNode);
		}
#line 1930 "y.tab.c"
break;
case 24:
#line 684 "parser.y"
	{
			string parent = "declaration_list";
			string children = "ID LSQUARE CONST_INT RSQUARE";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-3].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-3].parseTreeNode,yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
			/*$1->setType("ARRAY, "+return_or_variable_type);*/
			yystack.l_mark[-3].parseTreeNode->setType("ARRAY");
			variable_vector.push_back(yystack.l_mark[-3].parseTreeNode);
		}
#line 1947 "y.tab.c"
break;
case 25:
#line 700 "parser.y"
	{
			string parent = "statements";
			string children = "statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
		}
#line 1961 "y.tab.c"
break;
case 26:
#line 711 "parser.y"
	{
			string parent = "statements";
			string children = "statements statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-1].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
		}
#line 1975 "y.tab.c"
break;
case 27:
#line 724 "parser.y"
	{
			string parent = "statement";
			string children = "var_declaration";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
		}
#line 1989 "y.tab.c"
break;
case 28:
#line 735 "parser.y"
	{
			string parent = "statement";
			string children = "expression_statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
		}
#line 2003 "y.tab.c"
break;
case 29:
#line 746 "parser.y"
	{
			string parent = "statement";
			string children = "compound_statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
		}
#line 2017 "y.tab.c"
break;
case 30:
#line 757 "parser.y"
	{
			string parent = "statement";
			string children = "FOR LPAREN expression_statement expression_statement expression RPAREN statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-6].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-6].parseTreeNode, yystack.l_mark[-5].parseTreeNode, yystack.l_mark[-4].parseTreeNode,yystack.l_mark[-3].parseTreeNode,yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
		}
#line 2031 "y.tab.c"
break;
case 31:
#line 768 "parser.y"
	{
			string parent = "statement";
			string children = "IF LPAREN expression RPAREN statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-4].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-4].parseTreeNode,yystack.l_mark[-3].parseTreeNode,yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
		}
#line 2045 "y.tab.c"
break;
case 32:
#line 779 "parser.y"
	{
			string parent = "statement";
			string children = "IF LPAREN expression RPAREN statement ELSE statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-6].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-6].parseTreeNode,yystack.l_mark[-5].parseTreeNode,yystack.l_mark[-4].parseTreeNode,yystack.l_mark[-3].parseTreeNode,yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
		}
#line 2059 "y.tab.c"
break;
case 33:
#line 790 "parser.y"
	{
			string parent = "statement";
			string children = "WHILE LPAREN expression RPAREN statement";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-4].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-4].parseTreeNode,yystack.l_mark[-3].parseTreeNode,yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
		}
#line 2073 "y.tab.c"
break;
case 34:
#line 801 "parser.y"
	{
			string parent = "statement";
			string children = "PRINTLN LPAREN ID RPAREN SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-4].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-4].parseTreeNode,yystack.l_mark[-3].parseTreeNode,yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
		}
#line 2087 "y.tab.c"
break;
case 35:
#line 812 "parser.y"
	{
			string parent = "statement";
			string children = "RETURN expression SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-2].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
		}
#line 2101 "y.tab.c"
break;
case 36:
#line 825 "parser.y"
	{
			string parent = "expression_statement";
			string children = "SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
			 yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
		}
#line 2115 "y.tab.c"
break;
case 37:
#line 836 "parser.y"
	{
			string parent = "expression_statement";
			string children = "expression SEMICOLON";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-1].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
		}
#line 2129 "y.tab.c"
break;
case 38:
#line 849 "parser.y"
	{
			string parent = "variable";
			string children = "ID";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
             yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
			 errorout<<getIDName(yystack.l_mark[0].parseTreeNode)<<endl;
		
		  if(st.lookup(getIDName(yystack.l_mark[0].parseTreeNode),flog).empty())
		{
			errorout<<"Line# "+ to_string(yyval.parseTreeNode->getStartLine())+": Undeclared variable '"<<getIDName(yystack.l_mark[0].parseTreeNode)<<"'"<<endl;
			error_count++;
		} 
		current_func_param_list.push_back("INT");
		}
#line 2151 "y.tab.c"
break;
case 39:
#line 868 "parser.y"
	{
			string parent = "variable";
			string children = "ID LSQUARE expression RSQUARE";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-3].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-3].parseTreeNode,yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
		}
#line 2165 "y.tab.c"
break;
case 40:
#line 881 "parser.y"
	{
			string parent = "expression";
			string children = "logic_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
		}
#line 2179 "y.tab.c"
break;
case 41:
#line 892 "parser.y"
	{
			string parent = "expression";
			string children = "variable ASSIGNOP logic_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-2].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
		}
#line 2193 "y.tab.c"
break;
case 42:
#line 905 "parser.y"
	{
			string parent = "logic_expression";
			string children = "rel_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
		 }
#line 2207 "y.tab.c"
break;
case 43:
#line 916 "parser.y"
	{
			string parent = "logic_expression";
			string children = "rel_expression LOGICOP rel_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-2].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
		 }
#line 2221 "y.tab.c"
break;
case 44:
#line 929 "parser.y"
	{
			string parent = "rel_expression";
			string children = "simple_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
		}
#line 2235 "y.tab.c"
break;
case 45:
#line 940 "parser.y"
	{
			string parent = "rel_expression";
			string children = "simple_expression RELOP simple_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-2].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
		}
#line 2249 "y.tab.c"
break;
case 46:
#line 953 "parser.y"
	{
			string parent = "simple_expression";
			string children = "term";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
		  }
#line 2263 "y.tab.c"
break;
case 47:
#line 964 "parser.y"
	{
			string parent = "simple_expression";
			string children = "simple_expression ADDOP term";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-2].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
		  }
#line 2277 "y.tab.c"
break;
case 48:
#line 977 "parser.y"
	{
		string parent = "term";
		string children = "unary_expression";
		string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
	 }
#line 2291 "y.tab.c"
break;
case 49:
#line 988 "parser.y"
	{
		string parent = "term";
		string children = "term MULOP unary_expression";
		string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-2].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
	 }
#line 2305 "y.tab.c"
break;
case 50:
#line 1001 "parser.y"
	{
			string parent = "unary_expression";
			string children = "ADDOP unary_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-1].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
		 }
#line 2319 "y.tab.c"
break;
case 51:
#line 1012 "parser.y"
	{
			string parent = "unary_expression";
			string children = "NOT unary_expression";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-1].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
		 }
#line 2333 "y.tab.c"
break;
case 52:
#line 1023 "parser.y"
	{
			string parent = "unary_expression";
			string children = "factor";
			string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
		 }
#line 2347 "y.tab.c"
break;
case 53:
#line 1035 "parser.y"
	{
		string parent = "factor";
		string children = "variable";
		string rule = parent + " : " + children;
 		flog << rule << endl;
        int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
        int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
        yyval.parseTreeNode = getNode(rule, startLine, endLine);
        yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
		


	}
#line 2364 "y.tab.c"
break;
case 54:
#line 1049 "parser.y"
	{
		string parent = "factor";
		string children = "ID LPAREN argument_list RPAREN";
		string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-3].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-3].parseTreeNode,yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
			bool flag=false;
			FunctionRecord pt;
			for(auto& c : function_record)
			{
				if(c.is_exist(getIDName(yystack.l_mark[-3].parseTreeNode)))
				  {
					flag=true;
					pt=c;
					break;
					
				  }
			}
			if(!flag)
			{
				errorout<<"Line# "+ to_string(yyval.parseTreeNode->getStartLine())+": Undeclared function '"+getIDName(yystack.l_mark[-3].parseTreeNode)+"'"<<endl;
				error_count++;
			}
			if(flag)
			{
				/*errorout<<current_func_param_list.size()<<endl;*/
				if(!(pt.is_parameter_equivalent(current_func_param_list).first))
				{
					errorout<<"Line# "+ to_string(yyval.parseTreeNode->getStartLine())+": "+pt.is_parameter_equivalent(current_func_param_list).second<<endl;
					error_count++;
				}

			}
			current_func_param_list.clear();
	}
#line 2406 "y.tab.c"
break;
case 55:
#line 1088 "parser.y"
	{
		string parent = "factor";
		string children = "LPAREN expression RPAREN";
		string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[-2].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
		
	}
#line 2421 "y.tab.c"
break;
case 56:
#line 1100 "parser.y"
	{
		string parent = "factor";
		string children = "CONST_INT";
		string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
            yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
			current_func_param_list.push_back("INT");
		
	}
#line 2437 "y.tab.c"
break;
case 57:
#line 1113 "parser.y"
	{
		string parent = "factor";
		string children = "CONST_FLOAT";
		string rule = parent + " : " + children;
 			flog << rule << endl;
            int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
            int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
            yyval.parseTreeNode = getNode(rule, startLine, endLine);
             yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
			 current_func_param_list.push_back("FLOAT");
		
	}
#line 2453 "y.tab.c"
break;
case 58:
#line 1126 "parser.y"
	{
		string parent = "factor";
		string children = "variable INCOP";
		string rule = parent + " : " + children;
		flog << rule << endl;
        int startLine = yystack.l_mark[-1].parseTreeNode->getStartLine();
        int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
        yyval.parseTreeNode = getNode(rule, startLine, endLine);
        yyval.parseTreeNode->addChild({yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
		
	}
#line 2468 "y.tab.c"
break;
case 59:
#line 1138 "parser.y"
	{
		string parent = "factor";
		string children = "variable DECOP";
		string rule = parent + " : " + children;
		flog << rule << endl;
        int startLine = yystack.l_mark[-1].parseTreeNode->getStartLine();
        int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
        yyval.parseTreeNode = getNode(rule, startLine, endLine);
        yyval.parseTreeNode->addChild({yystack.l_mark[-1].parseTreeNode,yystack.l_mark[0].parseTreeNode});
		
	}
#line 2483 "y.tab.c"
break;
case 60:
#line 1152 "parser.y"
	{
		string parent = "argument_list";
		string children = "arguments";
		string rule = parent + " : " + children;
		flog << rule << endl;
        int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
        int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
        yyval.parseTreeNode = getNode(rule, startLine, endLine);
        yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
	}
#line 2497 "y.tab.c"
break;
case 61:
#line 1163 "parser.y"
	{
		string parent = "argument_list";
		string children = "";
		string rule = parent + " : " + children;
		flog << rule << endl;
        int startLine =line_count; /* Adjust accordingly based on your implementation*/
        int endLine = line_count; /* Adjust accordingly based on your implementation*/
        yyval.parseTreeNode = getNode(rule, startLine, endLine);
	}
#line 2510 "y.tab.c"
break;
case 62:
#line 1175 "parser.y"
	{
		string parent = "arguments";
		string children = "arguments COMMA logic_expression";
		string rule = parent + " : " + children;
		flog << rule << endl;
        int startLine = yystack.l_mark[-2].parseTreeNode->getStartLine();
        int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
        yyval.parseTreeNode = getNode(rule, startLine, endLine);
        yyval.parseTreeNode->addChild({yystack.l_mark[-2].parseTreeNode,yystack.l_mark[-1].parseTreeNode, yystack.l_mark[0].parseTreeNode});
		
	}
#line 2525 "y.tab.c"
break;
case 63:
#line 1187 "parser.y"
	{
		string parent = "arguments";
		string children = "logic_expression";
		string rule = parent + " : " + children;
		flog << rule << endl;
        int startLine = yystack.l_mark[0].parseTreeNode->getStartLine();
        int endLine = yystack.l_mark[0].parseTreeNode->getEndLine();
        yyval.parseTreeNode = getNode(rule, startLine, endLine);
        yyval.parseTreeNode->addChild({yystack.l_mark[0].parseTreeNode});
		
	}
#line 2540 "y.tab.c"
break;
#line 2542 "y.tab.c"
    default:
        break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
        {
            fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[YYFINAL], yyval));
#endif
            fprintf(stderr, "shifting from state 0 to final state %d\n", YYFINAL);
        }
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yyloc;
#endif
        if (yychar < 0)
        {
#if YYBTYACC
            do {
            if (yylvp < yylve)
            {
                /* we're currently re-reading tokens */
                yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylloc = *yylpp++;
#endif
                yychar = *yylexp++;
                break;
            }
            if (yyps->save)
            {
                /* in trial mode; save scanner results for future parse attempts */
                if (yylvp == yylvlim)
                {   /* Enlarge lexical value queue */
                    size_t p = (size_t) (yylvp - yylvals);
                    size_t s = (size_t) (yylvlim - yylvals);

                    s += YYLVQUEUEGROWTH;
                    if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL)
                        goto yyenomem;
                    if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL)
                        goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL)
                        goto yyenomem;
#endif
                    yylvp   = yylve = yylvals + p;
                    yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp   = yylpe = yylpsns + p;
                    yylplim = yylpsns + s;
#endif
                    yylexp  = yylexemes + p;
                }
                *yylexp = (YYINT) YYLEX;
                *yylvp++ = yylval;
                yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                *yylpp++ = yylloc;
                yylpe++;
#endif
                yychar = *yylexp++;
                break;
            }
            /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
            yychar = YYLEX;
#if YYBTYACC
            } while (0);
#endif /* YYBTYACC */
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)\n",
                                YYDEBUGSTR, yydepth, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[yystate], yyval));
#endif
        fprintf(stderr, "shifting from state %d to state %d\n", *yystack.s_mark, yystate);
    }
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    *++yystack.p_mark = yyloc;
#endif
    goto yyloop;
#if YYBTYACC

    /* Reduction declares that this path is valid. Set yypath and do a full parse */
yyvalid:
    if (yypath) YYABORT;
    while (yyps->save)
    {
        YYParseState *save = yyps->save;
        yyps->save = save->save;
        save->save = yypath;
        yypath = save;
    }
#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%s[%d]: state %d, CONFLICT trial successful, backtracking to state %d, %d tokens\n",
                        YYDEBUGSTR, yydepth, yystate, yypath->state, (int)(yylvp - yylvals - yypath->lexeme));
#endif
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    yylvp          = yylvals + yypath->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yylpp          = yylpsns + yypath->lexeme;
#endif
    yylexp         = yylexemes + yypath->lexeme;
    yychar         = YYEMPTY;
    yystack.s_mark = yystack.s_base + (yypath->yystack.s_mark - yypath->yystack.s_base);
    memcpy (yystack.s_base, yypath->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
    yystack.l_mark = yystack.l_base + (yypath->yystack.l_mark - yypath->yystack.l_base);
    memcpy (yystack.l_base, yypath->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base + (yypath->yystack.p_mark - yypath->yystack.p_base);
    memcpy (yystack.p_base, yypath->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
    yystate        = yypath->state;
    goto yyloop;
#endif /* YYBTYACC */

yyoverflow:
    YYERROR_CALL("yacc stack overflow");
#if YYBTYACC
    goto yyabort_nomem;
yyenomem:
    YYERROR_CALL("memory exhausted");
yyabort_nomem:
#endif /* YYBTYACC */
    yyresult = 2;
    goto yyreturn;

yyabort:
    yyresult = 1;
    goto yyreturn;

yyaccept:
#if YYBTYACC
    if (yyps->save) goto yyvalid;
#endif /* YYBTYACC */
    yyresult = 0;

yyreturn:
#if defined(YYDESTRUCT_CALL)
    if (yychar != YYEOF && yychar != YYEMPTY)
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval, &yylloc);
#else
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */

    {
        YYSTYPE *pv;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYLTYPE *pp;

        for (pv = yystack.l_base, pp = yystack.p_base; pv <= yystack.l_mark; ++pv, ++pp)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv, pp);
#else
        for (pv = yystack.l_base; pv <= yystack.l_mark; ++pv)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
    }
#endif /* defined(YYDESTRUCT_CALL) */

#if YYBTYACC
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    while (yyps)
    {
        YYParseState *save = yyps;
        yyps = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
    while (yypath)
    {
        YYParseState *save = yypath;
        yypath = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
#endif /* YYBTYACC */
    yyfreestack(&yystack);
    return (yyresult);
}
