#include <stdio.h>
#include <stdlib.h>
#include "ts.h"
#include "lexico.h"
#include "abb.h"
#include "definiciones.h"
#include "cores.h"

// Variable global para a taboa de simbolos
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

CompLexico *buscarLexemaTS(char *lexema) {
    TIPOELEM *e = malloc(sizeof(TIPOELEM));
    TCLAVE clave = lexema;
    buscarNodo(taboaSimbolos, clave, e);

    return e;
}

int existeLexemaTS(char *lexema) {
    TIPOELEM *e = malloc(sizeof(TIPOELEM));
    e->lexema = lexema;

    unsigned comprobacion = esMiembroAbb(taboaSimbolos, *e);
    free(e);
    return comprobacion;
}

void engadirEntradaTS(CompLexico *entrada){
    insertarEntrada(&taboaSimbolos, *entrada);
}

void liberarMemoriaTS() {
    eliminarAbb(&taboaSimbolos);
    printf(BRIGHT_GREEN"Memoria da taboa de símbolos eliminada correctamente :)\n"RESET);
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
    printf(BRIGHT_BLUE"~*~*~*~*~ Taboa de símbolos ~*~*~*~*~\n");
    _recorrer_TS(taboaSimbolos);
    printf("~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~\n"RESET);
}
