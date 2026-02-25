CC = gcc
CFLAGS = -Wall
TARGET = compilador

OBJS = main.o lexico.o ts.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main: main.c lexico.h ts.h definiciones.h
	$(CC) $(CFLAGS) main.c -o main

lexico: lexico.c lexico.h definiciones.h
	$(CC) $(CFLAGS) lexico.c -o lexico

ts: ts.c ts.h definiciones.h
	$(CC) $(CFLAGS) ts.c -o ts

clean:
	rm -f $(TARGET) main lexico ts