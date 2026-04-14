#include <stdio.h>
#include <stdlib.h>
#include "ts.h"
#include "funcions_calc.h"
#include "cores.h"

extern void yylex_destroy(void);

extern int yyparse();

int main(int argc, char** argv){

    // Primeiro inicializamos a taboa de simbolos
    inicializarTS();

    // Metemos a stdin no buffer de flex
    prepararEntradaEstandar();

    // Comprobamos os parametros de entrada e abrimos o arquivo que se pase por liña de comandos
    // Nota: se non se pasa arquivo, emprégase a entrada estándar
    if (argc > 1) {
        printf(BRIGHT_BOLD_MAGENTA"Cargando arquivo %s ...\n"RESET, argv[1]);
        cargarFicheiro(argv[1]);
    }

    printf(BRIGHT_BOLD_MAGENTA "Para ver o menu de axuda escriba o comando help() :)\n" RESET);
    
    // Lanzar o analizador
    yyparse();

    return 0;
}



