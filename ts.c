#include <stdio.h>
#include <stdlib.h>
#include "ts.h"
#include "lexico.h"
#include "abb.h"
#include "definiciones.h"

TABB taboaSimbolos;


void inicializarTS(){
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

// Recorre a árbore de esquerda a dereita (inorde) imprimindo o contido dos nodos
void _recorrer_TS(TABB A){
    if (esAbbVacio(A)) return;

    TIPOELEM nodo;

    leerElementoAbb(A, &nodo);

    _recorrer_TS(izqAbb(A));

    printf("<%s, %d>\n", nodo.lexema, nodo.id);
    
    _recorrer_TS(derAbb(A));
}


void imprimirTS() {
    printf("~*~*~*~*~*~*~ Taboa de símbolos ~*~*~*~*~*~*~\n");

    _recorrer_TS(taboaSimbolos);
}
