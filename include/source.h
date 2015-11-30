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

/* Class for source. Opcode by opcode */

#ifndef SOURCE_H
#define SOURCE_H
#include <iostream>
#include "object.h"

class Source {
    public:
        /* Constructor for source
        * Parametrs:
        * 1. Opcode
        * 2. Argument
        * 3-4 - debug info
        * 3 - First symbol of debug info in the source
        * 4 - Last symbol
        * 5 - Previous point
        */
        inline Source(int, Object*, int, int,Source*);
        inline ~Source();

        inline int getOpcode();
        inline Object* getArg();

        inline int getDbg();
        inline int getDbgSize();

        inline void setArg(Object*);
        inline void setNext(Source*);
        inline Source* getNext();
        inline Source* getPrev();

        inline Source *getFlowCtrl();
        inline void setFlowCtrl(Source *);

    protected:
        // need a constructor for B93Source
        // need to be initialized because of 
        // deconstructor
        Source() : arg(NULL), next(NULL) {} // for derivation only
	// need to be protected
	// because destroyed in deconstructor
	Object* arg;
        Source* next;

       
    private:

        int opcode;

        
        int dbg, dbgs;
        Source* prev;

	Source *flowctrl;
};

Source::Source(int _opcode, Object* _arg, int _dbg, int _dbgs,Source* _prev){
    opcode = _opcode;
    arg = _arg;
    dbg = _dbg;
    dbgs = _dbgs;
    prev = _prev;
    // flowctrl defaults to NULL if not explicitly set
    flowctrl = NULL;
    if (_prev != NULL)
        _prev->setNext(this);
    next = NULL;
}

Source::~Source() {
    if (next != NULL)
        delete next;
    // delete argument
    delete arg;
}

int Source::getOpcode() {
    return opcode;
}

Object * Source::getArg() {
    return arg;
}

Source * Source::getFlowCtrl() {
    return flowctrl;
}

void Source::setFlowCtrl(Source *_flowctrl) {
    flowctrl = _flowctrl;
}

int Source::getDbg() {
    return dbg;
}

int Source::getDbgSize() {
    return dbgs;
}

void Source::setArg(Object* _arg) {
    arg = _arg;
}

void Source::setNext(Source* _src) {
    next = _src;
}

Source* Source::getNext() {
    return next;
}

Source* Source::getPrev() {
    return prev;
}


#endif



