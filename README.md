libscrypt
=========
Linux scrypt shared library.

Full credit to algorithm designer and example code from Colin Percival here:
http://www.tarsnap.com/scrypt.html

Utilises BASE64 encoding library from ISC.

Official project page, including stable tarballs found here:
http://www.lolware.net/libscrypt.html

Simple hashing interface

The (reference) internal hashing function can be directly called as follows:

    int libscrypt_scrypt(const uint8_t *passwd, size_t passwdlen,
            const uint8_t *salt, size_t saltlen, uint64_t N, uint32_t r, 
            uint32_t p, /*@out@*/ uint8_t *buf, size_t buflen);

Libscrypt's easier to use interface wraps this up to deal with the salt and produce BASE64 output as so:

    int libscrypt_hash(char *dst, char *passphrase, uint32_t N, uint8_t r, uint8_t p);

Sane constants have been created for N, r and p so you can create a has like this:

    libscrypt_hash(outbuf, "My cats's breath smells like cat food", SCRYPT_N, SCRYPT_r, SCRYPT_p);

Output stored in "outbuf" is stored in a standardised MCF form, which means includes the randomly created, 128 bit salt, all N, r and p values, and a BASE64 encoded version of the hash. The entire MCF can be stored in a database, and compared for use as below:

    retval = libscrypt_check(mcf, "pleasefailme");
    retval < 0 error
    retval = 0 password incorrect
    retval > 0 pass

mcf should be defined as at least SCRYPT_MCF_LEN in size.

A number of internal functions are exposed, and users wishing to create more complex use cases should consult the header file, which is aimed at documenting the API fully.

The test reference is also aimed at providing a well documented use case.
Building
--------
    make
    make check
Check the Makefile for advice on linking against your application.

OSX
-----
Please compile and install with:

    make LDFLAGS= CFLAGS_EXTRA=
    make install-osx


BUGS
----
SCRYPT_* constants are probably a little high for something like a Raspberry pi. Using '1' as SCRYPT_p is acceptable from a security and performance standpoint if needed. 

Notes on Code Development
------------------------

Code is now declared "stable", the master branch will always be "stable" and development will be done on branches.
The reference machines are Fedora, CentOS, FreeBSD and Raspbian, and the code is expected to compile and run on all of these before being moved to stable branch.
Full transparancy on the regular application of thorough testing can be found by reviewing recent test harness results here:
http://www.lolware.net/libscrypttesting.txt

Changenotes
-----------
v1.1a: Single Makefile line change. I wouldn't ordinarily tag this as a new "release", but the purpose here is to assist with packaging in distributions.

v1.12: The static library is built, but no longer installed by default. You can install it with "make install-static". This is because static libraries are not typically bundled in packages.

v1.13: Minor packaging related update

v1.15: Replaced the b64 libraries with more portable one from ISC. Now tested and verified on a wider variety of architectures. Note, libscrypt_b64_encrypt was originally an exported function. This is no longer the case as it is considered an internal function only.

v1.18: God damnit Apple

