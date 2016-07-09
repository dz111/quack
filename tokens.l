%{
#include <iostream>
#include <string>
#include "node.h"
#include "parser.hpp"

#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng);
#define TOKEN(t) (yylval.token = t)

//extern "C" int yywrap() { }


extern FILE* fin;
extern unsigned long curr_lineno;
#undef YY_INPUT
#define YY_INPUT(buf, result, max_size) \
      if ((result = fread((char*)buf, sizeof(char), max_size, fin)) < 0) \
             YY_FATAL_ERROR("read() in flex scanner failed");

%}

%option noyywrap
%option yylineno

%%

[ \t]                   ;
[\n]                    ++curr_lineno;
"return"                return TOKEN(TRETURN);
[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return TIDENTIFIER;
[0-9]+\.[0-9]*          SAVE_TOKEN; return TDOUBLE;
[0-9]+                  SAVE_TOKEN; return TINTEGER;
"{"                     return TOKEN(TLBRACE);
"}"                     return TOKEN(TRBRACE);
"("                     return TOKEN(TLPAREN);
")"                     return TOKEN(TRPAREN);
","                     return TOKEN(TCOMMA);
":"                     return TOKEN(TCOLON);
";"                     return TOKEN(TSEMICOLON);
"->"                    return TOKEN(TRARROW);
.                       printf("unexpected symbol!\n"); yyterminate();

%%

