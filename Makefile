CC=gcc
CFLAGS=-O2 -Wall 
all: reference

OBJS= crypto_scrypt-nosse.o sha256.o 


library: $(OBJS)
	ar rcs libscrypt.a  $(OBJS)

reference: library main.o
	gcc -o reference main.o libscrypt.a
clean:
	rm -f *.o reference libscrypt.a

