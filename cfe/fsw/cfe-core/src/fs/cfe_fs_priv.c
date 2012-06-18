/*
** $Id: cfe_fs_priv.c 1.4 2010/11/03 15:09:10EDT jmdagost Exp  $
**
** Purpose:  cFE File Services (FS) library API Initialization 
**
** Author:  Alan Cudmore/NASA GSFC 
**
** Notes:
**
** $Log: cfe_fs_priv.c  $
** Revision 1.4 2010/11/03 15:09:10EDT jmdagost 
** Added cfe.h include file.
** Revision 1.3 2009/06/10 09:13:52EDT acudmore 
** Converted OS_Mem* and OS_BSP* calls to CFE_PSP_*
** Revision 1.2 2008/07/08 15:40:40EDT apcudmore 
** Added CFE_FS global data, lock/unlock functions, log messages for decompress API.
** Revision 1.1 2008/07/08 13:37:12EDT apcudmore 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/src/fs/project.pj
** Revision 1.2 2008/07/08 13:32:06EDT apcudmore 
** Added Mutex Lock/Unlock functions
** Revision 1.1 2008/07/08 12:50:15EDT apcudmore 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/src/fs/project.pj
**
*/


/*
** Required header files
*/
#include "osapi.h"
#include "cfe.h"
#include "cfe_es.h"
#include "cfe_fs.h"
#include "cfe_fs_priv.h"

#include <string.h>

/*
** Global data
** 
*/
CFE_FS_t CFE_FS;

/******************************************************************************
**  Function:  CFE_FS_EarlyInit()
**
**  Purpose:
**    Initialize the FS data structures before the cFE runs.
**
**  Arguments:
**
**  Notes:
**    This function MUST be called before any FS API's are called.
**
**  Return:
**    CFE_SUCCESS
*/
int32 CFE_FS_EarlyInit (void) 
{
    int32 Stat = CFE_SUCCESS;

    Stat = OS_MutSemCreate(&CFE_FS.SharedDataMutexId, "CFE_FS_SharedMutex", 0);
    if( Stat != OS_SUCCESS )
    {
      CFE_ES_WriteToSysLog("FS Shared Data Mutex creation failed! RC=0x%08x",Stat);
      return Stat;
    }/* end if */

    return Stat;

}/* end CFE_FS_EarlyInit */

/******************************************************************************
**  Function:  CFE_FS_LockSharedData()
**
**  Purpose:
**    FS internal function to handle a semaphore take failure for the FS 
**    Data Mutex
**
**  Arguments:
**    FunctionName   - the Function Name of the code that generated the error.
**
**  Return:
**    None
*/
void CFE_FS_LockSharedData(const char *FunctionName)
{
    int32   Status;
    uint32  AppId = 0;

    Status = OS_MutSemTake(CFE_FS.SharedDataMutexId);
    if (Status != OS_SUCCESS) 
    {
        CFE_ES_GetAppID(&AppId);

        CFE_ES_WriteToSysLog("FS SharedData Mutex Take Err Stat=0x%x,App=%d,Function=%s",
                 Status,AppId,FunctionName);

    }/* end if */

    return;

}/* end CFE_FS_LockSharedData */

/******************************************************************************
**  Function:  CFE_FS_UnlockSharedData()
**
**  Purpose:
**    FS internal function to handle a semaphore give failure for the Shared
**    Data Mutex
**
**  Arguments:
**    FunctionName - the Function containing the code that generated the error.
**
**  Return:
**    None
*/
void CFE_FS_UnlockSharedData(const char *FunctionName)
{
   int32   Status;
   uint32  AppId = 0;

   Status = OS_MutSemGive(CFE_FS.SharedDataMutexId);
   if (Status != OS_SUCCESS) 
   {
       CFE_ES_GetAppID(&AppId);
       CFE_ES_WriteToSysLog("FS SharedData Mutex Give Err Stat=0x%x,App=%d,Function=%s",
                                Status,AppId,FunctionName);

   }/* end if */
   return;

}/* end CFE_FS_UnlockSharedData */

/************************/
/*  End of File Comment */
/************************/
