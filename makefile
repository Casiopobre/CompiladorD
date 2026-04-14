# Nome do executable final
TARGET = compilador

# Compilamos con gcc coa opción -Wall (erros), -lfl (flex), -lm (lib. matematica)
CC = gcc
CFLAGS = -Wall -lfl -lm

# Localizamos os .c
SRCS = *.c

.PHONY: all clean run $(TARGET)

all: $(TARGET)

bison:
	bison -d gramatica.y

flex:
	flex analizador_lex.l


# Para compilar o programa
$(TARGET):
	bison -d gramatica.y
	flex analizador_lex.l
	$(CC) $(SRCS) $(CFLAGS) -o $(TARGET)

# Operacións de limpeza
clean:
	rm -f $(TARGET)

# Para compilar e executar
run: $(TARGET)
	./$(TARGET)