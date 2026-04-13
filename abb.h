#ifndef ABB_H
#define ABB_H
#include "ts.h"

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
void insertarEntrada(TABB *A, TIPOELEM elem);

/**
 * Busca un nodo con clave <cl> en el arbol y, si existe
 * modifica el puntero <nodo> para que lo apunte.
 * @param A Arbol binario en el que se busca el nodo.
 * @param cl Clave que se buscara.
 * @param nodo Puntero a un tipoelem.
 */
void buscarNodo(TABB A, TCLAVE clave, TIPOELEM *nodo);

/**
 * Busca un nodo con clave <cl> en el arbol y devuelve un puntero
 * directamente al elemento encontrado (sin copiar).
 * @param A Arbol binario en el que se busca el nodo.
 * @param cl Clave que se buscara.
 * @return Puntero al elemento encontrado, o NULL si no existe.
 */
TIPOELEM* buscarNodoPtr(TABB A, TCLAVE clave);

/**
 * Lee o elemento almacenado na raíz da árbore.
 * @param A Árbore binaria.
 * @param E Punteiro onde se almacenará o elemento lido.
 */
void leerElementoAbb(TABB A, TIPOELEM *elem);

/**
 * Devolve a subárbore esquerda da árbore A.
 * @param A Árbore binaria.
 * @return A subárbore esquerda.
 */
TABB izqAbb(TABB A);

/**
 * Devolve a subárbore dereita da árbore A.
 * @param A Árbore binaria.
 * @return A subárbore dereita.
 */
TABB derAbb(TABB A);

/**
 * Comproba se un elemento forma parte da árbore.
 * @param A Árbore binaria.
 * @param E Elemento a buscar.
 * @return 1 se o elemento está na árbore, 0 en outro caso.
 */
unsigned esMiembroAbb(TABB A, TIPOELEM elem);

/**
 * Elimina completamente a árbore, liberando toda a memoria.
 * @param A Punteiro á árbore que se quere eliminar.
 */
void eliminarAbb(TABB *A);
/**
 * Elimina un nó da arbore
 * @param A Punteiro á árbore na que se quere eliminar un nó
 * @param clave Clave do nó a eliminar
 */
void eliminarNodo(TABB *A, TCLAVE clave);

#endif //ABB_H