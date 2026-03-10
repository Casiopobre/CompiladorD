#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"

void sintactico(){
    CompLexico *compLexico;

    //compLexico = sigCompLexico();
    //printf("<%s, %d>\n", (*compLexico).lexema, (*compLexico).id);

    //? Cambiar por un do while(???
    while ((compLexico = sigCompLexico()) != NULL) {
        printf("<%s, %d>\n", (*compLexico).lexema, (*compLexico).id);
    }
}