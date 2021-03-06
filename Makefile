CC = gcc
INCLUDE_PATH = /home/xboxhigh/msort/
LIB_PATH =-L/home/xboxhigh/msort/
CFLAGS =-O7 -static -Wall
LIBS=
OBJ= Main.o MSort.o Heap.o 
BIN= msort


.SUFFIXES: .o .c

.c.o:
	${CC}  ${CFLAGS} -c $*.c

all:Target

Target: ${OBJ}
	${CC} ${CFLAGS} -o ${BIN} ${OBJ} libgais_util.a

clean:
	rm -rf ${BIN} ${OBJ} ${BIN}.core

cleanbak:
	rm -rf *~
