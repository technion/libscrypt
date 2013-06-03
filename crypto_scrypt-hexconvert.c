#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

int crypto_scrypt_hexconvert(uint8_t *buf, size_t s, char *outbuf, size_t obs)
{

        int i;

        if (!buf || s < 1 || obs < (s * 2 + 1))
                return 0;

         memset(outbuf, 0, obs);
	

        for(i=0; i<=(s-1); i++)
        {
                sprintf(outbuf, "%s%02x", outbuf, (unsigned char) buf[i]);
        }

	return 1;
}

