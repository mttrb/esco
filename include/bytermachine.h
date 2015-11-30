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

#ifndef _BYTER_MACHINE_H
#define _BYTER_MACHINE_H

#include "machine.h"
#include "source.h"
#include "envtouch.h"
#include "bytermemory.h"

class ByterMachine : public Machine {
    public:
        ByterMachine(Source*, EnvTouch*);
        ~ByterMachine();

        int step();
        const char* getCode();

        int getDbg();
        int getDbgSize();

    private:
        inline void putChar() {et->putChar(y*0x10+x);};
        inline void backupPosition() {
            px = x;
            py = y;
        }
        inline void incX() {
            backupPosition();
            if (SIZE_X == ++x)
                x = 0;
        }
        inline void decX() {
            backupPosition();
            if (-1 == --x)
                x = SIZE_X-1;
        }
        inline void incY() {
            backupPosition();
            if (SIZE_Y == ++y)
                y = 0;
        }
        inline void decY() {
            backupPosition();
            if (-1 == --y)
                y = SIZE_Y-1;
        }

        int PrevCell();
        int MoveLeft();
        int MoveRight();
        int MoveDown();
        int MoveUp();
        int PrintLeft();
        int PrintRight();
        int PrintUp();
        int PrintDown();
        int PrintBegin();
        int Exit();

        int x,y, px, py;
        ByterMemory* bm;
        EnvTouch* et;

        typedef int (ByterMachine::*operation[]) ();
        static operation functions;
};

#endif



