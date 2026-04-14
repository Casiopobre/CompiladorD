%{
#include <stdio.h>
#include <math.h> /* Para funcions matematicas */
#include "ts.h"
#include "cores.h"

int yylex();
void yyerror(char const *s);
%}

%union {
        double val; /* Para devolver numeros */
        char *sval; /* Para a string de load */
        CompLexico *tptr; /* Para devolver punteiros a taboa de simbolos */
}

/* Numero simple en doble precision */
%token <val> NUM 

/* Strings */
%token <sval> STRING

/* Variable e Funcion */
%token <tptr> VAR FNCT CALC LOAD CONST

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
line:   '\n'                        { printf("> "); } /* Ignorar unha liña en branco */
        | exp '\n'                  { printf(">> %.10g\n> ", $1); } /* Imprime a solución do cálculo */
        | exp ';' '\n'              { printf("> "); } /* Non imprime a solución ao levar o ; ao final */
        | LOAD '(' STRING ')' '\n'      { ($1->valor.loadptr)($3); free($3); }
        | LOAD '(' STRING ')' ';' '\n'      { ($1->valor.loadptr)($3); free($3); }
        | cmd '\n'                  { printf("> "); }
        | cmd ';' '\n'              { printf("> "); }
        | error '\n'                { limparVarsNonInicializadas(); yyerrok; printf("> "); }
;

cmd:    CALC '(' ')'       { ($1->valor.calcptr)(); }

exp:    NUM                 { $$ = $1; }   
        | VAR               {
                                if (!$1->inicializada) {
                                    fprintf(stderr, "Erro: variable '%s' non asignada\n", $1->lexema);
                                    YYERROR;
                                }
                                $$ = $1->valor.var; 
                            }
        | VAR '=' exp       {   
                                $$ = $3; 
                                $1->valor.var = $3;
                                $1->inicializada = 1;
                            }
        | CONST             { $$ = $1->valor.var; }

        | FNCT '(' exp ')'  { $$ = (*($1->valor.fnctptr))($3); } /* Chama a unha funcion pasandolle $3 como argumento */

        | exp '+' exp       { $$ = $1 + $3; }
        | exp '-' exp       { $$ = $1 - $3; }
        | exp '*' exp       { $$ = $1 * $3; }
        | exp '/' exp       {
                                if ($3 == 0) {
                                    if ($1 == 0)
                                        fprintf(stderr, "Erro: indeterminación (0/0)\n");
                                    else
                                        fprintf(stderr, "Erro: división por cero\n");
                                    YYERROR;
                                }
                                $$ = $1 / $3;
                            }
        | '-' exp %prec NEG { $$ = -$2; }
        | exp '^' exp       { $$ = pow ($1, $3); }
        | '(' exp ')'       { $$ = $2; }
;

%%

extern int yylineno;
extern char *_nome_ficheiro_actual;

void yyerror(char const *s) {
    if (_nome_ficheiro_actual) {
        fprintf(stderr, "Erro en '%s', liña %d: %s\n",
                _nome_ficheiro_actual, yylineno, s);
    } else {
        fprintf(stderr, "Erro: %s\n", s);
    }
}

