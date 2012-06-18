/*
** File:
** $Id: ut_stubs.c 1.8 2009/08/04 13:34:27EDT aschoeni Exp  $
**
** Purpose:
** Unit test stubs 
** 
** Notes:
** Minimal work is done, only what is required for unit testing
**
** $Data:$
** $Revision: 1.8 $
** $Log: ut_stubs.c  $
** Revision 1.8 2009/08/04 13:34:27EDT aschoeni 
** Added heap information to ES telemetry
** Revision 1.7 2009/07/29 19:23:43EDT aschoeni 
** Added GetPoolInfoRtn
** Revision 1.6 2009/05/06 09:55:00EDT rmcgraw 
** DCR7366:1 Clear event history in UT_InitData
** Revision 1.5 2009/04/28 16:25:14EDT rmcgraw 
** DCR7366:1 Added utility UT_CheckForOpenSockets
** Revision 1.4 2009/04/27 10:39:49EDT rmcgraw 
** DCR7366:1 Added printing number of tests that passed in output.
** Revision 1.3 2009/04/01 16:14:10EDT rmcgraw 
** DCR7366:1 Added poolCreateEx items and moved some utilities from sb_UT.c
** Revision 1.2 2008/08/15 11:26:55EDT njyanchik 
** Check in of ES Unit Test
** Revision 1.1 2008/04/17 13:05:47BST ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
** Revision 1.24 2007/09/21 19:02:55EDT David Kobe (dlkobe) 
** Updated table unit tests to stop segmentation fault
** Revision 1.23 2007/07/09 15:31:48EDT dlkobe 
** Improper order of "memset" parameters was fixed
** Revision 1.22 2007/07/08 22:30:54EDT dlkobe 
** Enhanced CDS simulation
** Revision 1.21 2007/07/05 15:04:26EDT dlkobe 
** Baseline following complete TBL unit testing
** Revision 1.20 2007/05/30 15:15:22EDT njyanchik 
** check in of ES Unit test
** Revision 1.19 2007/05/30 08:56:41EDT njyanchik 
** Update
** Revision 1.18 2007/05/25 13:13:43EDT njyanchik 
** update of ES unit test
** Revision 1.17 2007/05/24 15:48:09EDT njyanchik 
** Update for ES Unit Test
** Revision 1.16 2007/05/22 13:03:58EDT njyanchik 
** Updated changes to not lose my work
** Revision 1.15 2007/05/16 11:14:37EDT njyanchik 
** Update ES's unit test driver to match code for build 4.1
** Revision 1.14 2007/05/10 15:14:26EDT njyanchik 
** Another update of Jonathans UT
** Revision 1.13 2007/05/07 13:45:38EDT njyanchik 
** EVS's Unit test drivers have been updated
** Revision 1.12 2007/05/04 09:10:24EDT njyanchik 
** Check in of Time UT and related changes
** Revision 1.10 2007/05/01 13:28:13EDT njyanchik 
** I updated the ut stubs to get the each of the subsytems to compile under the unit test. I did not
** change the unit tests themselves to cover more of the files, however.
** Revision 1.9 2007/03/02 15:18:05EST njyanchik 
** Jonathan provided me with a newer tbl unit test to check in
** Revision 1.8 2007/01/17 09:26:07GMT-05:00 njyanchik 
** Check in of ES Unit Test changed files
** Revision 1.7 2006/11/08 15:06:08GMT-05:00 rjmcgraw 
** Added FSWriteHdrRtn variable
** Revision 1.6 2006/11/02 13:53:58EST njyanchik 
** Unit test for TIME was updated to match the changes made for this DCR
** Revision 1.5 2006/11/01 12:46:54GMT-05:00 njyanchik 
** Changed the Unit test to reflect the changes from removing the CDS functionality from TIME
** Revision 1.4 2006/07/26 07:10:41GMT-05:00 rjmcgraw 
** Changed SemGiv to SemGive and added CreatePoolRtn
** Revision 1.3 2006/06/08 19:12:37GMT rjmcgraw 
** Added support for function UT_SetRtnCode
** Revision 1.2 2006/05/17 13:56:42GMT jjhageman 
** Added UT_Queue for use by the osapi stubs (OS_Queue create, get, set)
** Revision 1.1 2006/03/02 15:10:26EST jjhageman 
** Initial revision
** Member added to project d:/mksdata/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
*/

/*
** Include section
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ut_stubs.h"
#include "common_types.h"

/*
** Exported Data
*/

FILE *UT_logfile;
int   UT_lognum;
char  UT_appname[80];

uint8        UT_CDS[UT_CDS_SIZE];
UT_CDS_Map_t UT_CDS_Map;

/* Used for getting path coverage */
uint32 UT_OS_Fail = 0x0000;
uint32 UT_BSP_Fail = 0x0000;

CFE_SB_MsgId_t UT_RcvMsgId = 0;
uint16 UT_RcvMsgCode = 0;
uint32 UT_PutPool_Fail = 0;
uint32 UT_AppID = 0;
uint32 UT_StatusCDS = -1; /* Initialize to invalid value */
uint32 UT_SignatureCDS = 0;
uint32 UT_LocTimeSec = 0;
uint32 UT_LocTimeMSec = 0;
uint32 UT_BinSemFail = 0;
UT_Queue_t UT_Queue[OS_MAX_QUEUES];
UT_SetRtn_t MutSemCreateRtn;
UT_SetRtn_t MutSemGiveRtn;
UT_SetRtn_t MutSemTakeRtn;
UT_SetRtn_t QueueCreateRtn;
UT_SetRtn_t QueueDelRtn;
UT_SetRtn_t QueueGetRtn;
UT_SetRtn_t QueuePutRtn;
UT_SetRtn_t FileWriteRtn;
UT_SetRtn_t OSReadRtn;
UT_SetRtn_t OSReadRtn2; 
UT_SetRtn_t	SBSendMsgRtn;
UT_SetRtn_t	FSSetTimestampRtn;
UT_SetRtn_t FSWriteHdrRtn;
UT_SetRtn_t FSReadHdrRtn;
UT_SetRtn_t CreatePoolRtn;
UT_SetRtn_t PoolCreateExRtn;
UT_SetRtn_t PutPoolRtn;
UT_SetRtn_t GetPoolRtn;
UT_SetRtn_t GetPoolInfoRtn;
UT_SetRtn_t BSPWriteCDSRtn;
UT_SetRtn_t BSPReadCDSRtn;
UT_SetRtn_t OS_BinSemCreateRtn;
UT_SetRtn_t ES_DeleteCDSRtn;
UT_SetRtn_t ES_RegisterCDSRtn;
UT_SetRtn_t ES_CopyToCDSRtn;
UT_SetRtn_t ES_RestoreFromCDSRtn;
UT_SetRtn_t BlocksFreeRtn;
UT_SetRtn_t UnmountRtn;
UT_SetRtn_t RmfsRtn;
UT_SetRtn_t BSPUnloadAppFileRtn;
UT_SetRtn_t GetAppIDRtn;
UT_SetRtn_t BSPGetCFETextRtn;
UT_SetRtn_t FSIsGzFileRtn;
UT_SetRtn_t FSDecompressRtn;
UT_SetRtn_t FSExtractRtn;
UT_SetRtn_t HeapGetInfoRtn;

boolean UT_CDS_GoodEnd = TRUE;
boolean UT_BSPCheckValidity = FALSE;
boolean UT_CDSReadBlock = FALSE;
uint16 UT_EventHistory[UT_EVENT_HISTORY_SIZE];
uint32 UT_EventHistoryCtr = 0;

UT_SetRtn_t ES_RegisterRtn;
UT_SetRtn_t ES_CreateChildRtn;

UT_SetRtn_t  SB_SubscribeRtn;
UT_SetRtn_t  SB_SubscribeLocalRtn;
UT_SetRtn_t  SB_SubscribeExRtn;
UT_SetRtn_t  SB_CreatePipeRtn;
UT_SetRtn_t  EVS_SendEventRtn;
UT_SetRtn_t  EVS_RegisterRtn;
UT_SetRtn_t OSlseekRtn; 
UT_SetRtn_t CountSemDelRtn;
UT_SetRtn_t MutSemDelRtn;
UT_SetRtn_t BinSemDelRtn;
UT_SetRtn_t SBCleanUpRtn;
UT_SetRtn_t EVSCleanUpRtn;

UT_SetRtn_t ModuleLoadRtn;
UT_SetRtn_t ModuleUnloadRtn;
UT_SetRtn_t ModuleInfoRtn;
UT_SetRtn_t SymbolLookupRtn;

UT_SetRtn_t TIMECleanUpRtn;


OS_time_t   BSP_Time;

extern char UT_ReadBuf [100000];
extern char UT_ReadHdr [10000]; /*new*/
extern int  UT_DummyFuncRtn;
extern int UT_BSPLoadGoodFile;
extern int UT_ReadBufOffset;
extern int UT_ReadHdrOffset;
extern int UT_CDS_Size;
extern int UT_SB_TotalMsgLen;
extern boolean UT_CDS_GoodEnd;
CFE_ES_CDSBlockDesc_t UT_CDSBlock;



int32 UT_StatusBSP = OS_SUCCESS;

CFE_EVS_GlobalData_t CFE_EVS_GlobalData;

CFE_ES_ResetData_t   UT_CFE_ES_ResetData;
CFE_ES_ResetData_t * UT_CFE_ES_ResetDataPtr;

/*
** File Data
*/

static int UT_failures = 0;
static int UT_passed = 0;

/*
** Function definitions
*/

/* Call before using any of the following routines */
/* Sets up log file */
void UT_Init (char *appname_in)
{
  char filename[90];

  /* Copy the application name for later use */
  strncpy(UT_appname,appname_in,80);;

#ifdef UT_LOGDIR
  sprintf(filename, "%s%s_log.txt", UT_LOGDIR, UT_appname);
#else
  sprintf(filename, "%s_log.txt", UT_appname);
#endif
  UT_logfile = fopen(filename, "w");

  UT_InitData();
}

void UT_InitData(void)
{
  int  i;
  
  /* Initialize Queue */
  for (i = 0; i < OS_MAX_QUEUES; i++) UT_Queue[i].free = TRUE;
  for (i = 0; i < UT_EVENT_HISTORY_SIZE; i++) UT_EventHistory[i] = 0;

  /* Initialize counters used for forcing return values from stubs */
  CreatePoolRtn.count = 0;
  PoolCreateExRtn.count = 0;
  PutPoolRtn.count  = 0;
  GetPoolRtn.count  = 0;
  QueuePutRtn.count = 0;
  MutSemGiveRtn.count = 0;
  MutSemTakeRtn.count = 0;
  QueueCreateRtn.count = 0;
  BSPWriteCDSRtn.count = 0;
  BSPReadCDSRtn.count = 0;
  QueueDelRtn.count = 0;
  QueueGetRtn.count = 0;
  QueuePutRtn.count = 0;
  FileWriteRtn.count = 0;
  OSReadRtn.count = 0;
  OSReadRtn2.count = 0;
  SBSendMsgRtn.count = 0;
  FSSetTimestampRtn.count = 0;
  FSWriteHdrRtn.count = 0;
  FSReadHdrRtn.count = 0;
  ES_RegisterRtn.count = 0;
  ES_CreateChildRtn.count = 0;
  SB_SubscribeRtn.count = 0;
  SB_SubscribeLocalRtn.count = 0;
  SB_CreatePipeRtn.count = 0;
  OS_BinSemCreateRtn.count = 0;
  EVS_SendEventRtn.count = 0;
  EVS_RegisterRtn.count = 0;
  SB_SubscribeExRtn.count =0;
  MutSemCreateRtn.count = 0;
  OSlseekRtn.count =0; 
  CountSemDelRtn.count = 0;
  MutSemDelRtn.count = 0;
  BinSemDelRtn.count =0;
  BlocksFreeRtn.count = 0;
  UnmountRtn.count = 0;
  RmfsRtn.count = 0;
  BSPUnloadAppFileRtn.count = 0;
  SBCleanUpRtn.count = 0;
  EVSCleanUpRtn.count = 0;
  GetAppIDRtn.count = 0;
  BSPGetCFETextRtn.count = 0;
  FSIsGzFileRtn.count =0;
  ModuleLoadRtn.count = 0;
  ModuleUnloadRtn.count = 0;
  ModuleInfoRtn.count = 0;
  FSDecompressRtn.count = 0;
  FSExtractRtn.count = 0;
  SymbolLookupRtn.count = 0;
  TIMECleanUpRtn.count = 0;
  HeapGetInfoRtn.count = 0;
  GetPoolInfoRtn.count = 0;
  
  UT_ResetCDS();
}


void UT_Report (boolean test, char *fun_name, char *info)
{
  if (test) {
    fprintf(UT_logfile," PASSED  %s - %s\n", fun_name, info);
    UT_passed++;
  } else {
    fprintf(UT_logfile," FAILED  %s - %s\n", fun_name, info);
    UT_failures++;
  }
}

void UT_ReportFailures (void)
{
   char message [80];


#if 0
   if (UT_failures == 0){
     sprintf(message,"\n%s PASSED all tests.\n\n", UT_appname);
   } else {
     sprintf(message,"\n%s FAILED %d test(s).\n\n", UT_appname, UT_failures);
   }
#endif

  sprintf(message,"\n%s PASSED %d tests.", UT_appname, UT_passed);
  printf(message);
  fprintf(UT_logfile, message);

  sprintf(message,"\n%s FAILED %d test(s).\n\n", UT_appname, UT_failures);
  printf(message);
  fprintf(UT_logfile, message);

   /* Ensure everything gets written */
   fflush(stdout);
   fflush(UT_logfile);
}

/* Simplify sending a message */
void UT_SendMsg (CFE_SB_MsgPtr_t msg_ptr, CFE_SB_MsgId_t id, uint16 code)
{
  CFE_SB_SetMsgId(msg_ptr, id);
  CFE_SB_SetCmdCode(msg_ptr, code);
  CFE_SB_SendMsg(msg_ptr);
}

void UT_SetAppID (int32 AppID_in)
{
  UT_AppID = AppID_in;
}

void UT_SetOSFail (uint32 fail)
{
  UT_OS_Fail = fail;
}

void UT_SetPutPoolFail (uint32 fail)
{
  UT_PutPool_Fail = fail;
}

void UT_SetRtnCode (UT_SetRtn_t *varPtr,int32 rtnVal,int32 cnt)
{
  varPtr->value = rtnVal;
  varPtr->count = cnt;
}  

/* For now assuming dataless command only */
void UT_SetRcvMsg (CFE_SB_MsgId_t id, uint16 code)
{
  UT_RcvMsgId = id;
  UT_RcvMsgCode = code;
}

void UT_SetStatusCDS (uint32 status)
{
  UT_StatusCDS = status;
}

void UT_SetSignatureCDS (uint32 sig)
{
  UT_SignatureCDS = sig;
}

void UT_SetLocTime (uint32 sec, uint32 msec)
{
  UT_LocTimeSec = sec;
  UT_LocTimeMSec = msec;
}

void UT_SetBinSemFail (uint32 fail)
{
  UT_BinSemFail = fail;
}

void UT_SetStatusBSPResetArea(int32 status, uint32 Signature)
{
    UT_CFE_ES_ResetData.TimeResetVars.Signature = Signature;
    UT_CFE_ES_ResetData.TimeResetVars.ClockSignal = CFE_TIME_TONE_PRI;

    UT_StatusBSP = status;
}


void UT_SetReadBuffer(void *Buff,int NumBytes)
{
    UT_ReadBufOffset = 0;
    memcpy(&UT_ReadBuf, Buff, NumBytes);
}

void UT_SetReadHeader(void *Hdr, int NumBytes)  /*new*/
{
	UT_ReadHdrOffset = 0;
	memcpy(&UT_ReadHdr, Hdr, NumBytes);
}

void UT_SetDummyFuncRtn(int Return)
{
    UT_DummyFuncRtn = Return;
}

void UT_SetBSPloadAppFileResult (int Result)
{
    UT_BSPLoadGoodFile= Result;
}
void UT_SetSizeofESResetArea(int32 Size)
{
    UT_SizeofESResetArea = Size;
}

void UT_SetBSPFail(uint32 Fail)
{
    UT_BSP_Fail = Fail;
}
void UT_SetCDSSize(int32 Size)
{
    UT_CDS_Size = Size;
}
void UT_SetSBTotalMsgLen(int Size)
{
    UT_SB_TotalMsgLen = Size;
}
void UT_SetCDSReadGoodEnd(boolean Truth)
{
    UT_CDS_GoodEnd = Truth;
}

void UT_SetCDSBSPCheckValidity(boolean Truth)
{
    UT_BSPCheckValidity = Truth;
}

void UT_SetBSP_Time(int seconds, int microsecs)
{
    BSP_Time.seconds = seconds;
    BSP_Time.microsecs = microsecs;
}   

void UT_ClearEventHistory(void)
{
    uint32 i=0;
    
    for (i=0; i<UT_EVENT_HISTORY_SIZE; i++)
    {
        UT_EventHistory[i] = 0;
    }
    UT_EventHistoryCtr = 0;
}

void UT_AddEventToHistory(uint16 EventID)
{
    uint16 Index = UT_EventHistoryCtr;
    
    /* Wrap Index if too many events have been sent */
    Index = UT_EventHistoryCtr % UT_EVENT_HISTORY_SIZE;
    
    UT_EventHistory[Index] = EventID;
    UT_EventHistoryCtr++;
}

boolean UT_EventIsInHistory(uint16 EventIDToSearchFor)
{
    boolean Result = FALSE;
    uint32  i=0;
    
    for (i=0; ((i<UT_EVENT_HISTORY_SIZE) && (i<UT_EventHistoryCtr)); i++)
    {
        if (UT_EventHistory[i] == EventIDToSearchFor)
        {
            i = UT_EVENT_HISTORY_SIZE;
            Result = TRUE;
        }
    }
    
    return Result;
}

uint16 UT_GetNumEventsSent(void)
{
    return UT_EventHistoryCtr;
}

void UT_ResetCDS(void)
{
  ES_DeleteCDSRtn.count = 0;
  ES_RegisterCDSRtn.count = 0;
  ES_CopyToCDSRtn.count = 0;
  ES_RestoreFromCDSRtn.count = 0;

  memset(&UT_CDS[0], 0, sizeof(UT_CDS));
  memset(&UT_CDS_Map.NextHandle, 0, sizeof(UT_CDS_Map));
}


void UT_DisplayPkt(CFE_SB_MsgPtr_t ptr,uint32 size){

  /* currently works only on big endian machines */

  uint8 *BytePtr = (uint8 *)ptr;
  uint32 i;

  for(i=0;i<size;i++){
    fprintf(UT_logfile,"%02x ",*BytePtr);
    BytePtr++;
  }

  fprintf(UT_logfile,"\n");

}/* end UT_DisplayPkt */


int16 UT_GetActualPktLenField(CFE_SB_MsgPtr_t MsgPtr){

  int16 *Ptr16Bit;

  Ptr16Bit = (int16 *)MsgPtr;
  Ptr16Bit += 2;

  return *Ptr16Bit;

}/* end UT_GetActualPktLenField */


uint16 UT_GetActualCmdCodeField(CFE_SB_MsgPtr_t MsgPtr){

/* Cmd Code is the 7 LSBs of the 7th byte in the pkt */

  uint8 *Ptr8Bit;

  Ptr8Bit = (uint8 *)MsgPtr;

/* Used when running on little endian machines */
  Ptr8Bit += 7;

/* Used when running on big endian machines */
/*  Ptr8Bit += 6; */

  return (((uint16)*Ptr8Bit) & 0x007F);

}/* end UT_GetActualCmdCodeField */


void UT_CheckForOpenSockets(void)
{

  int i;
  int InUse = 0;

  for(i=0;i<OS_MAX_QUEUES;i++){

    if(UT_Queue[i].free == FALSE){
      InUse++;
      fprintf(UT_logfile,"UT_Queue[%d].%s left open. ID=%d\n",
                    i,UT_Queue[i].name,UT_Queue[i].id);

      fprintf(UT_logfile,"Closing socket ID %d, close returned  %d\n\n",
                          UT_Queue[i].id, close(UT_Queue[i].id));

      /* clean up same as OS_QueueDelete stub */
      UT_Queue[i].free = TRUE;
      strcpy(UT_Queue[i].name, "");
      UT_Queue[i].id = 0;

    }/* end if */

  }/* end for */

  if(InUse > 0){
    printf("Sockets left open!!!!!!!!! See output file\n");
    fprintf(UT_logfile,"%d socket(s) open\n",InUse);
  }

}/* end UT_CheckForOpenSockets */




