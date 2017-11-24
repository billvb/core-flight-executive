/******************************************************************************
** File:  cfe_psp_ssr.c
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
** cFE includes
*/
#include "common_types.h"
#include "osapi.h"

/*
** Types and prototypes for this module
*/
#include "cfe_psp.h"
#include "cfe_psp_config.h"

/*
**  Standard Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/******************************************************************************
**  Function:  CFE_PSP_InitSSR
**
**  Purpose:
**    Initializes the Solid State Recorder device. This can be filled in for the platform.
**
**  Arguments:
**    bus, device, device name
**
**  Return:
**    (none)
*/

int32 CFE_PSP_InitSSR(uint32 bus, uint32 device, char *DeviceName )
{
   int32 Status;

   Status = CFE_PSP_ERROR;
   
   return(Status);

}

