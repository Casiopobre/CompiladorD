%{
#include <stdio.h>
#include <math.h> /* Para funcions matematicas */
#include "ts.h"

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

/* Ou ben un salto de liña eu unha expersión, load, comando ou erro seguindos dun salto de liña */
line:   '\n'                                { printf("> "); } /* Ignorar unha liña en branco */
        | exp '\n'                          { printf(">> %.10g\n> ", $1); } /* Imprime a solución do cálculo */
        | exp ';' '\n'                      { printf("> "); } /* Non imprime a solución ao levar o ; ao final */
        | LOAD '(' STRING ')' '\n'          { ($1->valor.loadptr)($3); free($3); }
        | LOAD '(' STRING ')' ';' '\n'      { ($1->valor.loadptr)($3); free($3); }
        | cmd '\n'                          { printf("> "); }
        | cmd ';' '\n'                      { printf("> "); }
        | error '\n'                        { limparVarsNonInicializadas(); yyerrok; printf("> "); }
;

/* Calquera comando da calculadora agas load (clear, exit...) */
cmd:    CALC '(' ')'       { ($1->valor.calcptr)(); }
;

/* Expresións de matematicas, de asignacion... */
exp:    NUM                 { $$ = $1; }   
        | VAR               {
                                /* Primeiro comprobamos se a variable está asignada; se non o está: erro */
                                if (!$1->inicializada) {
                                    fprintf(stderr, "Erro: variable '%s' non asignada\n", $1->lexema);
                                    YYERROR;
                                }
                                /* Se esta inicializada (ten un valor asignado), collemos o seu valor */
                                $$ = $1->valor.var; 
                            }
        | VAR '=' exp       {   
                                $$ = $3; 
                                /* Gardamos o valor da expresion na variable */
                                $1->valor.var = $3;
                                /* A marcamos como inicializada (xa que se lle asignou un valor correctamente) */
                                $1->inicializada = 1;
                            }

        | CONST             { $$ = $1->valor.var; }

        | FNCT '(' exp ')'  { $$ = (*($1->valor.fnctptr))($3); } /* Chama a unha funcion matematica pasandolle $3 como argumento */

        | exp '+' exp       { $$ = $1 + $3; }
        | exp '-' exp       { $$ = $1 - $3; }
        | exp '*' exp       { $$ = $1 * $3; }
        | exp '/' exp       {   /* Division entre 0 */
                                if ($3 == 0) {
                                    /* Se e 0/0 */
                                    if ($1 == 0)
                                        fprintf(stderr, "Erro: indeterminación (0/0)\n");
                                    /* Se e num/0 */
                                    else
                                        fprintf(stderr, "Erro: división por cero\n");
                                    YYERROR;
                                }
                                /* Se é unha division "valida" */
                                $$ = $1 / $3;
                            }

                            /* Menos unario */
        | '-' exp %prec NEG { $$ = -$2; }

        | exp '^' exp       { $$ = pow ($1, $3); }

        | '(' exp ')'       { $$ = $2; }
;

%%

extern int yylineno; /* Para levar a conta das liñas */
extern char *_nome_ficheiro_actual; /* Para poder indicar o ficheiro no que se produciu o erro */

/* Función de xestión de erros */
void yyerror(char const *s) {
    /* Se o erro se produciu nun arquivo cargado mediante load() */
    if (_nome_ficheiro_actual) {
        fprintf(stderr, "Erro en '%s', liña %d: %s\n", _nome_ficheiro_actual, yylineno, s);

    /* Se o erro se produce nunha expresion introducida polo usuario mediante a entrada estándar */
    } else {
        fprintf(stderr, "Erro: %s\n", s);
    }
}

