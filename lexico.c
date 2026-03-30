#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "definiciones.h"
#include "lexico.h"
#include "entrada.h"
#include "ts.h"

// Declaración de funcións
char *_analizar_identificador();
char *_analizar_string_literal();
char _analizar_comentario();
CompLexico *_analizar_num(char inicio);
CompLexico *crearCompLexico(char *lexema, int id);


CompLexico* sigCompLexico() {
    return yylex();
}

// Funcion auxiliar que crea unha compoñente lexica a partires dun lexema e un id
CompLexico *crearCompLexico(char *lexema, int id) {
    // Reserva de memoria
    CompLexico *cl = (CompLexico*) malloc(sizeof(CompLexico));
    if (cl == NULL) return NULL;

    // Incluimos os valores que correspondan
    cl->id = id;

    // Para que cada entrada da TS teña a súa propia copia
    cl->lexema = strdup(lexema);

    return cl;
}


// Analiza string literals da forma "loquesea"
char *_analizar_string_literal() {
    int c;
    char *lexema;
    while (1) {
        c = sig_char();
        // Se é unha secuencia de escape
        if (c == '\\') { 
            c = sig_char(); // O caracter escapado
            if (c == EOF) return NULL;
            c = sig_char();
        } else if (c == '"') {
            lexema = obtener_lexema();
            if (lexema == NULL) lexema = "ERROR";
            break;
        } else if (c == EOF){
            return NULL;
        }
    }
    return lexema;
}

// Analise de números -------------------------------------------------------

// Analiza a parte de expoñente dun numero (despois do E ou e) (son floats)
CompLexico *_analizarExponente() {
    char c, *lexema;
    CompLexico *cl;

    c = sig_char();
    // Despois do E ou e poden aparecer +, -, _ ou 0-9
    if (c == '+' || c == '-' || c == '_' || isdigit(c)) {
        // Agora so poden aparecer 0-9 ou _
        while (1) {
            c = sig_char();
            if (!isdigit(c) && c != '_') {
                devolver();
                lexema = obtener_lexema();
                cl = crearCompLexico(lexema, FLOAT_LITERAL);
                break;
            }
        }
    }
    return cl;
}


// Analiza a parte decimal (despois do punto)
CompLexico *_analizarFloatLiteral() {
    char c, *lexema;
    CompLexico *cl;
    c = sig_char();
    
    // Se non hai dixitos despois do punto (float tipo 123.)
    if (!isdigit(c)) {
        devolver();
        lexema = obtener_lexema();
        cl = crearCompLexico(lexema, FLOAT_LITERAL);
        
        // Se despois do punto hai polo menos un díxito, seguimos analizando
    } else {
        while (1) {
            
            // Cando lemos algo distinto de 0-9 ou _ comprobamos se é un expoñente
            if (!isdigit(c) && c != '_') {
                if (c == 'E' || c == 'e') {
                    cl = _analizarExponente();
                    break;

                    // Se rematou o float
                } else {
                    devolver();
                    lexema = obtener_lexema();
                    cl = crearCompLexico(lexema, FLOAT_LITERAL);
                    break;
                }
            }
            c = sig_char();
        }
    }
    
    
    return cl;
}

// Analiza secuencias con 0-9 e _ (DecimalDigitsUS na documentación de D)
CompLexico *_analizarIntegerLiteral() {
    char c, *lexema;
    CompLexico *cl;
    
    while (1) {
        c = sig_char();

        while(isdigit(c) || c == '_') {
            c = sig_char();
        }

        // Cando se lea un caracter distinto de 0-9 ou _
        // Se é un float (123.)
        if (c == '.') {
            cl = _analizarFloatLiteral();
            break;
        }
        
        // Se é un float con expoñente (123e)
        if (c == 'e' || c == 'E') {
            cl = _analizarExponente();
            break;
        }
        
        // Se é un número enteiro
        else {
            devolver();
            lexema = obtener_lexema();
            cl = crearCompLexico(lexema, INT_LITERAL);
        }
        break;

    }

    return cl;
}

// Analizamos o que ven despois do 0B ou 0b
CompLexico *_analizarBinario() {
    char c, *lexema;
    CompLexico *cl;
    // Pode haber 0, 1 ou _
    while (1) {
        c = sig_char();
        // Identificamos o numero binario
        if (c != '0' && c != '1' && c != '_') {
            // Como o automata recoñece en "outro", devolvemos o caracter
            devolver();
            lexema = obtener_lexema();
            cl = crearCompLexico(lexema, INT_LITERAL);
            break;
        }
    }
    return cl;
}

// Analiza numeros en xeral
CompLexico *_analizar_num(char inicio) {
    char c, *lexema;
    CompLexico *cl = NULL;

    if (isdigit(inicio)) {
        // Se comeza por 0
        if (inicio == '0') {
            c = sig_char();
            // Se é un numero binario (0b ou 0B)
            if (c == 'b' || c == 'B') {
                cl = _analizarBinario();

            // Se o numero comeza por '0.'
            } else if (c == '.') {
                cl = _analizarFloatLiteral();

            // Se o número é do tipo (0_)
            } else if(c == '_') {
                while (1) {
                    c = sig_char();
                    if (c != '_') {
                        devolver();
                        lexema = obtener_lexema();
                        cl = crearCompLexico(lexema, INT_LITERAL);
                        break;
                    
                    // Se é un decimal (0_.)
                    } else if (c == '.') {
                        cl = _analizarFloatLiteral();
                        break;
                    }
                }
            // Se é un 0 só
            } else {
                devolver();
                lexema = obtener_lexema();
                cl = crearCompLexico(lexema, INT_LITERAL);
            }
        
        // Se comeza por 1-9
        } else {
            cl = _analizarIntegerLiteral();
        }

    // Se comeza directamente polo punto
    } else if (inicio == '.') {
        c = sig_char();

        // Se é un float
        if (isdigit(c)) {
            devolver();
            cl = _analizarFloatLiteral();
            
        // Se se trata dun só punto
        } else {
            devolver();
            lexema = obtener_lexema();
            cl = crearCompLexico(lexema, '.');
        }
    }
    return cl;
}


// Analise de comentarios -------------------------------------------------------

// Automata de comentarios de bloque
void _analizarComentarioBloque() {
    int c;
    while ((c = sig_char()) != EOF) {
        if (c == '*') {
            // Se lemos o */ do final, o comentario rematou
            if ((c = sig_char()) == '/') return;
        }
    }
}

// Automata de comentarios de liña
void _analizarComentarioLina() {
    int c;
    while ((c = sig_char()) != EOF) {
        if (c == '\n') {
            break;
        }
    }
}

// Automata de comentarios anidados
void _analizarComentarioAnidado() {
    int c;
    // Contamos os '/+' que hai (os metemos nunha pila)
    int pila = 1;

    while ((c = sig_char()) != EOF) {
        // Empezamos un comentario anidade
        if (c == '/') {
            if ((c = sig_char()) == '+') pila++;
        }

        // Pechamos un comentario anidado
        if (c == '+') {
            if ((c = sig_char()) == '/') pila--;
        }

        // Se a pila está baleira, enton rematou o comentario
        if (pila == 0) break;
    }
    
}

// Autómata xeral de comentarios
char _analizar_comentario() {

    // Os comentarios son ignoraods polo compialdor
    ignorarEntrada(1);

    int c = sig_char();
    switch (c) {

        case '*':
        _analizarComentarioBloque();
        break;

        case '/':
        _analizarComentarioLina();
        break;

        case '+':
        _analizarComentarioAnidado();
        break;

        // Se so era un caracter de división e non un comentario
        default:
        ignorarEntrada(0);
        devolver();
        return '/';
    }

    // Significa que limos un comentario
    return '\0';
}