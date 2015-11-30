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
#include <cstdio>
#include "machine.h"
#include "bfmemory.h"
#include "bfmachine.h"
#include "machinecreater.h"
#include "envtouch.h"
#include "defs.h"
#include "hqmachine.h"
#include "wsmachine.h"
#include "b93machine.h"
#include "bytermachine.h"

#include "bfpreprocessor.h"
#include "ookpreprocessor.h"
#include "spoonpreprocessor.h"
#include "hodorpreprocessor.h"
#include "xdpreprocessor.h"
#include "hqpreprocessor.h"
#include "wspreprocessor.h"
#include "b93preprocessor.h"
#include "byterpreprocessor.h"

MachineCreater::MachineCreater(mtype _machine,SourceReader* _sr, EnvTouch* _et):
    et(_et), sr(_sr),pr(NULL), error(ERROR_OK), machine(_machine)
{
};

MachineCreater::~MachineCreater() {
    delete sr;
    delete pr;
}

int MachineCreater::getError(void) {
        return error;
}

Machine* MachineCreater::create(void) {
    switch (machine) {
#ifdef ENABLE_BRAINFUCK
        case TYPE_BRAINFUCK:
            pr = new BFPreProcessor(sr);
            break;
        case TYPE_OOK:
            pr = new OokPreProcessor(sr);
            break;
        case TYPE_SPOON:
            pr = new SpoonPreProcessor(sr);
            break;
        case TYPE_HODOR:
            pr = new HodorPreProcessor(sr);
            break;
        case TYPE_XD:
            pr = new XDPreProcessor(sr);
            break;
#endif
#ifdef ENABLE_HQ9
        case TYPE_HQ:
            pr = new HQPreProcessor(sr);
            break;
#endif
#ifdef ENABLE_WHITESPACE
        case TYPE_WS:
            pr = new WsPreProcessor(sr);
            break;
#endif
#ifdef ENABLE_BEFUNGE
        case TYPE_B93:
        case TYPE_B98:
            pr = new B93PreProcessor(sr);
            break;
#endif
#ifdef ENABLE_BYTER
        case TYPE_BYTER:
            pr = new ByterPreProcessor(sr);
            break;
#endif
        default:
            error = ERROR_OTHER;
            return 0;
    }
    Source* source = pr->process();
    if (source) {
        switch (machine) {
#ifdef ENABLE_BRAINFUCK
            case TYPE_BRAINFUCK:
            case TYPE_OOK:
            case TYPE_SPOON:
            case TYPE_HODOR:
            case TYPE_XD:

                return new BFMachine(source,pr->getSource(), new BFMemory(0, 0), et);
#endif
#ifdef ENABLE_HQ9
            case TYPE_HQ:
                return new HQMachine(source, pr->getSource(), et);
#endif
#ifdef ENABLE_WHITESPACE
            case TYPE_WS:
                return new WsMachine(source, et);
#endif
#ifdef ENABLE_BEFUNGE
            case TYPE_B93:
                return new B93Machine(source, et,
			((B93PreProcessor *)pr)->getAllSource(), MAXX, MAXY, BEFUNGE_VERSION_93);
            // MAXX, MAXY have to be changed later
            case TYPE_B98:
                return new B93Machine(source, et,
			((B93PreProcessor *)pr)->getAllSource(), MAXX, MAXY, BEFUNGE_VERSION_98);
#endif
#ifdef ENABLE_BYTER
            case TYPE_BYTER:
                return new ByterMachine(source, et);
#endif
            default:
                return NULL;
        }
    }
    error = pr->getError();
    return 0;
}



