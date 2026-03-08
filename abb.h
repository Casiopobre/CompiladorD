#ifndef ABB_H
#define ABB_H
#include "lexico.h"

// Tipo de dato da clave a empregar
typedef char *TCLAVE;

// Tipo de elemento que dse quere gardar en cada nodo
typedef CompLexico TIPOELEM;

// Tipo para o nodo
typedef struct nodo *TABB;

/**
 * Crea a arbore baleira
 * @param A Punteiro á árbore.
 */
void crearAbb(TABB *A);

/**
 * Elimina a arbore
 * @param A A árbore que se quere eliminar.
 */
void eliminarAbb(TABB *A);

/**
 * Devolve 1 se a arbore está baleira, e 0 en outro caso.
 * @param A A árbore binaria
 */
unsigned esAbbVacio(TABB A);

/**
 * Inserta un novo nodo na árbore para o elemento E
 * do que toma a súa clave.
 * @param A Árbore binaria.
 * @param E Informacion do novo nodo.
 */
void insertarEntrada(TABB *A, TIPOELEM E);

/**
 * Busca un nodo con clave <cl> en el arbol y, si existe
 * modifica el puntero <nodo> para que lo apunte.
 * @param A Arbol binario en el que se busca el nodo.
 * @param cl Clave que se buscara.
 * @param nodo Puntero a un tipoelem.
 */
void buscarNodo(TABB A, TCLAVE clave, TIPOELEM *nodo);

void leerElementoAbb(TABB A, TIPOELEM *E);

TABB izqAbb(TABB A);

TABB derAbb(TABB A);

unsigned esMiembroAbb(TABB A, TIPOELEM E);

void eliminarAbb(TABB *A);

#endif //ABB_H