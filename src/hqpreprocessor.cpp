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
#include "hqpreprocessor.h"
#include "hqmachine.h"

HQPreProcessor::HQPreProcessor(SourceReader* _sr) {
    error = ERROR_OK;
    source = _sr->read();
    if (source == NULL)
        error = _sr->getError();
}

HQPreProcessor::~HQPreProcessor() {
    delete [] source;
}

Source* HQPreProcessor::process() {
    if (source) {
        Source* first = NULL;
        Source* src = NULL;
        for (unsigned int i = 0; i < strlen(source); i++) {
            if (strchr("HhQq9+\n \t", source[i])) {
                hqop op = hqops_last;
                switch (source[i]) {
                    case 'H':
                    case 'h':
                        op = op_h;
                        break;
                    case 'q':
                    case 'Q':
                        op = op_q;
                        break;
                    case '+':
                        op = op_plus;
                        break;
                    case '9':
                        op = op_9;
                        break;
                    case '\n':
                    case ' ':
                    case '\t':
                        /* just ignore */
                        op = hqops_last;
                        break;
                }
                if (op < hqops_last) {
                    src = new Source((char)op, NULL, i, 1, src);
                    if (first == NULL)
                        first = src;
                }
            }
            else {
                delete first;
                error = ERROR_SYNTAX;
                return NULL;
            }
        }
        return first;
    }
    return NULL;
};

int HQPreProcessor::getError() {
    return error;
}

const char* HQPreProcessor::getSource() {
    return source;
}





