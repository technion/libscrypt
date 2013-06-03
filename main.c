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

	printf("TEST ONE: Direct call to reference function with password 'password' and salt 'NaCL'\n");

	retval = crypto_scrypt((uint8_t*)"password",strlen("password"), (uint8_t*)"NaCl", strlen("NaCl"), 1024, 8, 16, hashbuf, sizeof(hashbuf));

	if(retval != 0)
	{
		printf("TEST ONE FAILED: Failed to create hash of \"password\"\\n");
		exit(EXIT_FAILURE);
	}

	printf("TEST ONE: SUCCESSFUL\n");

	/* Convert the binary string to hex representation. Outbuf must be
	* at least sizeof(hashbuf) * 2 + 1
	* Returns 0 on fail, 1 on success
	*/
	printf("TEST TWO: Convert binary output to hex\n");
	retval = crypto_scrypt_hexconvert(hashbuf, sizeof(hashbuf), outbuf, sizeof(outbuf));
	if(!retval)
	{
		printf("TEST TWO: FAILED\n");
		exit(EXIT_FAILURE);
	}
	printf("TEST TWO: SUCCESSFUL, Hex output is:\n%s\n", outbuf);

	printf("TEST THREE: Compare hex output to reference hash output\n");

	/* REF1 is a reference vector from Colin's implementation. */
	if(strcmp(outbuf, REF1) != 0)
	{
		printf("TEST THREE: FAILED to match reference on hash\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("TEST THREE: SUCCESSUL, Test vector matched!\n");
	}

	printf("TEST FOUR: Direct call to reference function with pleaseletmein password and SodiumChloride as salt\n");

	/* Tests 4-6 repeat tests 1-3 with a different reference vector */

	retval = crypto_scrypt((uint8_t*)"pleaseletmein",strlen("pleaseletmein"), (uint8_t*)"SodiumChloride", strlen("SodiumChloride"), 16384, 8, 1, hashbuf, sizeof(hashbuf));

	if(retval != 0)
	{
		printf("TEST FOUR FAILED: Failed to create hash of 'pleaseletmein'\n");
		exit(EXIT_FAILURE);
	}

	printf("TEST FOUR: SUCCESSFUL\n");

	/* Convert the binary string to hex representation. Outbuf must be
	* at least sizeof(hashbuf) * 2 + 1
	*/
	printf("TEST FIVE: Convert binary output to hex\n");
	retval = crypto_scrypt_hexconvert(hashbuf, sizeof(hashbuf), outbuf, sizeof(outbuf));
	if(!retval)
	{
		printf("TEST FIVE: FAILED\n");
		exit(EXIT_FAILURE);
	}
	printf("TEST FIVE: SUCCESSFUL, Hex output is:\n%s\n", outbuf);

	printf("TEST SIX: Compare hex output to reference hash output\n");

	if(strcmp(outbuf, REF2) != 0)
	{
		printf("TEST SIX: FAILED to match reference on hash\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("TEST SIX: SUCCESSUL, Test vector matched!\n");
	}

	/* This function will convert the binary output to BASE64. Although
	* we converted to hex for the reference vectors, BASE64 is more useful.
	* Returns -1 on error, else returns length.
	* Correct buffer length can be determined using the below function if
	retuired.
	* char* dest = (char*) malloc(modp_b64_encode_len);
	*/

	printf("TEST SEVEN: BASE64 encoding the salt and hash output\n");

	retval = modp_b64_encode(outbuf, (char*)hashbuf, sizeof(hashbuf));
	if(retval == -1)
	{
		printf("TEST SEVEN FAILED\n");
		exit(EXIT_FAILURE);
	}
	retval = modp_b64_encode(saltbuf, "SodiumChloride", strlen("SodiumChloride"));
	if(retval == -1)
	{
		printf("TEST SEVEN FAILED\n");
		exit(EXIT_FAILURE);
	}

	printf("TEST SEVEN: SUCCESSFUL\n");

	printf("TEST EIGHT: Create an MCF format output\n");

	/* Creates a standard format output
	* int crypto_scrypt_mcf(uint32_t N, uint32_t r, uint32_t p, char *salt, char *hash, char *mcf); 
	* Returns 0 on error, most likely reason is log2(N) not an integer.
	*/
	retval = crypto_scrypt_mcf(16384, 8, 1, saltbuf, outbuf, mcf);
	if(!retval)
	{
		printf("TEST EIGHT FAILED\n");
		exit(EXIT_FAILURE);
	}

	printf("TEST EIGHT: SUCCESSFUL, calculated mcf\n%s\n", mcf);

	/* Since later calls to scrypt_check() butcher mcf, make a second */
	strcpy(mcf2, mcf);

	/* Couldn't be simpler - for a given mcf, check is the password is valid
	* Returns < 0 on failure to calculate hash
	* 0 if password incorrect
	* >1 if password correct
	*/
	
	printf("TEST NINE: Password verify on given MCF\n");
	retval = scrypt_check(mcf, "pleaseletmein");

	if(retval < 0)
	{
		printf("TEST NINE: FAILED, hash failed to calculate\n");
		exit(EXIT_FAILURE);
	}
	if(retval == 0)
	{
		printf("TEST NINE: FAILED, claimed pleaseletmein hash claimed did not verify\n");
		exit(EXIT_FAILURE);
	}
	/* retval >0 is a success */
	printf("TEST NINE: SUCCESSFUL,  tested pleaseletmein password\n");
	
	printf("TEST TEN: Password verify on same MCF, incorrect password\n");
	retval = scrypt_check(mcf2, "pleasefailme");

	if(retval < 0)
	{
		printf("TEST TEN: FAILED, hash failed to calculate\n");
		exit(EXIT_FAILURE);
	}
	if(retval > 0)
	{
		printf("TEST TEN: FAILED,  fail hash has passed\n");
		exit(EXIT_FAILURE);
	}

	printf("TEST TEN: SUCCESSFUL, refused incorrect password\n");

	printf("TEST ELEVEN: Testing salt generator\n");
	/* TODO: I'm not presently sure how this function could fail */
	scrypt_salt_gen(saltbuf, 16);

	retval = modp_b64_encode(saltbuf, (char*)saltbuf, 16);
	if(retval == -1)
	{
		printf("TEST ELEVEN FAILED\n");
		exit(EXIT_FAILURE);
	}
	printf("TEST ELEVEN: SUCCESSFUL, Generated %s\n", outbuf);

	printf("TEST TWELVE: Simple hash creation\n");

	retval = crypto_scrypt_hash(outbuf, "My cats's breath smells like cat food", 16384, 8, 16);
	if(!retval)
	{
		printf("TEST TWELVE: FAILED, Failed to create simple hash\n");
		exit(EXIT_FAILURE);
	}
	printf("TEST TWELVE: SUCCESSSFUL. Received the following from simple hash:\n%s\n", outbuf);

	return 0;
}

