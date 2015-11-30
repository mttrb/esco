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
#include <string>
#include <iostream>
#include <cassert>
#include <cstdio>

#include  <errno.h>
#include "filereader.h"
#include "defs.h"

using std::ifstream;
using std::ios;
using namespace std;

FileReader::FileReader(const char* _file) {
    file = _file;
    error = ERROR_OK;
};

FileReader::~FileReader() {
}

const char* FileReader::read() {
    FILE* pfile;
    char* buffer = NULL;
    int result;

    // open the file NOT in binary mode
    // for tolerating windows-style
    // newline 
    // t means textmode
    pfile = fopen(file, "rt");
    if (pfile != 0) {
        //obtain file size:
        fseek (pfile , 0 , SEEK_END);
        int size = ftell (pfile);
        rewind (pfile);

        buffer = new char [size+2];
        assert(buffer != NULL);
        result = fread(buffer, 1, size, pfile);

        fclose(pfile);
        // as in windows-compatible mode, 
	// all '\r' is skipped, the following
	// will always return an error, 
	// because result < size. 
	// Therefor i commented it out, even if 
	// there is no test now if the file was read
	// correctly
        /*if (result != size){
            perror(file);
            error = ERROR_READ;
            return NULL;
        }*/
        buffer[result] = '\00';
        return buffer;
    }
    else 
        if (errno == ENOENT)
            error = ERROR_FILENOTFOUND;
        else
            error = ERROR_READ;

    perror(file);
    if(pfile) {
    	fclose(pfile);
    }
    return NULL;
}

SourceRetCode FileReader::getError() {
    return error;
}

