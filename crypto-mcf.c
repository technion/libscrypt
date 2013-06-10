#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>

#include <math.h>

#include "libscrypt.h"

/* Although log2 exists in GNU99 C, more portable code shouldn't use it
* Note that this function returns a float and hence is not compatible with the
* GNU prototype
*/
static double scrypt_log2( uint32_t n )  
{  
	// log(n)/log(2) is log2.  
	double temp;
	/* Using the temp variable keeps splint happy */
	temp = log(2);
	return (log((double)n) / temp);
}

int libscrypt_mcf(uint32_t N, uint32_t r, uint32_t p, char *salt, char *hash, char *mcf)
{


	uint32_t params;
	int s;
	double t, t2, fracpart;

	if(!mcf || !hash)
		return 0;
	/* Although larger values of r, p are valid in scrypt, this mcf format
	* limits to 8 bits. If your number is larger, current computers will
	* struggle
	*/
	if(r > (uint8_t)(-1) || p > (uint8_t)(-1))
		return 0;


	t = scrypt_log2(N);

	/* The "whole numebr" check below is non-trivial due to precision
	* issues, where you could printf("%d", (int)t) and find yourself
	* looking at (expected value) -1
	*/
	fracpart = modf(t, &t2);
	if(fracpart > DBL_EPSILON)
		return 0;
		
	params = (r << 8) + p;
	params += (uint32_t)t << 16;
	
	/* Using snprintf - not checking for overflows. We've already
	* determined that mcf should be defined as at least SCRYPT_MCF_LEN
	* in length 
	*/
	s = snprintf(mcf, SCRYPT_MCF_LEN, "$s0$%06x$%s$%s", (unsigned int)params, salt, hash);
	if (s > SCRYPT_MCF_LEN)
		return 0;

	return 1;
}	
