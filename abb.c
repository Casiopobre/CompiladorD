#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"

struct nodo {
    TIPOELEM entradaTS;
    struct nodo *izq, *der;
};

// Extrae a clave dun nodo (o lexema, neste caso)
TCLAVE _claveElem(TIPOELEM *elem) {
    return elem->lexema;
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

// Para liberar a memoria dun componente lexico
void _eliminarEntrada(TIPOELEM *elem) {
    if (elem == NULL || elem->lexema == NULL) return;
    if (elem->tipo == MYVAR) free(elem->lexema);
    elem->lexema = NULL;
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

int _compararClaveElem(TCLAVE c, TIPOELEM elem) {
    return _compararClaves(c, _claveElem(&elem));
}

void insertarEntrada(TABB *A, TIPOELEM elem) {
    // Se a árbore está baleira, inseramos o elemento (reserva de memoria)
    if (esAbbVacio(*A)) {
        *A = (TABB) malloc(sizeof(struct nodo));
        (*A)->entradaTS = elem;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return;
    }

    TCLAVE ce = _claveElem(&elem);

    if (_compararClaveElem(ce, (*A)->entradaTS) > 0){
        insertarEntrada(&(*A)->der, elem);
    } else {
        insertarEntrada(&(*A)->izq, elem);
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

// Busca un nodo y devuelve un puntero directo al elemento (sin copiar)
TIPOELEM* buscarNodoPtr(TABB A, TCLAVE clave) {
    if (esAbbVacio(A)) return NULL;

    int comp = _compararClaveElem(clave, A->entradaTS);
    if (comp == 0) {
        return &(A->entradaTS);
    } else if (comp < 0) {
        return buscarNodoPtr(A->izq, clave);
    } else {
        return buscarNodoPtr(A->der, clave);
    }
}

void leerElementoAbb(TABB A, TIPOELEM *elem) {
    *elem = A->entradaTS;
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

unsigned esMiembroAbb(TABB A, TIPOELEM elem) {
    return _es_miembro_clave(A, _claveElem(&elem));
}

// Devuelve el nodo con la clave mínima (el más a la izquierda)
TABB _minimoAbb(TABB A) {
    if (esAbbVacio(A->izq)) return A;
    return _minimoAbb(A->izq);
}


void eliminarNodo(TABB *A, TCLAVE clave) {
    if (esAbbVacio(*A)) return;

    int comp = _compararClaves(clave, _claveElem(&(*A)->entradaTS));

    if (comp < 0) {
        // Buscar na subarbore esquerda
        eliminarNodo(&(*A)->izq, clave);
    } else if (comp > 0) {
        // Buscar na subarbore dereita
        eliminarNodo(&(*A)->der, clave);
    } else {
        // Nodo atopaado
        TABB tmp;

        if (esAbbVacio((*A)->izq) && esAbbVacio((*A)->der)) {
            // Nodo sen fillos
            _eliminarEntrada(&(*A)->entradaTS);
            free(*A);
            *A = NULL;

        } else if (esAbbVacio((*A)->izq)) {
            // Nodo con fillo dereito
            tmp = *A;
            *A = (*A)->der;
            _eliminarEntrada(&tmp->entradaTS);
            free(tmp);

        } else if (esAbbVacio((*A)->der)) {
            // Nodo con fillo esquerdo
            tmp = *A;
            *A = (*A)->izq;
            _eliminarEntrada(&tmp->entradaTS);
            free(tmp);

        } else {
            // Nodo con dous fillos
            TABB sucesor = _minimoAbb((*A)->der);
            // Copiamos o contido do sucesor ao no actual
            _eliminarEntrada(&(*A)->entradaTS);
            (*A)->entradaTS = sucesor->entradaTS;
            sucesor->entradaTS.lexema = NULL;
            eliminarNodo(&(*A)->der, _claveElem(&(*A)->entradaTS));
        }
    }
}

