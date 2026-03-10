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

/*TODO
* Comprobar lóxica ao inserir na ts (facer algo como que comprobe e insira)
* Revisar as funcións da ts
* CAndo chega ao bloque B, se ralla e nn da lido ben
*/

char *_analizarIdentificador();
char *_analizarStringLiteral();
char _analizarComentario();
CompLexico *_empaquetarCompLexico(char *lexema, int id);

CompLexico* sigCompLexico() {
    // Variables
    char *lexema;
    int c;
    CompLexico *compLexico;
    
    // Lemos o codigo fonte caracter a caracter
    while ((c = sig_char()) != EOF) {
        // Comprobamos se é un identificador 
        if (isalpha(c) || c == '_'){
            lexema = _analizarIdentificador();

            if (existeLexemaTS(lexema)) {
                compLexico = buscarLexemaTS(lexema);
            } else {
                compLexico = _empaquetarCompLexico(lexema, ID);
                engadirEntradaTS(compLexico);
            }
            break;

        //TODO Números
        } else if (isdigit(c) || c == '.'){
            compLexico = _analizarNum(c);
            break;

        } else {
            switch (c) {
            // Comprobamos se é un string literal
            case '"':
                lexema = _analizarStringLiteral();
                if (lexema == NULL) {
                    compLexico = _empaquetarCompLexico("END OF FILE", FIN_FICHEIRO);
                    break;
                }

                compLexico = _empaquetarCompLexico(lexema, STR_LITERAL);

                break;
            
            // En caso de que poida ser un coemntario
            case '/':
                char a;
                // Se non era un comentario senon unha / de división, devolvémola
                if ((a = _analizarComentario()) == '/') {
                    compLexico  =_empaquetarCompLexico("/", '/');
                }

                break;

            // Ignorar caarcteres
            case ' ': case '\n':
                obtener_lexema();

                break;

            // Por defecto para os tokens dun só caracter (., [, ], ...)
            default:
                lexema = obtener_lexema();
                compLexico = _empaquetarCompLexico(lexema, c);

                break;
            }
            break;
        }
    }

    if (c == EOF){
        compLexico = _empaquetarCompLexico("END OF FILE", FIN_FICHEIRO);
    }
    
    return compLexico;
}


CompLexico *_empaquetarCompLexico(char *lexema, int id) {
    // Reserva de memoria
    CompLexico *cl = (CompLexico*) malloc(sizeof(CompLexico));
    if (cl == NULL) return NULL;

    // Incluimos os valores que correspondan
    cl->id = id;
    cl->lexema = lexema;

    return cl;
}


char *_analizarIdentificador() {
    char c, *lexema;

    while (1){
        c = sig_char();
        // Seguimos no bucle de ler o lexema do identificador
        if (!(isalnum(c) || c == '_')) {
            // Devolvemos o delimitador antes de capturar o lexema
            devolver();
            lexema = obtener_lexema();
            break;
        }
    }
    
    return lexema;
}


// Analiza string literals da forma "loquesea"
char *_analizarStringLiteral() {
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
            break;
        } else if (c == EOF){
            return NULL;
        }
    }
    return lexema;
}

// Analise de números -------------------------------------------------------

// Analiza a parte decimal (despois do punto)
CompLexico *_analizarFloatLiteral() {
    int c;
    CompLexico *cl;
    


    return cl;
}

// Analizamos o que ven despois do 0B ou 0b
CompLexico *_analizarBinario() {
    char c, *lexema;
    CompLexico *cl;
    // Pode haber 0, 1 ou _
    while (1) {
        // Identificamos o numero binario
        if ((c = sig_char()) != '0' || c != '1' || c != '_') {
            // Como o automata recoñece en "outro", devolvemos o caracter
            devolver();
            lexema = obtener_lexema();
            cl = _empaquetarCompLexico(lexema, INT_LITERAL);
            break;
        }
    }
    return cl;
}

// Analiza numeros
CompLexico *_analizarNum(char inicio) {
    char c, *lexema;
    CompLexico *cl = NULL;

    if (isdigit(inicio)) {
        // Se comeza por 0
        if (inicio == '0') {
            c = sig_char();
            // Se é un numero binario
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
                        cl = _empaquetarCompLexico(lexema, INT_LITERAL);
                        break;
                    
                    // Se é un decimal (0_.)
                    } else if (c == '.') {
                        cl = _analizarFloatLiteral();
                        break;
                    }
                }
            }
        
        // Se comeza por 1-9
        } else {
            
        }

    // Se comeza directamente polo punto
    } else if (inicio == '.') {
        cl = _analizarFloatLiteral();
    }
    return cl;
}

// Analise de comentarios -------------------------------------------------------

void _analizarComentarioBloque() {
    int c;
    while ((c = sig_char()) != EOF) {
        if (c == '*') {
            // Se lemos o */ do final, o comentario rematou
            if ((c = sig_char()) == '/') return;
        }
    }
}

void _analizarComentarioLina() {
    int c;
    while ((c = sig_char()) != EOF) {
        if (c == '\n')
        break;
    }
}

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


char _analizarComentario() {
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

        // Caracter de división
        default:
        devolver();
        return '/';
    }

    return '\0';
}