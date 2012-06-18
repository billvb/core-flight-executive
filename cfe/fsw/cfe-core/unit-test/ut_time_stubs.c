/*
** File:
** $Id: ut_time_stubs.c 1.2 2008/08/15 11:26:55EDT njyanchik Exp  $
**
** Purpose:
** Unit test stubs for Time routines
** 
** Notes:
** Minimal work is done, only what is required for unit testing
**
** $Data:$
** $Revision: 1.2 $
** $Log: ut_time_stubs.c  $
** Revision 1.2 2008/08/15 11:26:55EDT njyanchik 
** Check in of ES Unit Test
** Revision 1.1 2008/04/17 13:05:48BST ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
** Revision 1.3 2007/05/01 13:28:14EDT njyanchik 
** I updated the ut stubs to get the each of the subsytems to compile under the unit test. I did not
** change the unit tests themselves to cover more of the files, however.
** Revision 1.2 2007/01/17 09:26:07EST njyanchik 
** Check in of ES Unit Test changed files
** Revision 1.1 2006/03/02 15:10:27GMT-05:00 jjhageman 
** Initial revision
** Member added to project d:/mksdata/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
*/

/*
** Include section
*/

#include "cfe_time.h"
#include "../src/time/cfe_time_utils.h"

#include "ut_stubs.h"


/*
** Function definitions
*/

extern UT_SetRtn_t TIMECleanUpRtn;
int32 CFE_TIME_EarlyInit(void)
{
    return CFE_SUCCESS;
}

void CFE_TIME_TaskMain(void)
{
}

void CFE_TIME_Print(char *PrintBuffer, CFE_TIME_SysTime_t TimeToPrint)
{
    sprintf(PrintBuffer,"CFE_TIME_Print Secs: %lu, SubSecs: %lu -",TimeToPrint.Seconds, TimeToPrint.Subseconds);
    return;

} /* End of CFE_TIME_Print() */

CFE_TIME_SysTime_t CFE_TIME_GetTime (void)
{

  static CFE_TIME_SysTime_t time;

  time.Seconds++;
  time.Subseconds++;

  return time;
}


int32 CFE_TIME_CleanUpApp (void)
{
    if(TIMECleanUpRtn.count > 0)
    {
        TIMECleanUpRtn.count--;
        if(TIMECleanUpRtn.count == 0)
            return TIMECleanUpRtn.value;
    }
    return CFE_SUCCESS;
}