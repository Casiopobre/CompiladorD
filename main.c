#include <stdio.h>
#include <stdlib.h>
#include "ts.h"
#include "lexico.h"
#include "definiciones.h"
#include "sintactico.h"
#include "entrada.h"

int main(){

    inicializarTS();

    imprimirTS();

    iniciar_SE();

    sintactico();

    return 0;
}
