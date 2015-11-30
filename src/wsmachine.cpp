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

#include <cassert>
#include <limits.h>
#include "defs.h"
#include "wsmachine.h"
#include "wsmemory.h"
#include "wsdefs.h"
#include "wspreprocessor.h"
#include "escoconf.h"

//To Matthias Boehm: dbgprint were moved to ../inlcude/escoconf.h

WsMachine::WsMachine(Source* _src, EnvTouch* _et) {
    src = _src;
    env = _et;
    first = _src; // for memory deletion only
};

WsMachine::~WsMachine() {
    delete first; 
};

int WsMachine::step() {
	if (src != NULL) {
		int op = src->getOpcode();
		void* arg = src->getArg();
		Source *flowctrl = src->getFlowCtrl();
		src = src->getNext();

		// dbgprint("-----------------------\n");
		if(conf.getdebug()) {
			dbgprint("Stack: ");
			deque<WsNumber>::iterator it;
			for(it = theStack.begin(); it != theStack.end(); it++) {
				char *number = it->get();
				dbgprint("%li ", (long int)number);
				delete [] number;
			}
			dbgprint("\n");
		}

		switch(op) {
		case OPERATION_JMPZERO:
		case OPERATION_JMPNEGATIVE:
		case OPERATION_CALLROUTINE:
		case OPERATION_JMP:
			assert(flowctrl);
		};

		// test for stack underflow
		switch(op) {
		// min 1 element
		case OPERATION_DUPL:
		case OPERATION_POP:
		case OPERATION_JMPZERO:
		case OPERATION_JMPNEGATIVE:
		case OPERATION_PRINTCHAR:
		case OPERATION_PRINTNUMBER:
		case OPERATION_READCHAR:
		case OPERATION_READNUMBER:
		case OPERATION_RETRIEVE:
			if(theStack.size() < 1) {
				dbgprint("error < 1, %i\n", op);
				return STEP_STACK_ERR;
			}
			break;

		// min 2 elements
		case OPERATION_SWAP:
		case OPERATION_ADD:
		case OPERATION_SUB:
		case OPERATION_MULT:
		case OPERATION_DIV:
		case OPERATION_MOD:
		case OPERATION_STORE:
			if(theStack.size() < 2) {
				dbgprint("error < 2\n");
				return STEP_STACK_ERR;
			}
			break;		

		// min n elements
		case OPERATION_COPY_N_TO_TOP:
			// copy 5 to top
			// before:
			// 1 2 3 4 5 6 7 8
			// a b c d e f g h
			// after:
			// 1 2 3 4 5 6 7 8 9
			// e a b c d e f g h

			if(theStack.size() == 0) {
				dbgprint("error == 0 (n)\n");
				return STEP_STACK_ERR;
			}
			if(!((WsNumber *)arg)->fitsulong()) {
				return STEP_STACK_ERR;
			}
			if(theStack.size() < ((WsNumber *)arg)->getulong()) {
				dbgprint("error < n\n");
				return STEP_STACK_ERR;
			}
			break;

		// min n+1 elements
		case OPERATION_SLIDE_N:
			// slide 3
			// before:
			// 1 2 3 4 5 6 7 8
			// a b c d e f g h
			// after:
			// 1 2 3 4 5
			// a e f g h
			if(theStack.size() == 0) {
				dbgprint("error == 0 (n+1)\n");
				return STEP_STACK_ERR;
			}
			if(!((WsNumber *)arg)->fitsulong()) {
				return STEP_STACK_ERR;
			}
			if(((WsNumber *)arg)->getulong() == ULONG_MAX) {
				return STEP_STACK_ERR;
			}
			if(theStack.size() < ((WsNumber *)arg)->getulong() + 1) {
				dbgprint("error < n+1 \n");
				return STEP_STACK_ERR;
			}
			break;
		} /* end switch */

		// dbgprint("no error");
		
		
		
		switch(op) {
		case OPERATION_PUSH:
		{
			dbgprint("push\n");
			theStack.push_front(*(WsNumber *)arg);
			break;
		}
		case OPERATION_DUPL:
		{
			dbgprint("dupl\n");
			theStack.push_front(theStack.front());
			break;
		}
		case OPERATION_COPY_N_TO_TOP:
		{
			dbgprint("copy %li to top\n", ((WsNumber *)arg)->getulong() );
			theStack.push_front(theStack.at( ((WsNumber *)arg)->getulong()));
			break;
		}
		case OPERATION_SWAP:
		{
			dbgprint("swap\n");
			WsNumber tmp1, tmp2;
			tmp1 = theStack.front();
			theStack.pop_front();
			tmp2 = theStack.front();
			theStack.pop_front();
			theStack.push_front(tmp1);
			theStack.push_front(tmp2);
			break;
		}
		case OPERATION_POP:
			dbgprint("pop\n");
			theStack.pop_front();
			break;
		case OPERATION_SLIDE_N:
		{
			dbgprint("slide %li\n", ((WsNumber *)arg)->getulong() );
			WsNumber tmp = theStack.front();

			theStack.pop_front();

			for(unsigned int i = 0; i < ((WsNumber *)arg)->getulong(); i++) {
				theStack.pop_front();
			}

			theStack.push_front(tmp);

			break;
		}
		case OPERATION_ADD:
		{
			dbgprint("add \n");
			WsNumber tmp;
			tmp = theStack.at(1) + theStack.at(0);

			theStack.pop_front();
			theStack.pop_front();

			theStack.push_front(tmp);
			break;
		}
		case OPERATION_SUB:
		{
			dbgprint("sub\n");
			WsNumber tmp;
			tmp = theStack.at(1) - theStack.at(0);

			theStack.pop_front();
			theStack.pop_front();

			theStack.push_front(tmp);
			break;
		}
		case OPERATION_DIV:
		{
			dbgprint("div\n");
			WsNumber tmp;
			tmp = theStack.at(1) / theStack.at(0);

			theStack.pop_front();
			theStack.pop_front();

			theStack.push_front(tmp);
			break;
		}
		case OPERATION_MULT:
		{
			dbgprint("mult\n");
			WsNumber tmp;
			tmp = theStack.at(1) * theStack.at(0);

			theStack.pop_front();
			theStack.pop_front();

			theStack.push_front(tmp);
			break;
		}
		case OPERATION_MOD:
		{
			dbgprint("mod\n");
			WsNumber tmp;
			tmp = theStack.at(1) % theStack.at(0);

			theStack.pop_front();
			theStack.pop_front();

			theStack.push_front(tmp);
			break;
		}
		case OPERATION_STORE:
		{
			// dbgprint("store %li -> %li\n", theStack.at(0).getdata(), theStack.at(1).getdata());
			dbgprint("store\n");
			mem.Write(theStack.at(1), theStack.at(0));
			theStack.pop_front();
			theStack.pop_front();
			break;
		}
		case OPERATION_RETRIEVE:
		{
			// dbgprint("retrieve %li\n", theStack.front().getdata());
			dbgprint("retrieve\n");
			WsNumber tmp;
			bool bRes;

			bRes = mem.Read(theStack.front(), tmp);

			theStack.pop_front();

			if(!bRes) {
				return STEP_DATA_OUT;
			}
			theStack.push_front(tmp);
			break;
		}
		case OPERATION_LABEL:
			dbgprint("label\n");
			// do nothing
			break;
		case OPERATION_CALLROUTINE:
			dbgprint("call \n");
			// hmmm...
			theCallStack.push(src);
			src = flowctrl;
			break;
		case OPERATION_JMP:
			dbgprint("jmp \n");
			src = flowctrl;
			break;
		/* the jcondition instructions seem to pop the top value 
		   from stack */
		case OPERATION_JMPZERO:
			dbgprint("jz\n");
			if(theStack.front() == WsNumber(0L)) {
				src = flowctrl;
			}
			theStack.pop_front();
			break;
		case OPERATION_JMPNEGATIVE:
			dbgprint("jnegative\n");
			if(theStack.front() < WsNumber(0L)) {
				src = flowctrl;
			}
			theStack.pop_front();
			break;
		case OPERATION_RETURN:
			dbgprint("return\n");
			if(theCallStack.size() == 0) {
				dbgprint("error!");
				return STEP_STACK_ERR;
			}
			src = theCallStack.top();
			theCallStack.pop();
			break;
		case OPERATION_EXIT:
			dbgprint("exit\n");
			return STEP_EXEC_END;
		/* Print operations seem to pop the top of the stack */
		case OPERATION_PRINTCHAR:
		{
			dbgprint("printc\n");
			WsNumber tmp;
			tmp = theStack.front();
			env->putChar((char)tmp.getlong());
			theStack.pop_front();
			break;
		}
		case OPERATION_PRINTNUMBER:
		{
			dbgprint("printn\n");
			WsNumber tmp;
			tmp = theStack.front();
			char *number;
			number = tmp.get();
			env->putStr(number);
			delete [] number;
			theStack.pop_front();
			break;
		}
		case OPERATION_READCHAR:
		{
			dbgprint("readc\n");
			WsNumber tmp;
			WsNumber loc = theStack.front();
			tmp = WsNumber((long)env->getChar());
			mem.Write(loc, tmp);
			theStack.pop_front();
			break;
		}
		case OPERATION_READNUMBER:
		{
			WsNumber tmp;
			WsNumber loc = theStack.front();
			dbgprint("readn\n");
			const int bufsize = 2000;
			char buffer[bufsize+1];

			env->getStr(buffer, bufsize);

			tmp.set(buffer);
			
			mem.Write(loc, tmp);
			theStack.pop_front();
			break;
		}
		default:
			dbgprint("error : %i not known! \n", op);
			return STEP_EXEC_ERR;
		} /* switch(operation) */

		// dbgprint("Stack after: ");
		// for(it = theStack.begin(); it != theStack.end(); it++) {
		// 	dbgprint("%li ", it->getdata());
		// }
		// dbgprint("\n");

	return STEP_EXEC_OK;
	} 
	return STEP_EXEC_ERR;
}




