/******************************************************************************
** File:  cfe_psp_support.c
**
**
**      Copyright (c) 2004-2011, United States Government as represented by 
**      Administrator for The National Aeronautics and Space Administration. 
**      All Rights Reserved.
**
**      This is governed by the NASA Open Source Agreement and may be used,
**      distributed and modified only pursuant to the terms of that agreement.
**
**
** Purpose:
**   This file contains glue routines between the cFE and the OS Board Support Package ( BSP ).
**   The functions here allow the cFE to interface functions that are board and OS specific
**   and usually dont fit well in the OS abstraction layer.
**
** History:
**   2005/06/05  Alan Cudmore    | Initial version,
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
#include "errnoLib.h"
#include "usrLib.h"
#include "cacheLib.h"
#include "drv/hdisk/ataDrv.h"
#include "cacheLib.h"
#include "rebootLib.h"


/*
** cFE includes
*/
#include "common_types.h"
#include "osapi.h"
#include "cfe_es.h"            /* For reset types */
#include "cfe_platform_cfg.h"  /* for processor ID */
#include "cfe_mission_cfg.h"   /* for spacecraft ID */

/*
** Types and prototypes for this module
*/
#include "cfe_psp.h"
#include "cfe_psp_memory.h"

/******************************************************************************
**  Function:  CFE_PSP_Restart()
**
**  Purpose:
**    Provides a common interface to the processor reset.
**
**  Arguments:
**    reset_type  : Type of reset.
**
**  Return:
**    (none)
*/

void CFE_PSP_Restart(uint32 reset_type)
{

   if ( reset_type == CFE_PSP_RST_TYPE_POWERON )
   {
      CFE_PSP_ReservedMemoryPtr->bsp_reset_type = CFE_PSP_RST_TYPE_POWERON;
      CFE_PSP_FlushCaches(1, (uint32 )CFE_PSP_ReservedMemoryPtr, sizeof(CFE_PSP_ReservedMemory_t));
      reboot(BOOT_CLEAR);
   }
   else
   {
      CFE_PSP_ReservedMemoryPtr->bsp_reset_type = CFE_PSP_RST_TYPE_PROCESSOR;
      CFE_PSP_FlushCaches(1, (uint32 )CFE_PSP_ReservedMemoryPtr, sizeof(CFE_PSP_ReservedMemory_t));
      reboot(BOOT_NORMAL);
   }

}

/******************************************************************************
**  Function:  CFE_PSP_Panic()
**
**  Purpose:
**    Provides a common interface to abort the cFE startup process and return
**    back to the OS.
**
**  Arguments:
**    ErrorCode  : Reason for Exiting.
**
**  Return:
**    (none)
*/

void CFE_PSP_Panic(int32 ErrorCode)
{
   printf("CFE_PSP_Panic Called with error code = 0x%08X. Exiting.\n",(unsigned int )ErrorCode);
   exit(-1); /* Need to improve this */
}

/******************************************************************************
**  Function:  CFE_PSP_FlushCaches()
**
**  Purpose:
**    Provides a common interface to flush the processor caches. This routine
**    is in the BSP because it is sometimes implemented in hardware and
**    sometimes taken care of by the RTOS.
**
**  Arguments:
**
**  Return:
**    (none)
*/

void CFE_PSP_FlushCaches(uint32 type, uint32 address, uint32 size)
{
   if ( type == 1 )
   {
      cacheTextUpdate((void *)address, size);
   }

}

/*
** Name: CFE_PSP_GetProcessorId
**
** Purpose:
**         return the processor ID.
**
**
** Parameters:
**
** Global Inputs: None
**
** Global Outputs: None
**
**
**
** Return Values: Processor ID
*/
uint32 CFE_PSP_GetProcessorId    (void)
{
    return(CFE_CPU_ID);
}


/*
** Name: CFE_PSP_GetSpacecraftId
**
** Purpose:
**         return the spacecraft ID.
**
** Parameters:
**
** Global Inputs: None
**
** Global Outputs: None
**
**
** Return Values: Spacecraft ID
*/
uint32 CFE_PSP_GetSpacecraftId   (void)
{
   return(CFE_SPACECRAFT_ID);
}

