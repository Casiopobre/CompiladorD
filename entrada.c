#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define T_BUF 8
#define T_BUF_TOTAL 2*T_BUF
#define INI_BLOQUE_B T_BUF+1

/*TODO
 //* Resolver o probelema de copiar un lexema que comeza nun bloque e remata en outro (copia parcial?)
 //* Resolevr o problema de devolver(): cando se acepta en "otro" e haiq ue tirar delantero atrás, 
 //* solucionar problma de volver a ler o EOF e machacar o bloque xa copiado anteriormente
*/

// Buffer físico que contén os dous buffers lóxicos (bloque A e bloque B)
char parBuffers[T_BUF_TOTAL+2];

// Punteiros inicio e delantero para o método do par de memorias intermedias
char *inicio, *delantero;

// Descriptor do arquivo: punteiro á posición de lectura actual
FILE *fd;

// Para gardar a lonxitude do lxema analizado
int lenLexema;

// Para gardar o trozo de lexema en caso de cambiar de bloque (por deseño, un lexema non pode ser máis grande que T_BUF)
char parteLexema[T_BUF];

// Declaración de funcións:
void _cargar_bloque_A();


// Función para iniciar o sistema de entrada
void iniciar_SE() {
    // Colocamos o primeiro centinela
    parBuffers[T_BUF] = EOF;
    // Colocamos o segundo centnela
    parBuffers[T_BUF_TOTAL+1] = EOF;

    // Colocamos inicio e delantero apuntando ao principio do buffer
    inicio = &(parBuffers[0]);
    delantero = &(parBuffers[0]);

    // Abrimos o arquivo
    fd = fopen("regression.d", "r");
    if (fd == NULL) {
        perror("Erro ao abrir o arquivo regression.d");
        return;
    }

    // Cargamos o primeiro bloque
    _cargar_bloque_A();

    // Inicializamos a lonxitude do lexema a 0
    lenLexema = 0;
}

// Función para cargar o bloque A
void _cargar_bloque_A(){
    // Lemos os caracteres suficientes para
    if (fread(parBuffers, sizeof(char), T_BUF, fd) == 0){
        perror("Erro na chamda a fread");
        return;
    }

    printf("BLOQUE A: %s\n", parBuffers);
}

// Función para cargar o bloque B
void _cargar_bloque_B() {
    // Lemos os caracteres suficientes para
    if (fread(parBuffers+INI_BLOQUE_B, sizeof(char), T_BUF, fd) == 0){
        perror("Erro na chamda a fread");
        return;
    }

    printf("BLOQUE B: %s\n", parBuffers+INI_BLOQUE_B);
}

// Función que devolve o seguinte caracter a ser procesado
char sig_char() {
    char c = *delantero;
    delantero++;
    lenLexema++;
    // Se nos atopamos cun EOF, hai que comprobar en que caso estamos
    if (*delantero == EOF) {
        // Estamos no EOF do bloque A
        if (delantero == &(parBuffers[T_BUF])){
            _cargar_bloque_B();
            delantero++;

        // Estamos no EOF do bloque B
        } else if (delantero == &(parBuffers[T_BUF_TOTAL+1])) {
            // Gardamos o trozo de lexema xa lido antes de cambiar de bloque
            _cargar_bloque_A();
            delantero = &(parBuffers[0]);

        // Estamos no EOF do ficheiro
        } else {
            return EOF;
        }
    }

    return c;
}

// Devolve o lexema actual (comprendido entre inicio e delantero)
char *obtener_lexema() {
    // Gardamos espacio para o lexema
    char *lexema = malloc(sizeof(char)*lenLexema+1);

    
}

