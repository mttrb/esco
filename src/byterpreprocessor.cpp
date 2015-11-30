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
#include "byterpreprocessor.h"
#include "bytermemory.h"

extern const char* byt_cmds;

ByterPreProcessor::ByterPreProcessor(SourceReader* _sr) {
    error = ERROR_OK;
    source = _sr->read();
    if (NULL == source)
        error = _sr->getError();
}

ByterPreProcessor::~ByterPreProcessor() {
    delete [] source;
}

Source* ByterPreProcessor::process() {
    if (NULL == source)
        return NULL;
    ByterMemory* bm = new ByterMemory();
    int line = 0, i = 0, ind = 0, ssize = strlen(source);
    for (;ind < ssize && i < SIZE_X; ind++) {
        const char* pos = strchr(byt_cmds, source[ind]);
        if (pos != NULL) {
            bm->setCell(line, i++, static_cast<byter_cell>(pos-byt_cmds));
            if (SIZE_X == i) {
                ++ind;
                if (source[ind] == '\n' ||
                    (source[ind] == '\r' && source[ind+1] == '\n')) { /* For windows users */
                    if(source[ind+1] == '\n') ind++;
                    if (SIZE_X == i) {
                        if (SIZE_X == ++line)
                            break;
                        i = 0;
                    }
                    else {
                        error = ERROR_SYNTAX;
                        break;
                    }
                }
            }
        }
        else {
            /* Unknow symbol */
            error = ERROR_SYNTAX;
            break;
        }
                
    }
    if (ERROR_OK != error) {
        delete bm;
        bm = NULL;
    }

    return reinterpret_cast<Source*>(bm);//Some hack...
}



