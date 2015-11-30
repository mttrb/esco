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

using namespace std;

#include "defs.h"
#include "bfmachine.h"
#include "bfmemory.h"


const char* bfcmds = "><+-[].,";

BFMachine::operation BFMachine::functions = {
    &BFMachine::incPtr, &BFMachine::decPtr,
    &BFMachine::incPtrData, &BFMachine::decPtrData,
    &BFMachine::whileBegin, &BFMachine::whileEnd,
    &BFMachine::ePutPtrData, &BFMachine::eGetPtrData
};

BFMachine::BFMachine(Source* _src, const char* source, BFMemory* _mem, EnvTouch* _et) {
    src = _src;
    first = _src;
    int ssize = strlen(source)+1;
    programm = new char [ssize];
    strncpy(programm, source, ssize);
    mem = _mem;
    et = _et;

};


BFMachine::~BFMachine() {
    delete [] programm;
    delete mem;
    delete first;
};


#define exec(funct) flags=funct(flowctrl);

int BFMachine::step() {
    if (src != NULL) {
        int flags;

        int op = src->getOpcode();
        flowctrl = src->getFlowCtrl();
        src = src->getNext();

        if (op < num_of_commands) {
            flags = (this->*functions[op])();
        }
        else
        {
            dbgprint("Error opcode number");
            return STEP_EXEC_ERR;
        }

        dbgprint("Operation: '%c'. Ptr[%u] = %u\n", bfcmds[op], mem->getPtr(), mem->getPtrData());

        return flags;
    }
    dbgprint("End of program.\n");
    return STEP_EXEC_END;
};

const char* BFMachine::getCode() {
    return programm;
}

int BFMachine::incPtr() {
    mem->incPtr();
    return STEP_EXEC_OK;
    /*
    if (mem->incPtr()) {
        return STEP_EXEC_OK;
    }
    return STEP_EXEC_ERR;
    */
}

int BFMachine::decPtr() {
    mem->decPtr();
    return STEP_EXEC_OK;
    /*
    if (mem->decPtr()) {
        return STEP_EXEC_OK;
    }
    return STEP_EXEC_ERR;
    */
}

int BFMachine::incPtrData() {
    bfcell data = mem->getPtrData();
    mem->setPtrData(++data);
    return STEP_EXEC_OK | STEP_DATA_CHG;
}

int BFMachine::decPtrData() {
    bfcell data = mem->getPtrData();
    mem->setPtrData(--data);
    return STEP_EXEC_OK | STEP_DATA_CHG;
}

int BFMachine::eGetPtrData() {
    dbgprint("Reading data: ");
    mem->setPtrData(et->getChar());
    dbgprint("\n");
    return STEP_EXEC_OK;
}

int BFMachine::ePutPtrData() {
    dbgprint("Outputing symbol: '");
    et->putChar(mem->getPtrData());
    dbgprint("'\n");
    return STEP_EXEC_OK | STEP_DATA_CHG;
}

int BFMachine::whileBegin() {
    dbgprint("ptr:%p\n", flowctrl);
    if (mem->getPtrData() == 0) {
        dbgprint("End of loop");
        src = flowctrl;
    }
    return STEP_EXEC_OK;
}

int BFMachine::whileEnd() {
    dbgprint("WhileEnd\n");
    src = flowctrl;
    return STEP_EXEC_OK;
}

int BFMachine::getDbg() {
    return src->getDbg();
}

int BFMachine::getDbgSize() {
    return src->getDbgSize();
}



