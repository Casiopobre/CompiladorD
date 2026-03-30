
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"
#include "cores.h"
#include "lexico.h"
#include "ts.h"

/* Cambiamos yylex para que devolva un punteiro a unha compoñentelexica*/
#define YY_DECL CompLexico *yylex()
%}

/* Para contar as liñas */
%option yylineno
/* Os comentarios anidados necesitan unha pila (cunha codicion de arranque) (hai que facelos na parte de regras)*/  
%option stack
%x NESTED_COMMENT

/*IDENTIFICADORES ------------------------------------*/
IDENTIFIER_START    [[:alpha:]_]
IDENTIFIER_CHARS    [[:alnum:]_]

IDENTIFIER          {IDENTIFIER_START}{IDENTIFIER_CHARS}*

/*Ignorase calquera caracter que vaia detras da \ (é como o tiña na practica 1)*/
SCAPE_SEQ           \\.       

/* STRING LITERALS ------------------------------------*/
/*Calquera cousa que non sexa un \n ou unha \ (se poño o punto, coincide co " de peche)*/
DOUBLE_QUOTED_CHAR  ([^\"\\]|{SCAPE_SEQ})

/*Para os string literals da forma "calquera cousa"*/
DOUBLE_QUOTED_STR   \"{DOUBLE_QUOTED_CHAR}*\"


/* COMENTARIOS ------------------------------------*/
BLOCK_COMMENT       "/*"([^*]|\*+[^*/])*\*+"/"

LINE_COMMENT        "//"[^\n]*

/* INTEGER LITERALS ------------------------------------*/
/* Decimais */
NON_ZERO_DIGIT      [1-9]

DECIMAL_DIGIT_US    [[:digit:]_]

DECIMAL_INTEGER     (0_*|{NON_ZERO_DIGIT}{DECIMAL_DIGIT_US}*)

/* Binarios */
BIN_PREFIX          (0b|0B)

BIN_DIGIT           (0|1)

BIN_DIGIT_US        ({BIN_DIGIT}|_)

BIN_INTEGER         ({BIN_PREFIX}{BIN_DIGIT_US}*{BIN_DIGIT}{BIN_DIGIT_US}*)


/* FLOAT LITERALS ------------------------------------*/
/* Para recoñecer a parte do expoñente (da forma E+43_322...) */
DECIMAL_EXPONENT    ([eE][+\-]?{DECIMAL_DIGIT_US}*[[:digit:]]{DECIMAL_DIGIT_US}*)

/* Decimal Digits No Starting UnderSccore (é deci, numeros decimais e _, pero sen comezar por _) */
DECIMAL_DIG_NST_US  ([[:digit:]]{DECIMAL_DIGIT_US}*)

/* O que hai antes do punto ou do exoñente */
LEADING_DECIMAL     ({DECIMAL_INTEGER}|0{DECIMAL_DIGIT_US}*[[:digit:]]{DECIMAL_DIGIT_US}*)

/* Numeros da forma numeros.numeros(opt) */
DEC_PTO_DEC         ({LEADING_DECIMAL}\.({DECIMAL_DIG_NST_US}*|{DECIMAL_DIG_NST_US}+{DECIMAL_EXPONENT}))

/* Numeros que comezan por punto (.numeros) */
PTO_DEC             (\.{DECIMAL_DIG_NST_US}+{DECIMAL_EXPONENT}?)

/* Numeros con expoñente sen punto (numerosEnumeros) */
DEC_EXP             ({LEADING_DECIMAL}{DECIMAL_EXPONENT})

DECIMAL_FLOAT       ({DEC_PTO_DEC}|{PTO_DEC}|{DEC_EXP})


%%


{DOUBLE_QUOTED_STR} {
                        CompLexico *cl = crearCompLexico(yytext, STR_LITERAL);
                        return cl;
                    }

{IDENTIFIER}        {
                        if (existeLexemaTS(yytext)) {
                            return buscarLexemaTS(yytext);
                        }

                        CompLexico *compLexicoTmp = crearCompLexico(yytext, ID);
                        engadirEntradaTS(compLexicoTmp);
                        free(compLexicoTmp);
                        return buscarLexemaTS(yytext);
                    }

{BLOCK_COMMENT}     {/* ignorar */}

{LINE_COMMENT}      {/* ignorar */}

    /* Cando atopamos o inicio dun comentario de bloque, pusheamos na pila (contador++) */
"/+"                { yy_push_state(NESTED_COMMENT); }
    /* Se dentro do comentario atopamos outro +/, contador++  (é decir, push na pila) */
<NESTED_COMMENT>"/+"    { yy_push_state(NESTED_COMMENT); }
    /* Por cada +/ que atopemos no cometnario, facemos un pop (contador--) */
<NESTED_COMMENT>"+/"    {yy_pop_state();}
<NESTED_COMMENT>\n      { /* ignorar saltos de liña */}
<NESTED_COMMENT>.       { /* ignorar caracteres */}
    /* Se nos atopamos un EOF e aínda non rematou o comentario, damos un erro */
<NESTED_COMMENT><<EOF>> {
                            printf(BOLD_RED"\nErro (liña %d): comentario anidado sen pechar :("RESET, yylineno);
                            CompLexico *cl = crearCompLexico("eof", FIN_FICHEIRO);
                            return cl;
                        }

{DECIMAL_FLOAT}     {
                        CompLexico *cl = crearCompLexico(yytext, FLOAT_LITERAL);
                        return cl;
                    }

{BIN_INTEGER}       {
                        CompLexico *cl = crearCompLexico(yytext, INT_LITERAL);
                        return cl;
                    }

{DECIMAL_INTEGER}   {
                        CompLexico *cl = crearCompLexico(yytext, INT_LITERAL);
                        return cl;
                    }


[ \t\n\r]+          { /* ignorar */ }

"=="                {  
                        CompLexico *cl = crearCompLexico(yytext, IGUAL_IGUAL);
                        return cl;
                    }

"+="                {  
                        CompLexico *cl = crearCompLexico(yytext, MAIS_IGUAL);
                        return cl;
                    }

"++"                {  
                        CompLexico *cl = crearCompLexico(yytext, MAIS_MAIS);
                        return cl;
                    }

.                   {  
                        CompLexico *cl = crearCompLexico(yytext, yytext[0]);
                        return cl;
                    }

<INITIAL><<EOF>>             {
                        CompLexico *cl = crearCompLexico("END OF FILE", FIN_FICHEIRO);
                        return cl;
                    }

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
