PROJ=ipk-sniffer 
FILES = sniffer.c
CC=gcc
CFLAGS=-std=c99 -Wextra -pedantic -lm -fcommon -pthread -D_DEFAULT_SOURCE -lpcap

all: $(PROJ)

$(PROJ) : $(FILES)
	$(CC) $(FILES) -o ipk-sniffer $(CFLAGS)

clean:
	rm -f *.o ipk-sniffer
