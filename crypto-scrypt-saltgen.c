#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sha256.h"


void libscrypt_salt_gen(uint8_t *rand, size_t len)
{

	unsigned char buf[32];
	time_t current_time;
	char *c_time_string;

	SHA256_CTX ctx;

	SHA256_Init(&ctx );
	current_time = time(NULL);
	c_time_string = ctime(&current_time);
	SHA256_Update(&ctx, c_time_string, strlen(c_time_string));
	SHA256_Final(buf, &ctx);

	memcpy(rand, buf, len);

}
