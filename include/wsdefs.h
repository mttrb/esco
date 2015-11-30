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

#ifndef WSDEFS_H
#define WSDEFS_H

// characters
#define SPACE ' '
#define TAB '\t'
#define NEWL ((char)10) // 10 == "\n" or "\r" ?


// and their strings
#define SSPACE " "
#define STAB "\t"
#define SNEWL "\n" // 10 == "\n"?



// list of operations
enum {
	// Stack operations
	OPERATION_PUSH, /* 0 */
	OPERATION_DUPL, 
	OPERATION_COPY_N_TO_TOP, 
	OPERATION_SWAP, 
	OPERATION_POP,  
	OPERATION_SLIDE_N, /* 5 */ // discard == pop from stack?

	// arithmetic
	OPERATION_ADD, 
	OPERATION_SUB, 
	OPERATION_MULT, 
	OPERATION_DIV,  
	OPERATION_MOD, /* 10 */

	// heap access
	OPERATION_STORE, 
	OPERATION_RETRIEVE, 

	// Flow control
	OPERATION_LABEL, 
	OPERATION_CALLROUTINE, 
	OPERATION_JMP, /* 15 */
	OPERATION_JMPZERO, 
	OPERATION_JMPNEGATIVE, 
	OPERATION_RETURN, 
	OPERATION_EXIT, 

	// i/o
	OPERATION_PRINTCHAR, /* 20 */
	OPERATION_PRINTNUMBER, 
	OPERATION_READCHAR, 
	OPERATION_READNUMBER 

};

#endif


