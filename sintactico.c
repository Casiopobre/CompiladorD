#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"
#include "definiciones.h"

void sintactico(){
    CompLexico *compLexico;

    //compLexico = sigCompLexico();
    //printf("<%s, %d>\n", (*compLexico).lexema, (*compLexico).id);

    //? Cambiar por un do while(???
    while (1) {
        compLexico = sigCompLexico();
        if (compLexico != NULL ){
            if (compLexico->id != FIN_FICHEIRO) {
                printf("<%s, %d>\n", (*compLexico).lexema, (*compLexico).id);
            } else {
                break;
            }
        }
    }
    printf("Analise lexica rematada!\n");
}