/*
**
**  File Name: cfe_evs_task.c
**  $Id: cfe_evs_task.c 1.6 2010/10/27 16:37:17EDT jmdagost Exp  $
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
**
**  Title: Event Service API Management Control Interfaces
**
**  Purpose: This module defines the top level functions of the
**           cFE Event Service task defining the control, command,
**           and telemetry interfaces
**
**  $Date: 2010/10/27 16:37:17EDT $
**  $Revision: 1.6 $
**  $Log: cfe_evs_task.c  $
**  Revision 1.6 2010/10/27 16:37:17EDT jmdagost 
**  Forced LogMode to default value on reset, added platform config and evs msg include files.
**  Revision 1.5 2010/09/21 16:12:13EDT jmdagost 
**  Added conditional statements around conditionally-used local variables.
**  Revision 1.4 2009/07/31 19:54:57EDT aschoeni 
**  Added length checking and fixed several comments and events
**  Revision 1.3 2009/06/10 09:13:01EDT acudmore 
**  Converted OS_Mem* and OS_BSP* calls to CFE_PSP_*
**  Revision 1.2 2008/07/31 15:41:38EDT apcudmore 
**  Added execution counter API:
**    -- Added execution counter to ES internal task data
**    -- Added code to increment counter in RunLoop
**    -- Added code to report counter in GetInfo APIs
**    -- Added API to increment counter for child tasks
**    -- Added code to cFE Core apps to increment counters.
**  Revision 1.1 2008/04/17 08:05:13EDT ruperera 
**  Initial revision
**  Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/src/evs/project.pj
**  Revision 1.1.2.72 2007/08/27 12:01:13EDT njyanchik 
**  I mustv'e accidentally overwritten DAve's change of adding an event ID to WriteAppData, so I
**  am fixing the mistake
**  Revision 1.1.2.71 2007/08/24 14:27:38EDT apcudmore 
**  Keep EVS from clearing out the log on a Processor reset
**  Revision 1.1.2.70 2007/07/18 15:48:00EDT njyanchik 
**  Jonathan added text into each error event in the two functions that write files (WriteLog and WriteAppData), as well as updated the doxygen comments in the header file
**  Revision 1.1.2.68 2007/06/05 15:37:36EDT njyanchik 
**  The log mode was not being set correctly in the telemetry. It was because the logmode variable
**  was assigned to the logfullflag variable in telemetry. I reassigned it to the appropriate variable.
**  Revision 1.1.2.67 2007/05/16 15:13:42EDT dlkobe 
**  Added MemPoolHandle members to HK packet
**  Revision 1.1.2.66 2007/05/16 08:44:36EDT njyanchik 
**  I re added the AppData Subtype to the AppData File header
**  Revision 1.1.2.65 2007/05/15 11:01:10EDT rjmcgraw 
**  DCR78:11 Exit main loop for CFE_SB_RcvMsg errors
**  Revision 1.1.2.64 2007/05/14 10:26:04EDT apcudmore 
**  Preserve the EVS Local event log on a processor restart:
**  - moved the EVS Log to the ES reset data structure
**  - Added a couple of EVS Log related variables into the EVS_Log_t structure
**  - Added a routine to the EVS early init to get a pointer to the EVS Log 
**  - Added code to the EVS housekeeping service routine to copy Log variables into TLM
**  Revision 1.1.2.63 2007/04/27 16:58:19EDT rjmcgraw 
**  Checked return from init event
**  Revision 1.1.2.62 2007/04/27 16:13:15EDT rjmcgraw 
**  DCR2987:4 Standardize task init error code
**  Revision 1.1.2.61 2007/04/27 12:31:06EDT njyanchik 
**  I changed EVS and TIME to not have a telemetry counter for SB pipe reads in the  
**  main loop. Instead, I have added a syslog message to state the problem
**  Revision 1.1.2.60 2007/04/26 14:45:39EDT rjmcgraw 
**  Added pool create and sem create error handling
**  Revision 1.1.2.59 2007/04/13 16:23:16EDT apcudmore 
**  Changed EarlyInit function prototype to return int32 status
**  Revision 1.1.2.58 2007/04/11 10:59:56EDT njyanchik 
**  Remove the TaskDelay in the main loop for EVS. I also addeda counterin telemtry 
**  that will notify the ground of an error in reading the command pipe.
**  Revision 1.1.2.57 2007/04/09 14:45:08EDT rjmcgraw 
**  Changed names of performance ids to include CFE_
**
*/

/* Include Files */
#include "cfe_evs_task.h"       /* EVS internal definitions */
#include "cfe_evs_log.h"        /* EVS log file definitions */
#include "cfe_evs_utils.h"      /* EVS utility function definitions */
#include "cfe_evs.h"            /* EVS API definitions */
#include "cfe_evs_msg.h"        /* EVS message definitions */

#include <string.h>

#include "cfe_error.h"        /* cFE error code definitions */
#include "cfe_es.h"           /* Executive Service definitions */
#include "cfe_fs.h"           /* File Service definitions */
#include "cfe_psp.h"          /* cFE Platform Support Package definitions */
#include "osapi.h"            /* OS API file system definitions */
#include "cfe_platform_cfg.h" /* cFE Platform Configuration definitions */

/* Global Data */
CFE_EVS_GlobalData_t CFE_EVS_GlobalData;


/*
** Local function prototypes.
*/
int32 CFE_EVS_TaskInit ( void );
void  CFE_EVS_ProcessCommandPacket ( CFE_SB_MsgPtr_t EVS_MsgPtr );
void  CFE_EVS_ProcessGroundCommand ( CFE_SB_MsgPtr_t EVS_MsgPtr );
void  CFE_EVS_ReportHousekeepingCmd ( void );
void  CFE_EVS_ResetCountersCmd ( void );
boolean CFE_EVS_VerifyCmdLength(CFE_SB_MsgPtr_t Msg, uint16 ExpectedLength);
boolean CFE_EVS_SetFilterMaskCmd (const CFE_EVS_AppNameEventIDMaskCmd_t *CmdPtr);
boolean CFE_EVS_EnablePortsCmd (const CFE_EVS_BitMaskCmd_t *CmdPtr);
boolean CFE_EVS_DisablePortsCmd (const CFE_EVS_BitMaskCmd_t *CmdPtr);
boolean CFE_EVS_EnableEventTypesCmd (const CFE_EVS_BitMaskCmd_t *CmdPtr);
boolean CFE_EVS_DisableEventTypesCmd (const CFE_EVS_BitMaskCmd_t *CmdPtr);
boolean CFE_EVS_SetEventFormatModeCmd (const CFE_EVS_ModeCmd_t *CmdPtr);
boolean CFE_EVS_EnableAppEventTypesCmd (const CFE_EVS_AppNameBitMaskCmd_t *CmdPtr);
boolean CFE_EVS_DisableAppEventTypesCmd (const CFE_EVS_AppNameBitMaskCmd_t *CmdPtr);
boolean CFE_EVS_EnableAppEventsCmd (const CFE_EVS_AppNameCmd_t *CmdPtr);
boolean CFE_EVS_DisableAppEventsCmd (const CFE_EVS_AppNameCmd_t *CmdPtr);
boolean CFE_EVS_ResetAppEventCounterCmd (const CFE_EVS_AppNameCmd_t *CmdPtr);
boolean CFE_EVS_ResetFilterCmd (const CFE_EVS_AppNameEventIDCmd_t *CmdPtr);
boolean CFE_EVS_AddEventFilterCmd (const CFE_EVS_AppNameEventIDMaskCmd_t *CmdPtr);
boolean CFE_EVS_DeleteEventFilterCmd (const CFE_EVS_AppNameEventIDCmd_t *CmdPtr);
boolean CFE_EVS_WriteAppDataCmd ( const CFE_EVS_AppDataCmd_t *CmdPtr );
boolean CFE_EVS_ResetAllFiltersCmd (const CFE_EVS_AppNameCmd_t *CmdPtr);

/* Function Definitions */


/*
**             Function Prologue
**
** Function Name:      EVS_TaskMain
**
** Purpose:  This is the main EVS task process loop.
**
** Assumptions and Notes:
**
*/
void CFE_EVS_TaskMain(void)
{
    int32 Status;    
    CFE_SB_MsgPtr_t    EVS_MsgPtr; /* Pointer to SB message */

    CFE_ES_PerfLogEntry(CFE_EVS_MAIN_PERF_ID);    
   
    Status = CFE_EVS_TaskInit();
    
    if(Status != CFE_SUCCESS)
    {
      CFE_ES_WriteToSysLog("EVS:Application Init Failed,RC=0x%08X\n", Status);
      CFE_ES_PerfLogExit(CFE_EVS_MAIN_PERF_ID);
      /* Note: CFE_ES_ExitApp will not return */
      CFE_ES_ExitApp(CFE_ES_CORE_APP_INIT_ERROR);
    }/* end if */      

    /* Main loop */
    while (Status == CFE_SUCCESS)
    {
        /* Increment the Main task Execution Counter */
        CFE_ES_IncrementTaskCounter();
    
        CFE_ES_PerfLogExit(CFE_EVS_MAIN_PERF_ID);

        /* Pend on receipt of packet */
        Status = CFE_SB_RcvMsg(&EVS_MsgPtr, 
                               CFE_EVS_GlobalData.EVS_CommandPipe, 
                               CFE_SB_PEND_FOREVER);

        CFE_ES_PerfLogEntry(CFE_EVS_MAIN_PERF_ID);

        if (Status == CFE_SUCCESS)
        {
            /* Process cmd pipe msg */
            CFE_EVS_ProcessCommandPacket(EVS_MsgPtr);
        }else{            
            CFE_ES_WriteToSysLog("EVS:Error reading cmd pipe,RC=0x%08X\n",Status);
        }/* end if */
    
    }/* end while */

    /* while loop exits only if CFE_SB_RcvMsg returns error */
    CFE_ES_ExitApp(CFE_ES_CORE_APP_RUNTIME_ERROR);

} /* end CFE_EVS_TaskMain */



/*
**             Function Prologue
**
** Function Name:      CFE_EVS_TaskInit
**
** Purpose:  This function performs any necessary EVS task initialization.
**
** Assumptions and Notes:
**
*/
int32 CFE_EVS_TaskInit ( void )
{
    int32 Status;
 
    Status = CFE_ES_RegisterApp();
    if(Status != CFE_SUCCESS)
    {
      CFE_ES_WriteToSysLog("EVS:Call to CFE_ES_RegisterApp Failed:RC=0x%08X\n",Status);
      return Status;
    }/* end if */
    
    /* Register EVS task for event services */
    Status = CFE_EVS_Register(NULL, 0, CFE_EVS_BINARY_FILTER);
    if(Status != CFE_SUCCESS)
    {
      CFE_ES_WriteToSysLog("EVS:Call to CFE_EVS_Register Failed:RC=0x%08X\n",Status);
      return Status;
    }/* end if */
    
    /* Create software bus command pipe */
    Status = CFE_SB_CreatePipe(&CFE_EVS_GlobalData.EVS_CommandPipe,
                                CFE_EVS_PIPE_DEPTH,
                                CFE_EVS_PIPE_NAME);
    if(Status != CFE_SUCCESS)
    {
      CFE_ES_WriteToSysLog("EVS:Call to CFE_SB_CreatePipe Failed:RC=0x%08X\n",Status);
      return Status;
    }/* end if */
      
    /* Subscribe to command and telemetry requests coming in on the command pipe */
    Status = CFE_SB_SubscribeEx(CFE_EVS_CMD_MID,
                                CFE_EVS_GlobalData.EVS_CommandPipe,
                                CFE_SB_Default_Qos,
                                CFE_EVS_MSG_LIMIT);
    if(Status != CFE_SUCCESS)
    {
      CFE_ES_WriteToSysLog("EVS:Subscribing to Cmds Failed:RC=0x%08X\n",Status);
      return Status;
    }/* end if */
  
    Status = CFE_SB_SubscribeEx(CFE_EVS_SEND_HK_MID,
                                CFE_EVS_GlobalData.EVS_CommandPipe,
                                CFE_SB_Default_Qos,
                                CFE_EVS_MSG_LIMIT);
    if(Status != CFE_SUCCESS)
    {
      CFE_ES_WriteToSysLog("EVS:Subscribing to HK Request Failed:RC=0x%08X\n",Status);
      return Status;
    }/* end if */
  
    /* Initialize housekeeping packet header */
    CFE_SB_InitMsg(&CFE_EVS_GlobalData.EVS_TlmPkt,
                    CFE_EVS_HK_TLM_MID,
                    CFE_EVS_HK_TLM_LNGTH, FALSE);
  
    CFE_EVS_GlobalData.EVS_TlmPkt.CommandCounter  = 0;
    CFE_EVS_GlobalData.EVS_TlmPkt.CommandErrCounter   =  0;  
    
    Status = CFE_EVS_SendEvent(CFE_EVS_STARTUP_EID, 
                               CFE_EVS_INFORMATION, 
                               "cFE EVS Initialized");
    if(Status != CFE_SUCCESS)
    {
      CFE_ES_WriteToSysLog("EVS:Error sending init event:RC=0x%08X\n",Status);
      return Status;
    }/* end if */


    return CFE_SUCCESS;

} /* End CFE_EVS_TaskInit */



/*
**             Function Prologue
**
** Function Name:      CFE_EVS_ProcessCommandPacket
**
** Purpose:  This function processes packets received on the EVS command pipe.
**
** Assumptions and Notes:
**
*/
void CFE_EVS_ProcessCommandPacket ( CFE_SB_MsgPtr_t EVS_MsgPtr )
{
    /* Process all SB messages */
    switch (CFE_SB_GetMsgId(EVS_MsgPtr))
    {
        case CFE_EVS_CMD_MID:
            /* EVS task specific command */
            CFE_EVS_ProcessGroundCommand(EVS_MsgPtr);
            break;

        case CFE_EVS_SEND_HK_MID:
            /* Housekeeping request */
            CFE_EVS_ReportHousekeepingCmd();
            break;

        default:
            /* Unknown command -- should never occur */
            CFE_EVS_GlobalData.EVS_TlmPkt.CommandErrCounter++;
            CFE_EVS_SendEvent(CFE_EVS_ERR_MSGID_EID,
                              CFE_EVS_ERROR,
                              "Invalid command packet, Message ID = 0x%08X",
                              CFE_SB_GetMsgId(EVS_MsgPtr));
            break;
    }

    return;

} /* End CFE_EVS_ProcessCommandPacket */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_ProcessGroundCommand
**
** Purpose:  This function processes a command, verifying that it is valid and of
**           proper length.
**
** Assumptions and Notes:
**
*/
void CFE_EVS_ProcessGroundCommand ( CFE_SB_MsgPtr_t EVS_MsgPtr )
{
   boolean Valid = TRUE;
   uint16 Size = 0;

   /* Process "known" EVS task ground commands */
   switch (CFE_SB_GetCmdCode(EVS_MsgPtr))
   {
      case CFE_EVS_NO_OPERATION_CC:
         Size = (uint16) sizeof(CFE_SB_CmdHdr_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            CFE_EVS_SendEvent(CFE_EVS_NOOP_EID, CFE_EVS_INFORMATION,"No-op command");
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_RESET_COUNTERS_CC:
         Size = (uint16) sizeof(CFE_SB_CmdHdr_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            CFE_EVS_ResetCountersCmd();
            /* Return to avoid incrementing the command/error counters after reset */
            return;
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_ENABLE_EVENT_TYPE_CC:
         Size = (uint16) sizeof(CFE_EVS_BitMaskCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_EnableEventTypesCmd((CFE_EVS_BitMaskCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_DISABLE_EVENT_TYPE_CC:
         Size = (uint16) sizeof(CFE_EVS_BitMaskCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_DisableEventTypesCmd((CFE_EVS_BitMaskCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_SET_EVENT_FORMAT_MODE_CC:
         Size = (uint16) sizeof(CFE_EVS_ModeCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_SetEventFormatModeCmd((CFE_EVS_ModeCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_ENABLE_APP_EVENT_TYPE_CC:
         Size = (uint16) sizeof(CFE_EVS_AppNameBitMaskCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_EnableAppEventTypesCmd((CFE_EVS_AppNameBitMaskCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_DISABLE_APP_EVENT_TYPE_CC:
         Size = (uint16) sizeof(CFE_EVS_AppNameBitMaskCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_DisableAppEventTypesCmd((CFE_EVS_AppNameBitMaskCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_ENABLE_APP_EVENTS_CC:
         Size = (uint16) sizeof(CFE_EVS_AppNameCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_EnableAppEventsCmd((CFE_EVS_AppNameCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_DISABLE_APP_EVENTS_CC:
         Size = (uint16) sizeof(CFE_EVS_AppNameCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_DisableAppEventsCmd((CFE_EVS_AppNameCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_RESET_APP_COUNTER_CC:
         Size = (uint16) sizeof(CFE_EVS_AppNameCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_ResetAppEventCounterCmd((CFE_EVS_AppNameCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_SET_FILTER_CC:

         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, (uint16) sizeof(CFE_EVS_AppNameEventIDMaskCmd_t))){
            Valid = CFE_EVS_SetFilterMaskCmd((CFE_EVS_AppNameEventIDMaskCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_ENABLE_PORTS_CC:
         Size = (uint16) sizeof(CFE_EVS_BitMaskCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_EnablePortsCmd((CFE_EVS_BitMaskCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_DISABLE_PORTS_CC:
         Size = (uint16) sizeof(CFE_EVS_BitMaskCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_DisablePortsCmd((CFE_EVS_BitMaskCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_RESET_FILTER_CC:
         Size = (uint16) sizeof(CFE_EVS_AppNameEventIDCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_ResetFilterCmd((CFE_EVS_AppNameEventIDCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_RESET_ALL_FILTERS_CC:
         Size = (uint16) sizeof(CFE_EVS_AppNameCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_ResetAllFiltersCmd((CFE_EVS_AppNameCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_ADD_EVENT_FILTER_CC:
         Size = (uint16) sizeof(CFE_EVS_AppNameEventIDMaskCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_AddEventFilterCmd((CFE_EVS_AppNameEventIDMaskCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_DELETE_EVENT_FILTER_CC:
         Size = (uint16) sizeof(CFE_EVS_AppNameEventIDCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_DeleteEventFilterCmd((CFE_EVS_AppNameEventIDCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_FILE_WRITE_APP_DATA_CC:
         Size = (uint16) sizeof(CFE_EVS_AppDataCmd_t);
         if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
            Valid = CFE_EVS_WriteAppDataCmd((CFE_EVS_AppDataCmd_t *) EVS_MsgPtr);
         }
         else{
            Valid = FALSE;
         }
         break;

      case CFE_EVS_SET_LOG_MODE_CC:
         #ifdef CFE_EVS_LOG_ON
            Size = (uint16) sizeof(CFE_EVS_ModeCmd_t);
            if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
               Valid = CFE_EVS_SetLoggingModeCmd((CFE_EVS_ModeCmd_t *) EVS_MsgPtr);
            }
            else{
               Valid = FALSE;
            }
         #else
            Valid = FALSE;
            CFE_EVS_SendEvent(CFE_EVS_NO_LOGSET_EID,
                              CFE_EVS_ERROR,
                              "Set Log Mode Command: No Event Log Availiable in Current cFE Configuration");
         #endif
         break;

      case CFE_EVS_CLEAR_LOG_CC:
         #ifdef CFE_EVS_LOG_ON
            Size = (uint16) sizeof(CFE_SB_CmdHdr_t);
            if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
               Valid = EVS_ClearLog();
            }
            else{
               Valid = FALSE;
            }

         #else
            Valid = FALSE;
            CFE_EVS_SendEvent(CFE_EVS_NO_LOGCLR_EID,
                              CFE_EVS_ERROR,
                              "Clear Log Command: No Event Log Availiable in Current cFE Configuration");
         #endif
         break;

      case CFE_EVS_FILE_WRITE_LOG_DATA_CC:
         #ifdef CFE_EVS_LOG_ON
            Size = (uint16) sizeof(CFE_EVS_LogFileCmd_t);
            if(CFE_EVS_VerifyCmdLength(EVS_MsgPtr, Size)){
               Valid = CFE_EVS_WriteLogFileCmd((CFE_EVS_LogFileCmd_t *) EVS_MsgPtr);
            }
            else{
               Valid = FALSE;
            }

         #else
            Valid = FALSE;
            CFE_EVS_SendEvent(CFE_EVS_NO_LOGWR_EID,
                              CFE_EVS_ERROR,
                              "Write Log Command: No Event Log Availiable in cFE Configuration");
         #endif
         break;

       /* default is a bad command code as it was not found above */
       default:
          Valid = FALSE;
          CFE_EVS_SendEvent(CFE_EVS_ERR_CC_EID, CFE_EVS_ERROR,
                            "Invalid command code -- ID = 0x%08x, CC = %d",
                            CFE_SB_GetMsgId(EVS_MsgPtr), CFE_SB_GetCmdCode(EVS_MsgPtr));
          break;
   }

   if(Valid == TRUE)
      CFE_EVS_GlobalData.EVS_TlmPkt.CommandCounter++;
   else
      CFE_EVS_GlobalData.EVS_TlmPkt.CommandErrCounter++;

} /* End of EVS_ProcessGroundCommand() */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_VerifyCmdLength
**
** Purpose:  This function validates the length of incoming commands.
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_VerifyCmdLength(CFE_SB_MsgPtr_t Msg, uint16 ExpectedLength)
{
    boolean result       = TRUE;
    uint16  ActualLength = CFE_SB_GetTotalMsgLength(Msg);

    /*
    ** Verify the command packet length
    */
    if (ExpectedLength != ActualLength)
    {
        CFE_SB_MsgId_t MessageID = CFE_SB_GetMsgId(Msg);
        uint16 CommandCode = CFE_SB_GetCmdCode(Msg);

        CFE_EVS_SendEvent(CFE_EVS_LEN_ERR_EID, CFE_EVS_ERROR,
           "Invalid cmd length: ID = 0x%X, CC = %d, Exp Len = %d, Len = %d",
                          MessageID, CommandCode, ExpectedLength, ActualLength);
        result = FALSE;
    }

    return(result);

} /* End of CFE_EVS_VerifyCmdLength() */

/*
**             Function Prologue
**
** Function Name:      CFE_EVS_ReportHousekeepingCmd
**
** Purpose:  Request for housekeeping status telemetry packet.
**
** Assumptions and Notes:
**
*/
void CFE_EVS_ReportHousekeepingCmd ( void )
{
   int i, j;


   /*
   ** Copy the log variables from the reset area over to the telemetry packet
   ** These variables are being updated in the global data area rather than the 
   ** telemetry packet.
   */
   #ifdef CFE_EVS_LOG_ON

   CFE_EVS_GlobalData.EVS_TlmPkt.LogFullFlag = CFE_EVS_GlobalData.EVS_LogPtr->LogFullFlag;
   CFE_EVS_GlobalData.EVS_TlmPkt.LogMode = CFE_EVS_GlobalData.EVS_LogPtr->LogMode;
   CFE_EVS_GlobalData.EVS_TlmPkt.LogOverflowCounter = CFE_EVS_GlobalData.EVS_LogPtr->LogOverflowCounter;

   #endif

   /* Write application data to telemetry packet */
   for(i=0, j=0; i<CFE_ES_MAX_APPLICATIONS; i++)
   {
      if(CFE_EVS_GlobalData.EVS_TablePtr[i].Registered == TRUE)
      {
         CFE_EVS_GlobalData.EVS_TlmPkt.AppData[j].AppID = i;
         CFE_EVS_GlobalData.EVS_TlmPkt.AppData[j].AppEnableStatus = CFE_EVS_GlobalData.EVS_TablePtr[i].Address->ActiveFlag;
         CFE_EVS_GlobalData.EVS_TlmPkt.AppData[j].AppMessageSentCounter = CFE_EVS_GlobalData.EVS_TablePtr[i].Address->EventCount;
         j++;
      }
   }

   CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &CFE_EVS_GlobalData.EVS_TlmPkt);

   CFE_SB_SendMsg((CFE_SB_Msg_t *) &CFE_EVS_GlobalData.EVS_TlmPkt);

} /* End of CFE_EVS_ReportHousekeepingCmd() */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_ResetCountersCmd
**
** Purpose:  This function resets all the global counter variables that are
**           part of the task telemetry.
**
** Assumptions and Notes:
**
*/
void CFE_EVS_ResetCountersCmd ( void )
{
    /* Status of commands processed by EVS task */
    CFE_EVS_GlobalData.EVS_TlmPkt.CommandCounter  = 0;
    CFE_EVS_GlobalData.EVS_TlmPkt.CommandErrCounter   =  0;

    /* EVS telemetry counters */
    CFE_EVS_GlobalData.EVS_TlmPkt.MessageSendCounter = 0;
    CFE_EVS_GlobalData.EVS_TlmPkt.MessageTruncCounter = 0;
    CFE_EVS_GlobalData.EVS_TlmPkt.UnregisteredAppCounter = 0;

    CFE_EVS_SendEvent(CFE_EVS_RSTCNT_EID,
                      CFE_EVS_DEBUG,
                      "Reset Counters Command Received");

} /* End of CFE_EVS_ResetCountersCmd() */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_EarlyInit
**
** Purpose:  This routine provides initialization for the EVS API.
**
** Assumptions and Notes: This routine allocates table ram for
**      registering applications for event service.  This routine
**      needs to be called before cFE task initialization.
**
*/
int32 CFE_EVS_EarlyInit ( void )
{
   int32  i;
   int32  Status;
#ifdef CFE_EVS_LOG_ON
   uint32 ResetType;
#endif

#ifdef CFE_EVS_LOG_ON

   /* Init the local log pointer */
   Status = EVS_InitLogPtr();
   if(Status != CFE_SUCCESS){
        CFE_ES_WriteToSysLog("EVS could not get EVS_Log pointer from ES: Status = 0x%08X\n",Status);
        CFE_EVS_GlobalData.EVS_LogPtr = NULL;
        return Status;
   }

   /* Only clear out the log on a Poweron Reset */                                                            
   Status = CFE_ES_GetResetType(&ResetType);
   if ( ResetType == CFE_ES_POWERON_RESET )                                                                   
   {
      EVS_ClearLog();                                                                                         
   }

   CFE_EVS_GlobalData.EVS_LogPtr->LogMode = CFE_EVS_DEFAULT_LOG_MODE;

#endif

   Status = OS_MutSemCreate(&CFE_EVS_GlobalData.EVS_SharedDataMutexID, "CFE_EVS_DataMutex", 0);
   if(Status != OS_SUCCESS){
      CFE_ES_WriteToSysLog("EVS shared data mutex creation failed! RC=0x%08x",Status);
      return Status;
   }/* end if */


   Status = CFE_ES_PoolCreate(&CFE_EVS_GlobalData.EVS_TableHdl,
                      CFE_EVS_GlobalData.EVS_TableRam,
                      sizeof(CFE_EVS_GlobalData.EVS_TableRam));
   if(Status != CFE_SUCCESS){
        CFE_ES_WriteToSysLog("PoolCreate failed for EVS Table Ram, gave adr 0x%x,size %d,stat=0x%x\n",
                             CFE_EVS_GlobalData.EVS_TableRam,sizeof(CFE_EVS_GlobalData.EVS_TableRam),Status);
        return Status;
    }/* end if */



   for(i=0; i<CFE_ES_MAX_APPLICATIONS; i++)
   {
      CFE_EVS_GlobalData.EVS_TablePtr[i].Registered = FALSE;
      CFE_EVS_GlobalData.EVS_TablePtr[i].Address = NULL;
   }

   CFE_EVS_GlobalData.EVS_TlmPkt.MessageSendCounter = 0;
   CFE_EVS_GlobalData.EVS_TlmPkt.MessageTruncCounter = 0;
   CFE_EVS_GlobalData.EVS_TlmPkt.UnregisteredAppCounter = 0;
   CFE_EVS_GlobalData.EVS_TlmPkt.MessageFormatMode = CFE_EVS_DEFAULT_MSG_FORMAT_MODE;
   CFE_EVS_GlobalData.EVS_TlmPkt.OutputPort = CFE_EVS_PORT_DEFAULT;
   CFE_EVS_GlobalData.EVS_TlmPkt.LogFullFlag = FALSE;
   CFE_EVS_GlobalData.EVS_TlmPkt.LogMode = CFE_EVS_DEFAULT_LOG_MODE;
   CFE_EVS_GlobalData.EVS_TlmPkt.MemPoolHandle = CFE_EVS_GlobalData.EVS_TableHdl;

   return(CFE_SUCCESS);

} /* End CFE_EVS_EarlyInit */



/*
**             Function Prologue
**
** Function Name:      CFE_EVS_SetEventFilterMaskCmd
**
** Purpose:  This routine sets the filter mask for the given event_id in the
**           calling task's filter array
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_SetFilterMaskCmd (const CFE_EVS_AppNameEventIDMaskCmd_t *CmdPtr)
{
   CFE_EVS_EVSBinFilter_t    *FilterPtr;
   uint32                    AppID = CFE_EVS_UNDEF_APPID;
   int32                     Status = TRUE;
   CFE_EVS_Table_t           *TablePtr;

   /* Retreive application data */
   Status = EVS_GetApplicationInfo(&AppID, (char *)CmdPtr->AppName);

   if(Status == CFE_SUCCESS)
   {
      TablePtr = CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address;

      FilterPtr = EVS_FindEventID(CmdPtr->EventID, (CFE_EVS_EVSBinFilter_t *)&TablePtr->BinFilters);

      if(FilterPtr != NULL)
      {
         /* Set application filter mask */
         FilterPtr->Mask = CmdPtr->Mask;

         CFE_EVS_SendEvent(CFE_EVS_SETFILTERMSK_EID,
                           CFE_EVS_DEBUG,
                           "Set Filter Mask Command Received with AppName=%s, EventID=0x%08x, Mask=0x%04x",
                           CmdPtr->AppName, CmdPtr->EventID, CmdPtr->Mask);

         Status = TRUE;
      }
      else
      {
         CFE_EVS_SendEvent(CFE_EVS_ERR_EVTIDNOREGS_EID,
                           CFE_EVS_ERROR,
                           "%s Event ID %d not registered for filtering: CC = %lu ",
                           CmdPtr->AppName, CmdPtr->EventID, CFE_EVS_SET_FILTER_CC);

         Status = FALSE;
      }
   }
   else if(Status == CFE_EVS_APP_NOT_REGISTERED)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_APPNOREGS_EID,
                        CFE_EVS_ERROR,
                        "%s not registered with EVS: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_SET_FILTER_CC);
      Status = FALSE;
   }
   else if(Status == CFE_EVS_APP_ILLEGAL_APP_ID)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_ILLAPPIDRANGE_EID,
                        CFE_EVS_ERROR,
                        "Illegal application ID %d retrieved for %s: CC = %lu",
                        AppID, CmdPtr->AppName, CFE_EVS_SET_FILTER_CC);
      Status = FALSE;
   }
   else
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_NOAPPIDFOUND_EID,
                        CFE_EVS_ERROR,
                        "Unable to retrieve application ID for %s: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_SET_FILTER_CC);
      Status = FALSE;
   }

   return (boolean)Status;

} /* End CFE_EVS_SetFilterMaskCmd */



/*
**             Function Prologue
**
** Function Name:      CFE_EVS_EnablePortsCmd
**
** Purpose:  This routine sets the command given ports to an enabled state
**
** Assumptions and Notes:
** Shifting is done so the value not masked off is placed in the ones spot:
** necessary for comparing with TRUE.
*/
boolean CFE_EVS_EnablePortsCmd (const CFE_EVS_BitMaskCmd_t *CmdPtr)
{
    boolean ReturnCode;

    /* Need to check for an out of range bitmask, since oue bit masks are only 4 bits */
    if (CmdPtr->BitMask == 0x0 || CmdPtr->BitMask > 0x0F)
    {
        CFE_EVS_SendEvent(CFE_EVS_ERR_INVALID_BITMASK_EID,
                          CFE_EVS_ERROR,
                          "Bit Mask = 0x%08x out of range: CC = %lu",
                          CmdPtr->BitMask, CFE_EVS_ENABLE_PORTS_CC);
        ReturnCode = FALSE;  
    }
    else
    {

        /* Process command data */
        if(((CmdPtr->BitMask & CFE_EVS_PORT1_BIT) >> 0) == TRUE)
        {
            CFE_EVS_GlobalData.EVS_TlmPkt.OutputPort |= CFE_EVS_PORT1_BIT;
        }
        if(((CmdPtr->BitMask & CFE_EVS_PORT2_BIT) >>1) == TRUE)
        {
            CFE_EVS_GlobalData.EVS_TlmPkt.OutputPort |= CFE_EVS_PORT2_BIT;
        }
        if(((CmdPtr->BitMask & CFE_EVS_PORT3_BIT) >> 2) == TRUE)
        {
            CFE_EVS_GlobalData.EVS_TlmPkt.OutputPort |= CFE_EVS_PORT3_BIT;
        }
        if(((CmdPtr->BitMask & CFE_EVS_PORT4_BIT) >>3) == TRUE)
        {
            CFE_EVS_GlobalData.EVS_TlmPkt.OutputPort |= CFE_EVS_PORT4_BIT;
        }

        CFE_EVS_SendEvent(CFE_EVS_ENAPORT_EID,
                        CFE_EVS_DEBUG,
                        "Enable Ports Command Received with Port Bit Mask = 0x%02x",
                        CmdPtr->BitMask);
        ReturnCode = TRUE;
    }

    return ReturnCode;

} /* End CFE_EVS_EnablePortsCmd */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_DisablePortsCmd
**
** Purpose:  This routine sets the command given ports to a disabled state
**
** Assumptions and Notes:
** Shifting is done so the value not masked off is placed in the ones spot:
** necessary for comparing with TRUE.
*/
boolean CFE_EVS_DisablePortsCmd (const CFE_EVS_BitMaskCmd_t *CmdPtr)
{
    
    boolean ReturnCode;

    /* Need to check for an out of range bitmask, since oue bit masks are only 4 bits */
    if (CmdPtr->BitMask == 0x0 || CmdPtr->BitMask > 0x0F)
    {
        CFE_EVS_SendEvent(CFE_EVS_ERR_INVALID_BITMASK_EID,
                          CFE_EVS_ERROR,
                          "Bit Mask = 0x%08x out of range: CC = %lu",
                          CmdPtr->BitMask, CFE_EVS_DISABLE_PORTS_CC);
        ReturnCode = FALSE;  
    }
    else
    {

        /* Process command data */
        if(((CmdPtr->BitMask & CFE_EVS_PORT1_BIT) >>0) == TRUE)
        {
            CFE_EVS_GlobalData.EVS_TlmPkt.OutputPort &= ~CFE_EVS_PORT1_BIT;
        }
        if(((CmdPtr->BitMask & CFE_EVS_PORT2_BIT) >> 1) == TRUE)
        {
            CFE_EVS_GlobalData.EVS_TlmPkt.OutputPort &= ~CFE_EVS_PORT2_BIT;
        }
        if(((CmdPtr->BitMask & CFE_EVS_PORT3_BIT) >> 2) == TRUE)
        {
            CFE_EVS_GlobalData.EVS_TlmPkt.OutputPort &= ~CFE_EVS_PORT3_BIT;
        }
        if(((CmdPtr->BitMask & CFE_EVS_PORT4_BIT) >>3) == TRUE)
        {
            CFE_EVS_GlobalData.EVS_TlmPkt.OutputPort &= ~CFE_EVS_PORT4_BIT;
        }

        CFE_EVS_SendEvent(CFE_EVS_DISPORT_EID,
                          CFE_EVS_DEBUG,
                         "Disable Ports Command Received with Port Bit Mask = 0x%02x",
                         CmdPtr->BitMask);
        
        ReturnCode = TRUE;
    }
   
    return ReturnCode;

} /* End CFE_EVS_DisablePortsCmd */



/*
**             Function Prologue
**
** Function Name:      CFE_EVS_EnableEventTypesCmd
**
** Purpose:  This routine sets the given event types to an enabled state across all
**           registered applications
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_EnableEventTypesCmd (const CFE_EVS_BitMaskCmd_t *CmdPtr)
{
   int      i;
   boolean  ReturnCode; 

   /* Need to check for an out of range bitmask, since our bit masks are only 4 bits */
   if (CmdPtr->BitMask == 0x0 || CmdPtr->BitMask > 0x0F)
   {
       CFE_EVS_SendEvent(CFE_EVS_ERR_INVALID_BITMASK_EID,
                         CFE_EVS_ERROR,
                         "Bit Mask = 0x%08x out of range: CC = %lu",
                         CmdPtr->BitMask, CFE_EVS_ENABLE_EVENT_TYPE_CC);
    ReturnCode = FALSE;  
   }
   else
   {

        

        for(i=0; i<CFE_ES_MAX_APPLICATIONS; i++)
        {
            /* Make sure application is registered for event services */
             if(CFE_EVS_GlobalData.EVS_TablePtr[i].Registered == TRUE)
            {
                EVS_EnableTypes(CmdPtr->BitMask, i);
            }
        }

        CFE_EVS_SendEvent(CFE_EVS_ENAEVTTYPE_EID,
                          CFE_EVS_DEBUG,
                        "Enable Event Type Command Received with Event Type Bit Mask = 0x%02x",
                         CmdPtr->BitMask);

       ReturnCode = TRUE;
   }

   return ReturnCode;

} /* End CFE_EVS_EnableEventTypesCmd */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_DisableEventTypesCmd
**
** Purpose:  This routine sets the given event types to a disabled state across all
**           registered applications
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_DisableEventTypesCmd (const CFE_EVS_BitMaskCmd_t *CmdPtr)
{
   int    i;
   boolean  ReturnCode; 

   /* Need to check for an out of range bitmask, since our bit masks are only 4 bits */
   if (CmdPtr->BitMask == 0x0 || CmdPtr->BitMask > 0x0F)
   {
       CFE_EVS_SendEvent(CFE_EVS_ERR_INVALID_BITMASK_EID,
                         CFE_EVS_ERROR,
                         "Bit Mask = 0x%08x out of range: CC = %lu",
                         CmdPtr->BitMask, CFE_EVS_DISABLE_EVENT_TYPE_CC);
    ReturnCode = FALSE;  
   }

   else
   {

        for(i=0; i<CFE_ES_MAX_APPLICATIONS; i++)
        {
            /* Make sure application is registered for event services */
            if(CFE_EVS_GlobalData.EVS_TablePtr[i].Registered == TRUE)
            {
                EVS_DisableTypes(CmdPtr->BitMask, i);
            }
         }

        CFE_EVS_SendEvent(CFE_EVS_DISEVTTYPE_EID,
                          CFE_EVS_DEBUG,
                         "Disable Event Type Command Received with Event Type Bit Mask = 0x%02x",
                         CmdPtr->BitMask);

        ReturnCode = TRUE;
   }

   return ReturnCode;

} /* End CFE_EVS_DisableEventTypesCmd */



/*
**             Function Prologue
**
** Function Name:      CFE_EVS_SetEventFormatModeCmd
**
** Purpose:  This routine sets the Event Format Mode
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_SetEventFormatModeCmd (const CFE_EVS_ModeCmd_t *CmdPtr)
{
   boolean Status = TRUE;

   if((CmdPtr->Mode == CFE_EVS_SHORT_FORMAT) || (CmdPtr->Mode == CFE_EVS_LONG_FORMAT))
   {
      CFE_EVS_GlobalData.EVS_TlmPkt.MessageFormatMode = CmdPtr->Mode;

      CFE_EVS_SendEvent(CFE_EVS_SETEVTFMTMOD_EID,
                        CFE_EVS_DEBUG,
                        "Set Event Format Mode Command Received with Mode = 0x%02x",
                        CmdPtr->Mode);
   }
   else
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_ILLEGALFMTMOD_EID,
                        CFE_EVS_ERROR,
                        "Set Event Format Mode Command: Invalid Event Format Mode = 0x%02x", CmdPtr->Mode);
      Status = FALSE;
   }

   return Status;

} /* End CFE_EVS_SetEventFormatModeCmd */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_EnableAppEventTypesCmd
**
** Purpose:  This routine sets the given event type for the given application identifier to an
**           enabled state
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_EnableAppEventTypesCmd (const CFE_EVS_AppNameBitMaskCmd_t *CmdPtr)
{
   uint32  AppID  = 0xFFFF;
   int32   Status = TRUE;

   /* Retrieve application data */
   Status = EVS_GetApplicationInfo(&AppID, (char *)CmdPtr->AppName);

   if(Status == CFE_SUCCESS)
   {

        /* Need to check for an out of range bitmask, since our bit masks are only 4 bits */
        if (CmdPtr->BitMask == 0x0 || CmdPtr->BitMask > 0x0F)
        {
            CFE_EVS_SendEvent(CFE_EVS_ERR_INVALID_BITMASK_EID,
                              CFE_EVS_ERROR,
                              "Bit Mask = 0x%08x out of range: CC = %lu",
                              CmdPtr->BitMask, CFE_EVS_ENABLE_APP_EVENT_TYPE_CC);
            Status = FALSE;  
        }
        else
        {
            EVS_EnableTypes(CmdPtr->BitMask, AppID);
            Status = TRUE;
        }
   }
   else if(Status == CFE_EVS_APP_NOT_REGISTERED)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_APPNOREGS_EID,
                        CFE_EVS_ERROR,
                        "%s not registered with EVS: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_ENABLE_APP_EVENT_TYPE_CC);
      Status = FALSE;
   }
   else if(Status == CFE_EVS_APP_ILLEGAL_APP_ID)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_ILLAPPIDRANGE_EID,
                        CFE_EVS_ERROR,
                        "Illegal application ID %d retrieved for %s: CC = %lu",
                        AppID, CmdPtr->AppName, CFE_EVS_ENABLE_APP_EVENT_TYPE_CC);
      Status = FALSE;
   }
   else
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_NOAPPIDFOUND_EID,
                        CFE_EVS_ERROR,
                        "Unable to retrieve application ID for %s: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_ENABLE_APP_EVENT_TYPE_CC);
      Status = FALSE;
   }

   if(Status == TRUE)
   {
      CFE_EVS_SendEvent(CFE_EVS_ENAAPPEVTTYPE_EID,
                        CFE_EVS_DEBUG,
                        "Enable App Event Type Command Received with AppName = %s, EventType Bit Mask = 0x%02x",
                        CmdPtr->AppName, CmdPtr->BitMask);
   }

   return (boolean)Status;

} /* End CFE_EVS_EnableAppEventTypesCmd */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_DisableAppEventTypesCmd
**
** Purpose:  This routine sets the given event type for the given application identifier to a
**           disabled state
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_DisableAppEventTypesCmd (const CFE_EVS_AppNameBitMaskCmd_t *CmdPtr)
{
   uint32 AppID  = CFE_EVS_UNDEF_APPID;
   int32  Status = TRUE;

   /* Retreive application data */
   Status = EVS_GetApplicationInfo(&AppID, (char *)CmdPtr->AppName);

   if(Status == CFE_SUCCESS)
   {

        /* Need to check for an out of range bitmask, since our bit masks are only 4 bits */
        if (CmdPtr->BitMask == 0x0 || CmdPtr->BitMask > 0x0F)
        {
            CFE_EVS_SendEvent(CFE_EVS_ERR_INVALID_BITMASK_EID,
                              CFE_EVS_ERROR,
                              "Bit Mask = 0x%08x out of range: CC = %lu",
                              CmdPtr->BitMask, CFE_EVS_DISABLE_APP_EVENT_TYPE_CC);
            Status = FALSE;  
        }
        else
        {
            EVS_DisableTypes(CmdPtr->BitMask, AppID);
            Status = TRUE;
        }
   }
   else if(Status == CFE_EVS_APP_NOT_REGISTERED)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_APPNOREGS_EID,
                        CFE_EVS_ERROR,
                        "%s not registered with EVS,: CC = %lu",
                        CmdPtr->AppName,CFE_EVS_DISABLE_APP_EVENT_TYPE_CC);
      Status = FALSE;
   }
   else if(Status == CFE_EVS_APP_ILLEGAL_APP_ID)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_ILLAPPIDRANGE_EID,
                        CFE_EVS_ERROR,
                        "Illegal application ID %d retrieved for %s: CC = %lu",
                        AppID, CmdPtr->AppName, CFE_EVS_DISABLE_APP_EVENT_TYPE_CC);
      Status = FALSE;
   }
   else
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_NOAPPIDFOUND_EID,
                        CFE_EVS_ERROR,
                        "Unable to retrieve application ID for %s: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_DISABLE_APP_EVENT_TYPE_CC);
      Status = FALSE;
   }

   if(Status == TRUE)
   {
      CFE_EVS_SendEvent(CFE_EVS_DISAPPENTTYPE_EID,
                        CFE_EVS_DEBUG,
                        "Disable App Event Type Command Received with AppName = %s, EventType Bit Mask = 0x%02x",
                        CmdPtr->AppName, CmdPtr->BitMask);
   }

   return (boolean)Status;

} /* End CFE_EVS_DisableAppEventTypes */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_EnableAppEventsCmd
**
** Purpose:  This routine enables application events for the given application identifier
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_EnableAppEventsCmd (const CFE_EVS_AppNameCmd_t *CmdPtr)
{
   uint32 AppID  = CFE_EVS_UNDEF_APPID;
   int32  Status = TRUE;

   /* Retrieve application data */
   Status = EVS_GetApplicationInfo(&AppID, (char *)CmdPtr->AppName);

   if(Status == CFE_SUCCESS)
   {
        CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address->ActiveFlag = TRUE;

        CFE_EVS_SendEvent(CFE_EVS_ENAAPPEVT_EID,
                          CFE_EVS_DEBUG,
                          "Enable App Events Command Received with AppName = %s",
                          CmdPtr->AppName);

        Status = TRUE;
   }
   else if(Status == CFE_EVS_APP_NOT_REGISTERED)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_APPNOREGS_EID,
                        CFE_EVS_ERROR,
                        "%s not registered with EVS: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_ENABLE_APP_EVENTS_CC);
      Status = FALSE;
   }
   else if(Status == CFE_EVS_APP_ILLEGAL_APP_ID)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_ILLAPPIDRANGE_EID,
                        CFE_EVS_ERROR,
                        "Illegal application ID %d retrieved for %s: CC = %lu",
                        AppID, CmdPtr->AppName, CFE_EVS_ENABLE_APP_EVENTS_CC);
      Status = FALSE;
   }
   else
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_NOAPPIDFOUND_EID,
                        CFE_EVS_ERROR,
                        "Unable to retrieve application ID for %s: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_ENABLE_APP_EVENTS_CC);
      Status = FALSE;
   }

   return (boolean)Status;

} /* End EVS_EnableAppEventsCmd */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_DisableAppEventsCmd
**
** Purpose:  This routine disables application events for the given application identifier
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_DisableAppEventsCmd (const CFE_EVS_AppNameCmd_t *CmdPtr)
{
   uint32 AppID  = CFE_EVS_UNDEF_APPID;
   int32  Status = TRUE;

   /* Retreive application data */
   Status = EVS_GetApplicationInfo(&AppID, (char *)CmdPtr->AppName);

   if(Status == CFE_SUCCESS)
   {
        CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address->ActiveFlag = FALSE;

        CFE_EVS_SendEvent(CFE_EVS_DISAPPEVT_EID,
                          CFE_EVS_DEBUG,
                          "Disable App Events Command Received with AppName = %s",
                          CmdPtr->AppName);

        Status = TRUE;
   }
   else if(Status == CFE_EVS_APP_NOT_REGISTERED)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_APPNOREGS_EID,
                        CFE_EVS_ERROR,
                        "%s not registered with EVS: CC = %lu",
                        CmdPtr->AppName,CFE_EVS_DISABLE_APP_EVENTS_CC);
      Status = FALSE;
   }
   else if(Status == CFE_EVS_APP_ILLEGAL_APP_ID)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_ILLAPPIDRANGE_EID,
                        CFE_EVS_ERROR,
                        "Illegal application ID %d retrieved for %s: CC = %lu",
                        AppID, CmdPtr->AppName,CFE_EVS_DISABLE_APP_EVENTS_CC);
      Status = FALSE;
   }
   else
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_NOAPPIDFOUND_EID,
                        CFE_EVS_ERROR,
                        "Disable App Events Command: Unable to retrieve application ID for %s: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_DISABLE_APP_EVENTS_CC);
      Status = FALSE;
   }

   return (boolean)Status;

} /* End CFE_EVS_DisableAppEventsCmd */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_ResetAppEventCounterCmd
**
** Purpose:  This routine sets the application event counter to zero for the given
**           application identifier
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_ResetAppEventCounterCmd (const CFE_EVS_AppNameCmd_t *CmdPtr)
{
   uint32 AppID  = CFE_EVS_UNDEF_APPID;
   int32  Status = TRUE;

   /* Retreive application data */
   Status = EVS_GetApplicationInfo(&AppID, (char *)CmdPtr->AppName);

   if(Status == CFE_SUCCESS)
   {
        CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address->EventCount = 0;

        CFE_EVS_SendEvent(CFE_EVS_RSTEVTCNT_EID,
                          CFE_EVS_DEBUG,
                          "Reset Event Counter Command Received with AppName = %s",
                          CmdPtr->AppName);

        Status = TRUE;
   }
   else if(Status == CFE_EVS_APP_NOT_REGISTERED)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_APPNOREGS_EID,
                        CFE_EVS_ERROR,
                        "%s not registered with EVS: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_RESET_APP_COUNTER_CC);
      Status = FALSE;
   }
   else if(Status == CFE_EVS_APP_ILLEGAL_APP_ID)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_ILLAPPIDRANGE_EID,
                        CFE_EVS_ERROR,
                        "Illegal application ID %d retrieved for %s: CC = %lu",
                        AppID, CmdPtr->AppName, CFE_EVS_RESET_APP_COUNTER_CC);
      Status = FALSE;
   }
   else
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_NOAPPIDFOUND_EID,
                        CFE_EVS_ERROR,
                        "Reset Event Counter Command: Unable to retrieve application ID for %s: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_RESET_APP_COUNTER_CC);
      Status = FALSE;
   }

   return (boolean)Status;

} /* End CFE_EVS_ResetAppEventCounterCmd */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_ResetFilterCmd
**
** Purpose:  This routine sets the application event filter counter to zero for the given
**           application identifier and event identifier
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_ResetFilterCmd (const CFE_EVS_AppNameEventIDCmd_t *CmdPtr)
{
   CFE_EVS_EVSBinFilter_t     *FilterPtr;
   uint32                     AppID = CFE_EVS_UNDEF_APPID;
   int32                      Status = TRUE;
   CFE_EVS_Table_t            *TablePtr;

   /* Retreive application data */
   Status = EVS_GetApplicationInfo(&AppID, (char *)CmdPtr->AppName);

   if(Status == CFE_SUCCESS)
   {
      TablePtr = CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address;

      FilterPtr = EVS_FindEventID(CmdPtr->EventID,
                                  (CFE_EVS_EVSBinFilter_t *)&TablePtr->BinFilters);

      if(FilterPtr != NULL)
      {
         FilterPtr->Count = 0;

         CFE_EVS_SendEvent(CFE_EVS_RSTFILTER_EID,
                           CFE_EVS_DEBUG,
                           "Reset Filter Command Received with AppName = %s, EventID = 0x%08x",
                           CmdPtr->AppName, CmdPtr->EventID);

         Status = TRUE;
      }
      else
      {
         CFE_EVS_SendEvent(CFE_EVS_ERR_EVTIDNOREGS_EID,
                           CFE_EVS_ERROR,
                           "%s Event ID %d not registered for filtering: CC = %lu",
                           CmdPtr->AppName, CmdPtr->EventID, CFE_EVS_RESET_FILTER_CC);
      }
   }
   else if(Status == CFE_EVS_APP_NOT_REGISTERED)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_APPNOREGS_EID,
                        CFE_EVS_ERROR,
                        "%s not registered with EVS: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_RESET_FILTER_CC);
      Status = FALSE;
   }
   else if(Status == CFE_EVS_APP_ILLEGAL_APP_ID)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_ILLAPPIDRANGE_EID,
                        CFE_EVS_ERROR,
                        "Illegal application ID %d retrieved for %s: CC = %lu",
                        AppID, CmdPtr->AppName, CFE_EVS_RESET_FILTER_CC);
      Status = FALSE;
   }
   else
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_NOAPPIDFOUND_EID,
                        CFE_EVS_ERROR,
                        "Unable to retrieve application ID for %s: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_RESET_FILTER_CC);
      Status = FALSE;
   }

   return (boolean)Status;

} /* End CFE_EVS_ResetFilterCmd */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_ResetAllFiltersCmd
**
** Purpose:  This routine sets all application event filter counters to zero for the given
**           application identifier
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_ResetAllFiltersCmd (const CFE_EVS_AppNameCmd_t *CmdPtr)
{
   uint32                    AppID  = CFE_EVS_UNDEF_APPID;
   int32                     Status = TRUE;
   uint32                    i;

   /* Retreive application data */
   Status = EVS_GetApplicationInfo(&AppID, (char *)CmdPtr->AppName);

   if(Status == CFE_SUCCESS)
   {
      for(i=0; i<CFE_EVS_MAX_EVENT_FILTERS; i++)
      {
         CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address->BinFilters[i].Count = 0;
      }

      CFE_EVS_SendEvent(CFE_EVS_RSTALLFILTER_EID,
                        CFE_EVS_DEBUG,
                        "Reset All Filters Command Received with AppName = %s",
                        CmdPtr->AppName);

      Status = TRUE;
   }
   else if(Status == CFE_EVS_APP_NOT_REGISTERED)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_APPNOREGS_EID,
                        CFE_EVS_ERROR,
                        "%s not registered with EVS: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_RESET_ALL_FILTERS_CC);
      Status = FALSE;
   }
   else if(Status == CFE_EVS_APP_ILLEGAL_APP_ID)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_ILLAPPIDRANGE_EID,
                        CFE_EVS_ERROR,
                        "Illegal application ID %d retrieved for %s: CC = %lu",
                        AppID, CmdPtr->AppName, CFE_EVS_RESET_ALL_FILTERS_CC);
      Status = FALSE;
   }
   else
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_NOAPPIDFOUND_EID,
                        CFE_EVS_ERROR,
                        "Unable to retrieve application ID for %s: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_RESET_ALL_FILTERS_CC);
      Status = FALSE;
   }

   return (boolean)Status;

} /* End CFE_EVS_ResetAllFiltersCmd */

/*
**             Function Prologue
**
** Function Name:      CFE_EVS_AddEventFilterCmd
**
** Purpose:  This routine adds the given event filter for the given application
**           identifier and event identifier.
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_AddEventFilterCmd (const CFE_EVS_AppNameEventIDMaskCmd_t *CmdPtr)
{
   CFE_EVS_EVSBinFilter_t     *FilterPtr;
   uint32                     AppID = CFE_EVS_UNDEF_APPID;
   int32                      Status = TRUE;
   CFE_EVS_Table_t            *TablePtr;

   /* Retreive application data */
   Status = EVS_GetApplicationInfo(&AppID, (char *)CmdPtr->AppName);

   if(Status == CFE_SUCCESS)
   {
      TablePtr = CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address;


      /* Check to see if this event is already registered for filtering */
      FilterPtr = EVS_FindEventID ( CmdPtr -> EventID,
                                  (CFE_EVS_EVSBinFilter_t *)&TablePtr->BinFilters);


      /* FilterPtr != NULL means that this Event ID was found as already being registered */
      if (FilterPtr != NULL)
      {
          CFE_EVS_SendEvent(CFE_EVS_EVT_FILTERED_EID,
                      CFE_EVS_ERROR,
                      "Add Filter Command:AppName = %s, EventID = 0x%08x is already registered for filtering",
                      CmdPtr->AppName, CmdPtr->EventID);
          Status = FALSE;
      }
      else
      {
          /* now check to see if there is a free slot */
           FilterPtr = EVS_FindEventID(CFE_EVS_FREE_SLOT,
                                      (CFE_EVS_EVSBinFilter_t *)&TablePtr->BinFilters);

            if(FilterPtr != NULL)
            {
               /* Add Filter Contents */
               FilterPtr->EventID = CmdPtr->EventID;
               FilterPtr->Mask = CmdPtr->Mask;
               FilterPtr->Count = 0;

               CFE_EVS_SendEvent(CFE_EVS_ADDFILTER_EID,
                                 CFE_EVS_DEBUG,
                                 "Add Filter Command Received with AppName = %s, EventID = 0x%08x, Mask = 0x%04x",
                                 CmdPtr->AppName, CmdPtr->EventID, CmdPtr->Mask);

               Status = TRUE;
            }
            else
            {
               CFE_EVS_SendEvent(CFE_EVS_ERR_MAXREGSFILTER_EID,
                                 CFE_EVS_ERROR,
                                 "Add Filter Command: number of registered filters has reached max = %d",
                                 CFE_EVS_MAX_EVENT_FILTERS);

               Status = FALSE;
            }
      }/* end else*/
   } /* end if (Status == CFE_SUCCESS) */

   else if(Status == CFE_EVS_APP_NOT_REGISTERED)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_APPNOREGS_EID,
                        CFE_EVS_ERROR,
                        "%s not registered with EVS: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_ADD_EVENT_FILTER_CC);
      Status = FALSE;
   }
   else if(Status == CFE_EVS_APP_ILLEGAL_APP_ID)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_ILLAPPIDRANGE_EID,
                        CFE_EVS_ERROR,
                        "Illegal application ID %d retrieved for %s: CC = %lu",
                        AppID, CmdPtr->AppName, CFE_EVS_ADD_EVENT_FILTER_CC);
      Status = FALSE;
   }
   else
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_NOAPPIDFOUND_EID,
                        CFE_EVS_ERROR,
                        "Unable to retrieve application ID for %s: CC = %lu",
                        CmdPtr->AppName,CFE_EVS_ADD_EVENT_FILTER_CC);
      Status = FALSE;
   }

   return (boolean)Status;

} /* CFE_End EVS_AddEventFilterCmd */

/*
**             Function Prologue
**
** Function Name:      CFE_EVS_DeleteEventFilterCmd
**
** Purpose:  This routine deletes the event filter for the given application
**           identifer and event identifier
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_DeleteEventFilterCmd (const CFE_EVS_AppNameEventIDCmd_t *CmdPtr)
{
   CFE_EVS_EVSBinFilter_t     *FilterPtr;
   uint32                     AppID = CFE_EVS_UNDEF_APPID;
   int32                      Status = TRUE;
   CFE_EVS_Table_t            *TablePtr;

   /* Retreive application data */
   Status = EVS_GetApplicationInfo(&AppID, (char *)CmdPtr->AppName);

   if(Status == CFE_SUCCESS)
   {
      TablePtr = CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address;

      FilterPtr = EVS_FindEventID(CmdPtr->EventID, (CFE_EVS_EVSBinFilter_t *)&TablePtr->BinFilters);

      if(FilterPtr != NULL)
      {
         /* Clear Filter Contents */
         FilterPtr->EventID = CFE_EVS_FREE_SLOT;
         FilterPtr->Mask = CFE_EVS_NO_MASK;
         FilterPtr->Count = 0;

         CFE_EVS_SendEvent(CFE_EVS_DELFILTER_EID,
                           CFE_EVS_DEBUG,
                           "Delete Filter Command Received with AppName = %s, EventID = 0x%08x",
                           CmdPtr->AppName, CmdPtr->EventID);

         Status = TRUE;
      }
      else
      {
         CFE_EVS_SendEvent(CFE_EVS_ERR_EVTIDNOREGS_EID,
                           CFE_EVS_ERROR,
                           "%s Event ID %d not registered for filtering: CC = %lu",
                           CmdPtr->AppName, CmdPtr->EventID, CFE_EVS_DELETE_EVENT_FILTER_CC);
         Status = FALSE;
      }
   }
   else if(Status == CFE_EVS_APP_NOT_REGISTERED)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_APPNOREGS_EID,
                        CFE_EVS_ERROR,
                        "%s not registered with EVS: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_DELETE_EVENT_FILTER_CC);
      Status = FALSE;
   }
   else if(Status == CFE_EVS_APP_ILLEGAL_APP_ID)
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_ILLAPPIDRANGE_EID,
                        CFE_EVS_ERROR,
                        "Illegal application ID %d retrieved for %s: CC = %lu",
                        AppID, CmdPtr->AppName, CFE_EVS_DELETE_EVENT_FILTER_CC);
      Status = FALSE;
   }
   else
   {
      CFE_EVS_SendEvent(CFE_EVS_ERR_NOAPPIDFOUND_EID,
                        CFE_EVS_ERROR,
                        "Unable to retrieve application ID for %s: CC = %lu",
                        CmdPtr->AppName, CFE_EVS_DELETE_EVENT_FILTER_CC);
      Status = FALSE;
   }

   return (boolean)Status;

} /* End EVS_DeleteEventFilterCmd */


/*
**             Function Prologue
**
** Function Name:      CFE_EVS_WriteAppDataCmd
**
** Purpose:  This routine writes all application data to a file for all applications that
**           have registered with the EVS.  The application data includes the Application ID,
**           Active Flag, Event Count, Event Types Active Flag, and Filter Data.
**
** Assumptions and Notes:
**
*/
boolean CFE_EVS_WriteAppDataCmd ( const CFE_EVS_AppDataCmd_t *CmdPtr )
{
   int32                             FileHandle;
   int32                             Status = TRUE;
   uint32                            FileSize = 0;
   uint32                            EntryCount = 0;
   int                               i, j;
   static CFE_EVS_AppDataFile_t      AppDataFile;
   CFE_FS_Header_t                   FileHdr;
   CFE_EVS_Table_t                   *TablePtr;
   char                              AppDataFilename[OS_MAX_PATH_LEN];

   /* Copy the commanded filename into local buffer to ensure size limitation and to allow for modification */
   CFE_PSP_MemCpy(AppDataFilename, (void *)CmdPtr->AppDataFilename, OS_MAX_PATH_LEN);

   /* Check to see if a default filename should be used */
    if (AppDataFilename[0] == '\0')
    {
        strncpy(AppDataFilename, CFE_EVS_DEFAULT_APP_DATA_FILE , OS_MAX_PATH_LEN);
    }

    /* Make sure all strings are null terminated before attempting to process them */
    AppDataFilename[OS_MAX_PATH_LEN-1] = '\0';

    /* Create Application Data File */
    FileHandle = OS_creat(AppDataFilename, OS_WRITE_ONLY);

   if( FileHandle >= OS_FS_SUCCESS)
   {

      /* Initialize file header */
      CFE_PSP_MemSet(&FileHdr, 0, sizeof(CFE_FS_Header_t));
      strcpy(&FileHdr.Description[0], "EVS Application Data File");

      /* Add the subtype for this file */
      FileHdr.SubType = CFE_FS_EVS_APPDATA_SUBTYPE;

      /* Output the Standard cFE File Header to the App File */
      Status = CFE_FS_WriteHeader(FileHandle, &FileHdr);

      /* Maintain statistics of amount of data written to file */
      FileSize += Status;

      if(Status == sizeof(CFE_FS_Header_t))
      {
         /* Write application data */
         for(i=0; i<CFE_ES_MAX_APPLICATIONS; i++)
         {
            if(CFE_EVS_GlobalData.EVS_TablePtr[i].Registered == TRUE)
            {
               TablePtr = CFE_EVS_GlobalData.EVS_TablePtr[i].Address;

               CFE_PSP_MemSet(&AppDataFile, 0, sizeof(CFE_EVS_AppDataFile_t));

               CFE_ES_GetAppName(AppDataFile.AppName, i, OS_MAX_API_NAME);
               AppDataFile.ActiveFlag = TablePtr->ActiveFlag;
               AppDataFile.EventCount = TablePtr->EventCount;
               AppDataFile.EventTypesActiveFlag = TablePtr->EventTypesActiveFlag;

               /* write application filter data to file */
               for(j=0; j<CFE_EVS_MAX_EVENT_FILTERS; j++)
               {
                  AppDataFile.Filters[j].EventID = TablePtr->BinFilters[j].EventID;
                  AppDataFile.Filters[j].Mask = TablePtr->BinFilters[j].Mask;
                  AppDataFile.Filters[j].Count = TablePtr->BinFilters[j].Count;
               }

               Status = OS_write(FileHandle, &AppDataFile, sizeof(CFE_EVS_AppDataFile_t));
               FileSize += Status;
               EntryCount++;

               if(Status < OS_FS_SUCCESS)
               {
                  CFE_EVS_SendEvent(CFE_EVS_ERR_WRDATFILE_EID,
                                    CFE_EVS_ERROR,
                                    "Write App Data Command: Error writing to application data file, OS_write returned 0x%08X, data filename = %s", Status, AppDataFilename);
               }
            }
         }
      }
      Status = OS_close(FileHandle);

      if(Status < OS_FS_SUCCESS)
      {
         CFE_EVS_SendEvent(CFE_EVS_ERR_CLOSEDATFILE_EID,
                           CFE_EVS_ERROR,
                           "Write App Data Command: Error closing application data file handle, OS_close returned 0x%08X, data filename = %s", Status, AppDataFilename);
      }
      else
      {
         Status = TRUE;
      }

   }
   
   else
   {
      Status = FALSE;
      CFE_EVS_SendEvent(CFE_EVS_ERR_CRDATFILE_EID,
                        CFE_EVS_ERROR,
                        "Write App Data Command: Error creating application data file, OS_creat returned 0x%08X, data filename = %s", FileHandle, AppDataFilename);
   }

   if(Status == TRUE)
   {
      CFE_EVS_SendEvent(CFE_EVS_WRDAT_EID,
                        CFE_EVS_DEBUG,
                        "Write App Data Command Received");

      CFE_EVS_SendEvent(CFE_EVS_WRLOG_EID,
                        CFE_EVS_DEBUG,
                        "%s written: Size = %d, Entries = %d",
                        AppDataFilename, FileSize, EntryCount);
   }

   return (boolean)Status;

} /* CFE_End EVS_WriteAppDataCmd */


/* End cfe_evs_task */
