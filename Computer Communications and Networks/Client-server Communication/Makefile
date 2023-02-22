PROJ=hinfosvc 
FILES = server.c
CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic -lm -fcommon -pthread

all: $(PROJ)

$(PROJ) : $(FILES)
	$(CC) $(FILES) -o hinfosvc $(CFLAGS)

clean:
	rm -f *.o hinfosvc
