#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "libscrypt.h"
#include "modp_b64.h"

int scrypt_check(char *mcf, char *password)
{

	uint32_t params;
	uint16_t N;
	uint8_t r, p;
	int retval;
	uint8_t hashbuf[64];
	char outbuf[128];
	char salt[32];
	char *tok;

	if(memcmp(mcf, "$s0", 3) != 0)
	{
		/* Only version 0 supported */
		return -1;
	}

	tok = strtok(mcf, "$");
	tok = strtok(NULL, "$");
	sscanf(tok, "%x", &params);

	tok = strtok(NULL, "$");

	p = params & 0xff;
	r = (params >> 8) & 0xff;
	N = params >> 16;
	N= pow(2, N);

	/* Useful debugging:
	printf("We've obtained salt 'N' r p of '%s' %d %d %d\n", tok, N,r,p);
	*/

	retval = modp_b64_decode(salt, tok, strlen(tok));
	retval = crypto_scrypt((uint8_t*)password,strlen(password), (uint8_t*)salt, retval, N, r, p, hashbuf, sizeof(hashbuf));

	if (retval != 0)
		return retval;

	modp_b64_encode(outbuf, (char*)hashbuf, sizeof(hashbuf));

	tok = strtok(NULL, "$");

	if(strcmp(tok, outbuf) == 0)
	{
		return 1;
	}

	return 0;
}

