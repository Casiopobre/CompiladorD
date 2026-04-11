#include <stdio.h>
#include <stdlib.h>
#include "ts.h"

extern void yylex_destroy(void);

extern int yyparse();

int main(int argc, char** argv){

    // Comprobar os parametros de entrada e abrir o arquivo que se pase por liña de comandos
    // Nota: se non se pasa arquivo, emprégase a entrada estándar
    /*
    if (argc > 1) {
        printf("Lendo do arquivo %s\n", argv[1]);
        abrirArquivo(argv[1]);
        pecharArquivo();
    }
    */

    // Primeiro inicializamos a taboa de simbolos
    inicializarTS();

    printf("Para ver o menu de axuda escribir o comando HELP() :)\n");
    
    // Lanzar o analizador
    yyparse();

    // Liberamos a memoria empregada por flex
    yylex_destroy();

    // Cando rematamos, imprimimos a taboa de símbolos ao final
    imprimirTS();

    // Liberamos a memoria da taboa de símbolos (operacións de limpeza)
    liberarMemoriaTS();

    return 0;
}


// Bison llama a esta función cuando hay un error sintáctico
void yyerror(char const *s) {
    fprintf(stderr, "Error: %s\n", s);
}
