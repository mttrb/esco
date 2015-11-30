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

#ifndef _BFMEMORY_H_
#define _BFMEMORY_H_

#include <iostream>
#include <cstdio>

typedef unsigned int bfcell; //Type of one cell

class BFMemory {
    public:
        BFMemory(int, bfcell); // Create memory, and fill it with symbol.
        ~BFMemory();

        const bfcell* getData();

        int getSize();

        inline int getPtr();
        int setPtr(unsigned int);

        inline void incPtr();
        inline void decPtr();

        inline bfcell getPtrData();
        inline void setPtrData(bfcell);

    private:
        unsigned int ptr; //data pointer

        bfcell* mem; // memory
        unsigned int size;  //memory size
};

void BFMemory::incPtr() {
    if (ptr < size)
        ++ptr;
    else {
        ptr=0;
        fprintf(stderr, "Pointer out of bounds\n");
    }
}

void BFMemory::decPtr() {
    if (ptr > 0)
        --ptr;
    else {
        ptr = size-1;
        fprintf(stderr, "Pointer out of bounds\n");
    }
}

bfcell BFMemory::getPtrData() {
    return mem[ptr];
};

void BFMemory::setPtrData(bfcell c) {
    mem[ptr] = c;
};

int BFMemory::getPtr() {
    return ptr;
};

#endif








