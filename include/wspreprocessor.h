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

#ifndef WSPREPROCESSOR_H
#define WSPREPROCESSOR_H

#include "preprocessor.h"
#include "sourcereader.h"
#include "wsdefs.h"

#include <set>
using namespace std;

bool IsWhiteSpace(char c);


// whitespace code <-> operation
struct WsOperation
{
	const char *code; // operation in whitespace
	int op; // operation
};

// operation idendifiers:

#define STACK SSPACE
#define ARITH STAB SSPACE
#define HEAP STAB STAB
#define FLOW SNEWL
#define IO STAB SNEWL

// list mapping between whitespace code and the operation

extern WsOperation WsOps[];

// for debugging purposes
char *replaceWhiteSpace(const char *);

class WsPreProcessor : public PreProcessor {
    public:
        WsPreProcessor(SourceReader*);
        ~WsPreProcessor();

        Source* process();
        int getError();
        const char* getSource();

    private:
        const char* source;
	char *newsource;
        int error;


	// Label (name and where in the source it points to)
	struct label {
		char *name;
		Source *srcptr;
	};

	// internal structure 
	struct less_label : public less <label> {
		bool operator ()(const label &a, const label &b) const {
			return strcmp(a.name, b.name) < 0;
		}
	};

	// smaller name
	typedef set<label, less_label> set_label;


	// all labels
	set_label labels;
};

#endif








