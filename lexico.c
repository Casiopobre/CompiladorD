#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"
#include "lexico.h"
#include "ts.h"


CompLexico* sigCompLexico() {
    return yylex();
}

// Funcion auxiliar que crea unha compoñente lexica a partires dun lexema e un id
CompLexico *crearCompLexico(char *lexema, int id) {
    // Reserva de memoria
    CompLexico *cl = (CompLexico*) malloc(sizeof(CompLexico));
    if (cl == NULL) return NULL;

    // Incluimos os valores que correspondan
    cl->id = id;

    // Para que cada entrada da TS teña a súa propia copia
    cl->lexema = strdup(lexema);

    return cl;
}
