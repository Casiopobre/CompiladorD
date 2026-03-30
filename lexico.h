#ifndef LEXICO_H
#define LEXICO_H

// Estrutura para a compoñente lexica
typedef struct {
    char* lexema;
    int id;
} CompLexico;

#define YY_DECL CompLexico *yylex()
CompLexico *yylex();

/**
 * Devolve a seguinte compoñente lexica
 * @returns A seguinte compoñente léxica identificada no codigo fonte
 */
CompLexico *sigCompLexico();


/**
 * Crea e devolve unha compoñente lexica
 * @param lexema Lexema da compoñente lexica a crear
 * @param id Identificador da compoñente lexica a crear 
 * @returns A compoñente lexica creada
 */
CompLexico *crearCompLexico(char *lexema, int id);

/**
 * Funcion para abrir o arquivo a analizar e indicarllo a flex
 * @param arquivo Nome do arquivo a analizar
 */
void abrirArquivo(char *arquivo);

/**
 * Función par pechar o arquivo de entrada
 */
void pecharArquivo();


#endif //LEXICO_H