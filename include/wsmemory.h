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


#ifndef WSMEMORY_H
#define WSMEMORY_H

#include "wsnumber.h"
#include <set>
#include "object.h"
using namespace std;

class WsMemory
{
public:
	WsMemory();
	~WsMemory();

	void Write(const WsNumber &address, const WsNumber &data);
	bool Read(const WsNumber &address, WsNumber &data) const;


private:
	// atomic piece of memory
	struct PieceOfMem {
		WsNumber address;
		WsNumber data;
	};

	// internal use for set class
	struct less_PieceOfMem : less<PieceOfMem *> {
		bool operator ()(const PieceOfMem *a, const PieceOfMem *b) const{
			return a->address < b->address;
		}
	};

	typedef set<PieceOfMem *, less_PieceOfMem> Memory;
	
	// the memory
	Memory mem; 

};

#endif


