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
 * Function:  tools atomic.
 * Created on: 2019-12-19
 */
#ifndef __TATOMIC_HH_19910612
#define __TATOMIC_HH_19910612

#include <stdint.h>
typedef uint8_t		Tatomic_u8;
typedef uint16_t	Tatomic_u16;
typedef uint32_t	Tatomic_u32;
typedef uint64_t	Tatomic_u64;

typedef int8_t		Tatomic_8;
typedef int16_t		Tatomic_16;
typedef int32_t		Tatomic_32;
typedef int64_t		Tatomic_64;

#define CHECK_ZERO(e)    (sizeof(struct{int:-!(e);}))

#ifdef __GNUC__
#define Tatomic_addf(p, value) __atomic_add_fetch(p, value, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)
#define Tatomic_fadd(p, value) __atomic_fetch_add(p, value, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)

#define Tatomic_subf(p, value) __atomic_sub_fetch(p, value, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)
#define Tatomic_fsub(p, value) __atomic_fetch_sub(p, value, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)

#define Tatomic_andf(p, value) __atomic_and_fetch(p, value, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)
#define Tatomic_fand(p, value) __atomic_fetch_and(p, value, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)

#define Tatomic_orf(p, value) __atomic_or_fetch(p, value, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)
#define Tatomic_for(p, value) __atomic_fetch_or(p, value, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)

#define Tatomic_xorf(p, value) __atomic_xor_fetch(p, value, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)
#define Tatomic_fxor(p, value) __atomic_fetch_xor(p, value, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)

#define Tatomic_nandf(p, value) __atomic_nand_fetch(p, value, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)
#define Tatomic_fnand(p, value) __atomic_fetch_nand(p, value, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)

#define Tatomic_loadn(p)	__atomic_load_n(p, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)
#define Tatomic_load(p, ret)\
	__atomic_load_n(p, ret, __ATOMIC_SEQ_CST) + CHECK_ZERO(p) + CHECK_ZERO(ret)

#define Tatomic_storen(p)	__atomic_store_n(p, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)
#define Tatomic_store(p, value)	\
	__atomic_store_n(p, value, __ATOMIC_SEQ_CST) + CHECK_ZERO(p) + CHECK_ZERO(value)

#define Tatomic_exchangen(p)	__atomic_exchange_n(p, __ATOMIC_SEQ_CST) + CHECK_ZERO(p)
#define Tatomic_exchange(p, value, ret)	\
	__atomic_exchange_n(p, __ATOMIC_SEQ_CST) + CHECK_ZERO(p) + CHECK_ZERO(value) + CHECK_ZERO(ret)
#else
#error "Atomic operations are not supported. Please use the gcc !"
#endif

#endif
