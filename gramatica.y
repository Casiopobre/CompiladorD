%{
#include <stdio.h>
#include <math.h> /* Para funcions matematicas */
#include "ts.h"

int yylex();
void yyerror(char const *s);
%}

%union {
        double val; /* Para devolver numeros */
        CompLexico *tptr; /* Para devolver punteiros a taboa de simbolos */
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
/* Unha entrada é ou ben nada, ou ben unha ou mais liñas */
input:  /* baleiro */   { printf("> "); }
        | input line
;

/* Ou ben un salto de liña, ou unha expresión + salto de liña, ou erro + salto de liña */
line:   '\n'            { printf("> "); } /* Ignorar unha liña en branco */
        | exp '\n'      { printf(">> %.10g\n", $1); } /* Imprime a solución do cálculo */
        | error '\n'    { yyerrok; }
;

exp:    NUM                 { $$ = $1; }   
        | VAR               { $$ = $1->valor.var; }
        | VAR '=' exp       { $$ = $3; $1->valor.var = $3; }
        | FNCT '(' exp ')'  { $$ = (*($1->valor.fnctptr))($3); } /* Chama a unha funcion pasandolle $3 como argumento */
        | exp '+' exp       { $$ = $1 + $3; }
        | exp '-' exp       { $$ = $1 - $3; }
        | exp '*' exp       { $$ = $1 * $3; }
        | exp '/' exp       { $$ = $1 / $3; }
        | '-' exp %prec NEG { $$ = -$2; }
        | exp '^' exp       { $$ = pow ($1, $3); }
        | '(' exp ')'       { $$ = $2; }
;

%%



