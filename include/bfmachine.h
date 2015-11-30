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

/*
 * BFMachine for BrainFuck language
 */

#ifndef _BFMACHINE_H_
#define _BFMACHINE_H_

#include <config.h>
#include <string>
#include "bfmemory.h"
#include "machine.h"
#include "envtouch.h"
#include "defs.h"
#include "source.h"
#include "escoconf.h"

enum opcode {
    inc_ptr,
    dec_ptr,
    inc_ptr_data,
    dec_ptr_data,
    while_begin,
    while_end,
    output_ptr_data,
    input_ptr_data,

    num_of_commands
};

class BFMachine : public Machine {

    public:
        BFMachine(Source*, const char*, BFMemory*, EnvTouch*);//Code of programm and initializited memory. Also interface for ineraction between BFMachine and user.
        virtual ~BFMachine();

        virtual int step();
        virtual const char* getCode();

        virtual int getDbg();
        virtual int getDbgSize();

    private:
        int incPtr();// >
        int decPtr();// <

        int incPtrData();// +
        int decPtrData();// -

        int whileBegin();// [
        int whileEnd();// ]

        int ePutPtrData();// .
        int eGetPtrData();// ,

        // Data
        char* programm;
        Source *src, *flowctrl, *first;
        BFMemory* mem;
        EnvTouch* et;

        typedef int (BFMachine::*operation[]) ();
        static operation functions;
};

#endif



