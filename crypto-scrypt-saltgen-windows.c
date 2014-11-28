#ifdef _WIN32 //_WIN32 defined on 64-bit Windows too
#if _WIN32_WINNT < _WIN32_WINNT_WINXP
#error Not supported Windows version. Windows XP or higher is required
#endif
#include <stdint.h>
#include <windows.h>
#include <wincrypt.h>
#pragma comment(lib, "advapi32.lib")


int libscrypt_salt_gen(uint8_t *salt, size_t len)
{
	HCRYPTPROV hCryptProv = 0;
	BOOL returnCode;
	if (!CryptAcquireContextW(&hCryptProv, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
		return -1;
	returnCode = CryptGenRandom(hCryptProv, len, salt);
	CryptReleaseContext(hCryptProv, 0);
	return (returnCode == FALSE) ? -1 : 0;
}

#endif