/* esco - interpreter for a lot (in future) esoteric languages.
 * Copyright (C) 2007-2011, Zasenko Sergey, Matthias Boehm
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

#ifndef B93SOURCE_H
#define B93SOURCE_H

#include "source.h"
#include "b93defs.h"


// twodimensional Source
class B93Source : public Source {
public:
	// 1. opcode
	// /* 2. next instructionS */
	// inline B93Source() : opcode(B93OP_INVALID) {}
	// important: All must be UNSIGNED char!!
	inline B93Source(long);
	inline ~B93Source();

	inline long getOpcode() const { return opcode; }
	// this is possible because of self-modifying code
	inline void setOpcode(long c) { opcode = c; }

	inline B93Source *getNext(int i) const { return next[i]; }

	inline void setNext(int i, B93Source * z) { next[i] = z; }

private:
	// unsigned char opcode;
	long opcode;
	B93Source *next[4]; // [4]

};

B93Source::B93Source(long _opcode) : opcode(_opcode)// , next(_next)
{
}

B93Source::~B93Source() {
	// phew... memory deleting is not simple!
	// delete next[DIR_EAST];
	// delete next[DIR_
}



#endif


