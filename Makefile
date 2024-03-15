

CC=gcc                              # překladač jazyka C
CFLAGS=-std=c11 -Wall -pedantic -g  # parametry překladače

keyfilter: keyfilter.c
	$(CC) $(CFLAGS) keyfilter.c -o keyfilter

clean: 
	rm keyfilter
