
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>        
#include "ts.h"
#include "gramatica.tab.h" 
%}

/* Para contar as liñas */
%option yylineno

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
{ENTEIRO}   {
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


<<EOF>>             { return 0; }

.                   { return yytext[0]; }

%%


/*Función para que flex empregue un arquivo como entrada (en vez de a entrada estandar)*/
void abrirArquivo(char *arquivo) {
    FILE *fd = fopen(arquivo, "r");
    if (fd == NULL) {
        perror("Erro ao abrir o arquivo\n");
        return;
    }
    yyin = fd;
    
    return;
}

/* Función para pechar o arquivo de entrada */
void pecharArquivo() {
    if (yyin != NULL) {
        fclose(yyin);
    }
}

int yywrap() { return 1; }
