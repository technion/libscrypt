#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>

#ifdef _WIN32
#include <windows.h>
#else
#ifndef S_SPLINT_S /* Including this here triggers a known bug in splint */
#include <unistd.h>
#endif
#define RNGDEV "/dev/urandom"
#endif

int libscrypt_salt_gen(uint8_t *salt, size_t len)
{
#ifdef _WIN32
	HCRYPTPROV hCryptProv = NULL;
	int ret = 0;

  if (!CryptAcquireContext(&hCryptProv, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		return -1;

	if (!CryptGenRandom(hCryptProv, len, salt))
		ret = -1;

  if (!CryptReleaseContext(hCryptProv, 0))
		ret = -1;

	return ret;
#else
	unsigned char buf[len];
	size_t data_read = 0;
	int urandom = open(RNGDEV, O_RDONLY);

	if (urandom < 0)
	{
		return -1;
	}

	while (data_read < len) {
		ssize_t result = read(urandom, buf + data_read, len - data_read);

		if (result < 0)
		{
			if (errno == EINTR || errno == EAGAIN) {
				continue;	
			}

			else {
				(void)close(urandom);
				return -1;
			}
		}

		data_read += result;
	}

	/* Failures on close() shouldn't occur with O_RDONLY */
	(void)close(urandom);

	memcpy(salt, buf, len);

	return 0;
#endif
}
