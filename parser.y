%{
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

#include "node.h"

extern int yylineno;

extern "C"
void yyerror(const char* s) {
  std::printf("Parser error: %s at line %d\n", s, yylineno);
  std::exit(1);
}

extern int yylex();

NBlock* programBlock;

%}

%define parse.error verbose

%union {
  NBlock* block;
  NExpression* expr;
  NStatement* stmt;
  NIdentifier* ident;
  NVarDef* vardef;
  VarDefList* varlist;
  std::string* string;
  int token;
}

%token <string> TIDENTIFIER TINTEGER TDOUBLE
%token <token> TRETURN TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TCOLON TSEMICOLON TASSIGN TRARROW

%type <block> program stmts block
%type <expr> expr
%type <stmt> stmt func_def
%type <ident> ident
%type <vardef> var_def
%type <varlist> func_args

%start program

%%

program : stmts { programBlock = $1; }
        ;

ident : TIDENTIFIER { $$ = new NIdentifier(*$1); delete $1; }
      ;

stmts : stmt { $$ = new NBlock(); $$->stmts.push_back($<stmt>1); }
      | stmts stmt { $1->stmts.push_back($<stmt>2); }
      ;

stmt : func_def { }
     | TRETURN expr TSEMICOLON { $$ = new NReturnStmt($2); }
     | var_def TSEMICOLON { }
     ;

expr : TINTEGER { $$ = new NInteger(*$1); delete $1; }
     | ident { }
     ;

var_def : ident TCOLON ident { $$ = new NVarDef($1, $3); }
        | ident TASSIGN expr { $$ = new NVarDef($1, $3); }
        | ident TCOLON ident TASSIGN expr { $$ = new NVarDef($1, $3, $5); }
        ;

func_def : ident TCOLON TLPAREN TRPAREN block { $$ = new NFuncDef($1, $5); }
         | ident TCOLON TLPAREN TRPAREN TRARROW ident block { $$ = new NFuncDef($1, $6, $7); }
         | ident TCOLON TLPAREN func_args TRPAREN block { $$ = new NFuncDef($1, $4, $6); }
         | ident TCOLON TLPAREN func_args TRPAREN TRARROW ident block { $$ = new NFuncDef($1, $7, $4, $8); }
         ;

func_args : var_def { $$ = new VarDefList; $$->push_back($1); }
          | func_args TCOMMA var_def { $1->push_back($3); }
          ;

block : TLBRACE stmts TRBRACE { $$ = $2; }
      | TLBRACE TRBRACE { $$ = new NBlock(); }
      ;


