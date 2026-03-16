#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"
#include "definiciones.h"
#include "cores.h"

void sintactico(){
    CompLexico *compLexico;

    while (1) {
        // pedimoslle compoñentes léxicos ao analizador lexico
        compLexico = sigCompLexico();
        if (compLexico != NULL ){
            printf("<%s, %d>\n", (*compLexico).lexema, (*compLexico).id);
            
            // Se nos atopams co fin de ficheiro
            if (compLexico->id == FIN_FICHEIRO) {
                free(compLexico->lexema);
                free(compLexico);
                break;
            }
            
            // Liberar memoria agora, agás para as palabras reservadas (300-307) e os identificadores, que se liberan despois
            if (compLexico->id < 300 || compLexico->id > 308) {
                free(compLexico->lexema);
                free(compLexico);
            }

        }
    }

    // Mensasxe para indicar que rematou a analise lexica
    printf(BRIGHT_GREEN"\nAnalise lexica rematada! :)\n\n"RESET);
}