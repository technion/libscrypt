#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "libscrypt.h"
#include "modp_b64.h"

int crypto_scrypt_hash(char *dst, char *passphrase, uint32_t N, uint8_t r, uint8_t p)
{

	int retval;
	char salt[16];
	uint8_t	hashbuf[64];
	char outbuf[256];
	char saltbuf[256];

	scrypt_salt_gen(salt, 16);

	retval = crypto_scrypt((uint8_t*)passphrase,strlen(passphrase), (uint8_t*)salt, sizeof(salt), N, r, p, hashbuf, sizeof(hashbuf));
	if(retval == -1)
		return 0;

        retval = modp_b64_encode(outbuf, (char*)hashbuf, sizeof(hashbuf));
	if(retval == -1)
		return 0;
	
        retval = modp_b64_encode(saltbuf, salt, sizeof(salt));
	if(retval == -1)
		return 0;

        retval = crypto_scrypt_mcf(N, r, p, saltbuf, outbuf, dst);
	if(retval == -1)
		return 0;

	return 1;
}
