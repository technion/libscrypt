CC=gcc
CFLAGS=-O2 -Wall -g -D_FORTIFY_SOURCE=2 -fstack-protector
all: reference

OBJS= crypto_scrypt-nosse.o sha256.o crypto_scrypt-hexconvert.o crypto-mcf.o modp_b64.o crypto-scrypt-saltgen.o crypto_scrypt-check.o crypto_scrypt-hash.o


library: $(OBJS)
	ar rcs libscrypt.a  $(OBJS)

reference: library main.o
	gcc -Wall -o reference main.o libscrypt.a -lm
clean:
	rm -f *.o reference libscrypt.a

