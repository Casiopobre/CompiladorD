#include <stdio.h>
#include <stdlib.h>
#include "ts.h"
#include "lexico.h"
#include "definiciones.h"
#include "sintactico.h"

extern void yylex_destroy(void);

int main(int argc, char** argv){

    // Comprobar os parametros de entrada e abrir o arquivo que se pase por liña de comandos
    // Nota: se non se pasa arquivo, emprégase a entrada estándar
    if (argc > 1) {
        abrirArquivo(argv[1]);
    }

    // Primeiro inicializamos a taboa de simbolos
    inicializarTS();

    // Imprimimos a taboa de simbolos no seu estado incial (so coas palabras clave)
    imprimirTS();

    // Bucle principal de sintáctico
    sintactico();

    // Pechamos o arquivo de entrada
    pecharArquivo();

    // Liberamos a memoria empregada por flex
    yylex_destroy();

    // Cando rematamos, imprimimos a taboa de símbolos ao final
    imprimirTS();

    // Liberamos a memoria da taboa de símbolos (operacións de limpeza)
    liberarMemoriaTS();

    return 0;
}
