CC=gcc
CFLAGS=-O2 -Wall -g -D_FORTIFY_SOURCE=2 -fstack-protector -fpic
all: reference

OBJS= crypto_scrypt-nosse.o sha256.o crypto_scrypt-hexconvert.o crypto-mcf.o modp_b64.o crypto-scrypt-saltgen.o crypto_scrypt-check.o crypto_scrypt-hash.o


library: $(OBJS)
	gcc -shared -Wl,-soname,libscrypt.so -o libscrypt.so -lc  $(OBJS)

reference: library main.o
	gcc -Wall -o reference main.o -Wl,-rpath=. -L.  -lm  -lscrypt

clean:
	rm -f *.o reference libscrypt.so

check: all
	./reference
