/*
**  $Id: psp_version.h 1.2.1.1 2010/11/16 09:51:31EST bmedina Exp  $
**
**
**      Copyright (c) 2004-2012, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software(cFE) was created at NASA's Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement.
** 
**
**
**
**  Purpose:
**  Provide version identifiers for the cFE Platform Support Packages (PSP).
**
*/

#ifndef _psp_version_
#define _psp_version_


/*
** Macro Definitions
*/
#define CFE_PSP_MAJOR_VERSION          1
#define CFE_PSP_MINOR_VERSION          1
#define CFE_PSP_REVISION               0
#define CFE_PSP_MISSION_REV            0

/* For backwards compatibility */
#define CFE_PSP_SUBMINOR_VERSION       CFE_PSP_REVISION 


#endif  /* _psp_version_ */

