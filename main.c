#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "crypto_scrypt.h"


void scrypt_hexprint(const uint8_t *, size_t, const uint8_t *, size_t, uint64_t,
    uint32_t, uint32_t);

int main()
{
/**
 * crypto_scrypt(passwd, passwdlen, salt, saltlen, N, r, p, buf, buflen):
 * password; duh
 * N: CPU AND RAM cost (first modifier)
 * r: RAM Cost
 * p: CPU cost (parallelisation)
 * In short, N is your main performance modifier. Values or r = 8, p = 1 are
 * standard unless you want to modify the CPU/RAM ratio.
int crypto_scrypt(const uint8_t *, size_t, const uint8_t *, size_t, uint64_t,
    uint32_t, uint32_t, uint8_t *, size_t);
*/

	scrypt_hexprint((uint8_t*)"password",strlen("password"), (uint8_t*)"NaCl", strlen("NaCl"), 1024, 8, 16);

	return 0;
}

void scrypt_hexprint(const uint8_t *passwd, size_t passwdlen, 
	const uint8_t *salt, size_t saltlen, uint64_t N, uint32_t r, 
	uint32_t p)
{
	uint8_t buf[64];

	crypto_scrypt(passwd, passwdlen, salt, saltlen,N,r,p,buf,(size_t)64);

	printf("Hex out for password password with salt NaCL is:\n");
	int i;
	for(i=0; i<63; i++)
	{
		printf("%x ", (unsigned char) buf[i]);
	}

	printf("\n");
}
