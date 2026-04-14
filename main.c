#include <stdio.h>
#include <stdlib.h>
#include "ts.h"
#include "funcions_calc.h"

extern void yylex_destroy(void);

extern int yyparse();

int main(int argc, char** argv){

    // Primeiro inicializamos a taboa de simbolos
    inicializarTS();

    // Metemos o stdin no buffer de flex
    prepararEntradaEstandar();

    // Comprobar os parametros de entrada e abrir o arquivo que se pase por liña de comandos
    // Nota: se non se pasa arquivo, emprégase a entrada estándar
    if (argc > 1) {
        printf("Cargando arquivo %s ...\n", argv[1]);
        cargarFicheiro(argv[1]);
    }

    printf("Para ver o menu de axuda escribir o comando HELP() :)\n");
    
    // Lanzar o analizador
    yyparse();

    return 0;
}



