#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "lexico.h"

struct nodo {
    TIPOELEM entradaTS;
    struct nodo *izq, *der;
};

// Extrae a clave dun nodo (o lexema, neste caso)
TCLAVE _claveElem(TIPOELEM *E) {
    return E->lexema;
}

// Compara duas claves (orde alfabética)
int _compararClaves(TCLAVE c1, TCLAVE c2) {
    // Se son iguais
    if (strcmp(c1, c2) == 0) return 0;
    // Se a primeira é maior que a segunda
    else if (strcmp(c1, c2) > 0) return 1;
    // Se a segunda e maior que a primeira
    else return -1;
}

// Para liberar a memoria dun component lexico
void _eliminarEntrada(TIPOELEM *E) {
    // TODO
}

void crearAbb(TABB *A) {
    *A = NULL;
}

void eliminarAbb(TABB *A) {
    if (*A == NULL) return;

    eliminarAbb(&(*A)->izq);
    eliminarAbb(&(*A)->der);
    _eliminarEntrada(&((*A)->entradaTS));

    free(*A);
    *A = NULL;
}

unsigned esAbbVacio(TABB A) {
    return A == NULL;
}

int _compararClaveElem(TCLAVE c, TIPOELEM E) {
    return _compararClaves(c, _claveElem(&E));
}

void insertarEntrada(TABB *A, TIPOELEM E) {
    // Se a árbore está baleira, inseramos o elemento (reserva de memoria)
    if (esAbbVacio(*A)) {
        *A = (TABB) malloc(sizeof(struct nodo));
        (*A)->entradaTS = E;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return;
    }

    TCLAVE ce = _claveElem(&E);

    if (_compararClaveElem(ce, (*A)->entradaTS) > 0){
        insertarEntrada(&(*A)->der, E);
    } else {
        insertarEntrada(&(*A)->izq, E);
    }
}

void buscarNodo(TABB A, TCLAVE clave, TIPOELEM *nodo) {
    if (esAbbVacio(A)) return;

    int comp = _compararClaveElem(clave, A->entradaTS);
    if (comp == 0) {
        *nodo = A->entradaTS;
    } else if (comp < 0) {
        buscarNodo(A->izq, clave, nodo);
    } else {
        buscarNodo(A->der, clave, nodo);
    }
}

void leerElementoAbb(TABB A, TIPOELEM *E) {
    *E = A->entradaTS;
}

TABB izqAbb(TABB A) {
    return A->izq;
}

TABB derAbb(TABB A) {
    return A->der;
}

unsigned _es_miembro_clave(TABB A, TCLAVE cl) {
    if (esAbbVacio(A)) {
        return 0;
    }
    int comp = _compararClaveElem(cl, A->entradaTS);

    if (comp == 0) { //cl == A->info
        return 1;
    }
    if (comp > 0) { //cl > A->info
        return _es_miembro_clave(derAbb(A), cl);
    }
    //cl < A->info
    return _es_miembro_clave(izqAbb(A), cl);
}

//Funciones públicas

unsigned esMiembroAbb(TABB A, TIPOELEM E) {
    return _es_miembro_clave(A, _claveElem(&E));
}


