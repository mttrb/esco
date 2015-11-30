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

#include <cstdio>
#include <config.h>
#include <cstring>
#include "sourcereader.h"
#include "bfpreprocessor.h"
#include "defs.h"
#include "bfmachine.h"

extern const char* bfcmds;

BFPreProcessor::BFPreProcessor(SourceReader* _sr) {
    error = ERROR_OK;
    source = _sr->read();
    if (source == NULL) {
        error = _sr->getError();
    }
}

BFPreProcessor::~BFPreProcessor() {
    delete[] source;
}


Source* BFPreProcessor::process() {
    if (source) {
        Source* first = NULL;
        Source* src = NULL;
        for (unsigned int i = 0; i < strlen(source); i++) {
            const char* offs = strchr(bfcmds, source[i]);
            if (offs != NULL) {
                src = new Source(offs-bfcmds, NULL, i, 1, src);
                if (first == NULL) {
                    first = src;
                }
            }
        }

        src = first;
        /* Look thru cycles and bind [ and ] thru param arg */
        while (src != NULL ) {
            int op = src->getOpcode();
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
    fprintf(stderr, "Error SourceReader returned NULL\n");
    return NULL;
}

int BFPreProcessor::getError() {
    return error;
}

const char* BFPreProcessor::getSource() {
    return source;
}



