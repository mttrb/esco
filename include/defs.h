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

#ifndef DEFS_H
#define DEFS_H

enum StepRetCode {
    STEP_EXEC_OK = 0x001, //Instruction executed
    STEP_EXEC_END = 0x002,//End of programm detected
    STEP_EXEC_ERR = 0x004,//Error of executing
    STEP_DATA_CHG = 0x008,//Data at current Ptr changed
    STEP_EXEC_OUT = 0x010,//Instruction pointer out of range
    STEP_DATA_OUT = 0x020, //Data pointer out of range
    STEP_STACK_ERR = 0x040, // Stack UNDERflow
    STEP_EXEC_DIVZERO = 0x080 // division by zero
};

enum SourceRetCode {
    ERROR_OK = 0,
    ERROR_READ = 1,
    ERROR_FILENOTFOUND = 2,
    ERROR_SYNTAX = 3,
    ERROR_DUPLICATE_LABEL = 4, 
    ERROR_OTHER = 5
};

const int BFDEFMEMSIZE = 30000;//~ 30k Kb

#endif




