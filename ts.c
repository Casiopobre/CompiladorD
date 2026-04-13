#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "abb.h"
#include "cores.h"
#include "funcions_calc.h"


// Variable global para a taboa de simbolos
TABB taboaSimbolos;


void inicializarTS(){
    CompLexico funcions[] = 
    {
        // Funcións matemáticas
        {"sin", MYFNCT, .valor.fnctptr = sin},
        {"cos", MYFNCT, .valor.fnctptr = cos},
        {"tan", MYFNCT, .valor.fnctptr = tan},
        {"asin", MYFNCT, .valor.fnctptr = asin},
        {"acos", MYFNCT, .valor.fnctptr = acos},
        {"atan", MYFNCT, .valor.fnctptr = atan},
        {"sqrt", MYFNCT, .valor.fnctptr = sqrt},
        {"log", MYFNCT, .valor.fnctptr = log},
        {"exp", MYFNCT, .valor.fnctptr = exp},
        {"floor", MYFNCT, .valor.fnctptr = floor},
        {"ceil", MYFNCT, .valor.fnctptr = ceil},

        // Funcións/comandos da calculadora
        {"load", MYLOAD, .valor.loadptr = calc_load},
        {"workspace", MYCALC, .valor.calcptr = calc_workspace},
        {"exit", MYCALC, .valor.calcptr = calc_exit},
        {"help", MYCALC, .valor.calcptr = calc_help},
        {"clear", MYCALC, .valor.calcptr = calc_clear},
        {"clean", MYCALC, .valor.calcptr = calc_clean},

        // Constantes 
        { "pi", MYCONST, .valor.var =  M_PI},
        { "e", MYCONST, .valor.var = M_E},
        { "tau", MYCONST, .valor.var = M_PI*2}
    };

    // Creamos a árbore binaria
    crearAbb(&taboaSimbolos);

    // Insertamos as funcións na taboa de simbolos
    for (int i = 0; i < sizeof(funcions)/sizeof(CompLexico); i++) {
        insertarEntrada(&taboaSimbolos, funcions[i]);
    }
}


// Busca unha entrada na taboa de símbolos por lexema
CompLexico *buscarLexemaTS(char *lexema) {
    return buscarNodoPtr(taboaSimbolos, lexema);
}

int existeLexemaTS(char *lexema) {
    TIPOELEM *e = malloc(sizeof(TIPOELEM));
    e->lexema = lexema;

    unsigned comprobacion = esMiembroAbb(taboaSimbolos, *e);
    free(e);
    return comprobacion;
}

void engadirEntradaTS(CompLexico *entrada){
    insertarEntrada(&taboaSimbolos, *entrada);
}

void liberarMemoriaTS() {
    eliminarAbb(&taboaSimbolos);
    printf(BRIGHT_GREEN"Memoria da taboa de símbolos eliminada correctamente :)\n"RESET);
}

// Recorre a árbore de esquerda a dereita (inorde) imprimindo o contido dos nodos
void _recorrer_TS(TABB A){
    if (esAbbVacio(A)) return;

    TIPOELEM nodo;

    leerElementoAbb(A, &nodo);

    _recorrer_TS(izqAbb(A));

    printf("<%s, %s>\n", nodo.lexema, nodo.tipo == 1 ? "VAR" : "MYFNCT");
    
    _recorrer_TS(derAbb(A));
}


void imprimirTS() {
    printf(BRIGHT_BLUE"~*~*~*~*~ Taboa de símbolos ~*~*~*~*~\n");
    _recorrer_TS(taboaSimbolos);
    printf("~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~\n"RESET);
}

CompLexico *crearCompLexico(char *lexema, int tipo, double valor) {
    CompLexico *cl = (CompLexico *) malloc(sizeof(CompLexico));

    cl->lexema = strdup(lexema);
    cl->tipo = tipo;
    cl->valor.var = valor;

    return cl;
}
