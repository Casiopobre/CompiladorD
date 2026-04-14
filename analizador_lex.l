
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>        
#include "ts.h"
#include "gramatica.tab.h" 

// Pila de ficheiros abertos
#define MAX_INCLUDE_DEPTH 16
FILE  *_file_stack[MAX_INCLUDE_DEPTH];
char  *_name_stack[MAX_INCLUDE_DEPTH];
int    _stack_top = 0;

// Nome do arquivo actual para as mensaxes de erro
char *_nome_ficheiro_actual = NULL;
%}

/* Para contar as liñas */
%option yylineno
/* Para empregar a pila de flex (para a función de carga de arquivos load()) */
%option stack

/* STRINGS (para load()) */
STRING  \"[^\"]*\"

/*IDENTIFICADORES ------------------------------------*/
LETRA    [[:alpha:]_]
ALNUM    [[:alnum:]_]

IDENTIFICADOR          {LETRA}{ALNUM}*      

/* NUMEROS ------------------------------------*/
ENTEIRO     [[:digit:]]+

EXPONENTE   ([eE][+\-]?{ENTEIRO})
FLOAT       ({ENTEIRO}\.{ENTEIRO}*{EXPONENTE}?|\.{ENTEIRO}{EXPONENTE}?|{ENTEIRO}{EXPONENTE})



%%


[ \t\r]+            { /* ignorar */ }

\n                  { return '\n'; }


{FLOAT}     |                                      
{ENTEIRO}           {
                        yylval.val = atof(yytext);
                        return NUM;
                    }                        


{STRING}            {   /* Quitamos as comillas e devolvemos a string */
                        yytext[yyleng-1] = '\0';
                        yylval.sval = strdup(yytext + 1);
                        return STRING;
                    }


{IDENTIFICADOR}     {   /* Se non esta na taboa de simbolos, engadimolo (o da variable temporal e por como esta feito o abb, pero non me da tempo de solucionalo :( )) */
                        if (!existeLexemaTS(yytext)) {
                            CompLexico *cl_temp = crearCompLexico(yytext, MYVAR, 0.0);
                            engadirEntradaTS(cl_temp);
                            free(cl_temp);
                        }

                        CompLexico *cl = buscarLexemaTS(yytext);
                        yylval.tptr = cl;

                        /* Asigamoslle o tipo pertinente */
                        switch (cl->tipo) {
                            case MYFNCT: return FNCT;
                            case MYCALC: return CALC;
                            case MYLOAD: return LOAD;
                            case MYCONST: return CONST;
                            default: return VAR;
                        }
                    }                        


<<EOF>>             {   /* Fai un pop do buffer de flex para sair do arquivo (se e que está en un) */
                        yypop_buffer_state();

                        if (_stack_top > 0) {
                            // Pechamos o ficheiro que acaba de rematar
                            fclose(_file_stack[--_stack_top]);
                            free(_name_stack[_stack_top]);

                            // Restauramos o nome do ficheiro anterior
                            _nome_ficheiro_actual = (_stack_top > 0) ? _name_stack[_stack_top - 1] : NULL;
                        }

                        if (!YY_CURRENT_BUFFER) {
                            return 0;
                        }
                    }


.                   { return yytext[0]; }

%%

// Mete stdin na pila para que ao sair dun arquivo, flex se atope ca entrada estandar e lle devolva o prompt ao usuario
void prepararEntradaEstandar() {
    // Forzamos a que stdin sexa a base da pila
    yypush_buffer_state(yy_create_buffer(stdin, YY_BUF_SIZE));
}

// Carga un ficheiro pusheando un novo buffer na pila.
void cargarFicheiro(char *nome) {

    if (_stack_top >= MAX_INCLUDE_DEPTH) {
        fprintf(stderr, "Erro: demasiados ficheiros anidados\n");
        return;
    }

    // Abrimos o arquivo
    FILE *f = fopen(nome, "r");
    if (f == NULL) {
        fprintf(stderr, "Erro: non se puido abrir o ficheiro '%s'\n", nome);
        return;
    }

    // Gardamos o ficheiro e o nome na pila paralela
    _file_stack[_stack_top] = f;
    _name_stack[_stack_top] = strdup(nome);
    _stack_top++;

    // Actualizamos o nome actual para yyerror
    _nome_ficheiro_actual = _name_stack[_stack_top - 1];

    // Pushea un novo buffer, gardando o actual (stdin) no stack
    yypush_buffer_state(yy_create_buffer(f, YY_BUF_SIZE));
    yylineno = 0; // Para resetear as liñas
}

int yywrap() {
    return 1;
}
