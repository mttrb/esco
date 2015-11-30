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
#include <cstdio>
#include "defs.h"
#include "b93machine.h"
#include "b93defs.h"
#include "b93preprocessor.h"
#include "escoconf.h"
#include <math.h>
#include <time.h>

B93Machine::B93Machine(Source* _src, EnvTouch* _et, 
	B93Source ***_allsource, int _maxx, int _maxy, int _type) : allsource(_allsource), maxx(_maxx), maxy(_maxy), type(_type) {
    src = (B93Source *)_src;
    env = _et;
    first = (B93Source *)_src;

    // first direction is eastward
    dir = DIR_EAST;

    // stringmode off
    string_mode = false;

    // initialize random number
    // add argument to srand to command-line!!!
    time_t t;
    time(&t);
    srand(t);
};

B93Machine::~B93Machine() {
	DeleteAll();
	// do not access any source object after that call, even not first, 
	// because they are now ALL deleted (hopefully)
};

// if there is no element, push 0 on the stack

template <class T>
void stack_pop(stack<T> &_stack) {
	if(_stack.size() != 0) {
		_stack.pop();
	}
}

template <class T>
T stack_top(stack<T> &_stack) {
	if(_stack.size() == 0) {
		_stack.push(0);
	}
	return _stack.top();
}


int B93Machine::step() {
	// important: as char is usually signed, 
	// i have to convert all explicit char values
	// to unsigned char before applying to long value!
	if (src != NULL) {
		long op = src->getOpcode();
		long putx = -1, puty = -1, putvalue = -1;
		
		/* ********** debug *****************/
		if(conf.getdebug()) {
			int x, y, xx = -1, yy = -1;
			bool bFound = false;
			for(y = 0; y < maxy; y++) {
				for(x = 0; x < maxx; x++) {
					if(src == allsource[x][y]) {
						xx = x; yy = y;
						bFound = true;
						break;
					}
				}
			}

			if(!bFound) {
				dbgprint("Error: wrong pointer! \n");
			}
			else {
				dbgprint("IP: %03i/%03i ", xx, yy);
			}
		}
		if(!string_mode) {
			dbgprint("OP \'%c\' STACK ", (unsigned char)op);
		} else {
			dbgprint("OP \'\"\' STACK ");
		}
		/* ************* end debug *****************/


		// test for string mode 
		if(string_mode) {
			if(op == B93OP_STRINGMODE) {
				string_mode = false;
			}
			else {
				// push ascii value on to stack
				theStack.push(op);
			}
		}
		else { /* no string mode */

			// execution of operation
			switch(op) {
			case B93OP_NOP:
				// nothing to do!
				break;
			case B93OP_ADD:
			case B93OP_SUB:
			case B93OP_MULT:
			case B93OP_DIV:
			case B93OP_MOD:
			case B93OP_GT:
			{
				long a, b;
				b = stack_top(theStack);
				stack_pop(theStack);
				a = stack_top(theStack);
				stack_pop(theStack);
				switch(op) {
				case B93OP_ADD:
					theStack.push(a+b);
					break;
				case B93OP_SUB:
					theStack.push(a-b);
					break;
				case B93OP_DIV:
					theStack.push(a/b);
					break;
				case B93OP_MULT:
					theStack.push(a*b);
					break;
				case B93OP_MOD:
					theStack.push(a%b);
					break;
				case B93OP_GT:
					theStack.push(a>b ? 1 : 0);
					break;
				}
				break;
			}
			case B93OP_NOT:
			{
				long a;
				a = stack_top(theStack);
				stack_pop(theStack);
				theStack.push(a == 0 ? 1 : 0);
				break;
			}
			case B93OP_EAST:
			{
				dir = DIR_EAST;
				break;
			}
			case B93OP_WEST:
			{
				dir = DIR_WEST;
				break;
			}
			case B93OP_NORTH:
			{
				dir = DIR_NORTH;
				break;
			}
			case B93OP_SOUTH:
			{
				dir = DIR_SOUTH;
				break;
			}
			case B93OP_RAND:
			{
				dir = rand()%4;
				break;
			}
			case B93OP_HORZIF:
			{
				long a;
				a = stack_top(theStack);
				stack_pop(theStack);
				if(a != 0) {
					dir = DIR_WEST;
				} else {
					dir = DIR_EAST;
				}
				break;
			}
			case B93OP_VERTIF:
			{
				long a;
				a = stack_top(theStack);
				stack_pop(theStack);
				if(a != 0) {
					dir = DIR_NORTH;
				} else {
					dir = DIR_SOUTH;
				}
				break;
			}
			case B93OP_STRINGMODE:
			{
				string_mode = true;
				break;
			}
			case B93OP_DUP:
			{
				long a;
				a = stack_top(theStack);
				theStack.push(a);
				break;
			}
			case B93OP_SWAP:
			{
				long a, b;
				a = stack_top(theStack);
				stack_pop(theStack);
				b = stack_top(theStack);
				stack_pop(theStack);
				theStack.push(a);
				theStack.push(b);
				break;
			}
			case B93OP_POP:
			{
				stack_pop(theStack);
				break;
			}
			case B93OP_PRINTN:
			{
				const int bufsize = 30;
				char buf[bufsize+1];
				// number + space !
				sprintf(buf, "%li ", stack_top(theStack));
				env->putStr(buf);
				stack_pop(theStack);
				break;
			}
			case B93OP_PRINTC:
			{
				long a = stack_top(theStack);
				stack_pop(theStack);
				if(a < 0 || a > 255) {
					dbgprint("Warning: printc not in range! \n");
				}
				env->putChar(a); // no matter if it is too big/too small
				break;
			}
			case B93OP_SKIP:
			{
				// skip one instruction
				// + one instruction further at the end of the function
				// there is a possible source for errors!!!!!!!!!!!!!!!!!!!
				src = src->getNext(dir);
				break;
			}
			case B93OP_INPUTN:
			{
				const int bufsize = 1000;
				char buf[bufsize+1];
				env->getStr(buf, bufsize);
				long value;
				sscanf(buf, "%li", &value);
				theStack.push(value);
				break;
			}
			case B93OP_INPUTC:
			{
				long value = (long)(unsigned char)env->getChar();
				theStack.push(value);
				break;
			}
			case B93OP_EXIT:
			{
				return STEP_EXEC_END;
			}
			case B93OP_GET:
			{
				long x, y, value;
				y = stack_top(theStack);
				stack_pop(theStack);
				x = stack_top(theStack);
				stack_pop(theStack);
				// instead of the following do wrap-around?
				// no, silently ignore it
				if(x < 0 || x >= maxx || y < 0 || y >= maxy) {
					dbgprint("Error: index out of range! \n");
					// return STEP_DATA_OUT;
					value = B93OP_NOP;
				} else {
					value = allsource[x][y]->getOpcode();
				}
				theStack.push(value);
				break;
			}
			case B93OP_PUT:
			{
				long x, y, value;
				y = stack_top(theStack);
				stack_pop(theStack);
				x = stack_top(theStack);
				stack_pop(theStack);
				value = stack_top(theStack);
				stack_pop(theStack);

				// don't break, do wraparound?????????????????
				// no, silently ignore wrong address
				if(x < 0 || x >= maxx || y < 0 || y >= maxy) {
					dbgprint("Error: index out of range! \n");
					// return STEP_DATA_OUT;
				}
				else {
					// issue a warning
					// if(value < 0 || value > 255) {
					// 	dbgprint("Warning (get): out of range. Not a valid ASCII-value! \n");
					// }
				
					// difficult... what to do if it is not valid opcode/not in range 0-255?
					// language specification does not say anything about this, 
					// so i won't add anything special here. 
					// assign new opcode at the END of the function!!!!!
					putx = x; 
					puty = y;
					putvalue = value;
				}
				break;
			}
			case B93OP_ZERO:
			case B93OP_ONE:
			case B93OP_TWO:
			case B93OP_THREE:
			case B93OP_FOUR:
			case B93OP_FIVE:
			case B93OP_SIX:
			case B93OP_SEVEN:
			case B93OP_EIGHT:
			case B93OP_NINE:
			{
				long value = op-B93OP_ZERO;
				assert(value >= 0 && value <= 9);
				theStack.push(value);
				break;
			}
			default:
				dbgprint("error unknow opcode! \n");
				// simply ignore wrong op-codes ?!?!
				// at least some source files need this...
				// specification does not say anything
				// PROBLEM
				// return STEP_EXEC_ERR;
			} /* end switch(opcode) */

		} /* if !(stringmode) */

		/* ************* debug ***********************/
		if(conf.getdebug()) {
			int i;
			stack<long> tmp;
			stack<long> oldstack = theStack;
			int size = theStack.size();
			for(i = 0; i < size; i++) {
				int value = theStack.top();
				tmp.push(value);
				theStack.pop();
			}
			for(i =0; i < size; i++) {
				long value = tmp.top();
				tmp.pop();
				printf("%li ", value);
				// printf("%c ", value >= 32 && value <= 127 ? (unsigned char)value : ' ');
				theStack.push(value);
			}
			theStack = oldstack;
			printf("\n");
		}

		/******************** end debug *********************/

		if(putx != -1) {
			allsource[putx][puty]->setOpcode(putvalue); 

			int x, y;
			if(conf.getdebug() ) {
				printf("   ");
				for(int i = 0; i < MAXX; i++) {
					printf("%i", i/10);
				}
				printf("\n   ");
				for(int i = 0; i < MAXX; i++) {
					printf("%i", i%10);
				}
				printf("\n  +");
				for(int i = 0; i < MAXX; i++) {
					printf("-");
				}
				printf("\n");
				for(y = 0; y < MAXY; y++) {
					printf("%i%i|", y/10, y%10);
					for(x = 0; x < MAXX; x++) {
						printf("%c", (unsigned char)allsource[x][y]->getOpcode()) ;
					}
					printf("\n");
				}
				printf("  +");
				for(int i = 0; i < MAXX; i++) {
					printf("-");
				}
				printf("\n");
			}
		}

		src = src->getNext(dir);

		return STEP_EXEC_OK;
	} 
	return STEP_EXEC_ERR;
}

