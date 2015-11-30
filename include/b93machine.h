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

#ifndef B93MACHINE_H
#define B93MACHINE_H

#include "machine.h"
#include "envtouch.h"
#include "defs.h"
#include "b93source.h"
#include "b93defs.h"
#include <deque>
#include <stack>
using namespace std;

enum {
	BEFUNGE_VERSION_93, 
	BEFUNGE_VERSION_97, 
	BEFUNGE_VERSION_98
};

class B93Machine : public Machine {
    public:
        B93Machine(Source*, EnvTouch*, B93Source ***, int, int, int);
        virtual ~B93Machine();

        virtual int step();

    private: 
	// not that nice, but it works...
	inline void DeleteAll() {
		int x, y;
		for(x = 0; x < maxx; x++) {
			for(y = 0; y < maxy; y++) {
				delete allsource[x][y];
			}
			delete [] allsource[x];
		}
		delete [] allsource;
	}

        B93Source *src;
        EnvTouch* env;
	B93Source *first;

	// direction
	int dir;
	
	// string mode
	bool string_mode;

	// specification says long signed integer, so be it long signed integer
	stack<long> theStack;

	// for accesing the correct source with put and get
	B93Source *** allsource;
	int maxx, maxy;

	// type of machine
	int type;

};



#endif



