#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libscrypt.h"
#include "modp_b64.h"

#define REF1 "fdbabe1c9d3472007856e7190d01e9fe7c6ad7cbc8237830e77376634b3731622eaf30d92e22a3886ff109279d9830dac727afb94a83ee6d8360cbdfa2cc0640"

#define REF2 "7023bdcb3afd7348461c06cd81fd38ebfda8fbba904f8e3ea9b543f6545da1f2d5432955613f0fcf62d49705242a9af9e61e85dc0d651e40dfcf017b45575887"


int main()
{
	uint8_t hashbuf[64];
	char outbuf[132];
	char mcf[256];
	char mcf2[256];
	char saltbuf[64];
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

	printf("Second test vector: pleaseletmein with SodiumChloride as salt\n");
	retval = crypto_scrypt((uint8_t*)"pleaseletmein",strlen("pleaseletmein"), (uint8_t*)"SodiumChloride", strlen("SodiumChloride"), 16384, 8, 1, hashbuf, sizeof(hashbuf));

	if(retval != 0)
	{
		printf("Failed to create hash of \"pleaseletmein\"\\n");
		exit(EXIT_FAILURE);
	}

	/* Convert the binary string to hex representation. Outbuf must be
	* at least sizeof(hashbuf) * 2 + 1
	*/
	crypto_scrypt_hexconvert(hashbuf, sizeof(hashbuf), outbuf, sizeof(outbuf));
	printf("Hex output is:\n%s\n", outbuf);

	if(strcmp(outbuf, REF2) != 0)
	{
		printf("Failed to match reference on hash\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Test vector matched!\n");
	}

	modp_b64_encode(outbuf, (char*)hashbuf, sizeof(hashbuf));
	modp_b64_encode(saltbuf, "SodiumChloride", strlen("SodiumChloride"));

	crypto_scrypt_mcf(16384, 8, 1, saltbuf, outbuf, mcf);

	/* Since later calls to scrypt_check() butcher mcf, make a second */
	strcpy(mcf2, mcf);

	printf("Testing salt generator\n");
	scrypt_salt_gen(saltbuf, 16);
	modp_b64_encode(outbuf, (char*)saltbuf, 16);
	printf("Generated %s, I guess it's random?\n", outbuf);
	
	/* Since scrypt)check butchers mcf - make a copy */
	retval = scrypt_check(mcf, "pleaseletmein");

	if(retval < 0)
	{
		printf("pleaseletmein hash failed to calculate\n");
		exit(EXIT_FAILURE);
	}
	if(retval == 0)
	{
		printf("pleaseletmein hash claimed did not verify\n");
		exit(EXIT_FAILURE);
	}
	/* retval >0 is a success */
	printf("Successfully tested pleaseletmein\n");
	
	retval = scrypt_check(mcf2, "pleasefailme");

	if(retval < 0)
	{
		printf("deliberate failhash failed to calculate\n");
		exit(EXIT_FAILURE);
	}
	if(retval > 0)
	{
		printf("pleaseletmein deliberate fail hash has passed\n");
		exit(EXIT_FAILURE);
	}

	printf("deliberate failhash failed\n");

	return 0;
}

