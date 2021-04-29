CC=gcc
CFLAGS=-g -Wall
LFLAGS=
EXE=client server

all: $(EXE)

clean:
	rm -rf $(EXE)