#ifndef TS_H
#define TS_H
#include <stdbool.h>
#include "lexico.h"
#include "abb.h"
#include "definiciones.h"

/**
 * Inicializa a taboa de simbolos, introducindo as keywords
 */
void inicializarTS();

/**
 * Inserta unha compoñente lexica an taboa de simbolos
 * @param compLexicoInsertar a compoñente léxica (struct) que se queira insertar na ts
 */
bool insertar(CompLexico compLexicoInsertar);

/**
 * Busca unha compoñente lexica por lexema na taboa de simbolos
 */
CompLexico *buscar(char* lexema);

void imprimirTS();

#endif // TS_H