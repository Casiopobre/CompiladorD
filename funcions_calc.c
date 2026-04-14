#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ts.h"
#include "funcions_calc.h"
#include "cores.h"

extern void yylex_destroy(void);


void calc_load(char *nome_ficheiro) {
    printf(BRIGHT_BOLD_MAGENTA "Cargando ficheiro '%s' ... \n" RESET, nome_ficheiro);
    cargarFicheiro(nome_ficheiro);
}

void calc_workspace() {
    imprimirWorkspace();
}


void calc_exit() {
    // Imprimimos unha mensaxe de saída
    printf(BRIGHT_BOLD_MAGENTA "Saíndo da calculadora . . .\n\n" RESET);

    // Liberamos a memoria empregada por flex
    yylex_destroy();

    // Liberamos a memoria da taboa de símbolos (operacións de limpeza)
    liberarMemoriaTS();

    exit(0);
}


void calc_help() {
    printf(BRIGHT_BOLD_WHITE "~.~.~.~.~.~.~.~.~.~.~.~.~.~.~ Menú de axuda: ~.~.~.~.~.~.~.~.~.~.~.~.~.~.~\n" RESET);
    printf(BRIGHT_BOLD_GREEN "<><><><><><><><><><><><><><> Comandos: <><><><><><><><><><><><><><>\n" RESET);
    printf(BRIGHT_GREEN "    > load(\"nome_ficheiro\"): carga un ficheiro e o interpreta\n");
    printf("    > workspace(): imprime as variables asignadas xunto cos seus valores\n");
    printf("    > clean: eliminar as variables do workspace\n");
    printf("    > clear(): limpar a pantalla\n");
    printf("    > help(): imprimir o menú de axuda\n");
    printf("    > exit(): sair do interprete\n\n" RESET);
    
    printf(BRIGHT_BOLD_YELLOW "<><><><> Constantes: <><><><>\t<><><><> Funcións matemáticas: <><><><>\n" RESET);
    printf(BRIGHT_YELLOW" pi = %.10g\t\t Funcións trigonométricas: sin, atan...\n" , M_PI);
    printf(" e = %.10g\t\t Funcións floor e ceiling (ceil)\n", M_E);
    printf(" tau = 2 * pi = %.10g\t Raíz cadrada (sqrt) e logaritmo (log)\n\n" RESET, M_PI*2);

    printf(BRIGHT_BOLD_CYAN "<><><><><><> Exemplos: <><><><><><>\n" RESET);
    printf(BRIGHT_CYAN " > a = sin(pi)\n");
    printf(" > b = floor(a)\n");
    printf(" > asin(pi/4)\n\n" RESET);

}


void calc_clear() {
    // Executamos o comando clear de linux
    system("clear");
}


void calc_clean() {
    limparWorkspace();
}
