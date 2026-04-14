
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>        
#include "ts.h"
#include "gramatica.tab.h" 

// Nome do arquivo actual para as mensaxes de erro
char *_nome_ficheiro_actual = NULL;
// File descriptor para poder abrilo e pechalo
FILE *fd = NULL;
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
                        fclose(fd);
                        if (!YY_CURRENT_BUFFER) {
                            /* Non hai máis buffers*/
                            return 0;
                        } else {
                            /* Había un buffer anterior --> continuamos */
                            return yylex();
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
    // Abrimos o arquivo
    fd = fopen(nome, "r");
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
