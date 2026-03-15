#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "erros.h"

#define T_BUF 40
#define T_BUF_TOTAL 2*T_BUF
#define INI_BLOQUE_B T_BUF+1

// Buffer físico que contén os dous buffers lóxicos (bloque A e bloque B)
char parBuffers[T_BUF_TOTAL+2];

// Punteiros inicio e delantero para o método do par de memorias intermedias
char *inicio, *delantero;

// Descriptor do arquivo: punteiro á posición de lectura actual
FILE *fd;

// Para gardar a lonxitude do lxema analizado
int lenLexema;

// Para gardar o trozo de lexema en caso de cambiar de bloque (por deseño, un lexema non pode ser máis grande que T_BUF)
char parteLexema[T_BUF+1];

char lexemaActual[T_BUF+1];

// Para marcar cando teño que ignorar o eof
int ignorarEOF;

// Para marcar cando ignorar a entrada por estarse lendo un comentario
int ignorandoEntrada;


// Función para cargar o bloque A
void _cargar_bloque_A(){
    // Lemos os caracteres suficientes para encher o buffer do bloque A
    size_t charLeidos = fread(&parBuffers[0], sizeof(char), T_BUF, fd);

    if (charLeidos == 0) {
        perror ("Erro ao cargar o bloque A :(\n");
        return;
    }

    // Se se cargaron menos carateres que o tamaño dun bloque, colocamos o centinela ao final destes
    if (charLeidos < T_BUF) {
        parBuffers[charLeidos] = EOF;

    // Colocamos o centinela ao final do bloque A
    } else {
        parBuffers[T_BUF] = EOF;
    }
}

// Función para cargar o bloque B
void _cargar_bloque_B() {
    // Lemos os caracteres suficientes para encher o buffer do bloque B
    size_t charLeidos = fread(&parBuffers[INI_BLOQUE_B], sizeof(char), T_BUF, fd);

    if (charLeidos == 0) {
        perror ("Erro ao cargar o bloque B :(\n");
        return;
    }

    // Se se cargaron menos carateres que o tamaño dun bloque, colocamos o centinela ao final
    if (charLeidos < T_BUF) {
        parBuffers[INI_BLOQUE_B + charLeidos] = EOF;

    // Colocamos o centinela ao final do bloque B
    } else {
        parBuffers[T_BUF_TOTAL+1] = EOF;
    }
}

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

    // Inicializamos para non ignorar os EOF
    ignorarEOF = 0;
}

void _copiarParteLexema(){
    // Copiamos a parte do lexema que corresponda
    memcpy(parteLexema, inicio, lenLexema);
    // Poñemos o terminador de string
    parteLexema[lenLexema] = '\0';
}

// Función que devolve o seguinte caracter a ser procesado
int sig_char() {
    int c = (char) *delantero;
    delantero++;
    lenLexema++;

    // Se o tamaño do lexema excede o tamaño do bloque
    if (lenLexema > T_BUF && !ignorandoEntrada) {
        xestionarErro(MAX_LEX_LEN);
        ignorandoEntrada = 1;
    }

    // Se nos atopamos cun EOF, hai que comprobar en que caso estamos
    if (*delantero == EOF && !ignorarEOF) {
        // Estamos no EOF do bloque A
        if (delantero == &(parBuffers[T_BUF])){
            // Gardamos a parte de lexema xa lida antes de cambiar de bloque (só se non está en modo erro)
            if (!ignorandoEntrada) _copiarParteLexema();
            _cargar_bloque_B();
            delantero++;

        // Estamos no EOF do bloque B
        } else if (delantero == &(parBuffers[T_BUF_TOTAL+1])) {
            // Gardamos a parte de lexema xa lida antes de cambiar de bloque (só se non está en modo erro)
            if (!ignorandoEntrada) _copiarParteLexema();
            _cargar_bloque_A();
            delantero = &(parBuffers[0]);

        // Estamos no EOF do ficheiro
        } else {
            return EOF;
        }
    } else if (*delantero == EOF && ignorarEOF) {
        delantero++;
        ignorarEOF = 0;
    }

    return c;
}

// Devolve o lexema actual (comprendido entre inicio e delantero)
char *obtener_lexema() {
    // Se o lexema era demasiado longo, reseteamos o estado sen copiar nada
    if (ignorandoEntrada) {
        lenLexema = 0;
        parteLexema[0] = '\0';
        inicio = delantero;
        ignorandoEntrada = 0;
        return NULL;
    }
    // Comprobamos a lonxitude da parte do lexema xa gardada (se está entre dous bloques)
    int len = strlen(parteLexema);

    // Se o lexema está nun só bloque
    if (len == 0){
        memcpy(lexemaActual, inicio, delantero-inicio);
        lexemaActual[lenLexema] = '\0';
        // Reiniciamos lenLExema
        lenLexema = 0;
        // Avanzamos inicio
        inicio = delantero;
        return lexemaActual;

    // Se o lexema está en dous bloques
    } else {
        // Se inicio + len == EOF
        if (inicio+len == &(parBuffers[T_BUF_TOTAL+1])) {
            memcpy(parteLexema+len, &(parBuffers[0]), lenLexema - len);
        } else {
            memcpy(parteLexema+len, inicio+len+1, lenLexema-len);
        }
    }

    // Rematamos o lexema en \0
    parteLexema[lenLexema] = '\0';

    // Reiniciamos lenLexema e parteLexema, e copiamos resultado a lexemaActual
    memcpy(lexemaActual, parteLexema, lenLexema + 1);
    lenLexema = 0;
    parteLexema[0] = '\0';

    // Avanzamos inicio
    inicio = delantero;

    return lexemaActual;
}

void devolver() {
    delantero--;
    lenLexema--;
    if (*delantero == EOF) {
        delantero--;
        ignorarEOF = 1;
    }

    int len = strlen(parteLexema);
    if (len > lenLexema) {
        parteLexema[lenLexema] = '\0';
    }
}

// Función que lle di a entrada que ignore os caracteres para non dar erro de max length cando se lea un comentario.
void ignorarEntrada(int deboIgnorar) {
    ignorandoEntrada = deboIgnorar;
}
