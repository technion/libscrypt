#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "b64.h"
#include "libscrypt.h"

int libscrypt_hash(char *dst, char *passphrase, uint32_t N, uint8_t r, uint8_t p)
{

	int retval;
	uint8_t salt[16];
	uint8_t	hashbuf[64];
	char outbuf[256];
	char saltbuf[256];

	if(libscrypt_salt_gen(salt, 16) == -1)
	{
		return 0;
	}

	retval = libscrypt_scrypt((uint8_t*)passphrase,strlen(passphrase), (uint8_t*)salt, sizeof(salt), N, r, p, hashbuf, sizeof(hashbuf));
	if(retval == -1)
		return 0;

	retval = libscrypt_b64_encode((unsigned char*)hashbuf, sizeof(hashbuf),
			outbuf, sizeof(outbuf));
	if(retval == -1)
		return 0;
	
	retval = libscrypt_b64_encode((unsigned char *)salt, sizeof(salt),
			saltbuf, sizeof(saltbuf));
	if(retval == -1)
		return 0;

	retval = libscrypt_mcf(N, r, p, saltbuf, outbuf, dst);
	if(!retval)
		return 0;

	return 1;
}
