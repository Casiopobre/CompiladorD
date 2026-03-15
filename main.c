#include <stdio.h>
#include <stdlib.h>
#include "ts.h"
#include "lexico.h"
#include "definiciones.h"
#include "sintactico.h"
#include "entrada.h"

int main(){

    // Primeiro inicializamos a taboa de simbolos
    inicializarTS();

    // Imprimimos a taboa de simbolos no seu estado incial (so coas palabras clave)
    imprimirTS();

    // Inicializamos o sistema de entrada
    iniciar_SE();

    // Bucle principal de sintáctico
    sintactico();

    // Cando rematamos, imprimimos a taboa de símbolos ao final
    imprimirTS();

    // Liberamos a memoria da taboa de símbolos (operacións de limpeza)
    liberarMemoriaTS();

    return 0;
}
