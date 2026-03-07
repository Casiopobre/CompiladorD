#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "definiciones.h"
#include "lexico.h"
#include "entrada.h"
#include "ts.h"

/*TODO
* Comprobar lóxica ao inserir na ts (facer algo como que comprobe e insira)
* Revisar as funcións da ts
* CAndo chega ao bloque B, se ralla e nn da lido ben
*/

char *_analizarIdentificador();

CompLexico* sigCompLexico() {
    // Variables
    char c, *lexema;

    // Inicializamos a estrutura
    CompLexico *compLexico = (CompLexico*) malloc(sizeof(CompLexico));
    if (compLexico == NULL) return NULL;
    compLexico->id = 0;
    compLexico->lexema = "";
    
    // Lemos o codigo fonte caracter a caracter
    while ((c = sig_char()) != EOF) {
        // Comprobamos se é un identificador
        if (isalpha(c) || c == '_'){
            lexema = _analizarIdentificador();
            printf("LEXEMA: %s\n", lexema);

            if (existeLexemaTS(lexema)) {
                free(compLexico);
                compLexico = buscarLexemaTS(lexema);
            } else {
                compLexico->id = ID;
                compLexico->lexema = lexema;
                engadirEntradaTS(compLexico);
            }
            break;
        // Comprobamos se é un dixito
        } else if (c == ' ' || c == '.' || c == ';' || c == '\n') {
            // Avanzamos inicio para que non se inclúa o espazo no seguinte lexema
            obtener_lexema();
            printf("Espacio\n");
        }
        
    }
    
    return compLexico;
}

char *_analizarIdentificador() {
    char c, *lexema;

    while (1){
        c = sig_char();
        // Seguimos no bucle de ler o lexema do identificador
        if (!(isalnum(c) || c == '_')) {
            // Devolvemos o delimitador antes de capturar o lexema
            devolver();
            lexema = obtener_lexema();
            break;
        }
    }
    
    return lexema;
}
