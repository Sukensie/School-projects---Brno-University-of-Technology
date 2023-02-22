CC=gcc
CFLAGS=-std=c99 -Wextra -pedantic -lm -fcommon -pthread

all: sender receiver

sender: sender/dns_sender.c library.h
	$(CC) -g sender/dns_sender.c -o sender/dns_sender $(CFLAGS)

receiver: receiver/dns_receiver.c 
	$(CC) -g receiver/dns_receiver.c -o receiver/dns_receiver $(CFLAGS)

.PHONY: clean sender receiver

clean:
	rm -f *.o ./sender/dns_sender ./receiver/dns_receiver
