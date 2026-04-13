#ifndef TS_H
#define TS_H


#define MYVAR 1
#define MYFNCT 2
#define MYCALC 3
#define MYLOAD 4
#define MYCONST 5

// Estrutura para a compoñente lexica
typedef struct {
    char* lexema;
    int tipo;
    union {
        double var;
        double (*fnctptr)(double);
        void (*calcptr)(void);
        void (*loadptr)(char*);
    } valor;
} CompLexico;

/**
 * Inicializa a taboa de simbolos, introducindo as palabras clave
 */
void inicializarTS();

/**
 * Inserta unha compoñente lexica an taboa de simbolos
 * @param entrada A compoñente léxica (struct) que se queira insertar na ts
 */
void engadirEntradaTS(CompLexico *entrada);

/**
 * Busca unha compoñente lexica por lexema na taboa de simbolos
 * @param lexema O lexema que queremos buscar
 * @returns A compoñente léxica que conten ese lexema
 */
CompLexico *buscarLexemaTS(char* lexema);

/**
 * Función que imprime os nos MYVAR da taboa de símbolos mediante un recorrido inordre
 */
void imprimirWorkspace();

/**
 * Funciónq ue comproba se existe un lexema na taboa de simbolos
 * @param lexema Lexema a comprobar
 * @returns Devolve 1 se atopa o lexema na taboa de símbolos e 0 noutro caso
 */
int existeLexemaTS(char *lexema);

/**
 * Función que limpa a memoria almacenada para a taboa de simbolos
 */
void liberarMemoriaTS();

/**
 * Función que crea unha compoñente léxica.
 * @param lexema Lexema da compoñente léxica
 * @param tipo Tipo da compoñente léxica (MYVAR, MYFNCT...)
 * @param valor Valor do lexema (por defecto, poñer 0.0)
 */
CompLexico *crearCompLexico(char *lexema, int tipo, double valor);

/**
 * Elimina a memoria do espazo de traballo
 */
void limparWorkspace();

#endif // TS_H