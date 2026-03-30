# Nome do executable final
TARGET = compilador

# Compilamos con gcc coa opción -Wall
CC = gcc
CFLAGS = -Wall -lfl

# Localizamos os .c
SRCS = *.c

# Para o arquivo
FILE ?= regression.d

.PHONY: all clean run $(TARGET)

all: $(TARGET)

# Para compilar o programa
$(TARGET):
	flex especificacionD.l
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Operacións de limpeza
clean:
	rm -f $(TARGET)

# Para compilar e executar
run: $(TARGET)
	./$(TARGET) $(FILE)