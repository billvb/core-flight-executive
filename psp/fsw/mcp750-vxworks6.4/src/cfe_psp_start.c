/******************************************************************************
** File:  cfe_psp_start.c
**
**
**      Copyright (c) 2004-2006, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software(cFE) was created at NASA Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This software may be used only pursuant to a United States government 
**      sponsored project and the United States government may not be charged
**      for use thereof. 
**
**
** Purpose:
**   cFE PSP main entry point.
**
** History:
**   2004/09/23  J.P. Swinski    | Initial version,
**   2004/10/01  P.Kutt          | Replaced OS API task delay with VxWorks functions
**                                 since OS API is initialized later.
**
******************************************************************************/

/*
**  Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "vxWorks.h"
#include "sysLib.h"
#include "taskLib.h"
#include "ramDrv.h"
#include "dosFsLib.h"
#include "xbdBlkDev.h"
#include "errnoLib.h"
#include "usrLib.h"
#include "cacheLib.h"
#include "drv/hdisk/ataDrv.h"
/* #include "config.h" */
#include "mcpx750.h"

/*
** cFE includes 
*/
#include "common_types.h"
#include "osapi.h"
#include "cfe_es.h"            /* For reset types */
#include "cfe_platform_cfg.h"  /* for processor ID */

#include "cfe_psp.h" 
#include "cfe_psp_memory.h"           

/*
**  External Declarations
*/
extern void CFE_TIME_Local1HzISR(void);
IMPORT void sysPciWrite32 (UINT32, UINT32);

/******************************************************************************
**  Function:  CFE_PSP_Main()
**
**  Purpose:
**    vxWorks/BSP Application entry point.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/

void CFE_PSP_Main(  int ModeId, char *StartupFilePath )
{
   int    TicksPerSecond;
   uint32 reset_type;
   uint32 reset_subtype;
   char   reset_register; 


   /*
   ** Initialize the OS API
   */
   OS_API_Init();

   /* 
   ** Delay for one second. 
   */
   TicksPerSecond = sysClkRateGet();
   (void) taskDelay( TicksPerSecond );

   /* 
   ** This starts up the hardware timer on the board that
   ** will be used to get the local time 
   */
   sysPciWrite32(0xFC0011D0, 0x0D6937E5);

   /*
   ** Setup the pointer to the reserved area in vxWorks.
   ** This must be done before any of the reset variables are used.
   */
   CFE_PSP_ReservedMemoryPtr = (CFE_PSP_ReservedMemory_t *)sysMemTop();

   /*
   ** Determine Reset type by reading the hardware reset register.
   */
   reset_register = *(SYS_REG_BLRR);
   OS_printf("CFE_PSP: Reset Register = %02X\n",reset_register);
   
   if ( reset_register & SYS_REG_BLRR_PWRON )
   {    
      OS_printf("CFE_PSP: POWERON Reset: Power Switch ON.\n");
      reset_type = CFE_ES_POWERON_RESET;
      reset_subtype = CFE_ES_POWER_CYCLE;
   }
   else if ( reset_register & SYS_REG_BLRR_PBRST )
   {    
      OS_printf("CFE_PSP: POWERON Reset: CPCI Push Button Reset.\n");
      reset_type = CFE_ES_POWERON_RESET;
      reset_subtype = CFE_ES_PUSH_BUTTON;
   }
   else if ( reset_register & SYS_REG_BLRR_FBTN )
   {    
      OS_printf("CFE_PSP: POWERON Reset: Front Panel Push Button Reset.\n");
      reset_type = CFE_ES_PUSH_BUTTON;
      reset_subtype = 3;
   }
   else if ( reset_register & SYS_REG_BLRR_WDT2 )
   {    
      OS_printf("CFE_PSP: PROCESSOR Reset: Watchdog level 2 Reset.\n");
      reset_type = CFE_ES_PROCESSOR_RESET;
      reset_subtype = CFE_ES_HW_WATCHDOG;
   }
   else if ( reset_register & SYS_REG_BLRR_SWSRST )
   {    
      OS_printf("CFE_PSP: PROCESSOR Reset: Software Soft Reset.\n");
      reset_type = CFE_ES_PROCESSOR_RESET;
      reset_subtype = CFE_ES_RESET_COMMAND;
   }
   else if ( reset_register & SYS_REG_BLRR_SWHRST )
   {
   
      /*
      ** For a Software hard reset, we want to look at the special 
      ** BSP reset variable to determine if we wanted a 
      ** Power ON or a Processor reset. Because the vxWorks sysToMonitor and
      ** reboot functions use this reset type, we want to use this for a software
      ** commanded processor or Power on reset.
      */
      if ( CFE_PSP_ReservedMemoryPtr->bsp_reset_type == CFE_ES_POWERON_RESET)
      {
         OS_printf("CFE_PSP: POWERON Reset: Software Hard Reset.\n");
         reset_type = CFE_ES_POWERON_RESET;
         reset_subtype = CFE_ES_RESET_COMMAND;
      }
      else
      {
         OS_printf("CFE_PSP: PROCESSOR Reset: Software Hard Reset.\n");
         reset_type = CFE_ES_PROCESSOR_RESET;
         reset_subtype = CFE_ES_RESET_COMMAND;
      }
   }
   else 
   {
      OS_printf("CFE_PSP: POWERON Reset: UNKNOWN Reset.\n");
      reset_type = CFE_ES_POWERON_RESET;
      reset_subtype = CFE_ES_UNDEFINED_RESET; 
   }   

   /*
   ** Initialize the reserved memory 
   */
   CFE_PSP_InitProcessorReservedMemory(reset_type);


   /*
   ** Call cFE entry point. This will return when cFE startup
   ** is complete.
   */
   CFE_ES_Main(reset_type,reset_subtype, 1, CFE_ES_NONVOL_STARTUP_FILE); 

   /*
   ** Main loop for default task and simulated 1hz 
   */
   for ( ;; )
   {
      TicksPerSecond = sysClkRateGet();
      (void) taskDelay( TicksPerSecond );
      CFE_TIME_Local1HzISR();
   }
}

