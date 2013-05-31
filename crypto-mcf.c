#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include <math.h>

void crypto_scrypt_mcf(uint32_t N, uint8_t r, uint8_t p, char *salt, char *hash, char *mcf)
{


	uint32_t params;
	double t;

	t = log2(N);

	if (t != (int)t)
		return; /* Not a valid state */
		
	params = (r << 8) + p;
	params += (uint32_t)t << 16;
	
	sprintf(mcf, "$s0$%06x$%s$%s\n", params, salt, hash);
}	
