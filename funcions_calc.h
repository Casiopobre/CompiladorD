#ifndef FUNCIONS_CALC_H
#define FUNCIONS_CALC_H

// Funcións da calculadora

/**
 * Carga un ficheiro (script) no interprete e o executa
 * @param nome_ficheiro Nome do ficheiro a cargar (script)
 */
void calc_load(char *nome_ficheiro);

/**
 * Mostra o espazo de traballo, conformado por todas as variables declaradas xunto cos seus valores
 */
void calc_workspace();

/**
 * Finaliza a execución do programa liberando a memoria necesaria
 */
void calc_exit();

/**
 * Mostra un menu de axuda con comandos e exemplos
 */
void calc_help();

/**
 * Limpa a consola do interprete (elimina as liñas anteriores)
 */
void calc_clear();

/**
 * Elimina todalas variables do espazo de traballo
 */
void calc_clean();

/**
 * Función de flex que carga o ficheiro en flex (indicandoo como stdin)
 */
void cargarFicheiro(char *nome);

/**
 * Pon a stdin na base do stack empregado por flex para xestionar as chamadas a cargar ficheiro
 */
void prepararEntradaEstandar();


#endif // FUNCIONS_CALC_H