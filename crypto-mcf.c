#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>

#include <math.h>

#ifndef S_SPLINT_S /* Including this here triggers a known bug in splint */
#include <unistd.h>
#endif

#include "libscrypt.h"

/* ilog2 for powers of two */
static int scrypt_ilog2(uint32_t n)
{
	/* Check for a valid power of two */
	if (n < 2 || (n & (n - 1)))
		return -1;
	int t = 1;
	while ((1 << t) < n)
		t++;
	return t;
}

int libscrypt_mcf(uint32_t N, uint32_t r, uint32_t p, const char *salt,
		const char *hash, char *mcf)
{


	uint32_t params;
	int s, t;

	if(!mcf || !hash)
		return 0;
	/* Although larger values of r, p are valid in scrypt, this mcf format
	* limits to 8 bits. If your number is larger, current computers will
	* struggle
	*/
	if(r > (uint8_t)(-1) || p > (uint8_t)(-1))
		return 0;

	t = scrypt_ilog2(N);
	if (t < 1)
		return 0;
		
	params = (r << 8) + p;
	params += (uint32_t)t << 16;
	
	/* Using snprintf - not checking for overflows. We've already
	* determined that mcf should be defined as at least SCRYPT_MCF_LEN
	* in length 
	*/
	s = snprintf(mcf, SCRYPT_MCF_LEN,  SCRYPT_MCF_ID "$%06x$%s$%s", (unsigned int)params, salt, hash);
	if (s > SCRYPT_MCF_LEN)
		return 0;

	return 1;
}	
