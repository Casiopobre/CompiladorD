#ifndef LEXICO_H
#define LEXICO_H

// Estrutura para a compoñente lexica
typedef struct {
    char* lexema;
    int id;
} CompLexico;


/**
 * Devolve a seguinte compoñente lexica
 * @returns A seguinte compoñente léxica identificada no codigo fonte
 */
CompLexico *sigCompLexico();


#endif //LEXICO_H