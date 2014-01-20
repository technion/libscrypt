
/* BASE64 libraries used internally - should not need to be packaged */

#define b64_encode_len(A) ((A+2)/3 * 4 + 1)
#define b64_decode_len(A) (A / 4 * 3 + 2)

int	libscrypt_b64_encode(unsigned char const *, size_t, char *, size_t);
int	libscrypt_b64_decode(char const *, unsigned char *, size_t);
