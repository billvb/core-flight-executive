/*
** File:
** $Id: ut_evs_stubs.c 1.3 2009/05/07 15:04:48EDT rmcgraw Exp  $
**
** Purpose:
** Unit test stubs for Event Service routines
** 
** Notes:
** Minimal work is done, only what is required for unit testing
**
** $Data:$
** $Revision: 1.3 $
** $Log: ut_evs_stubs.c  $
** Revision 1.3 2009/05/07 15:04:48EDT rmcgraw 
** DCRR 7366:1 Moved call to AddEventToHistory
** Revision 1.2 2009/04/23 09:27:56EDT rmcgraw 
** DCR7366:1 Comment out the fprintfs that indicate the function was called
** Revision 1.1 2008/04/17 08:05:45EDT ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
** Revision 1.7 2007/07/05 15:04:30EDT David Kobe (dlkobe) 
** Baseline following complete TBL unit testing
** Revision 1.6 2007/05/30 15:15:18EDT njyanchik 
** check in of ES Unit test
** Revision 1.5 2007/05/04 09:10:25EDT njyanchik 
** Check in of Time UT and related changes
** Revision 1.4 2007/05/01 13:28:23EDT njyanchik 
** I updated the ut stubs to get the each of the subsytems to compile under the unit test. I did not
** change the unit tests themselves to cover more of the files, however.
** Revision 1.3 2007/01/17 09:26:08EST njyanchik 
** Check in of ES Unit Test changed files
** Revision 1.2 2006/04/25 10:17:04GMT-05:00 rjmcgraw 
** SendTimedEvent added
** Revision 1.1 2006/03/02 20:10:25GMT jjhageman 
** Initial revision
** Member added to project d:/mksdata/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
*/

/*
** Include section
*/

#include <stdio.h>
#include <stdarg.h>

#include "cfe_evs.h"
#include "common_types.h"
#include "cfe_error.h"
#include "../evs/cfe_evs_task.h"
#include "ut_stubs.h"

/*
** Exported Global Data
*/

extern FILE *UT_logfile;
extern UT_SetRtn_t  EVS_SendEventRtn;
extern UT_SetRtn_t  EVS_RegisterRtn;
extern UT_SetRtn_t EVSCleanUpRtn;


/*
** Function definitions
*/

int32 CFE_EVS_EarlyInit(void)
{
    return CFE_SUCCESS;
}
void CFE_EVS_TaskMain(void)
{
}


int32 CFE_EVS_SendEvent (uint16 EventID, uint16 EventType, const char *Spec, ... )
{

  char     BigBuf[CFE_EVS_BIG_BUFFER_SIZE];
  va_list  Ptr;

  va_start(Ptr, Spec);
  vsprintf(BigBuf, Spec, Ptr);
  va_end(Ptr);
  

  if (EVS_SendEventRtn.count > 0 )
  {
      EVS_SendEventRtn.count--;
      if (EVS_SendEventRtn.count == 0)
      {
          /*fprintf(UT_logfile, "  CFE_EVS_SendEvent: called: %d\n", EVS_SendEventRtn.value);*/
          return EVS_SendEventRtn.value;
      }
  }

  UT_AddEventToHistory(EventID);

  fprintf(UT_logfile, "  CFE_EVS_SendEvent: %d, %d - %s\n", EventID, EventType,
          BigBuf);

  return CFE_SUCCESS;
}


int32 CFE_EVS_SendTimedEvent (CFE_TIME_SysTime_t Time, uint16 EventID, uint16 EventType, const char *Spec, ... )
{
  char     BigBuf[CFE_EVS_BIG_BUFFER_SIZE];
  va_list  Ptr;

  va_start(Ptr, Spec);
  vsprintf(BigBuf, Spec, Ptr);
  va_end(Ptr);
  
  UT_AddEventToHistory(EventID);
  
  fprintf(UT_logfile, "  CFE_EVS_SendTimedEvent: %d, %d - %s\n", EventID, EventType,
          BigBuf);

  return CFE_SUCCESS;

} /* End CFE_EVS_SendTimedEvent */


int32 CFE_EVS_Register (void *Filters, uint16 NumEventFilters, uint16 FilterScheme)
{

    if (EVS_RegisterRtn.count > 0 )
    {
        EVS_RegisterRtn.count--;
        if (EVS_RegisterRtn.count == 0)
        {
            /*fprintf(UT_logfile, "  CFE_EVS_Register: called: %d\n", EVS_RegisterRtn.value);*/
            return EVS_RegisterRtn.value;
        }
    }
  /*fprintf(UT_logfile, "  CFE_EVS_Register: called\n");*/
  return CFE_SUCCESS;
}

int32 CFE_EVS_SendEventWithAppID (uint16 EventID, uint16 EventType,uint32 AppID, const char *Spec, ... )
{

  char     BigBuf[CFE_EVS_BIG_BUFFER_SIZE];
  va_list  Ptr;

  va_start(Ptr, Spec);
  vsprintf(BigBuf, Spec, Ptr);
  va_end(Ptr);
  
  UT_AddEventToHistory(EventID);
  
    if (EVS_SendEventRtn.count > 0 )
    {
        EVS_SendEventRtn.count--;
        if (EVS_SendEventRtn.count == 0)
        {
            /*fprintf(UT_logfile, "  CFE_EVS_SendEvent: called: %d\n", EVS_SendEventRtn.value);*/
            return EVS_SendEventRtn.value;
        }
    }

  fprintf(UT_logfile, "  CFE_EVS_SendEvent from app %lu: %d, %d - %s\n",AppID, EventID, EventType, BigBuf);

  return CFE_SUCCESS;
}
int32 CFE_EVS_CleanUpApp (uint32 AppId)
{
    if(EVSCleanUpRtn.count > 0)
    {
      EVSCleanUpRtn.count--;
      if(EVSCleanUpRtn.count == 0)
        return EVSCleanUpRtn.value;
    }


    return CFE_SUCCESS;
}
