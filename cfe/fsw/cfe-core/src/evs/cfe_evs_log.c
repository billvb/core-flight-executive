/*
**
**  File Name: cfe_evslog.c
**  $Id: cfe_evs_log.c 1.4 2010/10/04 17:08:18EDT jmdagost Exp  $
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
**  Title: Event Services API - Log Control Interfaces
**
**  Purpose: This module defines the top level functions of the
**           Event Services Log control interfaces
**
** $Date: 2010/10/04 17:08:18EDT $
** $Revision: 1.4 $
** $Log: cfe_evs_log.c  $
** Revision 1.4 2010/10/04 17:08:18EDT jmdagost 
** Cleaned up copyright symbol.
** Revision 1.3 2010/09/21 16:12:12EDT jmdagost 
** Added conditional statements around conditionally-used local variables.
** Revision 1.2 2009/06/10 09:12:42EDT acudmore 
** Converted OS_Mem* and OS_BSP* calls to CFE_PSP_*
** Revision 1.1 2008/04/17 08:05:12EDT ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/src/evs/project.pj
** Revision 1.24 2007/07/18 15:47:59EDT njyanchik 
** Jonathan added text into each error event in the two functions that write files (WriteLog and WriteAppData), as well as updated the doxygen comments in the header file
** Revision 1.23 2007/05/14 10:26:03EDT apcudmore 
** Preserve the EVS Local event log on a processor restart:
** - moved the EVS Log to the ES reset data structure
** - Added a couple of EVS Log related variables into the EVS_Log_t structure
** - Added a routine to the EVS early init to get a pointer to the EVS Log 
** - Added code to the EVS housekeeping service routine to copy Log variables into TLM
** Revision 1.22 2007/04/03 09:27:29EDT njyanchik 
** I added the SubType to the cFE file header for the event log and the write app data files in EVS
** Revision 1.21 2007/03/02 10:59:57EST njyanchik 
** The names of the command functions in cfe_evs_task.c didn't start with "CFE_", so that has been 
** fixed.
** Revision 1.20 2007/03/01 11:54:33EST njyanchik 
** This cp handles issues 1,6,7,8 as described in the DCR
** Revision 1.19 2006/11/14 15:02:55EST rjmcgraw 
** Removed file open check
** Revision 1.18 2006/10/20 16:26:59EDT kkaudra 
** Added cmd parameter for filenames
** Revision 1.17 2006/09/01 11:17:13EDT kkaudra 
** IV&V:Removed osapi-os-filesys.h
** Revision 1.16 2006/08/31 17:50:56EDT kkaudra 
** Added event messages to set log mode ()
** Revision 1.15 2006/06/08 14:14:53EDT njyanchik 
** added the appropriate legal headers to all of the evs files
**
*/

/* Include Files */
#include "cfe_evs_task.h"     /* EVS internal definitions */
#include "cfe_evs_log.h"      /* EVS log file definitions */
#include "cfe_evs.h"          /* EVS API definitions */
#include "cfe_fs.h"           /* File Service definitions */
#include "cfe_error.h"        /* cFE error code definitions */
#include "cfe_psp.h"          /* Get reset area function prototype */
#include "../es/cfe_es_global.h" /* Get reset area type defintion */


#include <string.h>

/* External Data */
extern CFE_EVS_GlobalData_t   CFE_EVS_GlobalData;


/* Function Definitions */

/*
**             Function Prologue
**
** Function Name:      EVS_InitLogPtr
**
** Purpose:  This routine Initializes the EVS Global Event Log pointer. EVS stores a copy of the pointer 
**           to the EVS Event Log which is located in the ES Reset data area. 
**
** Assumptions and Notes:
**  1. This function may be called from the EVS Early Init routine, so it should not use semaphores, mutexes,
**  or other blocking system calls.
**  2. It works like this: ES stores the EVS Local Event Log in it's reset area structure. EVS needs to get access to
**     this Log, so it must call CFE_PSP_GetResetDataPtr to get the base address of the ES Reset area. The type is 
**     in cfe_es_global.h. The pointer to this log is then stored in the EVS global data structure, so 
**
*/
int32 EVS_InitLogPtr ( void )
{
   int32                Status;
   #ifdef CFE_EVS_LOG_ON
   uint32               resetAreaSize;
   CFE_ES_ResetData_t  *CFE_EVS_ResetDataPtr;
   #endif

   #ifdef CFE_EVS_LOG_ON

   /*
   ** Get the pointer to the Reset area from the BSP
   */
   Status = CFE_PSP_GetResetArea (&(CFE_EVS_ResetDataPtr), &(resetAreaSize));
   if (Status == OS_ERROR)
   {
      Status = CFE_EVS_RESET_AREA_POINTER;
   }
   else
   {
      CFE_EVS_GlobalData.EVS_LogPtr = &(CFE_EVS_ResetDataPtr->EVS_Log);
      Status = CFE_SUCCESS;
   }
   
   #else
   
      Status = CFE_SUCCESS;
   
   #endif
   
   return Status;

} /* End EVS_LogFull */


/*
**             Function Prologue
**
** Function Name:      EVS_LogFull
**
** Purpose:  This routine returns true if the internal event log is Full.  Otherwise a 
**           value of false is returned.
**
** Assumptions and Notes:
**
*/
boolean EVS_LogFull ( void )
{
   boolean Status = FALSE;
   
   #ifdef CFE_EVS_LOG_ON

   if(CFE_EVS_GlobalData.EVS_LogPtr->LogCount == CFE_EVS_LOG_MAX)
      Status = TRUE;
   else
      Status = FALSE;
      
   #endif

   return Status;

} /* End EVS_LogFull */


/*
**             Function Prologue
**
** Function Name:      EVS_AddLog
**
** Purpose:  This routine adds an event packet to the internal event log.
**
** Assumptions and Notes:
**
*/
void EVS_AddLog (CFE_EVS_Packet_t *EventMsg)
{
   #ifdef CFE_EVS_LOG_ON
   
   if((EVS_LogFull()) && (CFE_EVS_GlobalData.EVS_LogPtr->LogMode == CFE_EVS_LOG_DISCARD))
   {
        CFE_EVS_GlobalData.EVS_LogPtr->LogOverflowCounter++;
   }
   else
   {
      if(EVS_LogFull())
      {
         CFE_EVS_GlobalData.EVS_LogPtr->LogOverflowCounter++;
      }

      CFE_PSP_MemSet(&CFE_EVS_GlobalData.EVS_LogPtr->LogEntry[CFE_EVS_GlobalData.EVS_LogPtr->Next], 0x00, sizeof(CFE_EVS_Packet_t));
      CFE_PSP_MemCpy(&CFE_EVS_GlobalData.EVS_LogPtr->LogEntry[CFE_EVS_GlobalData.EVS_LogPtr->Next], EventMsg, sizeof(CFE_EVS_Packet_t));
      CFE_EVS_GlobalData.EVS_LogPtr->Next++;
      if(CFE_EVS_GlobalData.EVS_LogPtr->Next >= CFE_EVS_LOG_MAX)
         CFE_EVS_GlobalData.EVS_LogPtr->Next = 0;
      if(CFE_EVS_GlobalData.EVS_LogPtr->LogCount < CFE_EVS_LOG_MAX)
         CFE_EVS_GlobalData.EVS_LogPtr->LogCount++;
      if(CFE_EVS_GlobalData.EVS_LogPtr->LogCount == CFE_EVS_LOG_MAX)
         CFE_EVS_GlobalData.EVS_LogPtr->LogFullFlag = TRUE;
    }
    
    #endif

} /* End EVS_AddLog */


/*
**             Function Prologue
**
** Function Name:      EVS_ClearLog
**
** Purpose:  This routine clears the contents of the internal event log.
**
** Assumptions and Notes:
**
*/
boolean EVS_ClearLog ( void )
{
   #ifdef CFE_EVS_LOG_ON

   /* Reset Log Counters and Flags */
   CFE_EVS_GlobalData.EVS_LogPtr->Next = 0;
   CFE_EVS_GlobalData.EVS_LogPtr->LogCount = 0;
   CFE_EVS_GlobalData.EVS_LogPtr->LogFullFlag = FALSE;
   CFE_EVS_GlobalData.EVS_LogPtr->LogOverflowCounter = 0;

   #endif

   return TRUE;

} /* End EVS_ClearLog */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_WriteLogFileCmd
**
** Purpose:  This routine writes the contents of the internal event log to a file
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_WriteLogFileCmd ( const CFE_EVS_LogFileCmd_t *CmdPtr )
{
   int32           Status = TRUE;
   #ifdef CFE_EVS_LOG_ON
   uint32          FileSize = 0;
   uint32          EntryCount = 0;
   int32           LogFileHandle;
   int32           i;
   CFE_FS_Header_t LogFileHdr;
   char            LogFilename[OS_MAX_PATH_LEN];
   #endif

   #ifdef CFE_EVS_LOG_ON

   /* Copy the commanded filename into local buffer to ensure size limitation and to allow for modification */
   CFE_PSP_MemCpy(LogFilename, (void *)CmdPtr->LogFilename, OS_MAX_PATH_LEN);

   /* Check to see if a default filename should be used */
   if (LogFilename[0] == '\0')
   {
       strncpy(LogFilename, CFE_EVS_DEFAULT_LOG_FILE, OS_MAX_PATH_LEN);
   }

    /* Make sure all strings are null terminated before attempting to process them */
    LogFilename[OS_MAX_PATH_LEN-1] = '\0';

    /* Create the log file */
    LogFileHandle = OS_creat(LogFilename, OS_WRITE_ONLY);

    if(LogFileHandle  >= OS_FS_SUCCESS)
    {
      /* Initialize the standard cFE File Header for the Log File */
      CFE_PSP_MemSet(&LogFileHdr, 0, sizeof(CFE_FS_Header_t));
      strcpy(&LogFileHdr.Description[0], "cFE EVS Log File");

      /* Add the subtype for this file */
      LogFileHdr.SubType = CFE_FS_EVS_EVENTLOG_SUBTYPE;

      /* Output the Standard cFE File Header to the Log File */
      Status = CFE_FS_WriteHeader(LogFileHandle, &LogFileHdr);

      /* Maintain statistics of amount of data written to file */
      FileSize += Status;

      if(EVS_LogFull())
      {
         i = CFE_EVS_GlobalData.EVS_LogPtr->Next;

         do
         {
            Status = OS_write(LogFileHandle, &CFE_EVS_GlobalData.EVS_LogPtr->LogEntry[i], sizeof(CFE_EVS_Packet_t));
            FileSize += Status;
            EntryCount++;

            if(Status < OS_FS_SUCCESS)
            {
               CFE_EVS_SendEvent(CFE_EVS_ERR_WRLOGFILE_EID,
                                 CFE_EVS_ERROR,
                                 "Write Log File Command: Error writing to log file, OS_write returned 0x%08X, data filename = %s", Status, LogFilename);
            }

            i++;

            if(i >= CFE_EVS_LOG_MAX)
            {
              i = 0;
            }

         } while (i != CFE_EVS_GlobalData.EVS_LogPtr->Next);
      }
      else
      {
         for(i=0; i<=CFE_EVS_GlobalData.EVS_LogPtr->Next-1; i++)
         {
            Status = OS_write(LogFileHandle, &CFE_EVS_GlobalData.EVS_LogPtr->LogEntry[i], sizeof(CFE_EVS_Packet_t));
            FileSize += Status;
            EntryCount++;

            if(Status < OS_FS_SUCCESS)
            {
               CFE_EVS_SendEvent(CFE_EVS_ERR_WRLOGFILE_EID,
                                 CFE_EVS_ERROR,
                                 "Write Log File Command: Error writing to log file, OS_write returned 0x%08X, data filename = %s", Status, LogFilename);
            }
         }
      }

      Status = OS_close(LogFileHandle);

      if(Status < OS_FS_SUCCESS)
      {
         CFE_EVS_SendEvent(CFE_EVS_ERR_CLOSELOGFILE_EID,
                           CFE_EVS_ERROR,
                           "Write Log File Command: Error closing log file handle, OS_close returned 0x%08X, data filename = %s", Status, LogFilename);
         Status = FALSE;
      }
      else
      {
         Status = TRUE;
      }
   }

   else
   {
      Status = FALSE;
      OS_close(LogFileHandle);
      CFE_EVS_SendEvent(CFE_EVS_ERR_CRLOGFILE_EID,
                        CFE_EVS_ERROR,
                        "Write Log File Command: Error creating log file, OS_creat returned 0x%08X, data filename = %s", LogFileHandle, LogFilename);
   }

   if(Status == TRUE)
   {
      CFE_EVS_SendEvent(CFE_EVS_WRLOG_EID,
                        CFE_EVS_DEBUG,
                        "%s written: Size = %d, Entries = %d",
                        LogFilename, FileSize, EntryCount);
   }

   #endif

   return (boolean)Status;

} /* End CFE_EVS_WriteLogFileCmd */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_SetLoggingModeCmd
**
** Purpose:  This routine sets the internal event log mode.
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_SetLoggingModeCmd (CFE_EVS_ModeCmd_t *CmdPtr)
{
   boolean Status = TRUE;

   #ifdef CFE_EVS_LOG_ON

   if((CmdPtr->Mode == CFE_EVS_LOG_OVERWRITE) || (CmdPtr->Mode == CFE_EVS_LOG_DISCARD))
   {
      CFE_EVS_GlobalData.EVS_LogPtr->LogMode = CmdPtr->Mode;
      CFE_EVS_SendEvent(CFE_EVS_LOGMODE_EID,
                        CFE_EVS_DEBUG,
                        "Set Log Mode Command Received with Log Mode = %d", CmdPtr->Mode);
   }
   else
   {
      Status = FALSE;
      CFE_EVS_SendEvent(CFE_EVS_ERR_LOGMODE_EID,
                        CFE_EVS_ERROR,
                        "Set Log Mode Command: Invalid Log Mode = %d", CmdPtr->Mode);
   }

   #endif

   return Status;

} /* End CFE_EVS_SetLoggingModeCmd */

/* END EVSLOG.C */
