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



CompLexico *crearCompLexico(char *lexema, int id);


#endif //LEXICO_H