#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libscrypt.h"

#define REF1 "fdbabe1c9d3472007856e7190d01e9fe7c6ad7cbc8237830e77376634b3731622eaf30d92e22a3886ff109279d9830dac727afb94a83ee6d8360cbdfa2cc0640"


int main()
{
	uint8_t hashbuf[64];
	char outbuf[132];
	int retval;
	/**
	 * crypto_scrypt(passwd, passwdlen, salt, saltlen, N, r, p, buf, buflen):
	 * password; duh
	 * N: CPU AND RAM cost (first modifier)
	 * r: RAM Cost
	 * p: CPU cost (parallelisation)
	 * In short, N is your main performance modifier. Values of r = 8, p = 1 are
	 * standard unless you want to modify the CPU/RAM ratio.
	int crypto_scrypt(const uint8_t *, size_t, const uint8_t *, size_t, uint64_t,
    uint32_t, uint32_t, uint8_t *, size_t);
*/

	printf("Hashing with password password with salt NaCL\n");

	retval = crypto_scrypt((uint8_t*)"password",strlen("password"), (uint8_t*)"NaCl", strlen("NaCl"), 1024, 8, 16, hashbuf, sizeof(hashbuf));

	if(retval != 0)
	{
		printf("Failed to create hash of \"password\"\\n");
		exit(EXIT_FAILURE);
	}

	/* Convert the binary string to hex representation. Outbuf must be
	* at least sizeof(hashbuf) * 2 + 1
	*/
	crypto_scrypt_hexconvert(hashbuf, sizeof(hashbuf), outbuf, sizeof(outbuf));
	printf("Hex output is:\n%s\n", outbuf);

	if(strcmp(outbuf, REF1) != 0)
	{
		printf("Failed to match reference on hash\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Test vector matched!\n");
	}

	return 0;
}

void crypto_scrypt_hexconvert(uint8_t *buf, size_t s, char *outbuf, size_t obs)
{

	if (!buf || s < 1 || obs < (s * 2 + 1))
		return; 

	memset(outbuf, 0, obs);

	int i;
	for(i=0; i<=(s-1); i++)
	{
		sprintf(outbuf, "%s%02x", outbuf, (unsigned char) buf[i]);
	}

}
