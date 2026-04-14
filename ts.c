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
        funcions[i].inicializada = 1;
        insertarEntrada(&taboaSimbolos, funcions[i]);
    }
}

// Recorre a os nodos MYVAR da ts
void _recorrer_myvar(TABB a) {
    if (esAbbVacio(a)) return;

    TIPOELEM nodo;
    leerElementoAbb(a, &nodo);

    _recorrer_myvar(izqAbb(a));

    if (nodo.tipo == MYVAR) {
        printf("%s = %g\n", nodo.lexema, nodo.valor.var);
    }

    _recorrer_myvar(derAbb(a));
}


void imprimirWorkspace() {
    printf(BRIGHT_BOLD_BLUE"~·~·~·~·~ Variables actuais: ~·~·~·~·~\n");
    _recorrer_myvar(taboaSimbolos);
    printf("~·~·~·~·~·~·~·~·~·~·~·~·~·~·~·~·~·~·~·\n"RESET);
}

// Función auxiliar para recoller todos os lexemas das cl MYBAR
void _recolectar_vars(TABB a, char **buf, int *n) {
    if (esAbbVacio(a)) return;
    TIPOELEM nodo;
    leerElementoAbb(a, &nodo);
    _recolectar_vars(izqAbb(a), buf, n);
    if (nodo.tipo == MYVAR) {
        buf[(*n)++] = nodo.lexema;
    }
    _recolectar_vars(derAbb(a), buf, n);
}

void limparWorkspace() {
    char *buf[256];
    int n = 0;

    _recolectar_vars(taboaSimbolos, buf, &n);

    for (int i = 0; i < n; i++) {
        eliminarNodo(&taboaSimbolos, buf[i]);
    }

    printf(BRIGHT_BOLD_BLUE "Memoria do espazo de traballo eliminada! :)\n" RESET);
}

void _recolectar_no_inicializadas(TABB a, char **buf, int *n) {
    if (esAbbVacio(a)) return;
    TIPOELEM nodo;
    leerElementoAbb(a, &nodo);
    _recolectar_no_inicializadas(izqAbb(a), buf, n);
    if (nodo.tipo == MYVAR && !nodo.inicializada) {
        buf[(*n)++] = nodo.lexema;
    }
    _recolectar_no_inicializadas(derAbb(a), buf, n);
}


void limparVarsNonInicializadas() {
    char *buf[256];
    int n = 0;
    _recolectar_no_inicializadas(taboaSimbolos, buf, &n);
    for (int i = 0; i < n; i++) {
        eliminarNodo(&taboaSimbolos, buf[i]);
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


CompLexico *crearCompLexico(char *lexema, int tipo, double valor) {
    CompLexico *cl = (CompLexico *) malloc(sizeof(CompLexico));

    cl->lexema = strdup(lexema);
    cl->tipo = tipo;
    cl->inicializada = 0;
    cl->valor.var = valor;

    return cl;
}
