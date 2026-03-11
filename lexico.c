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
CompLexico *_analizarNum(char inicio);
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
                    lexema = obtener_lexema();
                    compLexico  =_empaquetarCompLexico(lexema, '/');
                }

                break;

            // Ignorar caarcteres
            case ' ': case '\n':
                obtener_lexema();
                return NULL;

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
                cl = _empaquetarCompLexico(lexema, FLOAT_LITERAL);
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
        cl = _empaquetarCompLexico(lexema, FLOAT_LITERAL);
        
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
                    cl = _empaquetarCompLexico(lexema, FLOAT_LITERAL);
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
            cl = _empaquetarCompLexico(lexema, INT_LITERAL);
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
                        cl = _empaquetarCompLexico(lexema, INT_LITERAL);
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
                cl = _empaquetarCompLexico(lexema, INT_LITERAL);
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
            cl = _empaquetarCompLexico(lexema, '.');
        }
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