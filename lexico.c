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

// Estrutura para almacenar primeiro (garda o inicio do lexema actual) delanteiro (posición do caracter actual)
typedef struct {
    char *primeiro;
    char *delanteiro;
} Arquivo;

// Definición das funcións
Arquivo *_mapearArquivo();
char _sigChar(Arquivo* arquivo);
char *_analizarIdentificador(Arquivo *arquivo);


CompLexico* sigCompLexico() {
    // Variables
    char c, *lexema;

    // Inicializamos a estrutura
    CompLexico *compLexico = (CompLexico*) malloc(sizeof(CompLexico));
    if (compLexico == NULL) return NULL;
    compLexico->id = 0;
    compLexico->lexema = "";

    // Mapeamos o arquivo en memoria
    Arquivo *arquivo = _mapearArquivo();
    
    // Lemos o codigo fonte caracter a caracter
    while ((c = _sigChar(arquivo)) != EOF) {
        //*printf("%c\n", c);
        // Comprobamos se é un identificador
        if (isalpha(c) || c == '_'){
            lexema = _analizarIdentificador(arquivo);
            printf("%s\n", lexema);
            //! buscarLexema(*ts): se xa está, devolver o identificador; se non, engadilo
            break;
        // Comprobamos se é un dixito
        } /*else if (isdigit) {
            //TODO REMATAR AQUI!!!!!!!!!!!!!!!!!
        }
        */
    }
    
    return compLexico;
}

int main() {
    CompLexico *a = sigCompLexico();
    printf("%ld\n", sizeof(*a));
    free(a);
}

Arquivo *_mapearArquivo() {
    // Inicializamos a estrutura para gardar os datos do arquivo
    Arquivo *arquivo = (Arquivo*) malloc(sizeof(Arquivo));
    arquivo->primeiro = NULL;
    arquivo->delanteiro = NULL;

    // Abrimos o arquivo
    int fd = open("regression.d", O_RDONLY);
    if (fd < 0) return NULL;

    // Obtemos o tamaño do arquivo
    struct stat st;
    fstat(fd, &st);
    int tamano = st.st_size;

    // Mapeamos o arquivo en modo lectura
    char *stringCodigoFonte = mmap(NULL, tamano, PROT_READ, MAP_PRIVATE, fd, 0);
    // Podemos pechar o fd sen afectar ao mmap
    close(fd);

    // Actualizamos a estrutura dos punteiros para que ambos apunten ao inicio do lexema
    arquivo->primeiro = stringCodigoFonte;
    arquivo->delanteiro = stringCodigoFonte;

    return arquivo;
}

//! Posible problema ao ler un caracter de máis (hai que recuperalo de algunha forma)
char _sigChar(Arquivo *arquivo) {
    // Devolvemos o caracter que se atopa na posicion actual
    char c = *arquivo->delanteiro;
    //Incrementamos o ounteiro para apuntar ao seguinte char por ler
    arquivo->delanteiro++;
    
    return c;
}

char *_empaquetarString(int len, char* inicio){
    // Reservamos memoria para o lexema
    char *lexema = malloc (sizeof(char)*len);
    // Copiamos os caracteres que se corresponden ao lexema
    memcpy(lexema, inicio, len);
    // Engadimos o terminador de string
    lexema[len] = '\0';

    return lexema;
}

char *_analizarIdentificador(Arquivo *arquivo) {
    char c, *lexema;
    // Len = 2 para o char que xa se leu, e para o \0 do final do string
    int len = 2;

    while ((c = _sigChar(arquivo)) != EOF){
        // Seguimos no bucle de ler o lexema do identificador
        if (isalnum(c) || c == '_') {
            len++;
        // Cando lemos len++;"outro", rematamos de ler o lexema do identificador (hai que recuperar despois o char)
        } else {
            lexema =  _empaquetarString(len, arquivo->primeiro);
            break;
        }
    }
    // Decrementamos o punteiro para devolver o char consumido
    arquivo->delanteiro--;
    // Actualizamos primeiro
    arquivo->primeiro = arquivo->delanteiro;
    
    return lexema;
}
