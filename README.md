# Analizador léxico de D
Programa que simula un **analizador lexico** para a linguaxe de programación **D**. De momento só analiza correctamente o programa `regression.d`.

## Contorna de execución
* Necesaria versión de **gcc 13.3.0** ou superior
* Testeado en **Ubuntu 24.04.4 LTS**

### Instuccións de compilación
Inclúese un makefile:
* `make` para compilar o programa
* `make clean` para eliminar o executable
* `make run` para compilar e executar o programa

En caso de querer compilalo a man, simplemete situarse na carpeta do proxecto e executar `gcc -Wall -o compilador main.c lexico.c entrada.c sintactico.c abb.c ts.c erros.c`

## Notas
* Para o sistema de entrada emprégase *un buffer físico* con *dous buffers lóxicos* (bloques A e B). Para **cambiar o tamaño dos bloques** débese modificar a constante `T_BUF`, situada na liña 12 de entrada.c.
* O arquivo regression.d debe estar situado na carpeta dende a que se executa o programa. O arquivo a "compilar" pódese cambiar modificando a chamada a `fopen` na fucnión `void iniciar_SE()` (liña 91 aprox.). Compilar outro arquivo pode dar lugar a comportamentos inesperados.
