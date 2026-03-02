#include <stdio.h>
#include <stdlib.h>
#include "ts.h"
#include "lexico.h"
#include "abb.h"
#include "definiciones.h"

TABB taboaSimbolos;


void inicializar(){
    CompLexico palabrasClave[] = 
    {
        {"import", IMPORT},
        {"double", DOUBLE},
        {"while", WHILE},
        {"foreach", FOREACH},
        {"return", RETURN},
        {"void", VOID},
        {"cast", CAST},
        {"int", INT}
        
    };

    crearAbb(&taboaSimbolos);

    for (int i = 0; i < sizeof(palabrasClave)/sizeof(CompLexico); i++) {
        insertarEntrada(&taboaSimbolos, palabrasClave[i]);
    }
}

CompLexico *buscar(char *lexema) {
    TIPOELEM *e = malloc(sizeof(TIPOELEM));
    TCLAVE clave = lexema;
    buscarNodo(taboaSimbolos, clave, e);

    return e;
}

int main() {
    inicializar();

    CompLexico *e = buscar("void");

    printf("<%s, %d>\n", e->lexema, e->id);

    return 0;
}
