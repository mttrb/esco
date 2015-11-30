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

#ifndef B93PREPROCESSOR_H
#define B93PREPROCESSOR_H

#include "preprocessor.h"
#include "sourcereader.h"
#include "b93defs.h"
#include "b93source.h"

class B93PreProcessor : public PreProcessor {
    public:
        B93PreProcessor(SourceReader*);
        ~B93PreProcessor();

        Source* process();
        int getError();
        const char* getSource();
	B93Source *** getAllSource() { return allsource; }
	// int getMaxX() { return maxx; }
	// int getMaxY() { return maxy; }

    private:
        const char* source;
	unsigned char **newsource;
        int error;
	// for accesing the correct source with put and get
	B93Source *** allsource; /* yes, triple - * */
	// int maxx, maxy;

};



#endif









