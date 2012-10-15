%option yylineno

%{
	#include <stdlib.h>
        #include <string.h>
        #include "symtab/Entity.h"
        #include "parsetree/PTNodes.hh"
	#include "y.tab.h"
        extern void yyerror(const char * msg);
        extern int yylex(void);
        extern int yyparse(void);

%}

%%


"boolean" { return BOOLEAN; }
"char" { return CHAR; }
"class" { return CLASS; }
"continue" { return CONTINUE; }
"else" { return ELSE; }
"extends" { return EXTENDS; }
"false" { return FALSE; }
"if" { return IF; }
"input" { return INPUT; }
"int" { return INT; }
"new" { return NEW; }
"null" { return NIL; }
"output" { return OUTPUT; }
"return" { return RETURN; }
"string" { return STRING; }
"super" { return SUPER; }
"this" { return THIS; }
"true" {  return TRUE; }
"void" { return VOID; }
"while" { return WHILE; }


[a-zA-Z][a-zA-Z0-9]*                               { yylval.str_val = strdup(yytext); return ID; }

"'".?"'"                                   { yylval.char_val = yytext[1]; return CHAR_CONST; }
"'\\n'"                                    { yylval.char_val = '\n'; return CHAR_CONST; }
"'\\t'"                                    { yylval.char_val = '\t'; return CHAR_CONST; }
"'\\b'"                                    { yylval.char_val = '\b'; return CHAR_CONST; }
"'\\f'"                                    { yylval.char_val = '\f'; return CHAR_CONST; }
'[\\][\']'                                 { yylval.char_val = '\''; return CHAR_CONST; }
'[\\][\"]'                                 { yylval.char_val = '\"'; return CHAR_CONST; }

"\"".*"\""                                 { yylval.str_val = strdup(yytext); return STR_CONST; }

[0-9]+                                   { yylval.int_val = atoi(yytext); return INT_CONST; }

"==" { return EQ; }
"!=" { return NEQ; }
"&&" { return AND; }
"||" { return OR; }
">=" { return GEQ; }
"<=" { return LEQ; }

[-+!)(}{=&><,\;./*] { return *yytext; }

"\/\/".*\n ; /* skip comments */

[ \t\n]	; /* skip whitespace */


. { yyerror("illegal character"); }

%%

int yywrap() { return 1; }