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

#include "wsmemory.h"

WsMemory::WsMemory() {
	// nothing to do here
}

WsMemory::~WsMemory() {
	Memory::iterator it;

	for(it = mem.begin(); it != mem.end(); it++) {
		delete *it;
	}
}

void WsMemory::Write(const WsNumber &address, const WsNumber &data) {
	Memory::iterator it;
	WsMemory::PieceOfMem p;
	WsMemory::PieceOfMem *pp;
	
	// does address exist already?
	p.address = address;
	it = mem.find(&p);

	if (it != mem.end()) {
		// address exists already
		(*it)->data = data;
	} 
	else {
		// address does not yet exist
		pp = new PieceOfMem;
		pp->data = data;
		pp->address = address;
		mem.insert(pp);
	}
}

bool WsMemory::Read(const WsNumber &address, WsNumber &data) const
{
	PieceOfMem p;
	Memory::const_iterator it;

	p.address = address;

	it = mem.find(&p);

	// not found
	if(it == mem.end()) {
		return false;
	}
	else {
		data = (*it)->data;
		return true;
	}
	
}


