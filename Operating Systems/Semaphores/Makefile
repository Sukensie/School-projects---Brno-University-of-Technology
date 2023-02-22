#Autor: Tomáš Souček (xsouce15)

CC=gcc
CFLAGS=-std=gnu99 -pthread -Wall -Wextra -pedantic -lrt -Werror
LDFLAG=-lm

all: proj2

semafor.o: proj2.c

proj2: proj2.o
	$(CC) $(CFLAGS) $^ $(LDFLAG) -o $@