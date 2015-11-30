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

#include <cstring>
#include "sourcereader.h"
#include "wspreprocessor.h"
#include "defs.h"
#include "wsnumber.h"
#include <assert.h>

bool IsWhiteSpace(char c) {
	return c == SPACE || c == TAB || c == NEWL;
}

WsOperation WsOps[] = {
	// stack operations
	{ STACK SSPACE, OPERATION_PUSH }, 
	{ STACK SNEWL SSPACE, OPERATION_DUPL }, 
	{ STACK STAB SSPACE, OPERATION_COPY_N_TO_TOP }, 
	{ STACK SNEWL STAB, OPERATION_SWAP }, 
	{ STACK SNEWL SNEWL, OPERATION_POP }, 
	{ STACK STAB SNEWL, OPERATION_SLIDE_N }, 
	// Arithmetic
	{ ARITH SSPACE SSPACE, OPERATION_ADD }, 
	{ ARITH SSPACE STAB, OPERATION_SUB }, 
	{ ARITH SSPACE SNEWL, OPERATION_MULT }, 
	{ ARITH STAB SSPACE, OPERATION_DIV }, 
	{ ARITH STAB STAB, OPERATION_MOD }, 
	// Heap
	{ HEAP SSPACE, OPERATION_STORE }, 
	{ HEAP STAB, OPERATION_RETRIEVE }, 
	// Flow Ctrl
	{ FLOW SSPACE SSPACE, OPERATION_LABEL }, 
	{ FLOW SSPACE STAB, OPERATION_CALLROUTINE }, 
	{ FLOW SSPACE SNEWL, OPERATION_JMP }, 
	{ FLOW STAB SSPACE, OPERATION_JMPZERO }, 
	{ FLOW STAB STAB, OPERATION_JMPNEGATIVE }, 
	{ FLOW STAB SNEWL, OPERATION_RETURN },
	{ FLOW SNEWL SNEWL, OPERATION_EXIT }, 
	// i/o
	{ IO SSPACE SSPACE, OPERATION_PRINTCHAR }, 
	{ IO SSPACE STAB, OPERATION_PRINTNUMBER }, 
	{ IO STAB SSPACE, OPERATION_READCHAR }, 
	{ IO STAB STAB, OPERATION_READNUMBER }
};

WsPreProcessor::WsPreProcessor(SourceReader* _sr) {
	error = ERROR_OK;
	source = _sr->read();
	if (source == NULL)
	        error = _sr->getError();
	newsource = NULL;
}

WsPreProcessor::~WsPreProcessor() {
	delete[] source;
	delete [] newsource;
}

// for better reading (guess try to read          	            
//     		 
//            )
// pretty difficult, isn't it?
// for debugging purposes
char *replaceWhiteSpace(const char *str) {
	unsigned int i;
	char *newstr = new char[strlen(str)+1];

	for(i = 0; i < strlen(str); i++) {
		switch(str[i]) {
		case TAB:
			newstr[i] = 't';
			break;
		case SPACE:
			newstr[i] = 's';
			break;
		case NEWL:
			newstr[i] = 'n';
			break;
		}
	}
	newstr[i] = 0;
	return newstr;
}


Source* WsPreProcessor::process() {
	unsigned int i, j;
	if(source) {
		Source *first = NULL;
		Source *src = NULL;

		// first delete everything non-whitespacy
		// makes further processing much more simple
		newsource = new char[strlen(source)+1];

		for(i = 0, j = 0; i < strlen(source); i++) {
			if(IsWhiteSpace(source[i])) {
				newsource[j] = source[i];
				j++;
			}
		}
		// terminate new source string
		newsource[j] = 0;

		// now analysing the string

		int curpos = 0;

		do {
			int operation = -1;

			// printf("curpos: %i\n", curpos);

			for(i = 0; i < sizeof(WsOps)/sizeof(WsOps[0]); i++) {
				// which code?
				if(strncmp(WsOps[i].code, newsource+curpos, 
					strlen(WsOps[i].code)) == 0) {
					operation = WsOps[i].op;
					break;
				}
			}

			// printf("operation: %i\n", operation);

			if(operation != -1) {
				char *tmp;

				curpos += strlen(WsOps[i].code);
			
				switch(operation) {
				// number argument
				case OPERATION_PUSH:
				case OPERATION_COPY_N_TO_TOP:
				case OPERATION_SLIDE_N:
				{
					// minimal two chars are required for number
					if(strlen(newsource) - curpos < 2 
						|| strchr(newsource+curpos, NEWL)-(newsource+curpos) < 2) {
						// printf("too short number! %i, %i\n", strlen(newsource), curpos);
						error = ERROR_SYNTAX;
						delete first;
						return NULL;
					}
					tmp = new char[strlen(newsource)-curpos+1];
					for(i = curpos; i < strlen(newsource); i++) {
						if(newsource[i] == TAB || newsource[i] == SPACE) {
							tmp[i-curpos] = newsource[i];
						}
						else {
							break;
						}
					}

					// closing string
					tmp[i-curpos] = 0;

					curpos = i+1;

					// this causes memory leaks, but i don't know where and how
					// to delete this memory later
					// it's the void * pointer which makes it very difficult to 
					// determine whether it's a pointer to WsNumber or not
					// the solution is Object class (see object.h)
					WsNumber *pNumber = new WsNumber();
					pNumber->set_ws(tmp);

					// printf("number %s\n", replaceWhiteSpace(tmp));

					src = new Source(operation, pNumber, 0, 0, src);

					delete [] tmp;

					break;
				}
				// special case: declaration of label
				case OPERATION_LABEL:

				// label as argument
				case OPERATION_CALLROUTINE:
				case OPERATION_JMP:
				case OPERATION_JMPZERO:
				case OPERATION_JMPNEGATIVE:
					
					// minimal one char required for label
					if(strlen(newsource)-curpos < 1 || 
						strchr(newsource+curpos, NEWL)-(newsource+curpos) < 1) {
						// printf("too short label! %i/%i\n", strlen(newsource), curpos);
						error = ERROR_SYNTAX;
						delete first;
						return NULL;
					}

					tmp = new char[strlen(newsource)-curpos+1];

					for(i = curpos; i < strlen(newsource); i++) {
						if(newsource[i] != NEWL) {
							tmp[i-curpos] = newsource[i];
						}
						else {
							break;
						}
					}

					// closing string
					tmp[i-curpos] = 0;

					curpos = i+1;

					// printf("label %s\n", replaceWhiteSpace(tmp));

					// temporarily attach a wrong type to arg
					src = new Source(operation, (Object *)(void *)tmp, 0, 0, src);

					// do not delete tmp, because it is used later
					
					if(operation == OPERATION_LABEL) {

						// check if label exists
						label l;

						l.name = tmp;
						if(labels.find(l) != labels.end()) {
							delete first;
							error = ERROR_DUPLICATE_LABEL;
							return NULL;
						}
						l.srcptr = src;
						l.name = tmp;
						labels.insert(l);

						// printf("inserting label %s\n", replaceWhiteSpace(tmp));
						src->setArg(NULL);
					}
					break;

				// no argument
				default:
					src = new Source(operation, NULL, 0, 0, src);
				} /* switch (operation) */

				if(first == NULL) {
					first = src;
				}
			}
			else { /* operation == -1 */
				src = NULL;
			}
		} while(src != NULL);

		// printf("first pass completed\n");

		src = first;

		while(src != NULL) {
			set_label::iterator it;
			label l;

			switch(src->getOpcode()) {
			case OPERATION_CALLROUTINE:
			case OPERATION_JMP:
			case OPERATION_JMPZERO:
			case OPERATION_JMPNEGATIVE:
				
				// get the (not Object * type) argument
				l.name = (char *)(void *)src->getArg();

				it = labels.find(l);

				// label not found
				if(it == labels.end()) {
					// printf("label %s not found! \n", replaceWhiteSpace(l.name));
					delete first;
					error = ERROR_SYNTAX;
					return NULL;
				}

				// delete name which isn't needed any more
				delete [] l.name;

				// direct connection to the right Source *
				src->setFlowCtrl(it->srcptr);
				src->setArg(NULL);

				// printf("connecting label\n");
				break;
			default:
				; // do nothing
			}

			src = src->getNext();
		}

		// clean up
		set_label::iterator it;

		for(it = labels.begin(); it != labels.end(); it++) {
			delete [] it->name;
		}

		// printf("second pass completed! \n");

		return first;
	} /* if (source) */
	return NULL;
}

int WsPreProcessor::getError() {
    return error;
}

const char* WsPreProcessor::getSource() {
    return source;
}



