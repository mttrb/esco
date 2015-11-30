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

/* Preprocessors for different languages will provides: transfer the source into executable form
 * Also it will process the macros (in future)
 */

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "sourcereader.h"
#include "source.h"

class PreProcessor {
    public:
        inline virtual ~PreProcessor();

        virtual Source* process() = 0;
        virtual int getError() = 0;
        virtual const char* getSource() = 0;
};

PreProcessor::~PreProcessor() {
};

#endif



