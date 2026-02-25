#ifndef TS_H
#define TS_H
#include "lexico.h"

/**
 * Inicializa a taboa de simbolos
 */
void inicializar();

/**
 * Inserta unha compoñente lexica an taboa de simbolos
 */
bool insertar(CompLexico compLexicoInsertar);

/**
 * Busca unha compoñente lexica por lexema na taboa de simbolos
 */
CompLexico buscar(char* lexema);
