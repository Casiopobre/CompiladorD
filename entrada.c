#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "erros.h"

//* Constantes para definir o tamaño do buffer (T_BUF refírese a cada buffer lóxico (bloques A e B)) */ 
#define T_BUF 64
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

// Para marcar cando chegamos ao EOF do ficheiro (para non perder o último caracter)
int arquivoEOF;

// Contador para rexistrar o numero de liña (conta \n) para os erros
int numLinea;


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

    // Inicializamos a variable para non ignorar os EOF
    ignorarEOF = 0;

    // Inicializamos a variable para non ignorar a entrada
    ignorandoEntrada = 0;

    // Inicializamos EOF do ficheiro a 0
    arquivoEOF = 0;

    // Iniciamos o numero de liña a 1
    numLinea = 1;
}

// Función para copiar a parte do lexema lido ao cambiar de bloque
void _copiarParteLexema(){
    // Copiamos a parte do lexema que corresponda
    memcpy(parteLexema, inicio, lenLexema);
    // Poñemos o terminador de string para que funcione despois o strlen
    parteLexema[lenLexema] = '\0';
}


// Función principal do sistema de entrada: devolve o seguinte caracter a ser procesado
int sig_char() {
    // Se xa chegamos ao EOF do ficheiro, devolvemos EOF
    if (arquivoEOF){
        fclose(fd);
        return EOF;
    } 

    // Obtemos o caracter ao que apunta delantero
    int c = (char) *delantero;
    
    // Contamos saltos de liña
    if (c == '\n') {
        numLinea++;
    }
    
    // Avanzamos delantero e aumentamos a lonxitude do lexema 
    delantero++;
    lenLexema++;

    // Se o tamaño do lexema excede o tamaño do bloque
    if (lenLexema > T_BUF && !ignorandoEntrada) {
        ignorandoEntrada = 1;
        xestionarErro(MAX_LEX_LEN_ERR, numLinea, T_BUF);
    }

    // Se nos atopamos cun EOF, hai que comprobar en que caso estamos
    if (*delantero == EOF && !ignorarEOF) {
        // Estamos no EOF do bloque A
        if (delantero == &(parBuffers[T_BUF])){
            // Gardamos a parte de lexema xa lida antes de cambiar de bloque (se non estamos ignorando a entrada)
            if (!ignorandoEntrada) _copiarParteLexema();
            _cargar_bloque_B();
            delantero++;

        // Estamos no EOF do bloque B
        } else if (delantero == &(parBuffers[T_BUF_TOTAL+1])) {
            // Gardamos a parte de lexema xa lida antes de cambiar de bloque (se non estamos ignorando a entrada)
            if (!ignorandoEntrada) _copiarParteLexema();
            _cargar_bloque_A();
            delantero = &(parBuffers[0]); // Colocamos delantero ao inicio do buffer

        // Estamos no EOF do ficheiro: marcamos o flag pero devolvemos o último caracter lido
        } else {
            arquivoEOF = 1;
        }
    
    // Se estamos ignorando o EOF porque devolver pasou por un centinela (para non machacar o seguinte bloque)
    } else if (*delantero == EOF && ignorarEOF) {
        delantero++;
        // Deixamos de ignorar o EOF
        ignorarEOF = 0;
    }

    return c;
}


// Devolve o lexema actual (comprendido entre inicio e delantero)
char *obtener_lexema() {

    // Se o lexema era demasiado longo (erro), reseteamos sen copiar nada
    if (ignorandoEntrada) {
        lenLexema = 0;
        parteLexema[0] = '\0';
        inicio = delantero;
        ignorandoEntrada = 0;
        return NULL;
    }

    // Comprobamos a lonxitude da parte do lexema xa gardada (se está entre dous bloques)
    int lenParteLex = strlen(parteLexema);

    // Se o lexema está nun só bloque
    if (lenParteLex == 0){
        memcpy(lexemaActual, inicio, delantero-inicio);
        lexemaActual[lenLexema] = '\0';
        // Reiniciamos lenLExema
        lenLexema = 0;
        // Avanzamos inicio
        inicio = delantero;
        return lexemaActual;

    // Se o lexema está en dous bloques
    } else {
        // Se o lexema comeza no bloque B e remata no bloque A
        if (inicio+lenParteLex == &(parBuffers[T_BUF_TOTAL+1])) {
            // Copiamos dende o inicio do bloque A a lonxitude que lle quede por copiar ao lexema
            memcpy(parteLexema+lenParteLex, &(parBuffers[0]), lenLexema-lenParteLex);

        // Se comeza no A e remata no B
        } else {
            // Copiamos dende o inicio do bloque B ata a lonxitude que lle falte ao lexema por copiar
            memcpy(parteLexema+lenParteLex, inicio+lenParteLex+1, lenLexema-lenParteLex);
        }
    }

    // Rematamos o lexema en \0 (para crear un "string valido")
    parteLexema[lenLexema] = '\0';

    // Ccopiamos o resultado a lexemaActual e reiniciamos lenLexema e parteLexema
    memcpy(lexemaActual, parteLexema, lenLexema + 1);
    lenLexema = 0;
    parteLexema[0] = '\0';

    // Avanzamos inicio á posición de delantero
    inicio = delantero;

    return lexemaActual;
}

void devolver() {
    // Decrementamos delantero
    delantero--;
    lenLexema--;
    // Se xusto delantero cae nun centinela, decrementámolo outra vez e entramos nun estado de ignorar o EOF (centinela)
    if (*delantero == EOF) {
        delantero--;
        ignorarEOF = 1;
    }

    // Se o lexema está en máis dun bloque, se ao retroceder a lonxitude do lexema gardada é menor á da parte do lexema, truncamolo
    int lenParteLex = strlen(parteLexema);
    if (lenParteLex > lenLexema) {
        parteLexema[lenLexema] = '\0';
    }
}

// Función que lle di a entrada que ignore os caracteres para non dar erro de max length cando se lea un comentario.
void ignorarEntrada(int deboIgnorar) {
    ignorandoEntrada = deboIgnorar;
}