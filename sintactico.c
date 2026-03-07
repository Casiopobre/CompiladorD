#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"

void sintactico(){
    CompLexico *compLexico;

    while ((compLexico = sigCompLexico()) != NULL) {
        printf("<%s, %d>\n", (*compLexico).lexema, (*compLexico).id);
    }
}