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

#ifndef WSNUMBER_H
#define WSNUMBER_H

// later implement an arbitrary precision number, 
// as whitespace language specification says

#include "wsdefs.h" // for TAB, SPACE and NEWL
#include <string.h>
#include "object.h"
#include <config.h>

#if defined HAVE_LIB_GMP_ && HAVE_LIB_GMP_ == 1
#include <gmp.h>
#else
#include <gmpfake.h>
#endif

class WsNumber : public Object
{
public:
	WsNumber();
	// this is ambigous
	// WsNumber(const char *);
	WsNumber(const WsNumber &);
	WsNumber & operator =(const WsNumber &);

	WsNumber(long);
	WsNumber(unsigned long);

	virtual ~WsNumber();

	// initialize from whitespace format number
	bool set_ws(const char *);
	// initialize from "normal" number
	bool set(const char *);
	// returns the number in "normal" format, memory must be deallocated by 
	// caller
	char * get() const;

	long getlong() const;
	bool fitslong() const;
	unsigned long getulong() const;
	bool fitsulong() const;

	WsNumber operator +(const WsNumber &z) const;
	WsNumber operator -(const WsNumber &z) const;
	WsNumber operator /(const WsNumber &z) const;
	WsNumber operator *(const WsNumber &z) const;
	WsNumber operator %(const WsNumber &z) const;

	WsNumber &operator +=(const WsNumber &z);
	WsNumber &operator -=(const WsNumber &z);
	WsNumber &operator /=(const WsNumber &z);
	WsNumber &operator *=(const WsNumber &z);
	WsNumber &operator %=(const WsNumber &z);

	bool operator < (const WsNumber &z) const;
	bool operator <= (const WsNumber &z) const;
	bool operator > (const WsNumber &z) const;
	bool operator >= (const WsNumber &z) const;

	bool operator == (const WsNumber &z) const;
	bool operator != (const WsNumber &z) const;

	
	// later exchange this by infinite length number (but how?)
	//mpz_t getdata() const {
	//	return data;
	//}

	// void setdata(long _data) {
	// 	data = _data;
	// }

private:
	// WsNumber(mpz_t _data) : data(_data) {}
	
	// long data;
	mpz_t data;

};

#endif


