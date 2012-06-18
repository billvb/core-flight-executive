/*
**
**  File Name: cfe_evs.c
**  $Id: cfe_evs.c 1.4 2010/10/26 17:01:42EDT jmdagost Exp  $
**
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
**  Title: Event Services API library
**
**  Purpose: This module defines the library functions of the
**           Event Services API
**
**  $Date: 2010/10/26 17:01:42EDT $
**  $Revision: 1.4 $
**  $Log: cfe_evs.c  $
**  Revision 1.4 2010/10/26 17:01:42EDT jmdagost 
**  Made CFE_EVS_ResetFilter() argument a signed int and added test for positive.
**  Revision 1.3 2010/10/25 15:01:44EDT jmdagost 
**  Corrected bad apostrophe in prologue.
**  Revision 1.2 2010/10/04 15:27:15EDT jmdagost 
**  Cleaned up copyright symbol.
**  Revision 1.1 2008/04/17 08:05:12EDT ruperera 
**  Initial revision
**  Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/src/evs/project.pj
**  Revision 1.35 2007/08/07 12:52:41EDT David Kobe (dlkobe) 
**  Modified CFE_ES_GetPoolBuf API's first parameter to be of type uint32**
**  Revision 1.34 2007/07/18 11:59:00EDT dlkobe 
**  Replaced memory address reference with CFE_EVS_GlobalData.EVS_TableHdl.
**  Revision 1.33 2007/03/21 14:35:21EST njyanchik 
**  I didn't previously fix this DCR for these two other Send Event APIs.
**  Revision 1.32 2007/02/06 14:42:37EST njyanchik 
**  this CP checks in the changes to fix the broken message truncation counter
**  Revision 1.31 2006/07/31 12:38:49GMT-05:00 njyanchik 
**  LDRA complained about passing an uninitialized variable into a function (even
**  when the function itself initializes the variable).
**  
**  In a structure definition, there was a uint16 AppID variable. It should be uint32.
**  Revision 1.30 2006/06/09 16:27:25EDT rjmcgraw 
**  Swapped parameters for memory pool API's
**  Revision 1.29 2006/06/09 19:28:15GMT kkaudra 
**  Changed size of BigBuf, removed static declaration from BigBuf, replaced vsprintf with vsnprintf
**  Revision 1.28 2006/06/08 14:14:54EDT njyanchik 
**  I added the appropriate legal headers to all of the evs files
**
*/

/* Include Files */
#include "cfe_evs.h"         /* EVS library function definitions */
#include "cfe_evs_task.h"    /* EVS internal definitions */
#include "cfe_evs_utils.h"   /* EVS utility function definitions */
#include "common_types.h"    /* Basic data types */
#include "cfe_es.h"          /* Executive Service definitions */
#include "cfe_error.h"       /* cFE error code definitions */

#include <stdarg.h>
#include <string.h>

/* External Data */
extern CFE_EVS_GlobalData_t   CFE_EVS_GlobalData;


/* Local Function Prototypes */


/* Function Definitions */

/*
**             Function Prologue
**
** Function Name:      CFE_EVS_Register
**
** Purpose:  This routine registers an application for receiving event services.
**
** Assumptions and Notes:
**
*/
int32 CFE_EVS_Register (void *Filters, uint16 NumEventFilters, uint16 FilterScheme)
{
   int32                  Status = CFE_SUCCESS;
   uint32                 AppID = 0xFFFFFFFF;
   CFE_EVS_Table_t        *TableEntry;

   /* Retreive the calling application's ID number */
   Status = CFE_ES_GetAppID(&AppID);

   if(Status == CFE_SUCCESS)
   {
      /* Check validity of AppID and application registration */
      Status = EVS_CheckApplication(AppID);

      if(Status == CFE_SUCCESS)
      {
         /* Unregister application if previously registered */
         Status = EVS_UnregisterInternal(AppID);
      }
      if((Status == CFE_SUCCESS) || (Status == CFE_EVS_APP_NOT_REGISTERED))
      {
         if(FilterScheme == CFE_EVS_BINARY_FILTER)
         {
            /* Create table entries for storing application information*/
            Status = CFE_ES_GetPoolBuf((uint32 **)&TableEntry,
                                       CFE_EVS_GlobalData.EVS_TableHdl,                                   
                                       (sizeof(CFE_EVS_Table_t)));

            if((Status == CFE_SUCCESS)||(Status > 0))
            {
               CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address = TableEntry;

               /* Initialize and coppy application data to EVS table */
               Status = EVS_InitCpyApplicationData(AppID, Filters, NumEventFilters);
            }
         }
         else
         {
            Status = CFE_EVS_UNKNOWN_FILTER;
         }
      }
   }

   return Status;

} /* End CFE_EVS_Register */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_Unregister
**
** Purpose:  This routine unregisters an application for receiving event services.
**
** Assumptions and Notes:
**
*/
int32 CFE_EVS_Unregister ( void )
{
   uint32           AppID = 0xFFFFFFFF;
   int32            Status = CFE_SUCCESS;

   /* Retreive the calling application's ID number */
   Status = CFE_ES_GetAppID(&AppID);

   if(Status == CFE_SUCCESS)
   {
      Status = EVS_CheckApplication(AppID);
   }

   if(Status == CFE_SUCCESS)
   {
      Status = EVS_UnregisterInternal(AppID);
   }

   return Status;

} /* End CFE_EVS_Unregister */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_SendEvent
**
** Purpose:  This routine sends an event message, with message ID containing the current
**           processor/box and task ID as well as the eventID.
**
** Assumptions and Notes:
*/
int32 CFE_EVS_SendEvent (uint16 EventID, uint16 EventType, const char *Spec, ... )
{
   int32               Status = CFE_SUCCESS;
   uint32              AppID = 0xFFFFFFFF;
   CFE_TIME_SysTime_t  Time;
   char                BigBuf[CFE_EVS_MAX_MESSAGE_LENGTH];
   char                GTBigBuf[CFE_EVS_MAX_MESSAGE_LENGTH + 2];
   va_list             Ptr;

   /* Get application identifier */
   Status = CFE_ES_GetAppID(&AppID);

   if(Status == CFE_SUCCESS)
   {
      /* Copy message to event packet */
      va_start(Ptr, Spec);
      vsnprintf(GTBigBuf, CFE_EVS_MAX_MESSAGE_LENGTH +2, Spec, Ptr);
      va_end(Ptr);


      /* We make a buffer greater than than the max allowed. We can
       * check the length of the string to see if it is bigger
       * than the allowed. If it is, we trunc it and increment
       * the counter. 
       *
       * Then we copy the max size of the message over to another
       * buffer to be sent 
       */ 
      if(strlen(GTBigBuf) > CFE_EVS_MAX_MESSAGE_LENGTH)
      {
         CFE_EVS_GlobalData.EVS_TlmPkt.MessageTruncCounter++;
      }

      strncpy(BigBuf,GTBigBuf,CFE_EVS_MAX_MESSAGE_LENGTH);
      
      /* Get system time */
      Time = CFE_TIME_GetTime();

      /* Send event */
      Status = EVS_SendEvent(AppID, Time, EventID, EventType, BigBuf);
   }

   return Status;

} /* End CFE_EVS_SendEvent */

/*
**             Function Prologue
**
** Function Name:      CFE_EVS_SendEventWithAppID
**
** Purpose:  This routine sends an event message, with message ID containing the current
**           processor/box and task ID as well as the eventID. Also forces the user to
**           supply an AppID to be sent with the message.
**
** Assumptions and Notes:
*/
int32 CFE_EVS_SendEventWithAppID (uint16 EventID, uint16 EventType,uint32 AppID, const char *Spec, ... )
{
   int32               Status = CFE_SUCCESS;
   CFE_TIME_SysTime_t  Time;
   char                BigBuf[CFE_EVS_MAX_MESSAGE_LENGTH];
   char                GTBigBuf[CFE_EVS_MAX_MESSAGE_LENGTH + 2];
   va_list             Ptr;

   
   /* Check to make sure the given AppID is legit */
   Status = EVS_CheckApplication(AppID);
   
   if(Status == CFE_SUCCESS)
   {
      /* Copy message to event packet */
      va_start(Ptr, Spec);
      vsnprintf(GTBigBuf, CFE_EVS_MAX_MESSAGE_LENGTH +2, Spec, Ptr);
      va_end(Ptr);

      if(strlen(GTBigBuf) > CFE_EVS_MAX_MESSAGE_LENGTH)
      {
         CFE_EVS_GlobalData.EVS_TlmPkt.MessageTruncCounter++;
      }

      strncpy(BigBuf,GTBigBuf,CFE_EVS_MAX_MESSAGE_LENGTH);

      /* Get system time */
      Time = CFE_TIME_GetTime();

      /* Send event */
      Status = EVS_SendEvent(AppID, Time, EventID, EventType, BigBuf);
   }

   return Status;

} /* End CFE_EVS_SendEventWithAppID */




/*
**             Function Prologue
**
** Function Name:      CFE_EVS_SendTimedEvent
**
** Purpose:  This routine sends a timed event message, with message ID containing the
**           current processor/box and task ID as well as the eventID.
**
** Assumptions and Notes:
*/
int32 CFE_EVS_SendTimedEvent (CFE_TIME_SysTime_t Time, uint16 EventID, uint16 EventType, const char *Spec, ... )
{
   int32              Status;
   uint32             AppID = 0xFFFFFFFF;
   char               BigBuf[CFE_EVS_MAX_MESSAGE_LENGTH];
   char                GTBigBuf[CFE_EVS_MAX_MESSAGE_LENGTH + 2];
   va_list            Ptr;

    /* Get application identifier */
   Status = CFE_ES_GetAppID(&AppID);

   if(Status == CFE_SUCCESS)
   {
      /* Copy message to event packet */
      va_start(Ptr, Spec);
      vsnprintf(GTBigBuf,CFE_EVS_MAX_MESSAGE_LENGTH + 2, Spec, Ptr);
      va_end(Ptr);


      if(strlen(GTBigBuf) > CFE_EVS_MAX_MESSAGE_LENGTH)
      {
         CFE_EVS_GlobalData.EVS_TlmPkt.MessageTruncCounter++;
      }

      strncpy(BigBuf,GTBigBuf,CFE_EVS_MAX_MESSAGE_LENGTH);


      Status = EVS_SendEvent(AppID, Time, EventID, EventType, BigBuf);
   }

   return Status;

} /* End CFE_EVS_SendTimedEvent */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_ResetFilter
**
** Purpose:  This routine clears the filter counter for the calling application's given event_id.
**
** Assumptions and Notes:
**
*/
int32 CFE_EVS_ResetFilter (int16 EventID)
{
   int32                  Status = CFE_SUCCESS;
   CFE_EVS_EVSBinFilter_t *FilterPtr = NULL;
   uint32                 AppID = 0xFFFFFFFF;
   CFE_EVS_Table_t        *TablePtr;

   /* Retreive calling application's ID */
   Status = CFE_ES_GetAppID(&AppID);

   if(Status == CFE_SUCCESS)
   {
      /* Check validity of AppID and application registration */
      Status = EVS_CheckApplication(AppID);

      if(Status == CFE_SUCCESS)
      {
         if((EventID >= 0) && (EventID <= CFE_EVS_MAX_EVENT_FILTERS))
         {
            TablePtr = CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address;
             
            FilterPtr = EVS_FindEventID(EventID, (CFE_EVS_EVSBinFilter_t *)&TablePtr->BinFilters);

            if(FilterPtr != NULL)
            {
               FilterPtr->Count = 0;
            }
         }
      }
   }

   return Status;

} /* End CFE_EVS_ResetFilter */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_ResetAllFilters
**
** Purpose:  This routine clears all filter counters for the calling application
**
** Assumptions and Notes:
**
*/
int32 CFE_EVS_ResetAllFilters ( void )
{
   int32                  Status = CFE_SUCCESS;
   uint32                 AppID = 0xFFFFFFFF;
   uint32                 i;

   /* Retreive calling application's ID */
   Status = CFE_ES_GetAppID(&AppID);

   if(Status == CFE_SUCCESS)
   {
       /* Check validity of AppID and application registration */
       Status = EVS_CheckApplication(AppID);

       if(Status == CFE_SUCCESS)
       {
          for(i=0; i<CFE_EVS_MAX_EVENT_FILTERS; i++)
          {
             CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address->BinFilters[i].Count = 0;
          }
       }
   }

   return Status;

} /* End CFE_EVS_ResetAllFilters */


/* End CFE_EVS.C */
