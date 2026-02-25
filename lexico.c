#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"

char _sigChar(FILE* arquivo);

CompLexico* sigCompLexico() {
    // Inicializamos a estrutura
    CompLexico *compLexico = (CompLexico*) malloc(sizeof(CompLexico));
    if (compLexico == NULL) return NULL;
    compLexico->id = 0;
    compLexico->lexema = "";
    
    // Abrimos o arquivo
    FILE* arquivo = fopen("regression.d", "r");
    if (arquivo == NULL) return NULL;

    char c;
    
    while ((c = _sigChar(arquivo)) != EOF) {
        printf("%c\n", c);
    }
    
    return compLexico;
}

int main() {
    CompLexico *a = sigCompLexico();
    printf("%ld\n", sizeof(*a));
    free(a);
}

char _sigChar(FILE* arquivo) {
    return fgetc(arquivo);
}
