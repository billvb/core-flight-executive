/*
**
**  File Name: cfe_evs_utils.c
**  $Id: cfe_evs_utils.c 1.7 2010/10/25 15:01:40EDT jmdagost Exp  $
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
**  Title: Event Services Utility functions
**
**  Purpose: This module defines the utility functions of the
**           Event Services Task and API
**
**  $Date: 2010/10/25 15:01:40EDT $
**  $Revision: 1.7 $
**  $Log: cfe_evs_utils.c  $
**  Revision 1.7 2010/10/25 15:01:40EDT jmdagost 
**  Corrected bad apostrophe in prologue.
**  Revision 1.6 2010/10/04 15:27:13EDT jmdagost 
**  Cleaned up copyright symbol.
**  Revision 1.5 2009/07/28 17:16:57EDT aschoeni 
**  Added event message when filtering limit is reached.
**  Revision 1.4 2009/06/10 09:13:23EDT acudmore 
**  Converted OS_Mem* and OS_BSP* calls to CFE_PSP_*
**  Revision 1.3 2009/04/03 10:03:58EDT sjudy 
**  Put the '\n' chars after the '%s' in the functions OutputPortX like the way is was in version 1.1.  
**  It was decided that this was a messy thing to do and the CFE community was unhappy with it.
**  Revision 1.2 2009/03/26 16:12:37EDT sjudy 
**  In each of  EVS_OutputPortx I changed "%s\n" to "\n%s" to put the linefeed before the output
**  message.
**  Revision 1.1 2008/04/17 08:05:14EDT ruperera 
**  Initial revision
**  Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/src/evs/project.pj
**  Revision 1.30 2007/09/13 09:46:11EDT rjmcgraw 
**  DCR4861 Added sem give prior to SendMsg call, then sem take after the call
**  Revision 1.29 2007/08/07 12:52:44EDT David Kobe (dlkobe) 
**  Modified CFE_ES_GetPoolBuf API's first parameter to be of type uint32**
**  Revision 1.28 2007/07/18 11:59:00EDT dlkobe 
**  Replaced memory address reference with CFE_EVS_GlobalData.EVS_TableHdl.
**  Revision 1.27 2007/05/07 13:44:45EDT njyanchik 
**  I'm removing things that shouldn't have been checked in
**  Revision 1.26 2007/05/07 10:42:05EDT njyanchik 
**  I added a sending of an Event message from EVS on the first time an EVS-unregistered
**  application tries to send an event message. A system log message is also written
**  Revision 1.25 2007/03/14 10:55:15EST njyanchik 
**  This is a check in of EVS's cleanup function for apps
**  Revision 1.24 2007/02/06 14:23:21EST njyanchik 
**  This change package fixes the issue where is a message was sent from an app
**  not registered with EVS, then the UnregisteredApp counter never incremented
**  Revision 1.23 2006/06/09 15:28:20GMT-05:00 rjmcgraw 
**  Swapped parameters for memory pool API's
**  Revision 1.22 2006/06/08 18:14:55GMT njyanchik 
**  I added the appropriate legal headers to all of the evs files
**  Revision 1.21 2006/04/25 16:14:36EDT kkaudra 
**  Added unregistered app evt msg 
**  Revision 1.20 2005/11/29 11:18:25EST kkaudra
**  Filters are now added at the next available slot instead of at the end of the buffer
**
*/

/* Include Files */
#include "cfe_evs.h"         /* EVS library function definitions */
#include "cfe_evs_log.h"     /* EVS local event log definitions */
#include "cfe_evs_task.h"    /* EVS internal definitions */
#include "cfe_evs_utils.h"   /* EVS utility function definitions */

#include <stdio.h>
#include <string.h>

#include "cfe_error.h"       /* cFE error code definitions */
#include "common_types.h"    /* Basic data types */
#include "osapi.h"           /* OS API interface definitions */
#include "cfe_psp.h"          /* cFE PSP glue functions */
#include "cfe_sb.h"          /* Software Bus library function definitions */
#include "cfe_es.h"

/* External Data */
extern CFE_EVS_GlobalData_t   CFE_EVS_GlobalData;


/* Local Function Prototypes */
void EVS_SendEventViaPorts(const CFE_EVS_Packet_t EVS_Packet);
void EVS_OutputPort1(char *Message);
void EVS_OutputPort2(char *Message);
void EVS_OutputPort3(char *Message);
void EVS_OutputPort4(char *Message);

/* Function Definitions */

/*
**             Function Prologue
**
** Function Name:      EVS_InitCpyApplicationData
**
** Purpose:  This routine initializes and copies registered application data
**           to the EVS table of application data
**
** Assumptions and Notes:
**
*/
int32 EVS_InitCpyApplicationData(uint32 AppID, void *Filters, uint16 NumEventFilters)
{
   int                    i;
   int32                  Status = CFE_SUCCESS;
   CFE_EVS_BinFilter_t    *AppFilters;
   CFE_EVS_Table_t        *TablePtr = CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address;

   /* Copy & Initialize Event Filters */
   if(Filters != NULL)
   {
      AppFilters = Filters;

      if(NumEventFilters <= CFE_EVS_MAX_EVENT_FILTERS)
      {
         for(i=0; i<NumEventFilters; i++, AppFilters++)
         {
            TablePtr->BinFilters[i].EventID = AppFilters->EventID;
            TablePtr->BinFilters[i].Mask = AppFilters->Mask;
            TablePtr->BinFilters[i].Count = 0;
         }
         for(i=NumEventFilters; i<CFE_EVS_MAX_EVENT_FILTERS; i++)
         {
            TablePtr->BinFilters[i].EventID = CFE_EVS_FREE_SLOT;
            TablePtr->BinFilters[i].Mask = 0;
            TablePtr->BinFilters[i].Count = 0;
         }
      }
      else
      {
         for(i=0; i<CFE_EVS_MAX_EVENT_FILTERS; i++, AppFilters++)
         {
            TablePtr->BinFilters[i].EventID = AppFilters->EventID;
            TablePtr->BinFilters[i].Mask = AppFilters->Mask;
            TablePtr->BinFilters[i].Count = 0;
         }

         Status = CFE_EVS_APP_FILTER_OVERLOAD;
      }
   }
   else
   {
      for(i=0; i<CFE_EVS_MAX_EVENT_FILTERS; i++)
      {
         TablePtr->BinFilters[i].EventID = CFE_EVS_FREE_SLOT;
         TablePtr->BinFilters[i].Mask = CFE_EVS_NO_MASK;
         TablePtr->BinFilters[i].Count = 0;
       }
    }

    /* Initialize Application Table Entry */
    TablePtr->AppID = AppID;
    TablePtr->ActiveFlag = TRUE;
    TablePtr->EventCount = 0;
    TablePtr->EventTypesActiveFlag = CFE_EVS_DEFAULT_TYPE_FLAG;
    CFE_EVS_GlobalData.EVS_TablePtr[AppID].Registered = TRUE;

    return Status;

} /* End EVS_InitCpyApplicationData */


/*
**             Function Prologue
**
** Function Name:      EVS_SendEvent
**
** Purpose:  This routine sends an EVS event message out the software bus and all
**           enabled output ports if the calling application has been determined to
**           be registered and the event message is unfiltered
**
** Assumptions and Notes:
*/
int32 EVS_SendEvent(uint32 AppID, CFE_TIME_SysTime_t Time, uint16 EventID, uint16 EventType, char *EventString)
{
   int32              Status = CFE_SUCCESS;
   int32              OS_Status;
   CFE_EVS_Packet_t   EVS_Packet;
   boolean            IsFiltered = FALSE;
   CFE_EVS_Table_t    *TableEntry;
   char               LocalEventString [CFE_EVS_MAX_MESSAGE_LENGTH];
   char               AppName [OS_MAX_API_NAME];
   uint32             EVS_AppId;
   
   /* Check validity of AppID and application registration */
   Status = EVS_CheckApplication(AppID);

   if(Status == CFE_SUCCESS)
   {
      /* Is the event filtered? */
      IsFiltered = EVS_IsFiltered(AppID, EventID, EventType);

      if (!(IsFiltered))
      {
         OS_Status = OS_MutSemTake(CFE_EVS_GlobalData.EVS_SharedDataMutexID);

         if(OS_Status != OS_SUCCESS)
         {
            /* Write error message to the system log */
         }

         /* Initialize CCSDS event packet */
         CFE_SB_InitMsg(&(EVS_Packet), CFE_EVS_EVENT_MSG_MID, sizeof(CFE_EVS_Packet_t), FALSE);

         /* Set the packet timestamp */
         CFE_SB_SetMsgTime((CFE_SB_Msg_t *) &EVS_Packet, Time);

         /* Initialize event message string */
         EVS_Packet.Message[0] = '\0';

         /* Obtain task and system information */
         EVS_Packet.PacketID.SpacecraftID = CFE_PSP_GetSpacecraftId();
         CFE_ES_GetAppName(EVS_Packet.PacketID.AppName, AppID, OS_MAX_API_NAME);
         EVS_Packet.PacketID.ProcessorID  = CFE_PSP_GetProcessorId();
         EVS_Packet.PacketID.EventID      = EventID;
         EVS_Packet.PacketID.EventType    = EventType;

         /* Copy message string to event packet message buffer */
         strncpy(EVS_Packet.Message, EventString, CFE_EVS_MAX_MESSAGE_LENGTH);

         /* Ensure that the packet is always terminated by a null character */
         EVS_Packet.Message[CFE_EVS_MAX_MESSAGE_LENGTH-1] = '\0';

         #ifdef CFE_EVS_LOG_ON

            /* write event to the event log */
            EVS_AddLog(&EVS_Packet);

         #endif

         if(CFE_EVS_GlobalData.EVS_TlmPkt.MessageFormatMode == CFE_EVS_SHORT_FORMAT)
         {
            /* Send an empty message if short format is enabled */
            EVS_Packet.Message[0] = '\0';
         }


         OS_MutSemGive(CFE_EVS_GlobalData.EVS_SharedDataMutexID);

         /* send event out software bus */
         Status = CFE_SB_SendMsg((CFE_SB_Msg_t *) &EVS_Packet);

         OS_MutSemTake(CFE_EVS_GlobalData.EVS_SharedDataMutexID);
 
         /* send event out selected ports */
         EVS_SendEventViaPorts(EVS_Packet);

         /* Increment message send counters */
         if(CFE_EVS_GlobalData.EVS_TlmPkt.MessageSendCounter < CFE_EVS_MAX_EVENT_SEND_COUNT)
         {
            CFE_EVS_GlobalData.EVS_TlmPkt.MessageSendCounter++;
         }

         if(CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address->EventCount < CFE_EVS_MAX_EVENT_SEND_COUNT)
         {
            CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address->EventCount++;
         }

         OS_Status = OS_MutSemGive(CFE_EVS_GlobalData.EVS_SharedDataMutexID);

         if(OS_Status != OS_SUCCESS)
         {
            /* Write error message to the system log */
         }
      }
   }
   /* if the application is not registered with EVS do not perform event service */
   else if(Status == CFE_EVS_APP_NOT_REGISTERED)
   {
      
      /* either we gave the address back (through an unregister), or
       * we never had one to begin with 
       */
      if( (CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address == NULL) || 
              (AppID != CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address->AppID))
      { 
         /* increment the unregistered application counter */
         CFE_EVS_GlobalData.EVS_TlmPkt.UnregisteredAppCounter++;

         /* Create table entries for storing application information*/
         Status = CFE_ES_GetPoolBuf((uint32 **)&TableEntry,
                                    CFE_EVS_GlobalData.EVS_TableHdl,                                    
                                    (sizeof(CFE_EVS_Table_t)));

         if((Status == CFE_SUCCESS)||(Status > 0))
         {
            CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address = TableEntry;

            /* Initialize and copy application data to EVS table */
            Status = EVS_InitCpyApplicationData(AppID, NULL, 0);
         }
         /*unregs app gets no evs service --keep track of them */
         CFE_EVS_GlobalData.EVS_TablePtr[AppID].Registered = FALSE;


       /*  Get the information about the app that sent the message */
       CFE_ES_GetAppName(AppName,AppID, OS_MAX_API_NAME);

       /* Get EVS's Id so that we can sent an event message from EVS */
       CFE_ES_GetAppIDByName(&EVS_AppId, "CFE_EVS");

       sprintf(LocalEventString, "App %s not registered with Event Services. Unable to send event.", AppName);
     
       CFE_EVS_SendEventWithAppID(CFE_EVS_ERR_UNREGISTERED_EVS_APP,
                          CFE_EVS_ERROR,
                          EVS_AppId,
                          "App %s not registered with Event Services. Unable to send event",
                          AppName);

        CFE_ES_WriteToSysLog("CFE ERROR: App %s tried to send an event but is not registered with EVS \n", AppName);


         if( Status == CFE_SUCCESS)
         {
             Status = CFE_EVS_APP_NOT_REGISTERED;
         }
      }
                                                      
   }
   return Status;
} /* End EVS_SendEvent */


/*
**             Function Prologue
**
** Function Name:      EVS_SendEventViaPorts
**
** Purpose:  This routine sends a string event message out all enabled
**           output ports
**
** Assumptions and Notes:
*/
void EVS_SendEventViaPorts(const CFE_EVS_Packet_t EVS_Packet)
{
   char PortMessage[CFE_EVS_MAX_PORT_MSG_LENGTH];

   if(((CFE_EVS_GlobalData.EVS_TlmPkt.OutputPort & CFE_EVS_PORT1_BIT) >> 0)== TRUE)
   {
      /* Copy event message to string format */
      sprintf(PortMessage, "EVS Port1 %d/%d/%s %d: %s", (uint16) EVS_Packet.PacketID.SpacecraftID,
                                                        (uint16) EVS_Packet.PacketID.ProcessorID,
                                                        EVS_Packet.PacketID.AppName,
                                                        EVS_Packet.PacketID.EventID,
                                                        EVS_Packet.Message);
      /* Send string event out port #1 */
      EVS_OutputPort1(PortMessage);
   }

   if(((CFE_EVS_GlobalData.EVS_TlmPkt.OutputPort & CFE_EVS_PORT2_BIT) >> 1) == TRUE)
   {
      /* Copy event message to string format */
      sprintf(PortMessage, "EVS Port2 %d/%d/%s %d: %s", (uint16) EVS_Packet.PacketID.SpacecraftID,
                                                        (uint16) EVS_Packet.PacketID.ProcessorID,
                                                        EVS_Packet.PacketID.AppName,
                                                        EVS_Packet.PacketID.EventID,
                                                        EVS_Packet.Message);
      /* Send string event out port #2 */
      EVS_OutputPort2(PortMessage);
   }

   if(((CFE_EVS_GlobalData.EVS_TlmPkt.OutputPort & CFE_EVS_PORT3_BIT) >> 2) == TRUE)
   {
      /* Copy event message to string format */
      sprintf(PortMessage, "EVS Port3 %d/%d/%s %d: %s", (uint16) EVS_Packet.PacketID.SpacecraftID,
                                                        (uint16) EVS_Packet.PacketID.ProcessorID,
                                                        EVS_Packet.PacketID.AppName,
                                                        EVS_Packet.PacketID.EventID,
                                                        EVS_Packet.Message);
      /* Send string event out port #3 */
      EVS_OutputPort3(PortMessage);
   }

   if(((CFE_EVS_GlobalData.EVS_TlmPkt.OutputPort & CFE_EVS_PORT4_BIT) >> 3) == TRUE)
   {
      /* Copy event message to string format */
      sprintf(PortMessage, "EVS Port4 %d/%d/%s %d: %s", (uint16) EVS_Packet.PacketID.SpacecraftID,
                                                        (uint16) EVS_Packet.PacketID.ProcessorID,
                                                        EVS_Packet.PacketID.AppName,
                                                        EVS_Packet.PacketID.EventID,
                                                        EVS_Packet.Message);
      /* Send string event out port #4 */
      EVS_OutputPort4(PortMessage);
   }

} /* End SendEventViaPorts */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_UnregisterInternal
**
** Purpose:  This routine unregisters an application for receiving event services.
**
** Assumptions and Notes:
**
*/
int32 EVS_UnregisterInternal (const uint32 AppID)
{
    int32 Status = CFE_SUCCESS;

    /* Set registered flag to false */
    CFE_EVS_GlobalData.EVS_TablePtr[AppID].Registered = FALSE;


    /* Clear Application EVS HK telemetry Information */
    CFE_EVS_GlobalData.EVS_TlmPkt.AppData[AppID - 1].AppID =  0;
    CFE_EVS_GlobalData.EVS_TlmPkt.AppData[AppID - 1].AppEnableStatus =  FALSE;
    CFE_EVS_GlobalData.EVS_TlmPkt.AppData[AppID - 1].AppMessageSentCounter = 0;

    /* Return unused memory to memory pool */
    Status = CFE_ES_PutPoolBuf((CFE_ES_MemHandle_t)CFE_EVS_GlobalData.EVS_TableRam,
                           (uint32 *)CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address);

    CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address = NULL;
    if(Status >= 0){
        return CFE_SUCCESS;
    }else{
        return Status;
    }

} /* End CFE_EVS_UnregisterInternal */


/*
**             Function Prologue
**
** Function Name:      EVS_GetApplicationInfo
**
** Purpose:  This routine returns the application ID and
**           status specifying the validity of the ID
**
** Assumptions and Notes:
**
*/
int32 EVS_GetApplicationInfo(uint32 *pAppID, char *pAppName)
{
   int32 Status = CFE_SUCCESS;

   if ((pAppName != NULL) && (pAppID != NULL))
   {
      Status = CFE_ES_GetAppIDByName(pAppID, pAppName);

      if(Status == CFE_SUCCESS)
      {
         Status = EVS_CheckApplication(*pAppID);
      }
   }
   else
   {
      Status = CFE_ES_ERR_BUFFER;
   }

   return Status;

} /* End EVS_GetApplicationInfo */


/*
**             Function Prologue
**
** Function Name:      EVS_CheckApplication
**
** Purpose:  This routine checks the validity of the input application ID
**           and if that application has been registered for event services
**
** Assumptions and Notes:
**
*/
int32 EVS_CheckApplication(uint32 AppID)
{
   int32 Status = CFE_SUCCESS;

   if(AppID >= CFE_ES_MAX_APPLICATIONS)
   {
      Status = CFE_EVS_APP_ILLEGAL_APP_ID;
   }
   if(Status == CFE_SUCCESS)
   {
      if(CFE_EVS_GlobalData.EVS_TablePtr[AppID].Registered != TRUE)
      {
         Status = CFE_EVS_APP_NOT_REGISTERED;
      }
   }

   return Status;

} /* End EVS_CheckApplication */


/*
**             Function Prologue
**
** Function Name:      EVS_FindEventID
**
** Purpose:  This routine searches and returns an index to the given Event ID with the
**           given application filter array.
**
** Assumptions and Notes:
**
*/
CFE_EVS_EVSBinFilter_t * EVS_FindEventID (const int16 EventID, CFE_EVS_EVSBinFilter_t *FilterArray)
{
   int i;
   CFE_EVS_EVSBinFilter_t *ReturnArray = (CFE_EVS_EVSBinFilter_t *)NULL;

   for(i=0; i<CFE_EVS_MAX_EVENT_FILTERS; i++, FilterArray++)
   {
      if((FilterArray->EventID == EventID) && (ReturnArray == NULL) )
      {
         ReturnArray = FilterArray;
      }
   }

   return ReturnArray;

} /* End EVS_FindEventID */


/*
**             Function Prologue
**
** Function Name:      EVS_IsFiltered
**
** Purpose:  This routine returns true if the given event identifier and event type
**           is filtered for the given application identifier.  Otherwise a value of
**           false is returned.
**
** Assumptions and Notes:
**
*/
boolean EVS_IsFiltered (const uint32 AppID, const uint16 EventID, const uint16 EventType)
{
   CFE_EVS_EVSBinFilter_t *FilterPtr;
   boolean                Filtered = FALSE;
   uint32                 EVS_AppId;
   char                   AppName [OS_MAX_API_NAME];
   CFE_EVS_Table_t        *TablePtr = CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address;

   /* Make sure application identifier is within range */
   if(AppID < CFE_ES_MAX_APPLICATIONS)
   {
      if(CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address->ActiveFlag == FALSE)
      {
         Filtered = TRUE;
      }
      else
      {
         switch(EventType)
         {
            case CFE_EVS_DEBUG:

               if(((TablePtr->EventTypesActiveFlag & CFE_EVS_DEBUG_BIT) >> 0) == FALSE)
               {
                  Filtered = TRUE;
               }
               break;

            case CFE_EVS_INFORMATION:

               if((( TablePtr->EventTypesActiveFlag & CFE_EVS_INFORMATION_BIT) >> 1) == FALSE)
               {
                  Filtered = TRUE;
               }
               break;

            case CFE_EVS_ERROR:

               if(((TablePtr->EventTypesActiveFlag & CFE_EVS_ERROR_BIT) >> 2) == FALSE)
               {
                  Filtered = TRUE;
               }
               break;

            case CFE_EVS_CRITICAL:

               if(((TablePtr->EventTypesActiveFlag & CFE_EVS_CRITICAL_BIT) >> 3) == FALSE)
               {
                  Filtered = TRUE;
               }
               break;

            default:
               /* Invalid Event Type */
               Filtered = TRUE;
               break;
         } /* End Switch */



      }/* end large else */


      /* Add to the counter if necessary */
      FilterPtr = EVS_FindEventID(EventID, (CFE_EVS_EVSBinFilter_t *)&TablePtr->BinFilters);

      if(FilterPtr != NULL)
      {
         if((FilterPtr->Mask & FilterPtr->Count) != 0)
         {
            Filtered = TRUE;
         }

         if(FilterPtr->Count < CFE_EVS_MAX_FILTER_COUNT)
         {
            FilterPtr->Count++;
            if(FilterPtr->Count == CFE_EVS_MAX_FILTER_COUNT)
            {
               CFE_ES_GetAppName(AppName,AppID, OS_MAX_API_NAME);

               /* Get EVS's Id so that we can send an event message from EVS */
               CFE_ES_GetAppIDByName(&EVS_AppId, "CFE_EVS");

               CFE_EVS_SendEventWithAppID(CFE_EVS_FILTER_MAX_EID,
                          CFE_EVS_INFORMATION,
                          EVS_AppId,
                          "Max filter count reached, AppName = %s, EventID = 0x%08x: Filter locked until reset",
                          AppName, EventID);
            }
         }
      }
   }

   return (Filtered);

} /* End EVS_IsFiltered */



/*
**             Function Prologue
**
** Function Name:      EVS_OutputPort1
**
** Purpose:  This routine sends the input message string out
**           EVS port 1
**
** Assumptions and Notes:
**
*/
void EVS_OutputPort1(char *Message)
{
    OS_printf("%s\n", Message);

} /* End ES_OutputPort1 */


/*
**             Function Prologue
**
** Function Name:      EVS_OutputPort2
**
** Purpose:  This routine sends the input message string out
**           EVS port 2
**
** Assumptions and Notes:
**
*/
void EVS_OutputPort2(char *Message)
{
   OS_printf("%s\n", Message);

} /* End ES_OutputPort2 */


/*
**             Function Prologue
**
** Function Name:      EVS_OutputPort3
**
** Purpose:  This routine sends the input message string out
**           EVS port 3
**
** Assumptions and Notes:
**
*/
void EVS_OutputPort3(char *Message)
{
   OS_printf("%s\n", Message);

} /* End ES_OutputPort3 */


/*
**             Function Prologue
**
** Function Name:      EVS_OutputPort4
**
** Purpose:  This routine sends the input message string out
**           EVS port 4
**
** Assumptions and Notes:
**
*/
void EVS_OutputPort4(char *Message)
{
   OS_printf("%s\n", Message);

} /* End ES_OutputPort4 */


/*
**             Function Prologue
**
** Function Name:      EVS_EnableTypes
**
** Purpose:  This routine enables all types with a bit mask of 1 for
**           the given input application
**
** Assumptions and Notes:
** The need to shift comes from trying to and (for example):
** 0x0002 and 0x0002. Using the only the last hex digit ,0010 AND 0010 comes
** out as 0010, not 1, as TRUE is defined, need to shift 1 spot to get TRUE.
**
*/
void EVS_EnableTypes(uint8 BitMask, uint32 AppID)
{
   CFE_EVS_Table_t *TablePtr = CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address;

   if(((BitMask & CFE_EVS_DEBUG_BIT) >> 0) == TRUE)
   {
      TablePtr->EventTypesActiveFlag |= CFE_EVS_DEBUG_BIT;
   }
   if(((BitMask & CFE_EVS_INFORMATION_BIT) >> 1) == TRUE)
   {
      TablePtr->EventTypesActiveFlag |= CFE_EVS_INFORMATION_BIT;
   }
   if(((BitMask & CFE_EVS_ERROR_BIT) >> 2) == TRUE)
   {
      TablePtr->EventTypesActiveFlag |= CFE_EVS_ERROR_BIT;
   }
   if(((BitMask & CFE_EVS_CRITICAL_BIT) >> 3) == TRUE)
   {
      TablePtr->EventTypesActiveFlag |= CFE_EVS_CRITICAL_BIT;
   }

} /* End EVS_EnableTypes */


/*
**             Function Prologue
**
** Function Name:      EVS_DisableTypes
**
** Purpose:  This routine disables all types with a bit mask of zero for
**           the given input application
**
** Assumptions and Notes:
**
*/
void EVS_DisableTypes(uint8 BitMask, uint32 AppID)
{
   CFE_EVS_Table_t *TablePtr = CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address;

   if(((BitMask & CFE_EVS_DEBUG_BIT) >> 0) == TRUE)
   {
      TablePtr->EventTypesActiveFlag &= ~CFE_EVS_DEBUG_BIT;
   }
   if(((BitMask & CFE_EVS_INFORMATION_BIT) >> 1) == TRUE)
   {
      TablePtr->EventTypesActiveFlag &= ~CFE_EVS_INFORMATION_BIT;
   }
   if(((BitMask & CFE_EVS_ERROR_BIT) >> 2) == TRUE)
   {
      TablePtr->EventTypesActiveFlag &= ~CFE_EVS_ERROR_BIT;
   }
   if(((BitMask & CFE_EVS_CRITICAL_BIT) >> 3) == TRUE)
   {
      TablePtr->EventTypesActiveFlag &= ~CFE_EVS_CRITICAL_BIT;
   }

} /* End EVS_DisableTypes */

/*
**             Function Prologue
**
** Function Name:      CFE_EVS_CleanUpApps
**
** Purpose:  This routine removes all EVS resouces used by the App 
**           that is specified in the parameter
**
** Assumptions and Notes:
**
*/
int32 CFE_EVS_CleanUpApp (uint32 AppId)
{
    int32 Status;
   
    Status = EVS_CheckApplication(AppId);

    if(Status == CFE_SUCCESS)
    {
        /* EVS_UnregisterInternal sets the registered flag to false,
         * and puts back the memory used to the memory pool for the
         * specified application */
      
        Status = EVS_UnregisterInternal(AppId);
    }

    return Status;
} /* End CFE_EVS_CleanUpApp */

/* End cfe_evs_utils */
