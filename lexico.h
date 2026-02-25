#ifndef LEXICO_H
#define LEXICO_H

// Estrutura para a compoñente lexica
typedef struct CompLexico {
    char* lexema;
    int num;
} CompLexico;


// Devolve a seguinte compoñente lexica
CompLexico sigCompLexico();


#endif //LEXICO_H