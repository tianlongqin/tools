/*
 * This file is tools member.
 *
 * Copyright (c) 2015-2019, tianlongqin, <qtl_linux@163.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function:  tools include swap.
 * Created on: 2019-11-21
 */
#ifndef __SWAP_HH_19910612
#define __SWAP_HH_19910612
#include <stdint.h>

#define bswap_16(x) ((uint16_t)(                         \
				(((uint16_t)(x) & (uint16_t)0x00ffU) << 8) |                  \
				(((uint16_t)(x) & (uint16_t)0xff00U) >> 8)))

#define bswap_32(x) ((uint32_t)(				\
				(((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) |		\
				(((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) |		\
				(((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) |		\
				(((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))

#define bswap_64(x) ((uint64_t)(				\
				(((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 56) |	\
				(((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 40) |	\
				(((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 24) |	\
				(((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  8) |	\
				(((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  8) |	\
				(((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 24) |	\
				(((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 40) |	\
				(((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 56)))

#define __swahw32(x) ((uint32_t)(			\
				(((uint32_t)(x) & (uint32_t)0x0000ffffUL) << 16) |		\
				(((uint32_t)(x) & (uint32_t)0xffff0000UL) >> 16)))

#define __swahb32(x) ((uint32_t)(			\
				(((uint32_t)(x) & (uint32_t)0x00ff00ffUL) << 8) |		\
				(((uint32_t)(x) & (uint32_t)0xff00ff00UL) >> 8)))


#if defined(__BYTE_ORDER) ? __BYTE_ORDER == __BIG_ENDIAN : defined(__BIG_ENDIAN)
#define cpu_to_le16 bswap_16
#define cpu_to_le32 bswap_32
#define cpu_to_le64 bswap_64
#define le16_to_cpu bswap_16
#define le32_to_cpu bswap_32
#define le64_to_cpu bswap_64
#define cpu_to_be16
#define cpu_to_be32
#define cpu_to_be64
#define be16_to_cpu
#define be32_to_cpu
#define be64_to_cpu
#else
#define cpu_to_le16
#define cpu_to_le32
#define cpu_to_le64
#define le16_to_cpu
#define le32_to_cpu
#define le64_to_cpu
#define cpu_to_be16 bswap_16
#define cpu_to_be32 bswap_32
#define cpu_to_be64 bswap_64
#define be16_to_cpu bswap_16
#define be32_to_cpu bswap_32
#define be64_to_cpu bswap_64
#endif

#endif
