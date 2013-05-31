CC=gcc
CFLAGS=-O2 -Wall 
all: reference

OBJS= crypto_scrypt-nosse.o sha256.o crypto_scrypt-hexconvert.o crypto-mcf.o modp_b64.o crypto-scrypt-saltgen.o crypto_scrypt-check.o

crypto-mcf.o: crypto-mcf.c
	gcc -Wall -std=gnu99 -c -o crypto-mcf.o crypto-mcf.c

library: $(OBJS)
	ar rcs libscrypt.a  $(OBJS)

reference: library main.o
	gcc -Wall -o reference main.o libscrypt.a -lm
clean:
	rm -f *.o reference libscrypt.a

