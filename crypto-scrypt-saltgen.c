#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>

int libscrypt_salt_gen(uint8_t *salt, size_t len)
{
	unsigned char buf[len];
	size_t data_read = 0;
	int urandom = open("/dev/urandom", O_RDONLY);

	if (urandom < 0)
	{
		return -1;
	}

	while (data_read < len) {
		ssize_t result = read(urandom, (unsigned char*)buf + data_read, len - data_read);

		if (result < 0)
		{
			if (errno == EINTR || errno == EAGAIN) {
				continue;	
			}

			else {
				return -1;
			}
		}

		data_read += result;
	}

	close(urandom);
	memcpy(salt, buf, len);
	return 0;
}