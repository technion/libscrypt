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
 * crypto_scrypt(passwd, passwdlen, salt, saltlen, N, r, p, buf, buflen):
 * password; duh
 * N: CPU AND RAM cost (first modifier)
 * r: RAM Cost
 * p: CPU cost (parallelisation)
 * In short, N is your main performance modifier. Values of r = 8, p = 1 are
 * standard unless you want to modify the CPU/RAM ratio.
 * Return 0 on success; or -1 on error.
 */
int crypto_scrypt(const uint8_t *, size_t, const uint8_t *, size_t, uint64_t,
    uint32_t, uint32_t, uint8_t *, size_t);

/**
 * Converts a binary string to a hex representation of that string
 * outbuf must have size of at least buf * 2 + 1.
 */
void crypto_scrypt_hexconvert(uint8_t *buf, size_t s, char *outbuf, size_t obs);

/* Converts a series of input parameters to a MCF form for storage */
void crypto_scrypt_mcf(uint32_t N, uint8_t r, uint8_t p, char *salt, char *hash, char *mcf);

/* Generates a salt. This is not a cryptographically unpredictable function,
 * but should produce appropriately randomised output for this purpose
 */
void scrypt_salt_gen(char *rand, size_t len);
#endif /* !_CRYPTO_SCRYPT_H_ */
