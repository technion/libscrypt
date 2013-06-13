#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "libscrypt.h"

/* pow() works with doubles. Sounds like it should cast to int correctly,
* but doesn't always. This is faster anyway
*/
static uint16_t ipow(uint16_t base, uint32_t exp)
{
    uint16_t result = 1;
    while (exp != 0)
    {
        if ((exp & 1) != 0)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}

int libscrypt_check(char *mcf, char *password)
{

	uint32_t params;
	uint16_t N;
	uint8_t r, p;
	int retval;
	uint8_t hashbuf[64];
	char outbuf[128];
	char salt[32];
	char *tok;

	if(memcmp(mcf, SCRYPT_MCF_ID, 3) != 0)
	{
		/* Only version 0 supported */
		return -1;
	}

	tok = strtok(mcf, "$");
	tok = strtok(NULL, "$");

	if ( !tok )
		return -1;

	params = (uint32_t)strtoul(tok, NULL, 16);
	if ( params == 0 )
		return -1;

	tok = strtok(NULL, "$");

	if ( !tok )
		return -1;

	p = params & 0xff;
	r = (params >> 8) & 0xff;
	N = params >> 16;
	N = ipow(2, N);

	/* Useful debugging:
	printf("We've obtained salt 'N' r p of '%s' %d %d %d\n", tok, N,r,p);
	*/

	retval = libscrypt_b64_decode(salt, tok, strlen(tok));
	if (retval < 1)
		return -1;
	retval = libscrypt_scrypt((uint8_t*)password,strlen(password), (uint8_t*)salt, (uint32_t)retval, N, r, p, hashbuf, sizeof(hashbuf));

	if (retval != 0)
		return retval;

	retval = libscrypt_b64_encode(outbuf, (char*)hashbuf, sizeof(hashbuf));
	if (retval == 0)
		return -1;

	tok = strtok(NULL, "$");

	if ( !tok )
		return -1;

	if(strcmp(tok, outbuf) == 0)
	{
		return 1;
	}

	return 0;
}

