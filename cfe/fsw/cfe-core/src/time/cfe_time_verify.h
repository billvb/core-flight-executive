/*
** $Id: cfe_time_verify.h 1.3 2010/10/25 15:00:05EDT jmdagost Exp  $
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
** Purpose:  cFE Time Services (TIME) configuration verification
**
** Author:   S.Walling/Microtel
**
** Notes:
**
** $Log: cfe_time_verify.h  $
** Revision 1.3 2010/10/25 15:00:05EDT jmdagost 
** Corrected bad apostrophe in prologue.
** Revision 1.2 2010/10/04 15:15:54EDT jmdagost 
** Cleaned up copyright symbol.
** Revision 1.1 2008/04/17 08:05:40EDT ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/src/time/project.pj
** Revision 1.2 2006/06/08 14:15:50EDT njyanchik 
** I added the appropriate legal headers to all of the time files
** Revision 1.1 2005/07/21 15:21:57EDT lswalling 
** Initial revision
** Member added to project d:/mksdata/MKS-CFE-REPOSITORY/cfe-core/time/project.pj
**
*/

/*
** Ensure that header is included only once...
*/
#ifndef _cfe_time_verify_
#define _cfe_time_verify_

/*************************************************************************/

/*
** Validate default time client/server selection...
*/
#ifdef CFE_TIME_CFG_SERVER
  #ifdef CFE_TIME_CFG_CLIENT
    #error Cannot define both CFE_TIME_CFG_SERVER and CFE_TIME_CFG_CLIENT!
  #endif
#else
  #ifndef CFE_TIME_CFG_CLIENT
    #error Must define either CFE_TIME_CFG_SERVER or CFE_TIME_CFG_CLIENT!
  #endif
#endif

/*
** Validate default time format selection...
*/
#ifdef CFE_TIME_CFG_DEFAULT_TAI
  #ifdef CFE_TIME_CFG_DEFAULT_UTC
    #error Cannot define both CFE_TIME_CFG_DEFAULT_UTC and CFE_TIME_CFG_DEFAULT_TAI!
  #endif
#else
  #ifndef CFE_TIME_CFG_DEFAULT_UTC
    #error Must define either CFE_TIME_CFG_DEFAULT_UTC or CFE_TIME_CFG_DEFAULT_TAI!
  #endif
#endif

/*
** Validate time source selection...
*/
#ifdef CFE_TIME_CFG_CLIENT
  #ifdef CFE_TIME_CFG_SOURCE
    #error Cannot define both CFE_TIME_CFG_CLIENT and CFE_TIME_CFG_SOURCE!
  #endif
#endif

#ifdef CFE_TIME_CFG_SOURCE
  #ifndef CFE_TIME_CFG_VIRTUAL
    #error Cannot define CFE_TIME_CFG_SOURCE without CFE_TIME_CFG_VIRTUAL!
  #endif
#endif

/*
** Validate local MET selections...
*/
#ifdef CFE_TIME_CFG_CLIENT
  #ifndef CFE_TIME_CFG_VIRTUAL
    #error Cannot define CFE_TIME_CFG_CLIENT without CFE_TIME_CFG_VIRTUAL!
  #endif
#endif

/*
** Validate time source type selection...
*/
#ifdef CFE_TIME_CFG_SRC_MET
  #ifndef CFE_TIME_CFG_SOURCE
    #error Cannot define CFE_TIME_CFG_SRC_MET without CFE_TIME_CFG_SOURCE!
  #endif
  #ifdef CFE_TIME_CFG_SRC_GPS
    #error Cannot define both CFE_TIME_CFG_SRC_MET and CFE_TIME_CFG_SRC_GPS!
  #endif
  #ifdef CFE_TIME_CFG_SRC_TIME
    #error Cannot define both CFE_TIME_CFG_SRC_MET and CFE_TIME_CFG_SRC_TIME!
  #endif
#endif

#ifdef CFE_TIME_CFG_SRC_GPS
  #ifndef CFE_TIME_CFG_SOURCE
    #error Cannot define CFE_TIME_CFG_SRC_GPS without CFE_TIME_CFG_SOURCE!
  #endif
  #ifdef CFE_TIME_CFG_SRC_TIME
    #error Cannot define both CFE_TIME_CFG_SRC_GPS and CFE_TIME_CFG_SRC_TIME!
  #endif
#endif

#ifdef CFE_TIME_CFG_SRC_TIME
  #ifndef CFE_TIME_CFG_SOURCE
    #error Cannot define CFE_TIME_CFG_SRC_TIME without CFE_TIME_CFG_SOURCE!
  #endif
#endif


/*
** Validate tone signal and data packet arrival selection...
*/
#ifdef CFE_TIME_AT_TONE_WAS
  #ifdef CFE_TIME_AT_TONE_WILL_BE
    #error Both CFE_TIME_AT_TONE_WAS and CFE_TIME_AT_TONE_WILL_BE have been defined!
  #endif
#else
  #ifndef CFE_TIME_AT_TONE_WILL_BE
    #error Either CFE_TIME_AT_TONE_WAS or CFE_TIME_AT_TONE_WILL_BE must be defined!
  #endif
#endif

/*
** Validate simulated tone signal and external time source selection...
*/
#ifdef CFE_TIME_CFG_FAKE_TONE
  #ifdef CFE_TIME_CFG_SOURCE
    #error Cannot define both CFE_TIME_CFG_FAKE_TONE and CFE_TIME_CFG_SOURCE!
  #endif
#endif

/*
** Validate simulated tone signal and data packet arrival selection...
*/
#ifdef CFE_TIME_CFG_FAKE_TONE
  #ifdef CFE_TIME_AT_TONE_WILL_BE
    #error Cannot define both CFE_TIME_CFG_FAKE_TONE and CFE_TIME_AT_TONE_WILL_BE!
  #endif
#endif

/*************************************************************************/

#endif /* _cfe_time_verify_ */

/************************/
/*  End of File Comment */
/************************/
