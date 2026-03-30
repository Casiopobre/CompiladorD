#include <stdio.h>
#include <stdlib.h>
#include "ts.h"
#include "lexico.h"
#include "definiciones.h"
#include "sintactico.h"
#include "entrada.h"

int main(int argc, char** argv){
    // Comprobar os parametros de entrada
    if (argc > 1) {
        abrirArquivo(argv[1]);
    }

    // Primeiro inicializamos a taboa de simbolos
    inicializarTS();

    // Imprimimos a taboa de simbolos no seu estado incial (so coas palabras clave)
    imprimirTS();

    

    // Bucle principal de sintáctico
    sintactico();

    // Cando rematamos, imprimimos a taboa de símbolos ao final
    imprimirTS();

    // Liberamos a memoria da taboa de símbolos (operacións de limpeza)
    liberarMemoriaTS();

    return 0;
}
