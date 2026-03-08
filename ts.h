#ifndef TS_H
#define TS_H
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
void engadirEntradaTS(CompLexico *entrada);

/**
 * Busca unha compoñente lexica por lexema na taboa de simbolos
 */
CompLexico *buscarLexemaTS(char* lexema);

void imprimirTS();

int existeLexemaTS(char *lexema);

void liberarMemoriaTS();

#endif // TS_H