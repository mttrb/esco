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

#include <config.h>
#include <cstring>
#include <cstdio>
#include "hqmachine.h"
#include "defs.h"

HQMachine::HQMachine(Source* _src,const char* _srctxt, EnvTouch* _env) {
    src = _src;
    bsrc = _src;
    program = new char [strlen(_srctxt)+5];
    strcpy(program, _srctxt);
    env = _env;
}

HQMachine::~HQMachine() {
    delete [] program;
    delete bsrc;
}

int HQMachine::step() {
    hqop op = (hqop)src->getOpcode();
    //Exec opcode;
    switch (op) {
        case op_h:
            fop_h();
            break;
        case op_q:
            fop_q();
            break;
        case op_plus:
            fop_plus();
            break;
        case op_9:
            fop_9();
            break;
        default:
            return STEP_EXEC_ERR;
    }
    src = src->getNext();
    if (src)
        return STEP_EXEC_OK;
    else
        return STEP_EXEC_END;
}

const char* HQMachine::getCode() {
    return program;
}

int HQMachine::getDbg() {
    return src->getDbg();
}

int HQMachine::getDbgSize() {
    return src->getDbgSize();
}

void HQMachine::fop_h() {
    env->putStr("Hello, world!\n");
}

void HQMachine::fop_q() {
    env->putStr(program);
}

void HQMachine::fop_plus() {
    //Increment accumulator ;)
    eax++;
}

void HQMachine::fop_9() {
    char tmp[255];
    for (unsigned int i = 99; i >= 1;i--) {
        sprintf(tmp, "%u bottles of beer on the wall\n%u bottles of beer!\nTake one down, pass it around\n%u bottles of beer on the wall!\n", i, i, i-1);
        env->putStr(tmp);
    }
}



