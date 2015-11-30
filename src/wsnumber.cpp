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

#include "wsnumber.h"
#include <stdio.h>

WsNumber::WsNumber() {
	mpz_init(data);
}

/*
WsNumber::WsNumber(const char *pNumber) {
	mpz_init(data);
	set(pNumber);
}
*/

WsNumber::WsNumber(long z) {
	mpz_init_set_si(data, z);
}

WsNumber::WsNumber(unsigned long z) {
	mpz_init_set_ui(data, z);
}

WsNumber::WsNumber(const WsNumber & rhs) {
	mpz_init_set(data, rhs.data);
}

WsNumber &WsNumber::operator =(const WsNumber &rhs) {
	if(this != &rhs) {
		mpz_set(data, rhs.data);
	}
    return *this;
}

WsNumber::~WsNumber() {
	mpz_clear(data);
}

WsNumber WsNumber::operator +(const WsNumber &z) const {
	WsNumber res;

	mpz_add(res.data, this->data, z.data);

	return res;
}

WsNumber WsNumber::operator -(const WsNumber &z) const {
	WsNumber res;

	mpz_sub(res.data, this->data, z.data);

	return res;
}

WsNumber WsNumber::operator *(const WsNumber &z) const {
	WsNumber res;

	mpz_mul(res.data, this->data, z.data);

	return res;
}

WsNumber WsNumber::operator /(const WsNumber &z) const {
	WsNumber res;

	mpz_div(res.data, this->data, z.data);

	return res;
}

WsNumber WsNumber::operator %(const WsNumber &z) const {
	WsNumber res;

	mpz_mod(res.data, this->data, z.data);

	return res;
}

WsNumber & WsNumber::operator += (const WsNumber &z) {
	mpz_add(data, data, z.data);
	return *this;
}

WsNumber & WsNumber::operator -= (const WsNumber &z) {
	mpz_sub(data, data, z.data);
	return *this;
}

WsNumber & WsNumber::operator *= (const WsNumber &z) {
	mpz_mul(data, data, z.data);
	return *this;
}

WsNumber & WsNumber::operator /= (const WsNumber &z) {
	mpz_div(data, data, z.data);
	return *this;
}

WsNumber & WsNumber::operator %= (const WsNumber &z) {
	mpz_mod(data, data, z.data);
	return *this;
}

bool WsNumber::operator < (const WsNumber &z) const {
	return mpz_cmp(data, z.data) < 0;
}
bool WsNumber::operator <= (const WsNumber &z) const {
	return mpz_cmp(data, z.data) <= 0;
}
bool WsNumber::operator > (const WsNumber &z) const {
	return mpz_cmp(data, z.data) > 0;
}
bool WsNumber::operator >= (const WsNumber &z) const {
	return mpz_cmp(data, z.data) >= 0;
}

bool WsNumber::operator == (const WsNumber &z) const {
	return mpz_cmp(data, z.data) == 0;
}
bool WsNumber::operator != (const WsNumber &z) const {
	return mpz_cmp(data, z.data) != 0;
}

bool WsNumber::set(const char *pNumber)
{
	return mpz_set_str(data, pNumber, 10) == 0;
}

char * WsNumber::get() const
{
	// +2 see documentation
	char *res = new char[mpz_sizeinbase(data, 10)+2];
	return mpz_get_str(res, 10, data);
}

long WsNumber::getlong() const
{
	return mpz_get_si(data);
}

bool WsNumber::fitslong() const
{
	return mpz_fits_slong_p(data);
}

unsigned long WsNumber::getulong() const
{
	return mpz_get_ui(data);
}

bool WsNumber::fitsulong() const
{
	return mpz_fits_ulong_p(data);
}

bool WsNumber::set_ws(const char *pNumber)
{
	long sign;
	unsigned int i;

	// at least one sign and one number have to exist
	if(strlen(pNumber) < 2) {
		return false;
	}
	
	if(pNumber[0] != TAB && pNumber[0] != SPACE) {
		return false;
	}

	sign = (pNumber[0] == SPACE ? 1 : -1);
	mpz_set_ui(data, 0);

	for(i = 1; i < strlen(pNumber); i++) {
		mpz_mul_ui(data, data, 2);
		mpz_add_ui(data, data, pNumber[i] == TAB ? 1 : 0);
	}

	mpz_mul_si(data, data, sign);

	// printf("created number %i\n", data);

	return true;
}


