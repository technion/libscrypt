#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "b64.h"
#include "slowequals.h"
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

int libscrypt_check(char *mcf, const char *password)
{
	/* Return values:
	* <0 error
	* == 0 password incorrect
	* >0 correct password
	*/

	uint32_t params;
	uint16_t N;
	uint8_t r, p;
	int retval;
	uint8_t hashbuf[64];
	char outbuf[128];
	uint8_t salt[32];
	char *tok;

	if(memcmp(mcf, SCRYPT_MCF_ID, 3) != 0)
	{
		/* Only version 0 supported */
		return -1;
	}

	tok = strtok(mcf, "$");
	if ( !tok )
		return -1;

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

	memset(salt, 0, sizeof(salt)); /* Keeps splint happy */
	retval = libscrypt_b64_decode(tok, (unsigned char*)salt, sizeof(salt));
	if (retval < 1)
		return -1;

	retval = libscrypt_scrypt((uint8_t*)password, strlen(password), salt,
            (uint32_t)retval, N, r, p, hashbuf, sizeof(hashbuf));

	if (retval != 0)
		return retval;

	retval = libscrypt_b64_encode((unsigned char*)hashbuf, sizeof(hashbuf), 
            outbuf, sizeof(outbuf));

	if (retval == 0)
		return -1;

	tok = strtok(NULL, "$");

	if ( !tok )
		return -1;

	if(slow_equals(tok, outbuf) == 0)
	{
		return 0;
	}

	return 1; /* This is the "else" condition */
}

