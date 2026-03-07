# Nombre del ejecutable final
TARGET = main

# Compilador y flags
CC = gcc
CFLAGS = -Wall

# Localizar los archivos .c
SRCS = main.c lexico.c entrada.c sintactico.c abb.c ts.c


# Regla por defecto: compilar el ejecutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)


clean:
	rm -f $(OBJS) $(TARGET)


run: $(TARGET)
	./$(TARGET)