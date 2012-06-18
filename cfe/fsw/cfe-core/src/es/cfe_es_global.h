/*
**  File: 
**  cfe_es_global.h 
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
**  Purpose:
**  This file contains the ES global data definitions.
**
**  References:
**     Flight Software Branch C Coding Standard Version 1.0a
**     cFE Flight Software Application Developers Guide
**
**  $Log: cfe_es_global.h  $
**  Revision 1.6 2010/11/05 15:54:35EDT aschoeni 
**  Added Generic Counter API to ES
**  Revision 1.5 2009/07/28 16:32:36EDT jmdagost 
**  Changed first argument in Lock/Unlock function prototypes.
**  Revision 1.4 2009/06/10 09:08:57EDT acudmore 
**  Converted OS_Mem* and OS_BSP* API to CFE_PSP_* API
**  Revision 1.3 2008/08/14 10:57:03EDT apcudmore 
**  Changed AppStartupCounter from uint32 to int32
**  Revision 1.2 2008/07/30 14:53:06EDT apcudmore 
**  Added global variables to ES to support startup sync.
**  Revision 1.1 2008/04/17 08:05:06EDT ruperera 
**  Initial revision
**  Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/src/es/project.pj
**  Revision 1.20 2007/05/30 10:26:07EDT apcudmore 
**  Added Exception handler support:
**   - BSP routine to attach processor exceptions
**   - context datatype and size define
**   - added default exception init for each app+task
**  Revision 1.19 2007/05/14 15:43:47EDT apcudmore 
**  Cleaned up events and disabled debug code for application restart.
**  Revision 1.18 2007/05/14 10:26:04EDT apcudmore 
**  Preserve the EVS Local event log on a processor restart:
**  - moved the EVS Log to the ES reset data structure
**  - Added a couple of EVS Log related variables into the EVS_Log_t structure
**  - Added a routine to the EVS early init to get a pointer to the EVS Log 
**  - Added code to the EVS housekeeping service routine to copy Log variables into TLM
**  Revision 1.17 2007/05/07 15:56:32EDT apcudmore 
**  Removed all ES startup semaphore code
**  Removed all ES startup delays.
**  Startup sync problems ( seem to be ) eliminated by protecting the ES global data tables with a mutex.
**  Revision 1.16 2007/05/04 15:58:36EDT apcudmore 
**  Implemented ES shared data protection. Eliminates Startup syncronization problem.
**  Revision 1.15 2007/04/02 11:01:55EDT rjmcgraw 
**  Renamed struct PerfState to PerfData
**  Revision 1.14 2007/03/29 13:29:16EST apcudmore 
**  Updated global variables and housekeeping to count:
**  - Registered Core Apps
**  - Registered External Apps
**  - ALL Tasks 
**  Revision 1.13 2007/03/22 10:26:24EST apcudmore 
**  Implemented all code walkthrough items related to ES Application logic, commands, and telemetry.
**  Revision 1.12 2007/03/06 16:01:35EST apcudmore 
**  ES Walkthrough changes related to cfe_es_global.h
*/

#ifndef _cfe_es_global_
#define _cfe_es_global_

/*
** Includes:
*/
#include "osapi.h"
#include "cfe.h"
#include "cfe_es.h"
#include "cfe_es_apps.h"
#include "cfe_es_cds.h"
#include "cfe_es_perf.h"
#include "cfe_time.h"
#include "cfe_platform_cfg.h"
#include "cfe_evs.h"
#include "cfe_psp.h"


/*
** Defines
*/

/*
** Typedefs
*/

/*
** Reset Variables type
*/
typedef struct
{
   uint32     ResetType;
   uint32     ResetSubtype;
   uint32     BootSource;
   uint32     ES_CausedReset;
   uint32     ProcessorResetCount;
   uint32     MaxProcessorResetCount;
      
} CFE_ES_ResetVariables_t;

/*
** Debug variables type
*/
typedef struct
{  
   uint32 DebugFlag;
   uint32 WatchdogWriteFlag;
   uint32 PrintfEnabledFlag;
   uint32 LastAppId;
   
} CFE_ES_DebugVariables_t;

/*
** Exception and Reset Log Structure
*/
typedef struct
{
     uint32                  LogEntryType;                  /* What type of log entry */
     uint32                  ResetType;                     /* Main cause for the reset */
     uint32                  ResetSubtype;                  /* The sub-type for the reset */
     uint32                  BootSource;                    /* The boot source  */
     uint32                  ProcessorResetCount;           /* The number of processor resets */
     uint32                  MaxProcessorResetCount;        /* The maximum number before a Power On */
     CFE_ES_DebugVariables_t DebugVars;                     /* ES Debug variables */
     CFE_TIME_SysTime_t      TimeCode;                      /* Time code */
     char                    Description[80];               /* The ascii data for the event */
     uint32                  ContextIsPresent;                  /* Indicates the context data is valid */
     uint32                  AppID;                          /* The application ID */
     uint32                  Context[CFE_PSP_CPU_CONTEXT_SIZE];  /* cpu  context */

} CFE_ES_ERLog_t;

/*
** CFE_ES_GenCounterRecord_t is an internal structure used to keep track of
** Generic Counters that are active in the system.
*/
typedef struct
{
   boolean        RecordUsed;                     /* Is the record used(1) or available(0) ? */
   uint32         Counter;                        /* The type of App: CORE or EXTERNAL */
   char           CounterName[OS_MAX_API_NAME];   /* Counter Name */      
} CFE_ES_GenCounterRecord_t;


/*
** Executive Services Global Memory Data
** This is the regular global data that is not preserved on a 
**  processor reset.
*/
typedef struct
{
   /*
   ** Debug Variables
   */
   CFE_ES_DebugVariables_t DebugVars;
   
   /*
   ** Shared Data Semaphore
   */
   uint32 SharedDataMutex;
   
   /*
   ** Startup Sync variables.
   */
   uint32  StartupSyncSemaphore;
   int32   AppStartupCounter;
   boolean StartupFileComplete;
   boolean StartupSemaphoreReleased;

   /*
   ** ES Task Table
   */ 
   uint32              RegisteredTasks;
   CFE_ES_TaskRecord_t TaskTable[OS_MAX_TASKS];

   /*
   ** ES App Table
   */
   uint32             RegisteredCoreApps;
   uint32             RegisteredExternalApps;
   CFE_ES_AppRecord_t AppTable[CFE_ES_MAX_APPLICATIONS];
   
   /*
   ** ES Shared Library Table
   */
   uint32             RegisteredLibs;
   CFE_ES_LibRecord_t LibTable[CFE_ES_MAX_LIBRARIES];

   /*
   ** ES Generic Counters Table
   */
   CFE_ES_GenCounterRecord_t CounterTable[CFE_ES_MAX_GEN_COUNTERS];

   /*
   ** Critical Data Store Management Variables
   */
   CFE_ES_CDSVariables_t CDSVars;

} CFE_ES_Global_t;

/*
** Executive Services Global Reset Data type
** This is the special memory area for ES that is preserved
** on a processor reset.
*/
typedef struct
{
   /*
   ** Exception and Reset log declaration
   */
   CFE_ES_ERLog_t    ERLog[CFE_ES_ER_LOG_ENTRIES];
   uint32            ERLogIndex;
   uint32            ERLogEntries;
   uint32            LastAppId;

   /*
   ** System Log declaration
   */
   char            SystemLog[CFE_ES_SYSTEM_LOG_SIZE];
   uint32          SystemLogIndex;
   uint32          SystemLogMode;
   uint32          SystemLogEntryNum;
 
   /*
   ** Performance Data
   */
   CFE_ES_PerfData_t       Perf;
 
   /*
   ** Reset Variables
   */
   CFE_ES_ResetVariables_t  ResetVars;

   /*
   ** Time variables that are 
   ** preserved on a processor reset
   */ 
   CFE_TIME_ResetVars_t TimeResetVars;

   #ifdef CFE_EVS_LOG_ON
      /*
      ** EVS Log and associated variables. This needs to be preserved on a processor reset.
      */
      CFE_EVS_Log_t              EVS_Log;
   #endif

} CFE_ES_ResetData_t;

/*
** The Executive Services Global Data declaration
*/
extern CFE_ES_Global_t CFE_ES_Global; 

/*
** The Executive Services Nonvolatile Data declaration
*/
extern CFE_ES_ResetData_t *CFE_ES_ResetDataPtr;


/*
** Functions used to lock/unlock shared data
*/
extern int32 CFE_ES_GetAppIDInternal(uint32 *AppIdPtr);
extern void  CFE_ES_LockSharedData(const char *FunctionName, int32 LineNumber);
extern void  CFE_ES_UnlockSharedData(const char *FunctionName, int32 LineNumber);


#endif
