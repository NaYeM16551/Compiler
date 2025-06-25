#ifndef _yy_defines_h_
#define _yy_defines_h_

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
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE {
	ParseTreeNode* parseTreeNode;
	
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#endif /* _yy_defines_h_ */
