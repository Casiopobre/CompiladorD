
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>        
#include "ts.h"
#include "gramatica.tab.h" 

// Nome do arquivo actual para as mensaxes de erro
char *_nome_ficheiro_actual = NULL;
%}

/* Para contar as liñas */
%option yylineno
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


{IDENTIFICADOR}     {
                        if (!existeLexemaTS(yytext)) {
                            CompLexico *clt = crearCompLexico(yytext, MYVAR, 0.0);
                            engadirEntradaTS(clt);
                            free(clt);
                        }

                        CompLexico *cl = buscarLexemaTS(yytext);
                        yylval.tptr = cl;

                        switch (cl->tipo) {
                            case MYFNCT: return FNCT;
                            case MYCALC: return CALC;
                            case MYLOAD: return LOAD;
                            case MYCONST: return CONST;
                            default: return VAR;
                        }
                    }                        


<<EOF>>             {
                        yypop_buffer_state();
                        if (!YY_CURRENT_BUFFER) {
                            /* Non hai máis buffers*/
                            return 0;
                        } else {
                            /* Había un buffer anterior (stdin): continuamos */
                            return yylex();
                        }
                    }


.                   { return yytext[0]; }

%%

void prepararEntradaEstandar() {
    // Forzamos a que stdin sea la base del stack
    yypush_buffer_state(yy_create_buffer(stdin, YY_BUF_SIZE));
}

// Carga un ficheiro pusheando un novo buffer no stack.
void cargarFicheiro(char *nome) {
    FILE *fd = fopen(nome, "r");
    if (fd == NULL) {
        fprintf(stderr, "Erro: non se puido abrir o ficheiro '%s'\n", nome);
        return;
    }
    // Pushea un novo buffer, gardando o actual (stdin) no stack
    yypush_buffer_state(yy_create_buffer(fd, YY_BUF_SIZE));
    yylineno = 1; // Para resetear as liñas
}

int yywrap() {
    return 1;
}
