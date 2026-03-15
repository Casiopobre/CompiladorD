#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"
#include "definiciones.h"
#include "cores.h"

void sintactico(){
    CompLexico *compLexico;

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
    printf(BRIGHT_GREEN"Analise lexica rematada!\n"RESET);
}