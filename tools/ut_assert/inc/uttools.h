/*
**
** File: uttools.h
**
** $Id: uttools.h 1.1 2016/02/08 20:53:29EST rperera Exp  $
**
** Copyright 2012-2013 United States Government as represented by the 
** Administrator of the National Aeronautics and Space Administration. 
** All Other Rights Reserved.  
**
** This software was created at NASA's Goddard Space Flight Center.
** This software is governed by the NASA Open Source Agreement and may be 
** used, distributed and modified only pursuant to the terms of that 
** agreement.
**
** Purpose: This file contains functions to implement a set of tools for use in unit testing.
**
** $Log: uttools.h  $
** Revision 1.1 2016/02/08 20:53:29EST rperera 
** Initial revision
** Member added to project /FSW-TOOLs-REPOSITORY/ut-assert/inc/project.pj
** Revision 1.4 2015/06/16 16:02:15EDT sstrege 
** Added copyright information
** Revision 1.3 2015/03/10 15:19:30EDT czogby 
** Add Missing Functionality to UT Assert Library
** --- Added comments ---  czogby [2015/03/31 18:03:04Z]
** No source code changes were made in this revision.
**
*/

#ifndef _uttools_
#define	_uttools_

/*
 * Includes
 */

#include "common_types.h"

/*
 * Macro Definitions
 */

#define             UtMemSet memset

/*
 * Exported Functions
 */

/* Copies a region of memory to a binary file.  This file can be reloaded by calling UtBinFile2Mem or it can be
 * used to verify test results by calling UtMem2BinFileCmp. */
boolean             UtMem2BinFile(void *Memory, char *Filename, uint32 Length);

/* Copies a binary file to a region of memory. */
boolean             UtBinFile2Mem(void *Memory, char *Filename, uint32 Length);

/* Copies a region of memory to a hex file */
boolean             UtMem2HexFile(void *Memory, char *Filename, uint32 Length);

/* Fills a region of memory with a byte count pattern. */
void                UtMemFill(void *Memory, uint32 Length);

/* Just like the standard printf except it will supress its output unless the macro UT_VERBOSE
 * is defined. */
void                UtPrintf(char *Spec, ...);

/* Just like the standard sprintf except it returns a pointer to the result string.  The result string
 * cannot be larger than 256 bytes.  */
char               *UtSprintf(char *Spec, ...);

/* Calls UtPrintf to print a range of memory as hex bytes. */
void                UtPrintx(void *Memory, uint32 Length);

/* Compares a region of memory to a static pattern and determines if they are equal.  Note: Use UtMemSet to
 * fill a region of memory with a static pattern. */
boolean             UtMemCmpValue(void *Memory, uint8 Value, uint32 Length);

/* Compares a region of memory to a byte count pattern and determines if they are equal.  Note: Use UtMemFill to
 * fill a region of memory with a byte count pattern. */
boolean             UtMemCmpCount(void *Memory, uint32 Length);

/* Compares a region of memory with the contents of a binary file and determines if they are equal.  Note: Use
 * UtMem2BinFile to copy a region of memory to a binary file. */
boolean             UtMem2BinFileCmp(void *Memory, char *Filename);

#endif

