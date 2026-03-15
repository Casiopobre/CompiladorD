#ifndef ERROS_H
#define ERROS_H

// Constantes para os tipso de erros
#define MAX_LEX_LEN_ERR 400

/**
 * Función que xestiona os erros do compilador
 * @param codigoErro O codigo do erro que queremos imprimir
 * @param numLinea O numero de liña na que ocorreu o erro
 * @param tamMax O tamaño máximo definido para o lexema
 */
void xestionarErro(int codigoErro, int numLinea, int tamMax);


#endif //ERROS_Hefine MAX_LEX_LEN 400