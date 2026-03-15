# Nome do executable final
TARGET = compilador

# Compilamos con gcc coa opción -Wall
CC = gcc
CFLAGS = -Wall

# Localizamos os .c
SRCS = main.c lexico.c entrada.c sintactico.c abb.c ts.c erros.c

.PHONY: all clean run $(TARGET)

all: $(TARGET)

# Para compilar o programa
$(TARGET):
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Operacións de limpeza
clean:
	rm -f $(TARGET)

# Para compilar e executar
run: $(TARGET)
	./$(TARGET)