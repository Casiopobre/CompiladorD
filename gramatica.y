%{
#include <math.h> /* Para funcions matematicas */

%}

%union {
double val; /* Para devolver numeros */
symrec *tptr; /* Para devolver punteiros a taboa ded simbolos */
}

/* Numero simple en doble precision */
%token <val> NUM 

/* Variable e Funcion */
%token <tptr> VAR FNCT 

/* Expresion */
%type <val> exp

/* Operadores */
%right '='
%left '+' '-'
%left '*' '/'
/* Negacion--menos unario */
%left NEG 
/* Exponenciacion */
%right '^' 


%%
/* REGRAS GRAMATICAIS ---------------- */

input:  /* baleiro */
        | input line
;

line:   '\n'
        | exp '\n'      { printf("\t%.10g\n", $1); }
        | error '\n'    { yyerrok }
;

exp:    NUM                 { $$ = $1; }   
        | VAR               { $$ = $1->value.var; }
        | VAR '=' exp       { $$ = $3; $1->value.var = $3; }
        | FNCT '(' exp ')'  { $$ = (*($1->value.fnctptr))($3); } /* Chama a unha funcion pasandolle $3 como argumento */
        | exp '+' exp       { $$ = $1 + $3; }
        | exp '-' exp       { $$ = $1 - $3; }
        | exp '*' exp       { $$ = $1 * $3; }
        | exp '/' exp       { $$ = $1 / $3; }
        | '-' exp %prec NEG { $$ = -$2; }
        | exp '^' exp       { $$ = pow ($1, $3); }
        | '(' exp ')'       { $$ = $2 }
;

%%



