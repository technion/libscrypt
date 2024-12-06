/*-
 * Copyright 2007-2009 Colin Percival
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file was originally written by Colin Percival as part of the Tarsnap
 * online backup system.
 */
#ifndef _SYSENDIAN_H_
#define _SYSENDIAN_H_


/* If we don't have be64enc, the <sys/endian.h> we have isn't usable. */
#if !HAVE_DECL_BE64ENC
#undef HAVE_SYS_ENDIAN_H
#endif

#ifdef HAVE_SYS_ENDIAN_H

#include <sys/endian.h>

#else

#include <stdint.h>
#ifdef _MSC_VER
  #define INLINE __inline
#else
  #define INLINE inline
#endif

static INLINE uint32_t
be32dec(const unsigned char* p)
{
	return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) |
		((uint32_t)p[3] << 0);
}

static INLINE void
be32enc(unsigned char* p, const uint32_t x)
{
	p[0] = (x >> 24);
	p[1] = (x >> 16);
	p[2] = (x >> 8);
	p[3] = (x >> 0);
}

static INLINE uint64_t
be64dec(const unsigned char* p)
{
	return ((uint64_t)p[0] << 0) | ((uint64_t)p[1] << 8) | ((uint64_t)p[2] << 16) |
		((uint64_t)p[3] << 24) | ((uint64_t)p[4] << 32) | ((uint64_t)p[5] << 40) |
		((uint64_t)p[6] << 48) | ((uint64_t)p[7] << 56);
}

static INLINE void
be64enc(unsigned char* p, const uint64_t x)
{
	p[0] = (x >> 56);
	p[1] = (x >> 48);
	p[2] = (x >> 40);
	p[3] = (x >> 32);
	p[4] = (x >> 24);
	p[5] = (x >> 16);
	p[6] = (x >> 8);
	p[7] = (x >> 0);
}

static INLINE uint32_t
le32dec(const unsigned char* p)
{
	return ((uint32_t)p[0] << 0) | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) |
		((uint32_t)p[3] << 24);
}

static INLINE void
le32enc(unsigned char* p, const uint32_t x)
{
	p[0] = (x >> 0);
	p[1] = (x >> 8);
	p[2] = (x >> 16);
	p[3] = (x >> 24);
}

static INLINE uint64_t
le64dec(const unsigned char* p)
{
	return ((uint64_t)p[0] << 0) | ((uint64_t)p[1] << 8) | ((uint64_t)p[2] << 16) |
		((uint64_t)p[3] << 24) | ((uint64_t)p[4] << 32) | ((uint64_t)p[5] << 40) |
		((uint64_t)p[6] << 48) | ((uint64_t)p[7] << 56);
}

static INLINE void
le64enc(unsigned char* p, const uint64_t x)
{
	p[0] = (x >> 0);
	p[1] = (x >> 8);
	p[2] = (x >> 16);
	p[3] = (x >> 24);
	p[4] = (x >> 32);
	p[5] = (x >> 40);
	p[6] = (x >> 48);
	p[7] = (x >> 56);
}
#endif /* !HAVE_SYS_ENDIAN_H */

#endif /* !_SYSENDIAN_H_ */
