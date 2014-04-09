#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#else
#include <fcntl.h>
#endif

int libscrypt_salt_gen(uint8_t *salt, size_t len)
{
	unsigned char buf[len];
#ifdef _WIN32
	static HCRYPTPROV provider;
	if (!CryptAcquireContext(&provider, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) 
	{
		return -1;
	}

	if (!CryptGenRandom(provider, len, buf))
	{
		return -1;
	}

	if(!CryptReleaseContext(provider, 0)) {
		return -1;
	}
#else
	int data_read = 0;
	int urandom = open("/dev/urandom", O_RDONLY);

	if (urandom < 0)
	{
		return -1;
	}

	while (data_read < len) {
		ssize_t result = read(urandom, buf, len);

		if (result < 0)
		{
			if (errno == EINTR) {
				continue;	
			}

			else {
				return -1;
			}
		}

		data_read += result;
	}

	close(urandom);
#endif
	memcpy(salt, buf, len);
	return 0;
}