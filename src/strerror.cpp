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
#include "defs.h"
#include "strerror.h"

/* Convert SourceReader error number to string */
const char* rerr2str(int error) {
    switch(error) {
        case ERROR_OK:
            return "No error occured";
        case ERROR_READ:
            return "Error during reading";
        case ERROR_FILENOTFOUND:
            return "File not found";
        case ERROR_SYNTAX:
            return "Syntax error";
        case ERROR_DUPLICATE_LABEL:
            return "Duplicate label";

        case ERROR_OTHER:
        default:
            return "Unknow error";
    }
}

/* Convert machine error to string */
const char* eerr2str(int error) {
    if (error==STEP_EXEC_OK)
        return "No eror occured";
    char* str = new char [255];
    if (error & STEP_EXEC_ERR)
        strcpy(str,"Error during execution\n");
    else if (error & STEP_EXEC_END)
        strcpy(str, "End of program\n");
    else if (error & STEP_DATA_OUT)
        strcpy(str, "Data out of range\n");
    else if (error & STEP_EXEC_OUT)
        strcpy(str, "Program pointer out of range\n");
    else if (error & STEP_STACK_ERR)
        strcpy(str, "Stack underflow\n");
    return str;
}




