#include <stdio.h>
#include <stdlib.h>
#include "erros.h"


void xestionarErro(int codigoErro) {
    switch (codigoErro) {
        // Erro de tamaño máximo de lexema sobrepasado
        case MAX_LEX_LEN:
            printf("\nErro: Tamaño máximo do lexema sobrepasado\n");
            break;

        default:
            printf("\nErro: Código de erro non identificado :(\n");
            break;
    }
}
