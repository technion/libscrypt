#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#pragma comment(lib, "advapi32")
#else
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#ifndef S_SPLINT_S /* Including this here triggers a known bug in splint */
#include <unistd.h>
#endif
#endif


#define RNGDEV "/dev/urandom"

int libscrypt_salt_gen(uint8_t *salt, size_t len)
{
#ifdef _WIN32
    HCRYPTPROV hp;
    BOOL ret = FALSE;
    if (CryptAcquireContextW(&hp, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT)) {
        ret = CryptGenRandom(hp, sz, p));
        CryptReleaseContext(hp, 0);
    }
    return ret ? 0 : -1;
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
