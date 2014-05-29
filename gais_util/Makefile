CFLAGS =-O3 -static -Wall
INCLUDE=FileUtil.h Util.h Error.h Inifile.h Log.h MFile.h TimeUtil.h
LIBS=
CC= gcc
LIB= libgais_util.a
OBJ= FileUtil.o Util.o Error.o Inifile.o Log.o MFile.o TimeUtil.o

.SUFFIXES: .o .c

.cpp.o:
	${CC}  ${CFLAGS} -c $*.c

all:Target

Target: ${OBJ}
	ar rcv $(LIB) $(OBJ) $(LIBS)
	ranlib $(LIB)
clean:
	rm -rf ${LIB} ${OBJ} ${BIN}.core
cleanbak:
	rm -rf *~
