# Analizador léxico de D
Programa que "simula" un **analizador lexico** para a linguaxe de programación **D** empregando a ferramenta FLEX. De momento só analiza correctamente o programa `regression.d`.

## Contorna de execución
* Necesaria versión de **gcc 13.3.0** ou superior
* Testeado en **Ubuntu 24.04.4 LTS**
* Versión de FLEX empregada: **flex 2.6.4**

### Instuccións de compilación
Inclúese un makefile:
* `make` para compilar o programa (incluíndo o ficheirio de especificación de flex)
* `make clean` para eliminar o executable
* `make run` para compilar e executar o programa. Por defecto executa o programa co ficheiro `regression.d`, pero se pode cambiar engadindo a opción `FILE="nome_ficheiro"`.

En caso de querer compilalo a man, simplemete situarse na carpeta do proxecto e executar: `gcc -Wall -o compilador *.c -lfl`

En caso de querer compilar o arquivo de definición de flex, executar o comando: `flex especificacionD.l`
