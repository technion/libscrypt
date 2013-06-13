/*-
 */
#ifndef _CRYPTO_SCRYPT_H_
#define _CRYPTO_SCRYPT_H_


#include <stdint.h>

/**
 * crypto_scrypt(passwd, passwdlen, salt, saltlen, N, r, p, buf, buflen):
 * Compute scrypt(passwd[0 .. passwdlen - 1], salt[0 .. saltlen - 1], N, r,
 * p, buflen) and write the result into buf.  The parameters r, p, and buflen
 * must satisfy r * p < 2^30 and buflen <= (2^32 - 1) * 32.  The parameter N
 * must be a power of 2 greater than 1.
 *
 * libscrypt_scrypt(passwd, passwdlen, salt, saltlen, N, r, p, buf, buflen):
 * password; duh
 * N: CPU AND RAM cost (first modifier)
 * r: RAM Cost
 * p: CPU cost (parallelisation)
 * In short, N is your main performance modifier. Values of r = 8, p = 1 are
 * standard unless you want to modify the CPU/RAM ratio.
 * Return 0 on success; or -1 on error.
 */
int libscrypt_scrypt(const uint8_t *, size_t, const uint8_t *, size_t, uint64_t,
    uint32_t, uint32_t, /*@out@*/ uint8_t *, size_t);

/**
 * Converts a binary string to a hex representation of that string
 * outbuf must have size of at least buf * 2 + 1.
 */
int libscrypt_hexconvert(uint8_t *buf, size_t s, char *outbuf, size_t obs);

/* Converts a series of input parameters to a MCF form for storage */
int libscrypt_mcf(uint32_t N, uint32_t r, uint32_t p, char *salt, char *hash, char *mcf);

/* Generates a salt. This is not a cryptographically unpredictable function,
 * but should produce appropriately randomised output for this purpose
 */
void libscrypt_salt_gen(/*@out@*/ char *rand, size_t len);

/* Checks a given MCF against a password */
int libscrypt_check(char *mcf, char *password);

/* Creates a hash of a passphrase using a randomly generated salt */
int libscrypt_hash(char *dst, char* passphrase, uint32_t N, uint8_t r, uint8_t p);

int libscrypt_b64_encode(/*@out@*/ char* dest, const char* str, size_t len);
int libscrypt_b64_decode(/*@out@*/ char* dest, const char* src, size_t len);


/* Sane default values */
#define SCRYPT_HASH_LEN 64 /* This can be user defined - 
 *but 64 is the reference size
 */
#define SCRYPT_MCF_LEN 124 /* mcf is 120 byte + nul */
#define SCRYPT_MCF_ID "$s1"
#define SCRYPT_N 16384
#define SCRYPT_r 8
#define SCRYPT_p 16
#endif /* !_CRYPTO_SCRYPT_H_ */
