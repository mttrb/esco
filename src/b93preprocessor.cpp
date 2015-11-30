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
#include <cstdio>
#include "sourcereader.h"
#include "b93preprocessor.h"
#include "b93defs.h"
#include "b93source.h"
#include "escoconf.h"

B93PreProcessor::B93PreProcessor(SourceReader* _sr) {
	error = ERROR_OK;
	source = _sr->read();
	if (source == NULL)
	        error = _sr->getError();
	allsource = NULL;
	// maxx = maxy = 0;
}

B93PreProcessor::~B93PreProcessor() {
	delete[] source;
}

Source* B93PreProcessor::process() {
	unsigned int i;
	if(source) {
		// B93Source *** allsource; /* yes, triple - * */
		unsigned int maxx = 0, maxy = 0;
		unsigned int x, y;
		unsigned int lasti;

		// treat EOF  as '\n'
		for(i = 0, lasti = 0; i <= strlen(source); i++) {
			if(i == strlen(source) || source[i] == '\n') {
				maxy++;
				if(i - lasti > maxx) {
					maxx = i - lasti;
				}
				lasti = i+1;
			}
		}

		if(maxx > MAXX || maxy > MAXY) {
			dbgprint("Error: field to big! \n");
			error = ERROR_SYNTAX;
			return NULL;
		}

		dbgprint("%i x %i\n", maxx, maxy);

		// recreate source with full coloums
		// -1 indicates empty cell

		// how creating new int[maxx][maxy]?
		newsource = new unsigned char *[maxx];
		for(i = 0; i < maxx; i++) {
			newsource[i] = new unsigned char[maxy];
		}
		

		for(y = 0, i = 0; y < maxy; y++) {
			for(x = 0; x < maxx; x++) {
				if(i < strlen(source) && source[i] != '\n') {
					newsource[x][y] = (unsigned char)source[i];
					i++;
				}
				else {
					// treat <nothing> as space ?!?
					// or as \0
					newsource[x][y] = ' ';
				}
			}
			i++; // because of '\n'
		}
		

		// dump
		if(conf.getdebug() ) {
			for(y = 0; y < maxy; y++) {
				for(x = 0; x < maxx; x++) {
					printf("%c", newsource[x][y] != 0 ? newsource[x][y] : ' ');
				}
				printf("\n");
			}
		}

		// create 80x25 field (MAXY x MAXX)
		allsource = new B93Source **[MAXX];
		for(i = 0; i < MAXX; i++) {
			allsource[i] = new B93Source *[MAXY];
		} 

		// B93OP_INVALID only used for empty cells!
		for(y = 0; y < MAXY; y++) {
			for(x = 0; x < MAXX; x++) {
				if(x < maxx && y < maxy) {
					allsource[x][y] = new B93Source(newsource[x][y]);
				} else {
					// insert space
					allsource[x][y] = new B93Source(B93OP_NOP);
				}
			}
		}

		// set connections
		// cannot get out over the initial field (maxx, maxy), 
		// but whole field is still MAXX x MAXY big
		// there the specification does say nothing: 
		// what is if new code is generated in new cells for example?
		// at the moment this code cannot be executed. 
		// hope that will not be needed
		// the upper is solved via empty cell == whitespace
		for(y = 0; y < MAXY; y++) {
			for(x = 0; x < MAXX; x++) {
				allsource[x][y]->setNext(DIR_EAST, allsource[(x+1)%MAXX][y]);
				allsource[x][y]->setNext(DIR_WEST, allsource[(x-1+MAXX)%MAXX][y]);
				allsource[x][y]->setNext(DIR_NORTH, allsource[x][(y-1+MAXY)%MAXY]);
				allsource[x][y]->setNext(DIR_SOUTH, allsource[x][(y+1)%MAXY]);
			}
		}
		
		// dump
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

		// that is not needed any more
		for(x = 0; x < maxx; x++) {
			delete [] newsource[x];
		}
		delete [] newsource;

		dbgprint("Preprocessor run finished! \n");

		return allsource[0][0];
	} /* if (source) */
	return NULL;
}

int B93PreProcessor::getError() {
    return error;
}

const char* B93PreProcessor::getSource() {
    return source;
}



