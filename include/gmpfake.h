/* esco - interpreter for a lot (in future) esoteric languages.
 * Copyright (C) 2007-2011, Zasenko Sergey
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef GMP_FAKE_H
#define GMP_FAKE_H

// show a message that you better should install gmp
// #pragma message You'd better install gmp-mp (ftp.gnu.org/gnu/gmp) for full whitespace support with bignumbers. 
// #pragma message Currently using only the long c type

// replace mpz_* functions by this, if not having gmp on your computer

#include <stdio.h>

typedef long mpz_t;

inline void mpz_init(mpz_t &a) { a = 0; }
inline void mpz_clear(mpz_t &a) { }

inline void mpz_init_set(mpz_t &a, const mpz_t &b) { a = b; }
inline void mpz_init_set_ui(mpz_t &a, unsigned long i) { a = i; }
inline void mpz_init_set_si(mpz_t &a, signed long i) { a = i; }

inline void mpz_set(mpz_t &a, const mpz_t &b) { a = b; }
inline void mpz_set_ui(mpz_t &a, unsigned long i) { a = i; }
inline void mpz_set_si(mpz_t &a, signed long i) { a = i; }

inline void mpz_add(mpz_t &a, const mpz_t &b, const mpz_t &c) { a = b+c; }
inline void mpz_sub(mpz_t &a, const mpz_t &b, const mpz_t &c) { a = b-c; }
inline void mpz_div(mpz_t &a, const mpz_t &b, const mpz_t &c) { a = b/c; }
inline void mpz_mul(mpz_t &a, const mpz_t &b, const mpz_t &c) { a = b*c; }
inline void mpz_mod(mpz_t &a, const mpz_t &b, const mpz_t &c) { a = b % c; }

inline void mpz_mul_ui(mpz_t &a, const mpz_t &b, unsigned long c) { a = b*c; }
inline void mpz_mul_si(mpz_t &a, const mpz_t &b, signed long c) { a = b*c; }
inline void mpz_add_ui(mpz_t &a, const mpz_t &b, unsigned long c) { a = b+c; }

inline int mpz_cmp(const mpz_t &a, const mpz_t &b) { return a - b; }

inline bool mpz_set_str(mpz_t &a, const char *str, int base)
{
	return sscanf(str, "%li", &a) == 1;
}

inline int mpz_sizeinbase(const mpz_t &a, int base) {
	// highest number is 4294967295 + "-" + "\0" = 12
	return 12;
}

inline char * mpz_get_str(char * str, int base, const mpz_t &z) 
{
	if(str) {
		sprintf(str, "%li", z);
		return str;
	}
	else {
		char *res;
		res = new char[mpz_sizeinbase(z, base)];
		sprintf(res, "%li", z);
		return res;
	}
}

inline signed long mpz_get_si(const mpz_t &z) { return z; }
inline unsigned long mpz_get_ui(const mpz_t &z) { return z; }
inline bool mpz_fits_ulong_p(const mpz_t &) { return true; }
inline bool mpz_fits_slong_p(const mpz_t &) { return true; }





#endif


