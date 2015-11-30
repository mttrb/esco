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

#ifndef HQMACHINE
#define HQMACHINE

#include "machine.h"
#include "source.h"
#include "envtouch.h"
#include "defs.h"

enum hqop {
    op_h,
    op_q,
    op_plus,
    op_9,
    hqops_last
};

class HQMachine : public Machine {
    public:
        HQMachine(Source*, const char*, EnvTouch*);
        ~HQMachine();
    
        int step();
        const char* getCode();

        int getDbg();
        int getDbgSize();

    private:
        unsigned int eax; //accumulator
        void fop_h();
        void fop_q();
        void fop_plus();
        void fop_9();

        // Array of functions whitch execute opcodes

        char* program;
        Source *src, *bsrc;
        EnvTouch* env;
};

#endif



