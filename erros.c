#include <stdio.h>
#include <stdlib.h>
#include "erros.h"
#include "cores.h"


void xestionarErro(int codigoErro, int numLinea, int tamMax) {
    switch (codigoErro) {
        // Erro de tamaño máximo de lexema sobrepasado
        case MAX_LEX_LEN_ERR:
            printf(BOLD_RED"\nErro: Tamaño máximo do lexema (%d) sobrepasado na liña %d :(\n\n"RESET, tamMax, numLinea);
            break;

        // Erro non identificado
        default:
            printf(BOLD_RED"\nErro: Código de erro non identificado :(\n"RESET);
            break;
    }
}
