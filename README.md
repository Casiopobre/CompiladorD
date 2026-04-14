# Calculadora
Intérprete de operacións matemáticas implementado en C empregando flex e bison.

## Contorna de execución
* Necesaria versión de **gcc 13.3.0** ou superior
* Testeado en **Ubuntu 24.04.4 LTS**
* Versión de FLEX empregada: **flex 2.6.4**
* Versión de BISON empregada: **bison 3.8.2**

### Instuccións de compilación
Inclúese un makefile:
* `make` para compilar o programa (incluíndo o ficheirio de especificación de flex e o de bison)
* `make clean` para eliminar os arquivos xerados por make
* `make run` para compilar e executar o programa
En caso de querer compilalo a man, simplemete situarse na carpeta do proxecto e executaros seguintes comandos:
`bison -d gramatica.y`
`flex analizador_lex.l`
`gcc -Wall *.c -o calculadora -lfl -lm`

## Funcionamento da calculadora
Para exexcutar o programa executar na terminal `./calculadora`. En caso de querer **pasarlle un arquivo** (por exemplo, script.txt), entón executar `./calculadora script.txt`; a calculadora interpretará o script e devolverá o prompt ao usuario.

A calculadora/intérprete permite as seguintes **funcionalidades**:
* Operacións matemáticas básicas, así como funcións trigonométricas, logaritmos, raíces cadradas e funcións floor e ceiling.
* **Carga de ficheiros** (scripts) mediante a función `load(nome_ficheiro)`
* **Memoria de traballo** (workspace) que contén as variables asigadas polo usuario asi como o seu valor. Esta información pode consultarse co comando `workspce()` e pode eliminarse mediante `clean()`
* Funcionaidade de **non impresión**: se o usuario termina unha sentencia con ';', entón o intérprete non imprimirá por pantalla o resultado desa operación.
* Números en notación científica (por exemplo, 35.6756e109). **Todas as varaibles son tratadas como doubles**.
* Algunhas outras funcionalidades miscelaneas (ver todas co comando `help()`).

## Notas sobre a implmentación
A **táboa de símbolos** está implementada mediante unha **árbore binaria** (especificación en abb.c e abb.h). Cada nó da árbore binaria contén unha **compoñente léxica** (estrutura `compLexico` definida en ts.h), que está definida por un tipo (MYVAR, MYCONST... dependendo de se é unha variable, unha constante...), un lexema, un valor (double en caso das variables e contantes, e punterio a función no resto) e unha **flag (inicializada) para marcar se a variable foi inicializada**. Esto último require explicación: flex, cando recoñece un identificador, o engade á taboa de símbolos, independentemente de se esa variable foi inicailizada correctamente, polo que se hai un erro na inicialización da variable, esta quédase na taboa de símbolos xa que cando flex a engadiu, aínda non sabía se habería un error (eso o debe comprobar bison). A flag inicializada serve para que, cando flex lee un identificador dunha variable, o engade á táboa de símbolos con inicializada = 0; se flex atopa un erro na asiganción da variable, elimina as entradas da táboa de símbolos que teñan o valor inicializada = 0, e en caso cntrario, pon inicializada a 1, xa que a variable puido ser asignada correctamente.
