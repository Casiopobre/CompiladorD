#ifndef ENTRADA_H
#define ENTRADA_H

/**
 * Función de inicialización do sistema de entrada: abre o arquivo, incializa os contadores e carga o Bloque A
 */
void iniciar_SE();

/**
 * Función que devolve o seginte caracter a ser lido polo sistema de entrada
 * Está declarada cmo int para xestionar os EOF (-1)
 * @returns O caracter lido polo sistema de entrada
 */
int sig_char();

/**
 * Función que devolve o lexema actual (dende inicio ata delantero)
 * @returns Un punteiro ao lexema actual
 */
char *obtener_lexema();

/**
 * Función que volve un caracter atrás (decrementa delantero)
 */
void devolver();

/**
 * Función que indica ao sistema de entrada se este debe ignorar a entrada (p.e cando se le un comentario) 
 * @param deboIgnorar Valor 1 para ignorar a entrada, e 0 para deixar de facelo
 */
void ignorarEntrada(int deboIgnorar);


void abrirArquivo(char *arquivo);


#endif // ENTRADA_H