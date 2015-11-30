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

/* It is like OokPreProcessor */

#include <config.h>
#include <cstring>
#include "defs.h"
#include "spoonpreprocessor.h"
#include "bfmachine.h"

#define PtrInc "1"
#define PtrDec "000"
#define IncPtr "010"
#define DecPtr "011"
#define WhileBegin "00100"
#define WhileEnd "0011"
#define PutChar "001010"
#define GetChar "0001011"
#define MaxCommandSize 7

const char* cmds [] = { IncPtr,DecPtr, PtrInc, PtrDec, WhileBegin, WhileEnd, PutChar, GetChar };
const unsigned int sizeof_cmds = sizeof cmds / sizeof cmds[0];

SpoonPreProcessor::SpoonPreProcessor(SourceReader* sr) {
    error = ERROR_OK;
    tsrc = sr->read();
    if (tsrc == NULL)
        error = sr->getError();
};

SpoonPreProcessor::~SpoonPreProcessor() {
    delete [] tsrc;
};

Source* SpoonPreProcessor::extractCommand( const char** p, Source *src, int *status ) {
    /* buffer for our command */
    char pcmd[ MaxCommandSize + 1 ];
    memset(pcmd, 0, MaxCommandSize+1);

    /* non zero is error */
    *status = 1;

    const char *fsymbol = *p;

    for ( int cln = 0; cln <= MaxCommandSize && **p != 0; ++*p ) {
        /* ignore separator */    
        if ( strchr(" \n\t\r", **p) ) {
            continue;
        }

        /* get next symbol of command */
        pcmd[cln] = **p;
        ++cln;

        /* Check possible commands */
        
        for (unsigned int j = 0; j < sizeof_cmds; j++) {
            if (!strcmp(cmds[j], pcmd)) {
                ++*p;
                *status = 0;
                return new Source(j, NULL, fsymbol-tsrc, *p-tsrc, src);
            }
        }
    
        /* nothing yet :( Try harder! */
    }
    
    /* end of script */
    if ( *pcmd == 0 )
        *status = 0;

    /* Unknown command? */

    return NULL;
}

Source* SpoonPreProcessor::process() {
    Source* first = NULL;
    Source* src = NULL;

    if (tsrc) {
        const char *p = tsrc;
        while ( *p != 0 ) {
            int status;
            src = extractCommand( &p, src, &status );

            if ( status != 0 ) {
                /* something unknown here */
                error = ERROR_SYNTAX;
                return NULL;
            }

            if (first == NULL)
                first = src;
        }
        
        src = first;
        /* Look thru cycles and bind [ and ] thru param arg */
        while (src != NULL ) {
            char op = src->getOpcode();
            if (op == while_begin) {
                int brkts = 1;
                Source* tmp = src->getNext();
                while (tmp != NULL) {
                    if (tmp->getOpcode() == while_end) {
                        --brkts;
                        if (brkts == 0)
                            break;
                    }
                    else if (tmp->getOpcode() == while_begin)
                        ++brkts;
                    tmp = tmp->getNext();
                }
                if (tmp==NULL) {
                    error = ERROR_SYNTAX;
                    return NULL;
                }
                src->setFlowCtrl(tmp->getNext());
                tmp->setFlowCtrl(src);
            }
            src = src->getNext();
        }
        return first;        
    }
    return NULL;
}

int SpoonPreProcessor::getError() {
    return error;
}

const char* SpoonPreProcessor::getSource() {
    return tsrc;
}


