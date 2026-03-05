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



CompLexico* sigCompLexico() {
    // Variables
    char c, *lexema;

    // Inicializamos a estrutura
    CompLexico *compLexico = (CompLexico*) malloc(sizeof(CompLexico));
    if (compLexico == NULL) return NULL;
    compLexico->id = 0;
    compLexico->lexema = "";

    // Inicializamos o sistema de entrada
    iniciar_SE();
    
    // Lemos o codigo fonte caracter a caracter
    while ((c = sig_char()) != EOF) {
        printf("%c\n", c);
        // Comprobamos se é un identificador
        if (isalpha(c) || c == '_'){
            lexema = _analizarIdentificador();
            //printf("%s\n", lexema);
            //! buscarLexema(*ts): se xa está, devolver o identificador; se non, engadilo
            break;
        // Comprobamos se é un dixito
        } /*else if (isdigit) {
            //TODO REMATAR !!!!!!!!!!!!!!!!!
        }
        */
    }
    
    return compLexico;
}

int main() {
    sigCompLexico();
}



char *_analizarIdentificador() {
    char c, *lexema;
    // Len = 2 para o char que xa se leu, e para o \0 do final do string
    int len = 2;

    while (1){
        c = sig_char();
        // Seguimos no bucle de ler o lexema do identificador
        if (isalnum(c) || c == '_') {
            len++;
        // Cando lemos len++;"outro", rematamos de ler o lexema do identificador (hai que recuperar despois o char)
        } else {
            lexema =  obtener_lexema();
            break;
        }
    }
    
    return lexema;
}
