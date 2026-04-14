#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ts.h"
#include "funcions_calc.h"

extern void yylex_destroy(void);


// load(): carga un ficheiro
// workspace(): imprimir as variables asignadas
// exit(): sair do interprete impamente
// help(): imprimir menu axuda
// clear(): limpar a pantalla
// clean: eliminar as variables do workspace

void calc_load(char *nome_ficheiro) {
    printf("Cargando ficheiro '%s' ... \n", nome_ficheiro);
    cargarFicheiro(nome_ficheiro);
}

void calc_workspace() {
    imprimirWorkspace();
}


void calc_exit() {
    // Imprimimos unha mensaxe de saída
    printf("Saíndo da calculadora . . .\n\n");

    // Liberamos a memoria empregada por flex
    yylex_destroy();

    // Liberamos a memoria da taboa de símbolos (operacións de limpeza)
    liberarMemoriaTS();

    exit(0);
}


void calc_help() {
    printf("\n ~.~.~.~.~.~.~.~.~ Menú de axuda: ~.~.~.~.~.~.~.~.~\n");


}


void calc_clear() {
    // Executamos o comando clear de linux
    system("clear");
}


void calc_clean() {
    limparWorkspace();
}
