# Nome do executable final
TARGET = compilador

# Compilamos con gcc coa opción -Wall para que mostre todolos warnings
CC = gcc
CFLAGS = -Wall

# Localizamos os .c
SRCS = main.c lexico.c entrada.c sintactico.c abb.c ts.c erros.c

# Para compilar o programa
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Operacións de limpeza (eliminar os executables)
clean:
	rm -f $(OBJS) $(TARGET)

# Para compilar e executar
run: $(TARGET)
	./$(TARGET)