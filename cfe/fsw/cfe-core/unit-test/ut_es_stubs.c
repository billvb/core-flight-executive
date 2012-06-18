/*
** File:
** $Id: ut_es_stubs.c 1.10 2010/11/03 15:15:09EDT jmdagost Exp  $
**
** Purpose:
** Unit test stubs for Executive Service routines
** 
** Notes:
** Minimal work is done, only what is required for unit testing
**
** $Data:$
** $Revision: 1.10 $
** $Log: ut_es_stubs.c  $
** Revision 1.10 2010/11/03 15:15:09EDT jmdagost 
** Added cfe.h include file.
** Revision 1.9 2010/10/20 12:39:20EDT jmdagost 
** Added ES memory pool creation with no semaphore option.
** Revision 1.8 2009/07/29 19:22:21EDT aschoeni 
** Added GetPoolBufInfo and GetPoolInfoRtn
** Revision 1.7 2009/05/07 15:02:28EDT rmcgraw 
** DCR7366:1 Modified ES_GetTaskInfo
** Revision 1.6 2009/05/06 09:53:02EDT rmcgraw 
** DCR7366:1 Changed PutPool to return positive integer for success
** Revision 1.5 2009/04/23 09:27:16EDT rmcgraw 
** DCR7366:1 Commented out the fprintfs that indicate the function was called
** Revision 1.4 2009/04/01 16:09:08EDT rmcgraw 
** DCR7366:1 Added Stub support for PoolCreateEx and removed alternating task names
** Revision 1.3 2008/08/15 11:27:04EDT njyanchik 
** Check in of ES Unit Test
** Revision 1.2 2008/08/07 03:43:52BST dkobe 
** Updated Unit Tests for CFE_TIME_RegisterSynchCallback, CFE_TIME_UnregisterSynchCallback and CFE_TIME_CleanUpApp
** Revision 1.1 2008/04/17 08:05:44EDT ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
** Revision 1.18 2007/09/24 08:01:56EDT njyanchik 
** Update of EVS unit test
** Revision 1.17 2007/09/21 15:40:18EDT David Kobe (dlkobe) 
** Modified pointer type definitions to eliminate Linux gcc compile warnings
** Revision 1.15 2007/07/08 22:30:12EDT dlkobe 
** Enhanced simulation of CDS
** Revision 1.14 2007/07/05 15:04:30EDT dlkobe 
** Baseline following complete TBL unit testing
** Revision 1.13 2007/05/10 15:14:25EDT njyanchik 
** Another update of Jonathans UT
** Revision 1.12 2007/05/04 09:12:06EDT njyanchik 
** Check in of Time UT and related changes
** Revision 1.10 2007/05/01 13:28:23EDT njyanchik 
** I updated the ut stubs to get the each of the subsytems to compile under the unit test. I did not
** change the unit tests themselves to cover more of the files, however.
** Revision 1.9 2007/03/19 10:12:50EST njyanchik 
** fix a small api change in ES
** Revision 1.8 2007/02/26 14:22:13EST njyanchik 
** initial check in files for TBL unit test, plus updates to current files
** Revision 1.7 2006/11/02 13:53:57EST njyanchik 
** Unit test for TIME was updated to match the changes made for this DCR
** Revision 1.6 2006/07/26 07:05:23GMT-05:00 rjmcgraw 
** Added CreatePoolRtn code
*/

/*
** Include section
*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "cfe.h"
#include "common_types.h"
#include "../time/cfe_time_utils.h"
#include "cfe_es.h"
#include "ut_stubs.h"

/*
** Exported Global Data
*/
extern FILE *UT_logfile;
extern char UT_appname[80];

extern uint32 UT_PutPool_Fail;
extern uint32 UT_AppID;
extern uint32 UT_SignatureCDS;
extern uint32 UT_StatusCDS;
extern UT_SetRtn_t CreatePoolRtn;
extern UT_SetRtn_t PoolCreateExRtn;
extern UT_SetRtn_t PutPoolRtn;
extern UT_SetRtn_t GetPoolRtn;
extern UT_SetRtn_t GetPoolInfoRtn;
extern UT_SetRtn_t ES_RegisterRtn;
extern UT_SetRtn_t ES_CreateChildRtn;
extern UT_SetRtn_t ES_DeleteCDSRtn; /* new */
extern UT_SetRtn_t ES_RegisterCDSRtn;
extern UT_SetRtn_t ES_CopyToCDSRtn;
extern UT_SetRtn_t ES_RestoreFromCDSRtn;
extern UT_SetRtn_t GetAppIDRtn;
extern uint8       UT_CDS[UT_CDS_SIZE];
extern UT_CDS_Map_t UT_CDS_Map;



/*
** Function definitions
*/
int32 TST_LIB_Init(void);

int32  CFE_ES_CreateChildTask(uint32       *TaskIdPtr,
                              const char   *TaskName,
                              CFE_ES_ChildTaskMainFuncPtr_t FunctionPtr,
                              const uint32 *StackPtr,
                              uint32        StackSize,
                              uint32        Priority,
                              uint32        Flags)
{

    if (ES_CreateChildRtn.count > 0)
    {
        ES_CreateChildRtn.count--;

        if(ES_CreateChildRtn.count == 0)
        {
            fprintf(UT_logfile, "  CFE_ES_CreateChildTask: called: %d\n",ES_CreateChildRtn.value);

            return ES_CreateChildRtn.value;        
        }
    }
  fprintf(UT_logfile, "  CFE_ES_CreateChildTask: called - %s\n", TaskName);
  return CFE_SUCCESS;
}

int32 CFE_ES_GetAppID (uint32 *pAppID)
{
  *pAppID = UT_AppID;
  
  if(GetAppIDRtn.count > 0)
  {
    GetAppIDRtn.count--;
    if(GetAppIDRtn.count == 0)
      return GetAppIDRtn.value;
  }
  
  return CFE_SUCCESS;
}

int32 CFE_ES_GetAppIDByName (uint32 *pAppID, char *pAppName)
{
  int32 status = CFE_SUCCESS;

  if (strcmp(pAppName, UT_appname) == 0)
    *pAppID = UT_AppID;
  else if (strcmp(pAppName, "unregistered app") == 0)
    *pAppID = CFE_ES_MAX_APPLICATIONS - 1;
  else if (strcmp(pAppName, "illegal id") == 0)
    *pAppID = CFE_ES_MAX_APPLICATIONS + 1;
  else
    status = CFE_ES_ERR_APPNAME;

  return status;
}

int32 CFE_ES_GetAppName (char *pAppName, uint32 AppID, uint32 BufferLength)
{
  strcpy(pAppName,UT_appname);
  return CFE_SUCCESS;
}

int32 CFE_ES_RegisterApp (void)
{
  

    if (ES_RegisterRtn.count > 0)
    {
        ES_RegisterRtn.count--;
        if(ES_RegisterRtn.count ==0)
        {
            /*fprintf(UT_logfile, "  CFE_ES_RegisterApp: called: %d\n",ES_RegisterRtn.value);*/

            return ES_RegisterRtn.value;        
        }
    }
    
    /*fprintf(UT_logfile, "  CFE_ES_RegisterApp: called: SUCCESS\n");*/

    return CFE_SUCCESS;
}

int32 CFE_ES_RegisterChildTask (void)
{
  /*fprintf(UT_logfile, "  CFE_ES_RegisterChildTask: called\n");*/
  return CFE_SUCCESS;
}

int32 CFE_ES_WriteToSysLog (const char *pSpecString, ...)
{

  va_list ap;
  char tmpString[128];

  va_start(ap, pSpecString);
  vsprintf(tmpString, pSpecString, ap);
  va_end(ap);

  fprintf(UT_logfile, "  CFE_ES_WriteToSysLog: %s\n", tmpString);
  return 0;
}

int32 CFE_ES_GetPoolBuf (uint32 **BufPtr,CFE_ES_MemHandle_t HandlePtr,uint32 Size)
{

  uint32 a = 0x8000000;

  if(GetPoolRtn.count > 0)
  {
    GetPoolRtn.count--;
    if(GetPoolRtn.count == 0)
      return GetPoolRtn.value;
  }

  if (Size > CFE_SB_MAX_SB_MSG_SIZE)
    return(0xFFFFFFFF);

  *BufPtr = (uint32)calloc(Size,1);

  /* Round up for return value, like CFE_ES_GetBlockSize */
  while(!(a>>1 & Size)) a >>= 1;

  return a;
}

int32 CFE_ES_PoolCreate(uint32 *HandlePtr, uint8 *MemPtr, uint32 Size)
{
  fprintf(UT_logfile, "  CFE_ES_PoolCreate: called\n");
  
  if(CreatePoolRtn.count > 0)
  {
    CreatePoolRtn.count--;
    if(CreatePoolRtn.count == 0)
      return CreatePoolRtn.value;
  }  
  return OS_SUCCESS;
}

int32 CFE_ES_PoolCreateNoSem(CFE_ES_MemHandle_t *HandlePtr, uint8 *MemPtr, uint32 Size)
{
  fprintf(UT_logfile, "  CFE_ES_PoolCreateNoSem: called\n");
  
  if(CreatePoolRtn.count > 0)
  {
    CreatePoolRtn.count--;
    if(CreatePoolRtn.count == 0)
      return CreatePoolRtn.value;
  }  
  return OS_SUCCESS;
}

int32 CFE_ES_PoolCreateEx(uint32 *HandlePtr, uint8 *MemPtr, uint32 Size, 
                          uint32 NumBlockSizes,uint32 *BlockSizes, uint16 UseMutex)
{
  fprintf(UT_logfile, "  CFE_ES_PoolCreateEx: called\n");
   
  if(PoolCreateExRtn.count > 0)
  {
    PoolCreateExRtn.count--;
    if(PoolCreateExRtn.count == 0)
      return PoolCreateExRtn.value;
  }  
  return CFE_SUCCESS;
}

/* Put pool returns a positive integer equal to the size of the buffer release */
int32 CFE_ES_PutPoolBuf (CFE_ES_MemHandle_t HandlePtr, uint32 *BufPtr)
{

  int status = CFE_SUCCESS;

  if(PutPoolRtn.count > 0)
  {
    PutPoolRtn.count--;
    if(PutPoolRtn.count == 0)
      return PutPoolRtn.value;
  }

  if (UT_PutPool_Fail == 0){
    free(BufPtr);
    status = 16;/* return a positive integer to indicate success */
  }else{
    status = -1;
  }

  
  return status;
}

int32 CFE_ES_GetPoolBufInfo (CFE_ES_MemHandle_t HandlePtr, uint32 *BufPtr)
{

  int status = CFE_SUCCESS;

  if(GetPoolInfoRtn.count > 0)
  {
    GetPoolInfoRtn.count--;
    if(GetPoolInfoRtn.count == 0)
      return GetPoolInfoRtn.value;
  }

  status = 16;/* return a positive integer to indicate success */


  return status;
}


void CFE_ES_PerfLogAdd(uint32 Marker, uint32 EntryExit)
{
   /* fprintf(UT_logfile,"  CFE_ES_PerfLogAdd Called, EntryExit = %lu\n",EntryExit);*/
}


uint32 CFE_ES_CalculateCRC(void *DataPtr, uint32 DataLength, uint32 InputCRC, uint32 TypeCRC)
{
    return (332424);
}
 
int32 CFE_ES_GetTaskInfo(CFE_ES_TaskInfo_t *TaskInfo, uint32 TaskId)
{
       
    static uint32 cnt;

    TaskInfo->AppId = 3;/* fake number */
    strcpy(&TaskInfo->AppName,UT_appname);
    strcpy(&TaskInfo->TaskName,UT_appname);

    if (cnt % 2)  strcpy(&TaskInfo->TaskName,"");
    
    cnt++;

    return CFE_SUCCESS;
}

void CFE_ES_ExitApp(uint32 ExitStatus)
{

}

int32 CFE_ES_CopyToCDS(CFE_ES_CDSHandle_t Handle, void *DataToCopy)
{
    int32 i=0;
    
	if(ES_CopyToCDSRtn.count > 0)
	{
		ES_CopyToCDSRtn.count--;
		if(ES_CopyToCDSRtn.count == 0)
			return ES_CopyToCDSRtn.value;
	}
    
    while (UT_CDS_Map.Handles[i] != Handle)
    {
        i++;
    }
    
    memcpy(&UT_CDS[Handle], DataToCopy, (UT_CDS_Map.Handles[i+1] - UT_CDS_Map.Handles[i]));

    return CFE_SUCCESS;
}
int32 CFE_ES_RestoreFromCDS(void *RestoreToMemory, CFE_ES_CDSHandle_t Handle)
{
    int32 i=0;
    
	if(ES_RestoreFromCDSRtn.count > 0)
	{
		ES_RestoreFromCDSRtn.count--;
		if(ES_RestoreFromCDSRtn.count == 0)
			return ES_RestoreFromCDSRtn.value;
	}
    
    while (UT_CDS_Map.Handles[i] != Handle)
    {
        i++;
    }
    
    memcpy(RestoreToMemory, &UT_CDS[Handle], (UT_CDS_Map.Handles[i+1] - UT_CDS_Map.Handles[i]));
    
    return CFE_SUCCESS;
} /* End of CFE_ES_RestoreFromCDS() */


int32 CFE_ES_RegisterCDSEx(CFE_ES_CDSHandle_t *HandlePtr, int32 BlockSize, const char *Name, boolean CriticalTbl)
{
    *HandlePtr = UT_CDS_Map.Handles[UT_CDS_Map.NextHandle];
    UT_CDS_Map.Handles[UT_CDS_Map.NextHandle+1] = UT_CDS_Map.Handles[UT_CDS_Map.NextHandle] + BlockSize;
    UT_CDS_Map.NextHandle++;
    
    if (UT_CDS_Map.NextHandle >= UT_MAX_NUM_CDS)
    {
        fprintf(UT_logfile,"  CFE_ES_RegisterCDSEx Called, Number of CDSs exceeds UT maximum of %d\n", UT_MAX_NUM_CDS);
        
        exit(EXIT_FAILURE);
    }
    if (UT_CDS_Map.Handles[UT_CDS_Map.NextHandle] >= UT_CDS_SIZE)
    {
        fprintf(UT_logfile,"  CFE_ES_RegisterCDSEx Called, Size of CDSs (%ld) exceeds UT maximum of %d\n", 
                UT_CDS_Map.Handles[UT_CDS_Map.NextHandle], UT_CDS_SIZE);
        exit(EXIT_FAILURE);
    }
    
	if(ES_RegisterCDSRtn.count > 0)
	{
		ES_RegisterCDSRtn.count--;
		if(ES_RegisterCDSRtn.count == 0)
			return ES_RegisterCDSRtn.value;
	}
    return CFE_SUCCESS;
}  /* End of CFE_ES_RegisterCDSEx() */

int32 CFE_ES_DeleteCDS(const char *CDSName, boolean CalledByTblServices) /* new */
{
	if(ES_DeleteCDSRtn.count > 0)
	{
		ES_DeleteCDSRtn.count--;
		if(ES_DeleteCDSRtn.count == 0)
			return ES_DeleteCDSRtn.value;
	}

	return CFE_SUCCESS;
}
int32 CFE_ES_GetResetType(uint32 *ResetSubtypePtr)
{
    *ResetSubtypePtr = CFE_ES_POWERON_RESET;
    return CFE_SUCCESS;
}

void CFE_ES_IncrementTaskCounter(void)
{
}

int32 TST_LIB_Init(void)
{
    int32   status;
    
    OS_printf("TST_LIB Initializing\n");
    
    status = CFE_SUCCESS;
    
    return(status);
    
} /* End of TST_LIB_Init */

