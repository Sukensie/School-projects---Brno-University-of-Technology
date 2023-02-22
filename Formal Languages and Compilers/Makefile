PROJ=proj
FILES = scanner.c parser.c code.c symtable.c errors.c expr.c
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon -pthread

all: $(PROJ)

$(PROJ) : $(FILES)
	$(CC) $(FILES) -o proj $(CFLAGS)

clean:
	rm -f *.o $(PROJ)
