/*
** File:
**    sb_UT.c
**
** Purpose:
**    SB unit test
**
** References:
**    1. cFE Application Developers Guide
**    2. unit test standard 092503
**    3. C Coding Standard 102904
**
** Notes:
**    1. This is unit test code only, not for use in flight
**
** $Date: 2010/11/04 14:24:03EDT $
** $Revision: 1.10 $
**
*/

/*
** Includes
*/
#include <stdio.h>
#include <string.h>
#include "cfe.h"
#include "cfe_sb_events.h"
#include "../sb/cfe_sb_priv.h"
#include "osapi.h"
#include "common_types.h"
#include "ut_stubs.h"


/*
** Function Definitions
*/


void SB_ResetUnitTest(void);

void Test_SB_App(void);
void Test_SB_AppInit(void);
void Test_SB_AppInit_ESRegFail(void);
void Test_SB_AppInit_EVSRegFail(void);
void Test_SB_AppInit_EVSSendEvtFail(void);
void Test_SB_AppInit_CrPipeFail(void);
void Test_SB_AppInit_Sub1Fail(void);
void Test_SB_AppInit_Sub2Fail(void);
void Test_SB_AppInit_GetPoolFail(void);
void Test_SB_AppInit_PutPoolFail(void);

void Test_SB_MainRoutine(void);
void Test_SB_Main_RcvErr(void);
void Test_SB_Main_InitErr(void);
 
void Test_SB_Cmds(void);
void Test_SB_Cmds_Noop(void);
void Test_SB_Cmds_RstCtrs(void);
void Test_SB_Cmds_Stats(void);
void Test_SB_Cmds_RoutingInfoDef(void);
void Test_SB_Cmds_RoutingInfoSpec(void);
void Test_SB_Cmds_RoutingInfoCreateFail(void);
void Test_SB_Cmds_RoutingInfoHdrFail(void);
void Test_SB_Cmds_RoutingInfoWriteFail(void);
void Test_SB_Cmds_PipeInfoDef(void);
void Test_SB_Cmds_PipeInfoSpec(void);
void Test_SB_Cmds_PipeInfoCreateFail(void);
void Test_SB_Cmds_PipeInfoHdrFail(void);
void Test_SB_Cmds_PipeInfoWriteFail(void);
void Test_SB_Cmds_MapInfoDef(void);
void Test_SB_Cmds_MapInfoSpec(void);
void Test_SB_Cmds_MapInfoCreateFail(void);
void Test_SB_Cmds_MapInfoHdrFail(void);
void Test_SB_Cmds_MapInfoWriteFail(void);
void Test_SB_Cmds_EnRouteValParam(void);
void Test_SB_Cmds_EnRouteNonExist(void);
void Test_SB_Cmds_EnRouteInvParam(void);
void Test_SB_Cmds_EnRouteInvParam2(void);
void Test_SB_Cmds_EnRouteInvParam3(void);
void Test_SB_Cmds_DisRouteValParam(void);
void Test_SB_Cmds_DisRouteNonExist(void);
void Test_SB_Cmds_DisRouteInvParam(void);
void Test_SB_Cmds_DisRouteInvParam2(void);
void Test_SB_Cmds_DisRouteInvParam3(void);
void Test_SB_Cmds_SendHK(void);
void Test_SB_Cmds_SendPrevSubs(void);
void Test_SB_Cmds_SubRptOn(void);
void Test_SB_Cmds_SubRptOff(void);
void Test_SB_Cmds_UnexpCmdCode(void);
void Test_SB_Cmds_UnexpMsgId(void);


void Test_SB_Lib(void);
void Test_SB_EarlyInit(void);
void Test_SB_EarlyInit_SemCreateError(void);
void Test_SB_EarlyInit_PoolCreateError(void);
void Test_SB_EarlyInit_NoErrors(void);

void Test_SB_Utils(void);
void Test_CFE_SB_InitMsg(void);
void Test_CFE_SB_InitMsg_True(void);
void Test_CFE_SB_InitMsg_False(void);
void Test_CFE_SB_MsgHdrSize(void);
void Test_CFE_SB_GetUserData(void);
void Test_CFE_SB_SetGetMsgId(void);
void Test_CFE_SB_SetGetUserDataLength(void);
void Test_CFE_SB_SetGetTotalMsgLength(void);
void Test_CFE_SB_SetGetMsgTime(void);
void Test_CFE_SB_TimeStampMsg(void);
void Test_CFE_SB_SetGetCmdCode(void);
void Test_CFE_SB_ChecksumUtils(void);

void Test_SB_APIs(void);

void Test_CreatePipe_API(void);
void Test_CreatePipe_NullPtr(void);
void Test_CreatePipe_ValPipeDepth(void);
void Test_CreatePipe_InvalPipeDepth(void);
void Test_CreatePipe_EmptyPipeName(void);
void Test_CreatePipe_LongPipeName(void);
void Test_CreatePipe_SamePipeName(void);
void Test_CreatePipe_MaxPipes(void);

void Test_DeletePipe_API(void);
void Test_DeletePipe_NoSubs(void);
void Test_DeletePipe_WithSubs(void);
void Test_DeletePipe_InvalidPipeId(void);
void Test_DeletePipe_InvalidPipeOwner(void);
void Test_DeletePipe_WithAppid(void);

void Test_Subscribe_API(void);
void Test_Subscribe_SubscribeEx(void);
void Test_Subscribe_InvalidPipeId(void);
void Test_Subscribe_InvalidMsgId(void);
void Test_Subscribe_MaxMsgLim(void);
void Test_Subscribe_DuplicateSubscription(void);
void Test_Subscribe_LocalSubscription(void);
void Test_Subscribe_MaxDestCount(void);
void Test_Subscribe_MaxMsgIdCount(void);
void Test_Subscribe_SendPrevSubs(void);
void Test_Subscribe_FindGlobalMsgIdCnt(void);
void Test_Subscribe_PipeNonexistent(void);
void Test_Subscribe_SubscriptionReporting(void);
void Test_Subscribe_InvalidPipeOwner(void);


void Test_Unsubscribe_API(void);
void Test_Unsubscribe_Basic(void);
void Test_Unsubscribe_Local(void);
void Test_Unsubscribe_InvalParam(void);
void Test_Unsubscribe_NoMatch(void);
void Test_Unsubscribe_SubscriptionReporting(void);
void Test_Unsubscribe_InvalidPipe(void);
void Test_Unsubscribe_InvalidPipeOwner(void);
void Test_Unsubscribe_FirstDestWithMany(void);
void Test_Unsubscribe_MiddleDestWithMany(void);
void Test_Unsubscribe_GetDestPtr(void);

void Test_SendMsg_API(void);
void Test_SendMsg_NullPtr(void);
void Test_SendMsg_InvalidMsgId(void);
void Test_SendMsg_NoSubscribers(void);
void Test_SendMsg_MaxMsgSizePlusOne(void);
void Test_SendMsg_BasicSend(void);
void Test_SendMsg_SequenceCount(void);
void Test_SendMsg_QueuePutError(void);
void Test_SendMsg_PipeFull(void);
void Test_SendMsg_MsgIdToPipeLimitExceeded(void);
void Test_SendMsg_GetPoolBufErr(void);
void Test_SendMsg_GetPoolCallErr(void);
void Test_SendMsg_ZeroCopyGetPtr(void);
void Test_SendMsg_ZeroCopySend(void);
void Test_SendMsg_ZeroCopyPass(void);
void Test_SendMsg_ZeroCopyReleasePtr(void);
void Test_SendMsg_DisabledDestination(void);

void Test_RcvMsg_API(void);
void Test_RcvMsg_InvalidPipeId(void);
void Test_RcvMsg_InvalidTimeout(void);
void Test_RcvMsg_Poll(void);
void Test_RcvMsg_GetLastSenderNull(void);
void Test_RcvMsg_GetLastSenderInvalidPipe(void);
void Test_RcvMsg_GetLastSenderInvalidCaller(void);
void Test_RcvMsg_GetLastSenderSuccess(void);
void Test_RcvMsg_Timeout(void);
void Test_RcvMsg_PipeReadError(void);
void Test_RcvMsg_PendForever(void);
void Test_RcvMsg_MsgLimCntrErr(void);

void Test_CleanupApp_API(void);

void Test_SB_SpecialCases(void);
void Test_OS_MutSem_ErrLogic(void);
void Test_GetPipeName_ErrLogic(void);
void Test_ReqToSendEvent_ErrLogic(void);
void Test_PutDestBlk_ErrLogic(void);



/*
** Externally declared variables
*/
extern FILE             *UT_logfile;
extern cfe_sb_t         CFE_SB;
extern UT_SetRtn_t      QueueCreateRtn;
extern UT_SetRtn_t      QueuePutRtn;
extern UT_SetRtn_t      QueueGetRtn;
extern UT_SetRtn_t      PoolCreateExRtn;
extern UT_SetRtn_t      GetPoolRtn;
extern UT_SetRtn_t      GetPoolInfoRtn;
extern UT_SetRtn_t      PutPoolRtn;
extern UT_SetRtn_t      MutSemCreateRtn;
extern UT_SetRtn_t      MutSemGiveRtn;
extern UT_SetRtn_t      MutSemTakeRtn;
extern UT_SetRtn_t      ES_RegisterRtn;
extern UT_SetRtn_t      EVS_RegisterRtn;
extern UT_SetRtn_t      EVS_SendEventRtn;
extern UT_SetRtn_t      FSWriteHdrRtn;
extern UT_SetRtn_t      FileWriteRtn;
extern UT_Queue_t       UT_Queue[OS_MAX_QUEUES];


/*
** Macros/Defines
*/
#define CFE_FAIL 0
#define CFE_PASS 1


/*
** Structures
*/
typedef struct {
   CFE_SB_CmdHdr_t      Hdr; /* 8 bytes */
   uint32               Cmd32Param1;
   uint16               Cmd16Param1;
   uint16               Cmd16Param2;
   uint8                Cmd8Param1;
   uint8                Cmd8Param2;
   uint8                Cmd8Param3;
   uint8                Cmd8Param4;
}SB_UT_Test_Cmd_t;

typedef struct {
   CFE_SB_TlmHdr_t      Hdr; /* 12 bytes */
   uint32               Tlm32Param1;
   uint16               Tlm16Param1;
   uint16               Tlm16Param2;
   uint8                Tlm8Param1;
   uint8                Tlm8Param2;
   uint8                Tlm8Param3;
   uint8                Tlm8Param4;
}SB_UT_Test_Tlm_t;

typedef struct {
   CFE_SB_Msg_t         Pri; /* 6 bytes */
   uint8                Tlm8Param1;
   uint8                Tlm8Param2;
   uint32               Tlm32Param1;
   uint16               Tlm16Param1;
   uint16               Tlm16Param2;
}SB_UT_TstPktWoSecHdr_t;


/*
** sb_UT Main Function
*/
int main (void)
{
  /* Initialize UnitTest */
  UT_Init("ut_cfe_sb");
  fprintf(UT_logfile,"CFE SB Unit Test Output File\n\n");

  Test_SB_App();
  Test_SB_Lib();
  Test_SB_SpecialCases();
  UT_CheckForOpenSockets();
  UT_ReportFailures();

  exit(0);

}/* end main */


void SB_ResetUnitTest(void){

  /* if any sockets where left open...report and close them */
  UT_CheckForOpenSockets();
  UT_InitData();
  UT_ClearEventHistory();
  
  CFE_SB_EarlyInit();

}/* end if */








void Test_SB_App(void){

  Test_SB_AppInit();
  Test_SB_MainRoutine();
  Test_SB_Cmds();

}/* end Test_SB_App */


void Test_SB_AppInit(void){

  fprintf(UT_logfile, "\nBegin Test App Init\n");

  Test_SB_AppInit_ESRegFail();
  Test_SB_AppInit_EVSRegFail();
  Test_SB_AppInit_EVSSendEvtFail();
  Test_SB_AppInit_CrPipeFail();
  Test_SB_AppInit_Sub1Fail();
  Test_SB_AppInit_Sub2Fail();    
  Test_SB_AppInit_GetPoolFail();
  Test_SB_AppInit_PutPoolFail();
  
  fprintf(UT_logfile, "End Test App Init\n\n");

}/* end Test_SB_AppInit */



void Test_SB_AppInit_ESRegFail(void){

  int32 ExpRtn,ActRtn,TestStat,ForcedRtnVal;

  /* Task Init with ES_RegisterApp returning error */
  fprintf(UT_logfile,"Start of ES_RegisterApp failure test\n");
  SB_ResetUnitTest();  
  TestStat = CFE_PASS;
  ForcedRtnVal = -1;
  UT_SetRtnCode(&ES_RegisterRtn, ForcedRtnVal, 1);
  ExpRtn = ForcedRtnVal;
  ActRtn = CFE_SB_AppInit();
  if(ActRtn != ForcedRtnVal){
    fprintf(UT_logfile, "Unexpected return from AppInit in ES Reg Error Test,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  ExpRtn = 0;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat,"CFE_SB_AppInit","ES_RegisterApp Failure\n");  

}/* end Test_SB_AppInit_ESRegFail */
  


void Test_SB_AppInit_EVSRegFail(void){

  int32 ExpRtn,ActRtn,TestStat,ForcedRtnVal;

  /* Task Init with EVS_Register returning error */
  fprintf(UT_logfile,"Start of EVS_Register failure test\n");
  SB_ResetUnitTest();
  TestStat = CFE_PASS;
  ForcedRtnVal = -1;
  UT_SetRtnCode( &EVS_RegisterRtn, ForcedRtnVal, 1);
  ActRtn = CFE_SB_AppInit(); 
  if(ActRtn != ForcedRtnVal){
    fprintf(UT_logfile, "Unexpected return from AppInit in EVS Reg Error Test,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  } 
  
  ExpRtn = 0;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
    
  SB_ResetUnitTest();
  UT_Report(TestStat,"CFE_SB_AppInit","EVS Register Failure\n");

}/* end Test_SB_AppInit_EVSRegFail */



void Test_SB_AppInit_EVSSendEvtFail(void){

  int32 ExpRtn,ActRtn,TestStat,ForcedRtnVal;

  /* Task Init with EVS_SendEvent returning error on task init event*/
  fprintf(UT_logfile,"Start of EVS_SendEvent failure test\n");
  SB_ResetUnitTest();
  TestStat = CFE_PASS;
  ForcedRtnVal = -1;
  UT_SetRtnCode( &EVS_SendEventRtn, ForcedRtnVal, 4);/* 4 because 1st 3 events are: 
      pipe created, and 2 subscription rcvd. 4th is SB initialized. */
  ActRtn = CFE_SB_AppInit();
  if(ActRtn != ForcedRtnVal){
    fprintf(UT_logfile, "Unexpected return from AppInit in EVSSendEvtFail Test,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(CFE_SB.CmdPipe);
  SB_ResetUnitTest();
  UT_Report(TestStat,"CFE_SB_AppInit","EVS SendEvent Failure\n");
  
}/* end Test_SB_AppInit_EVSSendEvtFail */




void Test_SB_AppInit_CrPipeFail(void){
  
  int32 ExpRtn,ActRtn,TestStat;

  /* To fail the pripe create, we must call the SB_AppInit function twice */
  /* The first call to SB_AppInit should pass and the second call should fail */
  /* because it is an error to use the same pipe name when creating pipes */
  fprintf(UT_logfile,"Start of SB_CreatePipe failure test\n");
  SB_ResetUnitTest();
  TestStat = CFE_PASS;
  ExpRtn = CFE_SUCCESS;
  ActRtn = CFE_SB_AppInit();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from CFE_SB_AppInit in CreatePipe Error1 Test,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = CFE_SB_PIPE_CR_ERR;
  ActRtn = CFE_SB_AppInit();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from CFE_SB_AppInit in CreatePipe Error2 Test,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  ExpRtn = 5;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_CR_PIPE_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_CR_PIPE_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(0);
  SB_ResetUnitTest();  
  UT_Report(TestStat,"CFE_SB_AppInit","Create Pipe Failure\n");
  
}/* end Test_SB_AppInit_CrPipeFail */
  

void Test_SB_AppInit_Sub1Fail(void){
  
  int32 ExpRtn,ActRtn,TestStat;

  fprintf(UT_logfile,"Start of Subscription 1 failure test\n");
  SB_ResetUnitTest();
  TestStat = CFE_PASS;
  UT_SetRtnCode(&GetPoolRtn, -1, 1);
  ExpRtn = CFE_SB_BUF_ALOC_ERR;
  ActRtn = CFE_SB_AppInit();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from CFE_SB_AppInit in Sub1 Test,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_DEST_BLK_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_DEST_BLK_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(0);
  SB_ResetUnitTest();  
  UT_Report(TestStat,"CFE_SB_AppInit","Sub1 Failure\n");

}/* end Test_SB_AppInit_Sub1Fail */


void Test_SB_AppInit_Sub2Fail(void){
  
  int32 ExpRtn,ActRtn,TestStat;

  fprintf(UT_logfile,"Start of Subscription 2 failure test\n");
  SB_ResetUnitTest();
  TestStat = CFE_PASS;
  UT_SetRtnCode(&GetPoolRtn, -1, 2);
  ExpRtn = CFE_SB_BUF_ALOC_ERR;
  ActRtn = CFE_SB_AppInit();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from CFE_SB_AppInit in Sub2 Test,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_DEST_BLK_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_DEST_BLK_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }  
  
  CFE_SB_DeletePipe(0);
  SB_ResetUnitTest();  
  UT_Report(TestStat,"CFE_SB_AppInit","Sub1 Failure\n");

}/* end Test_SB_AppInit_Sub2Fail */


void Test_SB_AppInit_GetPoolFail(void){
  
  int32 ExpRtn,ActRtn,TestStat, ForcedRtnVal;

  fprintf(UT_logfile,"Start of GetPool failure test\n");
  SB_ResetUnitTest();
  TestStat = CFE_PASS;
  ForcedRtnVal = -1;
  UT_SetRtnCode( &GetPoolRtn, ForcedRtnVal, 3);
  ExpRtn = ForcedRtnVal;
  ActRtn = CFE_SB_AppInit();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from CFE_SB_AppInit in GetPool Test,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(0);
  SB_ResetUnitTest();  
  UT_Report(TestStat,"CFE_SB_AppInit","GetPool Failure\n");

}/* end Test_SB_AppInit_GetPoolFail */



void Test_SB_AppInit_PutPoolFail(void){
  
  int32 ExpRtn,ActRtn,TestStat, ForcedRtnVal;

  fprintf(UT_logfile,"Start of PutPool failure test\n");
  SB_ResetUnitTest();
  TestStat = CFE_PASS;
  ForcedRtnVal = -1;
  UT_SetRtnCode( &PutPoolRtn, ForcedRtnVal, 1);
  ExpRtn = ForcedRtnVal;
  ActRtn = CFE_SB_AppInit();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from CFE_SB_AppInit in PutPool Test,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(0);
  SB_ResetUnitTest();  
  UT_Report(TestStat,"CFE_SB_AppInit","PutPool Failure\n");


}/* end Test_SB_AppInit_PutPoolFail */


void Test_SB_MainRoutine(void){
 
  fprintf(UT_logfile, "\nBegin Test for MainRoutine\n");
 
   Test_SB_Main_RcvErr();
   Test_SB_Main_InitErr();
   
   fprintf(UT_logfile, "End Test for MainRoutine\n\n"); 
  
}/* end Test_SB_MainRoutine */


void Test_SB_Main_RcvErr(void){
  
  int32 ExpRtn,ActRtn,TestStat = CFE_PASS;
  
  fprintf(UT_logfile,"Start of main loop, Rcv Error test\n");
  SB_ResetUnitTest();
  UT_SetRtnCode(&QueueGetRtn, -1, 1);
  CFE_SB_TaskMain();
  
  ExpRtn = 5;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_INIT_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_INIT_EID not sent\n");
    TestStat = CFE_FAIL;
  } 
  
  if(UT_EventIsInHistory(CFE_SB_Q_RD_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_Q_RD_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }   
  
  CFE_SB_DeletePipe(0);
  SB_ResetUnitTest();
  UT_Report(TestStat,"CFE_SB_TaskMain","Rcv Error Failure\n");
  
}/* end Test_SB_Main_RcvErr */


void Test_SB_Main_InitErr(void){
  
  int32 ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile,"Start of main loop, Init Error test\n");
  SB_ResetUnitTest();
  UT_SetRtnCode( &PutPoolRtn, -1, 1);
  CFE_SB_TaskMain();

  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(0);
  SB_ResetUnitTest();
  UT_Report(TestStat,"CFE_SB_TaskMain","Init Error Failure\n");
  
}/* end Test_SB_Main_InitErr */




void Test_SB_Cmds(void){
  
  fprintf(UT_logfile, "\nBegin Test for SB Cmds\n");
  
  Test_SB_Cmds_Noop();
  Test_SB_Cmds_RstCtrs();
  Test_SB_Cmds_Stats();
  Test_SB_Cmds_RoutingInfoDef();
  Test_SB_Cmds_RoutingInfoSpec();
  Test_SB_Cmds_RoutingInfoCreateFail();
  Test_SB_Cmds_RoutingInfoHdrFail();
  Test_SB_Cmds_RoutingInfoWriteFail();
  Test_SB_Cmds_PipeInfoDef();
  Test_SB_Cmds_PipeInfoSpec();
  Test_SB_Cmds_PipeInfoCreateFail();
  Test_SB_Cmds_PipeInfoHdrFail();
  Test_SB_Cmds_PipeInfoWriteFail();
  Test_SB_Cmds_MapInfoDef();
  Test_SB_Cmds_MapInfoSpec();
  Test_SB_Cmds_MapInfoCreateFail();
  Test_SB_Cmds_MapInfoHdrFail();
  Test_SB_Cmds_MapInfoWriteFail();
  Test_SB_Cmds_EnRouteValParam();
  Test_SB_Cmds_EnRouteNonExist();
  Test_SB_Cmds_EnRouteInvParam();
  Test_SB_Cmds_EnRouteInvParam2();
  Test_SB_Cmds_EnRouteInvParam3();
  Test_SB_Cmds_DisRouteValParam();
  Test_SB_Cmds_DisRouteNonExist();
  Test_SB_Cmds_DisRouteInvParam();
  Test_SB_Cmds_DisRouteInvParam2();
  Test_SB_Cmds_DisRouteInvParam3();
  Test_SB_Cmds_SendHK();
  Test_SB_Cmds_SendPrevSubs();
  Test_SB_Cmds_SubRptOn();
  Test_SB_Cmds_SubRptOff();
  Test_SB_Cmds_UnexpCmdCode();
  Test_SB_Cmds_UnexpMsgId();
  
  fprintf(UT_logfile, "End Test_Cmds\n\n");
  
}/* end Test_SB_Cmds */






void Test_SB_Cmds_Noop(void){

  CFE_SB_CmdHdr_t      NoParamCmd;
  int32                ExpRtn,ActRtn,TestStat;

  fprintf(UT_logfile, "Begin Test for Cmd - No-op\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&NoParamCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_CmdHdr_t),TRUE);
  
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd, CFE_SB_NOOP_CC);

  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&NoParamCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_CMD0_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_CMD0_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();

  UT_Report(TestStat, "Test_Cmds","No-op Cmd Test\n");

}/* end Test_SB_Cmds_Noop */




void Test_SB_Cmds_RstCtrs(void){

  CFE_SB_CmdHdr_t      NoParamCmd;
  int32                ExpRtn,ActRtn,TestStat= CFE_PASS;
                
  fprintf(UT_logfile, "Begin Test for Cmd - Reset Counters\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&NoParamCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_CmdHdr_t),TRUE);
  
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd, CFE_SB_RESET_CTRS_CC);

  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&NoParamCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_CMD1_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_CMD1_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  UT_Report(TestStat,"Test_Cmds","Reset Counters Cmd Test\n");
  
  SB_ResetUnitTest();

}/* Test_SB_Cmds_RstCtrs */


void Test_SB_Cmds_Stats(void){

  CFE_SB_CmdHdr_t      NoParamCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Send SB Stats\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&NoParamCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_CmdHdr_t),TRUE);

  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd, CFE_SB_SEND_SB_STATS_CC);

  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&NoParamCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SND_STATS_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SND_STATS_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SND_STATS_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SND_STATS_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Send SB Stats Cmd Test\n");

}/* end Test_SB_Cmds_Stats */  


void Test_SB_Cmds_RoutingInfoDef(void){
   
  CFE_SB_WriteFileInfoCmd_t WriteFileCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Routing Info\n");
  fprintf(UT_logfile, "Using Default Filename\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&WriteFileCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_WriteFileInfoCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd, CFE_SB_SEND_ROUTING_INFO_CC);
  WriteFileCmd.Filename[0] = '\0';

  /* make some routing info by calling CFE_SB_AppInit */
  ExpRtn = CFE_SUCCESS;
  ActRtn = CFE_SB_AppInit();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from CFE_SB_AppInit,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&WriteFileCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 5;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_INIT_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_CMD1_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_INIT_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_INIT_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SND_RTG_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SND_RTG_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(0);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Send Routing Info Cmd Test\n");

}/* end Test_SB_Cmds_RoutingInfoDef */


void Test_SB_Cmds_RoutingInfoSpec(void){
  
  CFE_SB_WriteFileInfoCmd_t WriteFileCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Routing Info2\n");
  fprintf(UT_logfile, "Using Specified Filename\n");  
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&WriteFileCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_WriteFileInfoCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd, CFE_SB_SEND_ROUTING_INFO_CC);
  strcpy(&WriteFileCmd.Filename[0],"RoutingTstFile");

  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&WriteFileCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SND_RTG_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SND_RTG_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Send Routing Info2 Cmd Test\n");

}/* end Test_SB_Cmds_RoutingInfoSpec */  


void Test_SB_Cmds_RoutingInfoCreateFail(void){
  
  CFE_SB_WriteFileInfoCmd_t WriteFileCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Routing Info3,File Create fails\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&WriteFileCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_WriteFileInfoCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd, CFE_SB_SEND_ROUTING_INFO_CC);
  strcpy(&WriteFileCmd.Filename[0],"RoutingTstFile");

  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&WriteFileCmd;
    
  /* make function CFE_SB_SendRtgInfo return CFE_SB_FILE_IO_ERR */
  UT_SetOSFail(OS_CREAT_FAIL); 
  CFE_SB_ProcessCmdPipePkt();
  UT_SetOSFail(0);
 
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SND_RTG_ERR1_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SND_RTG_ERR1_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  UT_Report(TestStat,"Test_Cmds","Send Routing Info3 Cmd Test\n");
  SB_ResetUnitTest();
  
}/* end Test_SB_Cmds_RoutingInfoCreateFail */  


void Test_SB_Cmds_RoutingInfoHdrFail(void){
  
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Routing Info4,FileHdrWrite fails\n");
  SB_ResetUnitTest();
  UT_SetRtnCode(&FSWriteHdrRtn,-1,1);
  ExpRtn = CFE_SB_FILE_IO_ERR;
  ActRtn = CFE_SB_SendRtgInfo("RoutingTstFile");
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from CFE_SB_SendRtgInfo,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
    
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_FILEWRITE_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_FILEWRITE_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  UT_Report(TestStat,"Test_Cmds","Send Routing Info4 Cmd Test\n");
  SB_ResetUnitTest();
            
}/* end Test_SB_Cmds_RoutingInfoHdrFail */  


void Test_SB_Cmds_RoutingInfoWriteFail(void){

  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Routing Info5,2nd FileWrite fails\n");
  SB_ResetUnitTest();
   /* make some routing info by calling CFE_SB_AppInit */
  ExpRtn = CFE_SUCCESS;
  ActRtn = CFE_SB_AppInit();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from CFE_SB_AppInit,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
 
  UT_SetRtnCode(&FileWriteRtn,-1,2);
  ExpRtn = CFE_SB_FILE_IO_ERR;
  ActRtn = CFE_SB_SendRtgInfo("RoutingTstFile");
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from CFE_SB_SendRtgInfo,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
    
  ExpRtn = 5;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_INIT_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_INIT_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_FILEWRITE_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_FILEWRITE_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(0);
  SB_ResetUnitTest();
  UT_Report(TestStat,"Test_Cmds","Send Routing Info5 Cmd Test\n");
   
}/* end Test_SB_Cmds_RoutingInfoWriteFail */  


void Test_SB_Cmds_PipeInfoDef(void){
  
  CFE_SB_WriteFileInfoCmd_t WriteFileCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t      PipeId1,PipeId2,PipeId3;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Pipe Info\n");
  fprintf(UT_logfile, "Using Default Filename\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&WriteFileCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_WriteFileInfoCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd, CFE_SB_SEND_PIPE_INFO_CC);
  WriteFileCmd.Filename[0] = '\0';

  /* create some pipe info */
  CFE_SB_CreatePipe(&PipeId1,10,"TstPipe1");
  CFE_SB_CreatePipe(&PipeId2,10,"TstPipe2");
  CFE_SB_CreatePipe(&PipeId3,10,"TstPipe3");


  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&WriteFileCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 4;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SND_RTG_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SND_RTG_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId1);
  CFE_SB_DeletePipe(PipeId2);
  CFE_SB_DeletePipe(PipeId3);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Send Pipe Info Cmd Test\n");

}/* end Test_SB_Cmds_PipeInfoDef */  


void Test_SB_Cmds_PipeInfoSpec(void){
  
  CFE_SB_WriteFileInfoCmd_t WriteFileCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Pipe Info2\n");
  fprintf(UT_logfile, "Using Specified Filename\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&WriteFileCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_WriteFileInfoCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd, CFE_SB_SEND_PIPE_INFO_CC);
  strcpy(&WriteFileCmd.Filename[0],"PipeTstFile");

  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&WriteFileCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SND_RTG_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SND_RTG_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Send Pipe Info2 Cmd Test\n");
  
}/* end Test_SB_Cmds_PipeInfoSpec */  


void Test_SB_Cmds_PipeInfoCreateFail(void){
  
   int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Pipe Info3,File Create fails\n");
  SB_ResetUnitTest();
  UT_SetOSFail(OS_CREAT_FAIL);
  ExpRtn = CFE_SB_FILE_IO_ERR;
  ActRtn = CFE_SB_SendPipeInfo("PipeTstFile");
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from CFE_SB_SendPipeInfo,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  UT_SetOSFail(0);
 
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SND_RTG_ERR1_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SND_RTG_ERR1_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  UT_Report(TestStat,"Test_Cmds","Send Pipe Info3 Cmd Test\n");
  SB_ResetUnitTest();

  
}/* end Test_SB_Cmds_PipeInfoCreateFail */  


void Test_SB_Cmds_PipeInfoHdrFail(void){
  
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Pipe Info4,FileHdrWrite fails\n");
  SB_ResetUnitTest();
  UT_SetRtnCode(&FSWriteHdrRtn,-1,1);
  ExpRtn = CFE_SB_FILE_IO_ERR;
  ActRtn = CFE_SB_SendPipeInfo("PipeTstFile");
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from CFE_SB_SendPipeInfo,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
    
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_FILEWRITE_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_FILEWRITE_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  UT_Report(TestStat,"Test_Cmds","Send Pipe Info4 Cmd Test\n");
  SB_ResetUnitTest();
  
}/* end Test_SB_Cmds_PipeInfoHdrFail */  


void Test_SB_Cmds_PipeInfoWriteFail(void){
  
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t      PipeId1,PipeId2,PipeId3;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Pipe Info5,2nd FileWrite fails\n");
  SB_ResetUnitTest();
  /* create some pipe info */
  CFE_SB_CreatePipe(&PipeId1,10,"TstPipe1");
  CFE_SB_CreatePipe(&PipeId2,10,"TstPipe2");
  CFE_SB_CreatePipe(&PipeId3,10,"TstPipe3");


  UT_SetRtnCode(&FileWriteRtn,-1,2);
  ExpRtn = CFE_SB_FILE_IO_ERR;
  ActRtn = CFE_SB_SendPipeInfo("PipeTstFile");
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from CFE_SB_SendPipeInfo,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
    
  ExpRtn = 4;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_FILEWRITE_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_FILEWRITE_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }  

  CFE_SB_DeletePipe(PipeId1);
  CFE_SB_DeletePipe(PipeId2);
  CFE_SB_DeletePipe(PipeId3);
  SB_ResetUnitTest();
  UT_Report(TestStat,"Test_Cmds","Send Pipe Info5 Cmd Test\n");

}/* end Test_SB_Cmds_PipeInfoWriteFail */  


void Test_SB_Cmds_MapInfoDef(void){
  
  CFE_SB_WriteFileInfoCmd_t WriteFileCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t      PipeId1,PipeId2,PipeId3;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Map Info\n");
  fprintf(UT_logfile, "Using Default Filename\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&WriteFileCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_WriteFileInfoCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd, CFE_SB_SEND_MAP_INFO_CC);
  WriteFileCmd.Filename[0] = '\0';

  /* Create some map info */
  CFE_SB_CreatePipe(&PipeId1,10,"TstPipe1");
  CFE_SB_CreatePipe(&PipeId2,10,"TstPipe2");
  CFE_SB_CreatePipe(&PipeId3,10,"TstPipe3");
  
  CFE_SB_Subscribe(0x0809,PipeId1);
  CFE_SB_Subscribe(0x0809,PipeId2);
  CFE_SB_Subscribe(0x080A,PipeId1);
  CFE_SB_Subscribe(0x080B,PipeId3);
  CFE_SB_Subscribe(0x080C,PipeId3);
  CFE_SB_Subscribe(0x080D,PipeId3);
  CFE_SB_Subscribe(0x080E,PipeId2);

  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&WriteFileCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 11;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SND_RTG_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SND_RTG_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId1);
  CFE_SB_DeletePipe(PipeId2);
  CFE_SB_DeletePipe(PipeId3);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Send Map Info Cmd Test\n");

}/* end Test_SB_Cmds_MapInfoDef */  


void Test_SB_Cmds_MapInfoSpec(void){

  CFE_SB_WriteFileInfoCmd_t WriteFileCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;
  
  fprintf(UT_logfile, "Begin Test for Cmd - Send Map Info2\n");
  fprintf(UT_logfile, "Using Specified Filename\n");  
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&WriteFileCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_WriteFileInfoCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &WriteFileCmd, CFE_SB_SEND_MAP_INFO_CC);
  strcpy(&WriteFileCmd.Filename[0],"MapTstFile");

  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&WriteFileCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SND_RTG_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SND_RTG_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Send Map Info2 Cmd Test\n");

}/* end Test_SB_Cmds_MapInfoSpec */  


void Test_SB_Cmds_MapInfoCreateFail(void){
  
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Map Info3,File Create fails\n");
  SB_ResetUnitTest();
  UT_SetOSFail(OS_CREAT_FAIL);
  ExpRtn = CFE_SB_FILE_IO_ERR;
  ActRtn = CFE_SB_SendMapInfo("MapTstFile");
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from CFE_SB_SendMapInfo,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  UT_SetOSFail(0);
 
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SND_RTG_ERR1_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SND_RTG_ERR1_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  UT_Report(TestStat,"Test_Cmds","Send Map Info3 Cmd Test\n");
  SB_ResetUnitTest();

}/* end Test_SB_Cmds_MapInfoCreateFail */  


void Test_SB_Cmds_MapInfoHdrFail(void){
  
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Map Info4,FileHdrWrite fails\n");
  SB_ResetUnitTest();
  UT_SetRtnCode(&FSWriteHdrRtn,-1,1);
  ExpRtn = CFE_SB_FILE_IO_ERR;
  ActRtn = CFE_SB_SendMapInfo("MapTstFile");
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from CFE_SB_SendMapInfo,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
    
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_FILEWRITE_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_FILEWRITE_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  UT_Report(TestStat,"Test_Cmds","Send Map Info4 Cmd Test\n");
  SB_ResetUnitTest();
  
}/* end Test_SB_Cmds_MapInfoHdrFail */  


void Test_SB_Cmds_MapInfoWriteFail(void){
  
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t      PipeId1,PipeId2,PipeId3;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Map Info5,2nd FileWrite fails\n");
  SB_ResetUnitTest();
    /* Create some map info */
  CFE_SB_CreatePipe(&PipeId1,10,"TstPipe1");
  CFE_SB_CreatePipe(&PipeId2,10,"TstPipe2");
  CFE_SB_CreatePipe(&PipeId3,10,"TstPipe3");
  
  CFE_SB_Subscribe(0x0809,PipeId1);
  CFE_SB_Subscribe(0x0809,PipeId2);
  CFE_SB_Subscribe(0x080A,PipeId1);
  CFE_SB_Subscribe(0x080B,PipeId3);
  CFE_SB_Subscribe(0x080C,PipeId3);
  CFE_SB_Subscribe(0x080D,PipeId3);
  CFE_SB_Subscribe(0x080E,PipeId2);

  UT_SetRtnCode(&FileWriteRtn,-1,2);
  ExpRtn = CFE_SB_FILE_IO_ERR;
  ActRtn = CFE_SB_SendMapInfo("MapTstFile");
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from CFE_SB_SendMapInfo,exp=%lx,act=%lx\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
    
  ExpRtn = 11;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_FILEWRITE_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_FILEWRITE_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId1);
  CFE_SB_DeletePipe(PipeId2);
  CFE_SB_DeletePipe(PipeId3);
  SB_ResetUnitTest();
  UT_Report(TestStat,"Test_Cmds","Send Map Info5 Cmd Test\n");

}/* end Test_SB_Cmds_MapInfoWriteFail */  


void Test_SB_Cmds_EnRouteValParam(void){
  
  CFE_SB_EnRoutCmd_t   EnDisRouteCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t      PipeId;
  uint16               PipeDepth = 5;

  fprintf(UT_logfile, "Begin Test for Cmd - Enable Route,Valid Param\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"EnRouteTstPipe");
  CFE_SB_Subscribe(0x0829,PipeId);

  CFE_SB_InitMsg(&EnDisRouteCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_EnRoutCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd, CFE_SB_ENABLE_ROUTE_CC);
  EnDisRouteCmd.MsgId = 0x0829;
  EnDisRouteCmd.Pipe = PipeId;

  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&EnDisRouteCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_ENBL_RTE2_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_ENBL_RTE2_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Enable Route,Valid Param Test\n");

}/* end Test_SB_Cmds_EnRouteValParam */



void Test_SB_Cmds_EnRouteNonExist(void){
  
  CFE_SB_EnRoutCmd_t   EnDisRouteCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t      PipeId1,PipeId2;
  uint16               PipeDepth = 5;

  fprintf(UT_logfile, "Begin Test for Cmd - Enable Route,Non Exist\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId1,PipeDepth,"EnRouteTstPipe1");
  CFE_SB_CreatePipe(&PipeId2,PipeDepth,"EnRouteTstPipe2");
  CFE_SB_Subscribe(0x0829,PipeId1);

  CFE_SB_InitMsg(&EnDisRouteCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_EnRoutCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd, CFE_SB_ENABLE_ROUTE_CC);
  EnDisRouteCmd.MsgId = 0x0829;
  EnDisRouteCmd.Pipe = PipeId2;

  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&EnDisRouteCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 4;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_ENBL_RTE1_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_ENBL_RTE1_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId1);
  CFE_SB_DeletePipe(PipeId2);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Enable Route,Non Exist Test\n");

}/* end Test_SB_Cmds_EnRouteNonExist */



void Test_SB_Cmds_EnRouteInvParam(void){
  
  CFE_SB_EnRoutCmd_t   EnDisRouteCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Enable Route,Invalid Param\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&EnDisRouteCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_EnRoutCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd, CFE_SB_ENABLE_ROUTE_CC);
  EnDisRouteCmd.MsgId = 0x1800;
  EnDisRouteCmd.Pipe = 3;
  
  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&EnDisRouteCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_ENBL_RTE3_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_ENBL_RTE3_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();  
  UT_Report(TestStat, "Test_Cmds","Enable Route,Invalid Param Test\n");
  
}/* end Test_SB_Cmds_EnRouteInvParam */  


void Test_SB_Cmds_EnRouteInvParam2(void){
  
  CFE_SB_EnRoutCmd_t   EnDisRouteCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Enable Route,Invalid Param2\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&EnDisRouteCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_EnRoutCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd, CFE_SB_ENABLE_ROUTE_CC);
  EnDisRouteCmd.MsgId = 0xFFFF;
  EnDisRouteCmd.Pipe = 3;
  
  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&EnDisRouteCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_ENBL_RTE3_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_ENBL_RTE3_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Enable Route,Invalid Param2 Test\n");
  
}/* end Test_SB_Cmds_EnRouteInvParam2 */  


void Test_SB_Cmds_EnRouteInvParam3(void){

  CFE_SB_EnRoutCmd_t   EnDisRouteCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Enable Route,Invalid Param3\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&EnDisRouteCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_EnRoutCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd, CFE_SB_ENABLE_ROUTE_CC);
  EnDisRouteCmd.MsgId = 0x1845;
  EnDisRouteCmd.Pipe = 0;
  
  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&EnDisRouteCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_ENBL_RTE3_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_ENBL_RTE3_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Enable Route,Invalid Param3 Test\n");
  
}/* end Test_SB_Cmds_EnRouteInvParam3 */  


void Test_SB_Cmds_DisRouteValParam(void){
  
  CFE_SB_EnRoutCmd_t   EnDisRouteCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t      PipeId;
  uint16               PipeDepth = 5;

  fprintf(UT_logfile, "Begin Test for Cmd - Disable Route,Valid Param\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"DisRouteTstPipe");
  CFE_SB_Subscribe(0x0829,PipeId);

  CFE_SB_InitMsg(&EnDisRouteCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_EnRoutCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd, CFE_SB_DISABLE_ROUTE_CC);
  EnDisRouteCmd.MsgId = 0x0829;
  EnDisRouteCmd.Pipe = PipeId;

  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&EnDisRouteCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_DSBL_RTE2_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_DSBL_RTE2_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Disable Route,Valid Param Test\n");

}/* end Test_SB_Cmds_DisRouteValParam */


void Test_SB_Cmds_DisRouteNonExist(void){
  
  CFE_SB_EnRoutCmd_t   EnDisRouteCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t      PipeId1,PipeId2;
  uint16               PipeDepth = 5;

  fprintf(UT_logfile, "Begin Test for Cmd - Disable Route,Non Exist\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId1,PipeDepth,"DisRouteTstPipe1");
  CFE_SB_CreatePipe(&PipeId2,PipeDepth,"DisRouteTstPipe2");
  CFE_SB_Subscribe(0x0829,PipeId1);

  CFE_SB_InitMsg(&EnDisRouteCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_EnRoutCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd, CFE_SB_DISABLE_ROUTE_CC);
  EnDisRouteCmd.MsgId = 0x0829;
  EnDisRouteCmd.Pipe = PipeId2;

  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&EnDisRouteCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 4;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_DSBL_RTE1_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_DSBL_RTE1_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId1);
  CFE_SB_DeletePipe(PipeId2);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Disable Route,Non Exist Test\n");

}/* end Test_SB_Cmds_DisRouteNonExist */


void Test_SB_Cmds_DisRouteInvParam(void){
  
  CFE_SB_EnRoutCmd_t   EnDisRouteCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Disable Route,Invalid Param\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&EnDisRouteCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_EnRoutCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd, CFE_SB_DISABLE_ROUTE_CC);
  EnDisRouteCmd.MsgId = 0x1800;
  EnDisRouteCmd.Pipe = 3;
  
  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&EnDisRouteCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_DSBL_RTE3_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_DSBL_RTE3_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Disable Route,Invalid Param Test\n");

}/* end Test_SB_Cmds_DisRouteInvParam */  


void Test_SB_Cmds_DisRouteInvParam2(void){
  
  CFE_SB_EnRoutCmd_t   EnDisRouteCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Disable Route,Invalid Param2\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&EnDisRouteCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_EnRoutCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd, CFE_SB_DISABLE_ROUTE_CC);
  EnDisRouteCmd.MsgId = 0xFFFF;
  EnDisRouteCmd.Pipe = 3;
  
  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&EnDisRouteCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_DSBL_RTE3_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_DSBL_RTE3_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Disable Route,Invalid Param2 Test\n");
  
}/* end Test_SB_Cmds_DisRouteInvParam2 */  


void Test_SB_Cmds_DisRouteInvParam3(void){
  
  CFE_SB_EnRoutCmd_t   EnDisRouteCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Disable Route,Invalid Param3\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&EnDisRouteCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_EnRoutCmd_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &EnDisRouteCmd, CFE_SB_DISABLE_ROUTE_CC);
  EnDisRouteCmd.MsgId = 0x1865;
  EnDisRouteCmd.Pipe = 0;
  
  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&EnDisRouteCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_DSBL_RTE3_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_DSBL_RTE3_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Disable Route,Invalid Param3 Test\n");

}/* end Test_SB_Cmds_DisRouteInvParam3 */    


void Test_SB_Cmds_SendHK(void){
  
  CFE_SB_CmdHdr_t      NoParamCmd;
  int32                ExpRtn,ActRtn,TestStat;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Housekeeping Info\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&NoParamCmd,CFE_SB_SEND_HK_MID,sizeof(CFE_SB_CmdHdr_t),TRUE);
  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&NoParamCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SEND_NO_SUBS_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SEND_NO_SUBS_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Send Housekeeping Info Cmd Test\n");
  
}/* end Test_SB_Cmds_SendHK */  


void Test_SB_Cmds_SendPrevSubs(void){
  
  CFE_SB_CmdHdr_t      NoParamCmd;
  CFE_SB_PipeId_t      PipeId, PipeId2;
  int32                i,ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Send Prev Subscriptions\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&NoParamCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_CmdHdr_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd, CFE_SB_SEND_PREV_SUBS_CC);
  
  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&NoParamCmd;

  CFE_SB_CreatePipe(&PipeId,50,"TestPipe");
  CFE_SB_CreatePipe(&PipeId2,50,"TestPipe2");

  for(i=0;i < 45;i++){ /* 45 allows 2 full pkts to be sent plus 7 entries in a partial pkt */
    
    ActRtn = CFE_SB_Subscribe(i,PipeId);
    ExpRtn = CFE_SUCCESS;

    if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return subscribing in SendPrevSubs Test,i=%ld,exp=%lx,act=%lx\n",
              i,ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
    }/* end if */

  }/* end for */
  
  CFE_SB_SubscribeLocal(3,PipeId2,4);

  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 54;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SEND_NO_SUBS_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SEND_NO_SUBS_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_FULL_SUB_PKT_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_FULL_SUB_PKT_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_PART_SUB_PKT_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PART_SUB_PKT_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(0);
  CFE_SB_DeletePipe(PipeId);
  CFE_SB_DeletePipe(PipeId2);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Send Prev Subscriptions Test\n");

}/* end Test_SB_Cmds_SendPrevSubs */  


void Test_SB_Cmds_SubRptOn(void){
  
  CFE_SB_CmdHdr_t      NoParamCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Subscription Reporting On\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&NoParamCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_CmdHdr_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd, CFE_SB_ENABLE_SUB_REPORTING_CC);
  
  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&NoParamCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 0;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Subscription Reporting On Test\n");

}/* end Test_SB_Cmds_SubRptOn */  


void Test_SB_Cmds_SubRptOff(void){
  
  CFE_SB_CmdHdr_t      NoParamCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Subscription Reporting Off\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&NoParamCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_CmdHdr_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd, CFE_SB_DISABLE_SUB_REPORTING_CC);
  
  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&NoParamCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 0;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Subscription Reporting Off Test\n");
  
}/* end Test_SB_Cmds_SubRptOff */  


void Test_SB_Cmds_UnexpCmdCode(void){
  
  CFE_SB_CmdHdr_t      NoParamCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Unexpected Cmd Code\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&NoParamCmd,CFE_SB_CMD_MID,sizeof(CFE_SB_CmdHdr_t),TRUE);
  CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t) &NoParamCmd, 13);
  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&NoParamCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_BAD_CMD_CODE_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_BAD_CMD_CODE_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Unexpected Cmd Code Test\n");

}/* end Test_SB_Cmds_UnexpCmdCode */  


void Test_SB_Cmds_UnexpMsgId(void){
  
  CFE_SB_CmdHdr_t      NoParamCmd;
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Cmd - Unexpected MsgId on SB Cmd Pipe\n");
  SB_ResetUnitTest();
  CFE_SB_InitMsg(&NoParamCmd,0x0865,sizeof(CFE_SB_CmdHdr_t),TRUE);
  CFE_SB.CmdPipePktPtr = (CFE_SB_MsgPtr_t)&NoParamCmd;
  CFE_SB_ProcessCmdPipePkt();
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_BAD_MSGID_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_BAD_MSGID_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Cmds","Unexpected MsgId on SB Cmd Pipe Test\n");
  
}/* end Test_SB_Cmds_UnexpMsgId */  

  












void Test_SB_Lib(void){

  Test_SB_EarlyInit();
  Test_SB_APIs();
  Test_SB_Utils();

}/* end Test_SB_Lib */



/*
** Early Initialization test (for APIs)
*/
void Test_SB_EarlyInit (void)
{
  fprintf(UT_logfile,"\nBegin Test EarlyInit\n");

  Test_SB_EarlyInit_SemCreateError();
  Test_SB_EarlyInit_PoolCreateError();
  Test_SB_EarlyInit_NoErrors();

  fprintf(UT_logfile,"End Test EarlyInit\n\n");

}/* end Test_SB_EarlyInit */


void Test_SB_EarlyInit_SemCreateError(void){

  /* make the sem create in EarlyInit fail */
  fprintf(UT_logfile,"\nBegin Sem Create Error Logic test\n");
  SB_ResetUnitTest();
  UT_SetRtnCode (&MutSemCreateRtn,OS_ERR_NO_FREE_IDS,1);
  UT_Report(CFE_SB_EarlyInit() == OS_ERR_NO_FREE_IDS,
            "CFE_SB_EarlyInit","Sem Create Error Logic");
  SB_ResetUnitTest();
  fprintf(UT_logfile,"End Sem Create Error Logic test\n");

}/* end Test_SB_EarlyInit_SemCreateError */



void Test_SB_EarlyInit_PoolCreateError(void){

  /* make the pool create ex in EarlyInit(inside CFE_SB_InitBuffers) fail */
  fprintf(UT_logfile,"\nBegin PoolCreateEx Error Logic test\n");
  SB_ResetUnitTest();
  UT_SetRtnCode (&PoolCreateExRtn,CFE_ES_BAD_ARGUMENT,1);
  UT_Report(CFE_SB_EarlyInit() == CFE_ES_BAD_ARGUMENT,
            "CFE_SB_EarlyInit","PoolCreateEx Error Logic");
  SB_ResetUnitTest();
  fprintf(UT_logfile,"End PoolCreateEx Error Logic test\n");

}/* end Test_SB_EarlyInit_PoolCreateError */



void Test_SB_EarlyInit_NoErrors(void){

  /* run through the 'no errors' path in Earlyinit */
  fprintf(UT_logfile,"\nBegin EarlyInit No Errors test\n");
  SB_ResetUnitTest();
  CFE_SB_EarlyInit();
  UT_Report(CFE_SB_EarlyInit() == CFE_SUCCESS,
            "CFE_SB_EarlyInit","No Errors test");
  SB_ResetUnitTest();
  fprintf(UT_logfile,"End EarlyInit No Errors test\n");

}/* end Test_SB_EarlyInit_NoErrors */




/*
** Test SB API's
*/
void Test_SB_APIs(void)
{
  Test_CreatePipe_API();
  Test_DeletePipe_API();
  Test_Subscribe_API();
  Test_Unsubscribe_API();
  Test_SendMsg_API();
  Test_RcvMsg_API();
  Test_CleanupApp_API();

  fprintf(UT_logfile,"\n");

}/* end Test_SB_APIs */


void Test_CreatePipe_API(void){

  fprintf(UT_logfile, "\n\n\nBegin Test_CreatePipe_API\n\n");

  Test_CreatePipe_NullPtr();
  Test_CreatePipe_ValPipeDepth();
  Test_CreatePipe_InvalPipeDepth();
  Test_CreatePipe_EmptyPipeName();
  Test_CreatePipe_LongPipeName();
  Test_CreatePipe_SamePipeName();
  Test_CreatePipe_MaxPipes();

  fprintf(UT_logfile, "End Test_CreatePipe_API\n");

}/* end Test_CreatePipe_API */


void Test_CreatePipe_NullPtr(void)
{

  int32         ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Null Ptr\n");
  SB_ResetUnitTest();

  ExpRtn = CFE_SB_BAD_ARGUMENT;
  UT_SetRtnCode (&QueueCreateRtn,OS_SUCCESS,1);/* to avoid creating socket */
  ActRtn = CFE_SB_CreatePipe(NULL,10,"Testpipe1");

  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Null Ptr Test,exp=%lx,act=%lx\n",
                                                  CFE_SB_BAD_ARGUMENT,ActRtn);
      TestStat = CFE_FAIL;
  }/* end if */

  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_CR_PIPE_BAD_ARG_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_CR_PIPE_BAD_ARG_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_CreatePipe_API","Null Ptr Test\n");

}/* end Test_CreatePipe_NullPtr */



void Test_CreatePipe_ValPipeDepth(void)
{

  CFE_SB_PipeId_t  PipeIdReturned[4];
  int32            Rtn[4];
  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for ValidPipeDepth\n");
  SB_ResetUnitTest();

  /* test a few valid pipe depth values */
  UT_SetRtnCode (&QueueCreateRtn,OS_SUCCESS,1);/* to avoid creating a socket */
  Rtn[1] = CFE_SB_CreatePipe(&PipeIdReturned[1],99,"TestPipe99");

  UT_SetRtnCode (&QueueCreateRtn,OS_SUCCESS,1);
  Rtn[2] = CFE_SB_CreatePipe(&PipeIdReturned[2],255,"TestPipe255");

  UT_SetRtnCode (&QueueCreateRtn,OS_SUCCESS,1);
  Rtn[3] = CFE_SB_CreatePipe(&PipeIdReturned[3],CFE_SB_MAX_PIPE_DEPTH,"TestPipeMaxDepth");

  if((Rtn[1] != CFE_SUCCESS)||(Rtn[2] != CFE_SUCCESS)||(Rtn[3] != CFE_SUCCESS)){
    TestStat = CFE_FAIL;
    fprintf(UT_logfile, "Unexpected return in ValidPipeDepth Test\n");
    fprintf(UT_logfile, "Val=99,Rtn1=%lx,Expected Rtn1=%lx\n",Rtn[1],CFE_SUCCESS);
    fprintf(UT_logfile, "Val=255,Rtn2=%lx,Expected Rtn2=%lx\n",Rtn[2],CFE_SUCCESS);
    fprintf(UT_logfile, "Val=%d,Rtn3=%lx,Expected Rtn3=%lx\n",CFE_SB_MAX_PIPE_DEPTH,Rtn[3],CFE_SUCCESS);
  }
  
  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_CreatePipe_API","ValidPipeDepth Test\n");

}/* end Test_CreatePipe_ValPipeDepth */


void Test_CreatePipe_InvalPipeDepth(void)
{

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t  PipeIdReturned[3];
  int32            Rtn[3];

  fprintf(UT_logfile, "Begin Test for InvalidPipeDepth\n");
  SB_ResetUnitTest();

  /* test a few invalid pipe depth values */
  UT_SetRtnCode (&QueueCreateRtn,OS_SUCCESS,1);/* to avoid creating a socket */
  Rtn[1] = CFE_SB_CreatePipe(&PipeIdReturned[1],0,"TestPipe0");

  UT_SetRtnCode (&QueueCreateRtn,OS_SUCCESS,1);/* to avoid creating a socket */
  Rtn[2] = CFE_SB_CreatePipe(&PipeIdReturned[2],CFE_SB_MAX_PIPE_DEPTH + 1,"TstPipeMxDepthPlus1");

  UT_SetRtnCode (&QueueCreateRtn,OS_SUCCESS,1);/* to avoid creating a socket */
  Rtn[3] = CFE_SB_CreatePipe(&PipeIdReturned[3],65535,"TestPipe65535");

  if((Rtn[1] != CFE_SB_BAD_ARGUMENT)||(Rtn[2] != CFE_SB_BAD_ARGUMENT)||(Rtn[3] != CFE_SB_BAD_ARGUMENT)){
    TestStat = CFE_FAIL;
    fprintf(UT_logfile, "Unexpected return in PipeDepth Test for invalid settings.\n");
    fprintf(UT_logfile, "Val=0,Rtn1=%lx,Expected Rtn1=%lx\n",Rtn[1],CFE_SB_BAD_ARGUMENT);
    fprintf(UT_logfile, "Val=%d,Rtn2=%lx,Expected Rtn2=%lx\n",CFE_SB_MAX_PIPE_DEPTH + 1,Rtn[2],CFE_SB_BAD_ARGUMENT);
    fprintf(UT_logfile, "Val=65535,Rtn3=%lx,Expected Rtn3=%lx\n",Rtn[3],CFE_SB_BAD_ARGUMENT);
  }

  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_CR_PIPE_BAD_ARG_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_CR_PIPE_BAD_ARG_EID not sent\n");
    TestStat = CFE_FAIL;
  }  
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_CreatePipe_API","InvalidPipeDepth Test\n");

}/* end Test_CreatePipe_InvalPipeDepth */


void Test_CreatePipe_EmptyPipeName(void)
{

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t  PipeIdReturned;
  int32            Rtn;

  fprintf(UT_logfile, "Begin Test for Empty PipeName\n");
  SB_ResetUnitTest();
  
  /* Empty string test  - OS_QueueCreate currently returns OS_ERR_NAME_TAKEN error*/
  /* because all queues are intitialized to an empty string see DCR 501 */
  Rtn = CFE_SB_CreatePipe(&PipeIdReturned,50,"");

  if(Rtn != CFE_SB_PIPE_CR_ERR){
     fprintf(UT_logfile, "Unexpected return in EmptyPipeName Test exp=%lx,act=%lx\n",
                                                  CFE_SUCCESS,Rtn);
     TestStat = CFE_FAIL;
  }/* end if */

  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_CR_PIPE_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_CR_PIPE_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_CreatePipe_API","EmptyPipeName Test\n");

}/* end Test_CreatePipe_EmptyPipeName */



void Test_CreatePipe_LongPipeName(void)
{

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t  PipeIdReturned;
  int32            Rtn;

  fprintf(UT_logfile, "Begin Test for Long PipeName\n");
  SB_ResetUnitTest();
  
  /* Oversized string test */
  Rtn = CFE_SB_CreatePipe(&PipeIdReturned,50,"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  if(Rtn != CFE_SB_PIPE_CR_ERR){
      fprintf(UT_logfile, "Unexpected return in PipeName,oversized string Test,exp=%lx,act=%lx\n",
                                CFE_SB_PIPE_CR_ERR,Rtn);
      TestStat = CFE_FAIL;
  }/* end if */

  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_CR_PIPE_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_CR_PIPE_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_CreatePipe_API","Long PipeName Test\n");

}/* end Test_CreatePipe_LongPipeName */


void Test_CreatePipe_SamePipeName(void)
{

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t  PipeIdReturned[3];
  int32            Rtn[3];

  fprintf(UT_logfile, "Begin Test for Same PipeName\n");
  SB_ResetUnitTest();

  /* Same pipename test */
  Rtn[1] = CFE_SB_CreatePipe(&PipeIdReturned[1],50,"SamePipeName");
  Rtn[2] = CFE_SB_CreatePipe(&PipeIdReturned[2],50,"SamePipeName");

  if((Rtn[1] != CFE_SUCCESS)||(Rtn[2] != CFE_SB_PIPE_CR_ERR)){
    fprintf(UT_logfile, "Unexpected return in Same PipeName Test,Rtn1=%lx,ExpRtn1=%lx,Rtn2=%lx,ExpRtn2=%lx\n",
                                                Rtn[1],CFE_SUCCESS,Rtn[2],CFE_SB_PIPE_CR_ERR);
    TestStat = CFE_FAIL;
  }/* end if */

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_CR_PIPE_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_CR_PIPE_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  /* cleanup the pipe that was created */
  CFE_SB_DeletePipe(PipeIdReturned[1]);

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_CreatePipe_API","Same PipeName Test\n");

}/* end Test_CreatePipe_SamePipeName */



void Test_CreatePipe_MaxPipes(void)
{

  int32             i,ExpRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t   PipeIdReturned[CFE_SB_MAX_PIPES + 1];
  int32             Rtn[CFE_SB_MAX_PIPES + 1];
  char              PipeName[16];

  /* Create maximum number of pipes + 1. Only one 'create pipe' failure expected */

  fprintf(UT_logfile, "Begin Test for MaxPipes\n");
  SB_ResetUnitTest();
  
  for(i=0;i < (CFE_SB_MAX_PIPES + 1);i++){
      sprintf(&PipeName[0],"TstPipe%03d",(int)i);
        Rtn[i] = CFE_SB_CreatePipe(&PipeIdReturned[i],50,&PipeName[0]);

        if(i < CFE_SB_MAX_PIPES){
            ExpRtn = CFE_SUCCESS;
        }else{
            ExpRtn = CFE_SB_MAX_PIPES_MET;
        }/* end if */

        if(Rtn[i] != ExpRtn){
            fprintf(UT_logfile, "Unexpected return in MaxPipes Test,i=%ld,exp=%lx,act=%lx\n",
                          i,ExpRtn,Rtn[i]);
            TestStat = CFE_FAIL;
            break;
        }/* end if */
  }/* end for */

  if(UT_EventIsInHistory(CFE_SB_MAX_PIPES_MET_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_MAX_PIPES_MET_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  /* clean up */
  for(i=0;i<=(CFE_SB_MAX_PIPES - 1);i++){
     CFE_SB_DeletePipe(i);
  }/* end for */

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_CreatePipe_API","Max Pipes Test\n");

}/* end Test_CreatePipe_MaxPipes */







void Test_DeletePipe_API(void)
{
  fprintf(UT_logfile, "\n\n\nBegin Test_DeletePipe_API\n\n");

  Test_DeletePipe_NoSubs();
  Test_DeletePipe_WithSubs();
  Test_DeletePipe_InvalidPipeId();
  Test_DeletePipe_InvalidPipeOwner();
  Test_DeletePipe_WithAppid();
  
  fprintf(UT_logfile, "End Test_DeletePipe_API\n");

}/* end Test_DeletePipe_API */


void Test_DeletePipe_NoSubs(void)
{

  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t    PipedId;

  fprintf(UT_logfile, "Begin Test for No Subscriptions\n");
  SB_ResetUnitTest();

  CFE_SB_CreatePipe(&PipedId,10,"Testpipe5");
  ActRtn = CFE_SB_DeletePipe(PipedId);

  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Delete Pipe Test,exp=%lx,act=%lx\n",
                                              ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_PIPE_DELETED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_DELETED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_DeletePipe_API","No Subscriptions Test\n");

}/* end Test_DeletePipe_NoSubs */

void Test_DeletePipe_WithSubs(void)
{
  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t    PipedId;

  fprintf(UT_logfile, "Begin Test With Subscriptions\n");
  SB_ResetUnitTest();
  
  CFE_SB_CreatePipe(&PipedId,10,"Testpipe5");
  CFE_SB_Subscribe(1801,PipedId);
  CFE_SB_Subscribe(1802,PipedId);
  CFE_SB_Subscribe(1803,PipedId);
  CFE_SB_Subscribe(1804,PipedId);

  ActRtn = CFE_SB_DeletePipe(PipedId);

  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Delete Pipe Test,exp=%lx,act=%lx\n",
                                              ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }

  ExpRtn = 10;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_PIPE_DELETED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_DELETED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_DeletePipe_API","With Subscriptions Test\n");

}/* end Test_DeletePipe_WithSubs */


void Test_DeletePipe_InvalidPipeId(void)
{

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  
  fprintf(UT_logfile, "Begin Test for Invalid PipeId\n");
  SB_ResetUnitTest();
  
  ActRtn = CFE_SB_DeletePipe(30);

  ExpRtn = CFE_SB_BAD_ARGUMENT;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in Delete Pipe Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_DEL_PIPE_ERR1_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_DEL_PIPE_ERR1_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_DeletePipe_API","Invalid PipeId Test\n");

}/* end Test_DeletePipe_InvalidPipeId */


void Test_DeletePipe_InvalidPipeOwner(void)
{

  int32             ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t   PipedId;
  uint32            RealOwner;

  fprintf(UT_logfile, "Begin Test for Invalid Pipe Owner\n");
  SB_ResetUnitTest();

  ActRtn = CFE_SB_CreatePipe(&PipedId,10,"Testpipe6");
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from CreatePipe in DeletePipe,InvalOwner Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  /* Change owner of pipe through memory corruption */
  RealOwner = CFE_SB.PipeTbl[PipedId].AppId;
  CFE_SB.PipeTbl[PipedId].AppId = 42;/* 42 is random and is sure not to be owner */

  ActRtn = CFE_SB_DeletePipe(PipedId);
  ExpRtn = CFE_SB_BAD_ARGUMENT;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from DeletPipe in DeletePipe,InvalOwner Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_DEL_PIPE_ERR2_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_DEL_PIPE_ERR2_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  /* Restore owner id and delete pipe since test is complete */
  CFE_SB.PipeTbl[PipedId].AppId = RealOwner;
  CFE_SB_DeletePipe(PipedId);

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_DeletePipe_API","Invalid Pipe Owner Test\n");

}/* end Test_DeletePipe_InvalidPipeId */


void Test_DeletePipe_WithAppid(void)
{

  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t    PipedId;

  fprintf(UT_logfile, "Begin Test DeletePipeWithAppid API\n");
  SB_ResetUnitTest();
  
  CFE_SB_CreatePipe(&PipedId,10,"Testpipe5");
  CFE_SB_Subscribe(1801,PipedId);
  CFE_SB_Subscribe(1802,PipedId);
  CFE_SB_Subscribe(1803,PipedId);
  CFE_SB_Subscribe(1804,PipedId);

  ActRtn = CFE_SB_DeletePipeWithAppId(PipedId, 0);

  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Delete Pipe W/Appid Test,exp=%lx,act=%lx\n",
                                              ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }

  ExpRtn = 10;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_DeletePipe_API","With Appid Test\n");

}/* end Test_DeletePipe_WithAppid */



void Test_Subscribe_API(void)
{
  fprintf(UT_logfile, "\n\n\nBegin Test_Subscribe_API\n\n");

  Test_Subscribe_SubscribeEx();
  Test_Subscribe_InvalidPipeId();
  Test_Subscribe_InvalidMsgId();
  Test_Subscribe_MaxMsgLim();
  Test_Subscribe_DuplicateSubscription();
  Test_Subscribe_LocalSubscription();
  Test_Subscribe_MaxDestCount();
  Test_Subscribe_MaxMsgIdCount();
  Test_Subscribe_SendPrevSubs();
  Test_Subscribe_FindGlobalMsgIdCnt();
  Test_Subscribe_PipeNonexistent();
  Test_Subscribe_SubscriptionReporting();
  Test_Subscribe_InvalidPipeOwner();

  fprintf(UT_logfile, "End Test_Subscribe_API\n");

}/* end Test_Subscribe_API */


void Test_Subscribe_SubscribeEx(void)
{
  int32             ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_MsgId_t    MsgId = 0x1806;
  CFE_SB_PipeId_t   PipeId;
  uint16            MsgLim = 8;
  CFE_SB_Qos_t      Quality;

  fprintf(UT_logfile, "Begin Test for SubscribeEx\n");
  SB_ResetUnitTest();
  
  CFE_SB_CreatePipe(&PipeId,10,"TstPipe");

  ExpRtn = CFE_SUCCESS;
  ActRtn = CFE_SB_SubscribeEx(MsgId,PipeId,Quality,MsgLim);
  if(ActRtn != ExpRtn){
          fprintf(UT_logfile, "Unexpected return in SubscribeEx Test,exp=%lx,act=%lx\n",ExpRtn,ActRtn);
          TestStat = CFE_FAIL;
  }
  
  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Subscribe_API","SubscribeEx Test\n");

}/* end Test_Subscribe_SubscribeEx */


void Test_Subscribe_InvalidPipeId(void)
{
  int32             ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for InvalidPipeId\n");
  SB_ResetUnitTest();

  ExpRtn = CFE_SB_BAD_ARGUMENT;
  ActRtn = CFE_SB_Subscribe(0xFF00,2);
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Invalid PipeId Subscribe Test,exp=%lx,act=%lx\n",
                                    ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUB_INV_PIPE_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUB_INV_PIPE_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Subscribe_API","InvalidPipeId Test\n");

}/* end Test_Subscribe_InvalidPipeId */


void Test_Subscribe_InvalidMsgId(void)
{
  int32             ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t   PipeId;

  fprintf(UT_logfile, "Begin Test for InvalidMsgId\n");
  SB_ResetUnitTest();

  CFE_SB_CreatePipe(&PipeId,10,"TstPipe");

  ExpRtn = CFE_SB_BAD_ARGUMENT;
  ActRtn = CFE_SB_Subscribe(0xFF00, PipeId);
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Invalid MsgId Subscribe Test,exp=%lx,act=%lx\n",
                                    ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }
  
  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUB_ARG_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUB_ARG_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Subscribe_API","InvalidMsgId Test\n");

}/* end Test_Subscribe_InvalidMsgId */


void Test_Subscribe_MaxMsgLim(void)
{
  int32             ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t   PipeId;
  CFE_SB_Qos_t      Quality;
  uint16            MsgLim;

  fprintf(UT_logfile, "Begin Test for MaxMsgLim\n");
  SB_ResetUnitTest();

  CFE_SB_CreatePipe(&PipeId,10,"TstPipe");
  MsgLim = 0xFFFF;

  ExpRtn = CFE_SUCCESS;
  ActRtn = CFE_SB_SubscribeEx(0x1805,PipeId,Quality,MsgLim);
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in MaxMsgLim Subscribe Test,exp=%lx,act=%lx\n",
                                    ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }
  
  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Subscribe_API","MaxMsgLim Test\n");

}/* end Test_Subscribe_MaxMsgLim */


void Test_Subscribe_DuplicateSubscription(void)
{

  int32             ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t   PipeId;

  fprintf(UT_logfile, "Begin Test for DuplicateSubscription\n");
  SB_ResetUnitTest();

  CFE_SB_CreatePipe(&PipeId,10,"TstPipe");

  ExpRtn = CFE_SUCCESS;
  ActRtn = CFE_SB_Subscribe(0x1805,PipeId);
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in first subscribe of Duplicate Subscription Test,exp=%lx,act=%lx\n",
            ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = CFE_SUCCESS;
  ActRtn = CFE_SB_Subscribe(0x1805,PipeId);
  if(ActRtn != ExpRtn){
          fprintf(UT_logfile, "Unexpected return in second subscribe of Duplicate Subscription Test,exp=%lx,act=%lx\n",
                                              ExpRtn,ActRtn);
          TestStat = CFE_FAIL;
  }
  
  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Subscribe_API","DuplicateSubscription Test\n");

}/* end Test_Subscribe_DuplicateSubscription */


void Test_Subscribe_LocalSubscription(void)
{
  int32             ExpRtn,ActRtn,TestStat = CFE_PASS;
  uint16            MsgLim;
  CFE_SB_PipeId_t   PipeId;

  fprintf(UT_logfile, "Begin Test for LocalSubscription\n");
  SB_ResetUnitTest();

  CFE_SB_CreatePipe(&PipeId,10,"TstPipe");
  MsgLim = 4;

  ExpRtn = CFE_SUCCESS;
  ActRtn = CFE_SB_SubscribeLocal(0x0809,PipeId,MsgLim);
  if(ActRtn != ExpRtn){
          fprintf(UT_logfile, "Unexpected return in Local Subscription Test,exp=%lx,act=%lx\n",
                                              ExpRtn,ActRtn);
          TestStat = CFE_FAIL;
  }
  
  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Subscribe_API","LocalSubscription Test\n");

}/* end Test_Subscribe_LocalSubscription */


void Test_Subscribe_MaxDestCount(void)
{

  int32           ExpRtn,ActRtn,TestStat = CFE_PASS;
  int32           i,Rtn[CFE_SB_MAX_DEST_PER_PKT + 1];
  CFE_SB_PipeId_t PipeId[CFE_SB_MAX_DEST_PER_PKT];
  char            PipeName[16];


  fprintf(UT_logfile, "Begin Test for MaxDestCount\n");
  SB_ResetUnitTest();

  /* create pipes */
  for(i=0;i < (CFE_SB_MAX_DEST_PER_PKT + 1);i++){
      sprintf(&PipeName[0],"TstPipe%03d",(int)i);
        Rtn[i] = CFE_SB_CreatePipe(&PipeId[i],50,&PipeName[0]);

        ExpRtn = CFE_SUCCESS;

        if(Rtn[i] != ExpRtn){
            fprintf(UT_logfile, "Unexpected error creating pipes in MaxDestCount Test,i=%ld,exp=%lx,act=%lx\n",
                          i,ExpRtn,Rtn[i]);
            TestStat = CFE_FAIL;
            break;
        }/* end if */
  }/* end for */

  /* Do subscriptions */
  for(i=0;i<(CFE_SB_MAX_DEST_PER_PKT + 1);i++){
      ActRtn = CFE_SB_Subscribe(0x0811,i);
      if(i < CFE_SB_MAX_DEST_PER_PKT){
          ExpRtn = CFE_SUCCESS;
      }else{
          ExpRtn = CFE_SB_MAX_DESTS_MET;
      }/* end if */
      if(ActRtn != ExpRtn){
          fprintf(UT_logfile, "Unexpected return in Max Destination Count Test,i=%ld,exp=%lx,act=%lx\n",
                          i,ExpRtn,ActRtn);
          TestStat = CFE_FAIL;

          break;
      }/* end if */

  }/* end for */

  ExpRtn = 34;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  /* delete pipes */
  for (i=0; i < (CFE_SB_MAX_DEST_PER_PKT + 1);i++){
    CFE_SB_DeletePipe(PipeId[i]);
  }/* end for */

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Subscribe_API","MaxDestCount Test\n");

}/* end Test_Subscribe_MaxDestCount */


void Test_Subscribe_MaxMsgIdCount(void)
{

  int32           i,ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t PipeId0,PipeId1,PipeId2;

  fprintf(UT_logfile, "Begin Test for MaxMsgIdCount\n");

  /* three pipes created just to mix things up and not use pipeid 0 for a change */
  CFE_SB_CreatePipe(&PipeId0,50,"Pipe0");
  CFE_SB_CreatePipe(&PipeId1,50,"Pipe1");
  CFE_SB_CreatePipe(&PipeId2,50,"Pipe2");

  for(i=0;i < (CFE_SB_MAX_MSG_IDS + 1);i++){

    ActRtn = CFE_SB_Subscribe(i,PipeId2);

    if(i < CFE_SB_MAX_MSG_IDS){
      ExpRtn = CFE_SUCCESS;
    }else{
      ExpRtn = CFE_SB_MAX_MSGS_MET;
    }/* end if */

    if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Max MsgId Count Test,i=%ld,exp=%lx,act=%lx\n",
              i,ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
    }/* end if */

  }/* end for */

  if(UT_EventIsInHistory(CFE_SB_MAX_MSGS_MET_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_MAX_MSGS_MET_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId0);
  CFE_SB_DeletePipe(PipeId1);
  CFE_SB_DeletePipe(PipeId2);

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Subscribe_API","MaxMsgIdCount Test\n");

}/* end Test_Subscribe_MaxMsgIdCount */


void Test_Subscribe_SendPrevSubs(void)
{
  int32           ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t PipeId0,PipeId1,PipeId2;

  fprintf(UT_logfile, "Begin Test for SendPrevSubs\n");
  SB_ResetUnitTest();

  CFE_SB_CreatePipe(&PipeId0,50,"Pipe0");
  CFE_SB_CreatePipe(&PipeId1,50,"Pipe1");
  CFE_SB_CreatePipe(&PipeId2,50,"Pipe2");

  CFE_SB_Subscribe(0x0811,PipeId0);
  CFE_SB_Subscribe(0x0812,PipeId0);
  CFE_SB_Subscribe(0x0813,PipeId0);
  CFE_SB_Subscribe(0x0811,PipeId1);
  CFE_SB_Subscribe(0x0812,PipeId1);
  CFE_SB_Subscribe(0x0813,PipeId1);
  CFE_SB_Subscribe(0x0811,PipeId2);

  CFE_SB_SendPrevSubs();

  ExpRtn = 12;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PART_SUB_PKT_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PART_SUB_PKT_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId0);
  CFE_SB_DeletePipe(PipeId1);
  CFE_SB_DeletePipe(PipeId2);

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Subscribe_API","SendPrevSubs Test\n");

}/* end Test_Subscribe_SendPrevSubs */


void Test_Subscribe_FindGlobalMsgIdCnt(void)
{
  int32           ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t PipeId0,PipeId1,PipeId2;

  fprintf(UT_logfile, "Begin Test for FindGlobalMsgIdCnt\n");
  SB_ResetUnitTest();

  CFE_SB_CreatePipe(&PipeId0,50,"Pipe0");
  CFE_SB_CreatePipe(&PipeId1,50,"Pipe1");
  CFE_SB_CreatePipe(&PipeId2,50,"Pipe2");

  CFE_SB_Subscribe(0x0811,PipeId0);
  CFE_SB_Subscribe(0x0812,PipeId0);
  CFE_SB_Subscribe(0x0813,PipeId0);
  CFE_SB_Subscribe(0x0811,PipeId1);
  CFE_SB_Subscribe(0x0812,PipeId1);
  CFE_SB_Subscribe(0x0813,PipeId1);
  CFE_SB_SubscribeLocal(0x0811,PipeId2,4);

  ActRtn = CFE_SB_FindGlobalMsgIdCnt();
  ExpRtn = 3;/* 3 unique msg ids */

  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in FindGlobalMsgIdCnt Test,exp=%lx,act=%lx\n",
            ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }/* end if */
  
  ExpRtn = 10;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId0);
  CFE_SB_DeletePipe(PipeId1);
  CFE_SB_DeletePipe(PipeId2);

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Subscribe_API","FindGlobalMsgIdCnt Test\n");

}/* end Test_Subscribe_FindGlobalMsgIdCnt */


void Test_Subscribe_PipeNonexistent(void)
{
  int32             ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for PipeNonexistent\n");
  SB_ResetUnitTest();

  ExpRtn = CFE_SB_BAD_ARGUMENT;
  ActRtn = CFE_SB_Subscribe(0x800,55);
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in PipeNonexistent Test,exp=%lx,act=%lx\n",
            ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }/* end if */
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUB_INV_PIPE_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUB_INV_PIPE_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Subscribe_API","PipeNonexistent Test\n");

}/* end Test_Subscribe_PipeNonexistent */


void Test_Subscribe_SubscriptionReporting(void)
{

  int32             ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t   PipeId;
  
  fprintf(UT_logfile," Begin Test SubscribeFull, Subscription Reporting\n");
  SB_ResetUnitTest();

  ActRtn = CFE_SB_CreatePipe(&PipeId,10,"TstPipe");
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from CreatePipe in SubReporting Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  CFE_SB_SetSubscriptionReporting(CFE_SB_ENABLE);
   
  ActRtn = CFE_SB_Subscribe(0x8FD,PipeId);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Subscribe in SubReporting Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 4;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RPT_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RPT_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_SetSubscriptionReporting(CFE_SB_DISABLE);
   
  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Subscribe_API","SubscriptionReporting Test\n");

}/* end Test_Subscribe_SubscriptionReporting */


void Test_Subscribe_InvalidPipeOwner(void)
{
  int32           ExpRtn,ActRtn,TestStat = CFE_PASS;
  int32           RealOwner;
  CFE_SB_PipeId_t PipeId;

  fprintf(UT_logfile, "Begin Test for Invalid Pipe Owner\n");
  SB_ResetUnitTest();

  ActRtn = CFE_SB_CreatePipe(&PipeId,10,"Testpipe25");
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "First return was unexpected in Subscribe - non owner Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  /* Change owner of pipe through memory corruption */
  RealOwner = CFE_SB.PipeTbl[PipeId].AppId;
  CFE_SB.PipeTbl[PipeId].AppId = 42;/* 42 is random and is sure not to be owner */
  CFE_SB_Subscribe(0x877,PipeId);

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUB_INV_CALLER_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUB_INV_CALLER_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  /* Restore owner id and delete pipe since test is complete */
  CFE_SB.PipeTbl[PipeId].AppId = RealOwner;
  CFE_SB_DeletePipe(PipeId);

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Subscribe_API","Invalid Pipe Owner Test\n");

}/* end Test_Subscribe_InvalidPipeOwner */








void Test_Unsubscribe_API(void)
{
  fprintf(UT_logfile, "\n\n\nBegin Test_Unsubscribe_API\n\n");

  Test_Unsubscribe_Basic();
  Test_Unsubscribe_Local();
  Test_Unsubscribe_InvalParam();
  Test_Unsubscribe_NoMatch();
  Test_Unsubscribe_SubscriptionReporting();
  Test_Unsubscribe_InvalidPipe();
  Test_Unsubscribe_InvalidPipeOwner();
  Test_Unsubscribe_FirstDestWithMany();
  Test_Unsubscribe_MiddleDestWithMany();
  Test_Unsubscribe_GetDestPtr();

  fprintf(UT_logfile, "End Test_Unsubscribe_API\n");

}/* end Test_Unsubscribe_API */


void Test_Unsubscribe_Basic(void){

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t  TstPipe;
  CFE_SB_MsgId_t   MsgId = 0x1806;

  fprintf(UT_logfile, "Begin Test for Unsubscribe Basic\n");
  SB_ResetUnitTest();
  
  CFE_SB_CreatePipe(&TstPipe,50,"TstPipe");
  CFE_SB_Subscribe(MsgId,TstPipe);
  
  ExpRtn = CFE_SUCCESS;
  ActRtn = CFE_SB_Unsubscribe(MsgId,TstPipe);
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Unsubscribe Basic,exp=%lx,act=%lx\n",
                                          ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }/* end if */

  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(TstPipe);
  SB_ResetUnitTest();  
  UT_Report(TestStat, "Test_Unsubscribe_API","Unsubscribe Basic\n");

}/* end Test_Unsubscribe_Basic */


void Test_Unsubscribe_Local(void){

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t  TstPipe;

  fprintf(UT_logfile, "Begin Test for Local Unsubscribe\n");
  SB_ResetUnitTest();
  
  CFE_SB_CreatePipe(&TstPipe,50,"TstPipe");
  CFE_SB_Subscribe(0x0801,TstPipe);
  
  ExpRtn = CFE_SUCCESS;
  ActRtn = CFE_SB_UnsubscribeLocal(0x0801,TstPipe);
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Local Unsubscribe Test,exp=%lx,act=%lx\n",
                                          ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }

  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(TstPipe);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Unsubscribe_API","Local Unsubscribe Test\n");

}/* end Test_Unsubscribe_Local */


void Test_Unsubscribe_InvalParam(void){

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t  TstPipe;

  fprintf(UT_logfile, "Begin Test for Invalid Param, Unsubscribe\n");
  SB_ResetUnitTest();
  
  CFE_SB_CreatePipe(&TstPipe,50,"TstPipe");
  
  ExpRtn = CFE_SB_BAD_ARGUMENT;
  ActRtn = CFE_SB_Unsubscribe(0xFF00, TstPipe);
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Invalid Param Unsubscribe Test,exp=%lx,act=%lx\n",
                                          ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_UNSUB_ARG_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_UNSUB_ARG_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(TstPipe);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Unsubscribe_API","Invalid Param, Unsubscribe Test\n");

}/* end Test_Unsubscribe_InvalParam */


void Test_Unsubscribe_NoMatch(void){

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t  TstPipe;
  CFE_SB_MsgId_t   MsgId = 0x1806;

  fprintf(UT_logfile, "Begin Test for No Match\n");
  SB_ResetUnitTest();
  
  CFE_SB_CreatePipe(&TstPipe,50,"TstPipe");
  CFE_SB_Subscribe(MsgId,TstPipe); 

  ExpRtn = CFE_SUCCESS;
  ActRtn = CFE_SB_Unsubscribe(0x1813,TstPipe);
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in No Match Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }

  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_UNSUB_NO_SUBS_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_UNSUB_NO_SUBS_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(TstPipe);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Unsubscribe_API","No Match Test\n");

}/* end Test_Unsubscribe_NoMatch */


void Test_Unsubscribe_SubscriptionReporting(void){

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t  TstPipe;

  fprintf(UT_logfile," Begin Test Unsubscribe, Subscription Reporting");
  SB_ResetUnitTest();
  
  CFE_SB_CreatePipe(&TstPipe,50,"TstPipe");  
  CFE_SB_Subscribe(0x8FD, TstPipe);
  CFE_SB_SetSubscriptionReporting(CFE_SB_ENABLE);
  CFE_SB_Unsubscribe(0x8FD, TstPipe);
  CFE_SB_SetSubscriptionReporting(CFE_SB_DISABLE);
  /* end test */

  ExpRtn = 5;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_UNSUBSCRIPTION_RPT_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_UNSUBSCRIPTION_RPT_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(TstPipe);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Unsubscribe_API","Subscription Reporting Test\n");

}/* end Test_Unsubscribe_SubscriptionReporting */


void Test_Unsubscribe_InvalidPipe(void){

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t  TstPipe;

  fprintf(UT_logfile," Begin Test Unsubscribe, Invalid Pipe");
  SB_ResetUnitTest();
  
  CFE_SB_CreatePipe(&TstPipe,50,"TstPipe");  
  CFE_SB_Subscribe(0x8FD, TstPipe);
  
  ExpRtn = CFE_SB_BAD_ARGUMENT;
  ActRtn = CFE_SB_Unsubscribe(0x8FD,47);
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Invalid Pipe Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }

  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_UNSUB_INV_PIPE_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_UNSUB_INV_PIPE_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(TstPipe);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Unsubscribe_API","Invalid Pipe Test\n");

}/* end Test_Unsubscribe_InvalidPipe */


void Test_Unsubscribe_InvalidPipeOwner(void){
  
  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  uint32           RealOwner;
  CFE_SB_PipeId_t  PipeId;
  
  fprintf(UT_logfile, "Begin Test for Invalid Pipe Owner\n");
  SB_ResetUnitTest();

  ActRtn = CFE_SB_CreatePipe(&PipeId,10,"Testpipe6");
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "First return was unexpected in Unsubscribe - non owner Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  CFE_SB_Subscribe(0x877,PipeId);

  /* Change owner of pipe through memory corruption */
  RealOwner = CFE_SB.PipeTbl[PipeId].AppId;
  CFE_SB.PipeTbl[PipeId].AppId = 42;/* 42 is random and is sure not be owner */

  ActRtn = CFE_SB_Unsubscribe(0x877,PipeId);
  ExpRtn = CFE_SB_BAD_ARGUMENT;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Second return was unexpected in Unsubscribe - non owner Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_UNSUB_INV_CALLER_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_UNSUB_INV_CALLER_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB.PipeTbl[PipeId].AppId = RealOwner;
  CFE_SB_DeletePipe(PipeId);

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_Unsubscribe_API","Invalid Pipe Owner Test\n");

}/* end Test_Unsubscribe_InvalidPipeOwner */




void Test_Unsubscribe_FirstDestWithMany(void){

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t  TstPipe1,TstPipe2,TstPipe3;

  fprintf(UT_logfile, "Begin Test for Unsubscribe FirstDestWithMany\n");  
  SB_ResetUnitTest();
  
  CFE_SB_CreatePipe(&TstPipe1,50,"TstPipe1");
  CFE_SB_CreatePipe(&TstPipe2,50,"TstPipe2");
  CFE_SB_CreatePipe(&TstPipe3,50,"TstPipe3");

  CFE_SB_Subscribe(0x1805,TstPipe1);
  CFE_SB_Subscribe(0x1805,TstPipe2);
  CFE_SB_Subscribe(0x1805,TstPipe3);
  
  ExpRtn = CFE_SUCCESS;
  ActRtn = CFE_SB_Unsubscribe(0x1805,TstPipe3);
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Unsubscribe FirstDestWithMany,exp=%lx,act=%lx\n",
                                          ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }/* end if */

  ExpRtn = 7;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(TstPipe1);
  CFE_SB_DeletePipe(TstPipe2);
  CFE_SB_DeletePipe(TstPipe3);
  SB_ResetUnitTest();  
  UT_Report(TestStat, "Test_Unsubscribe_API","Unsubscribe FirstDestWithMany\n");

}/* end Test_Unsubscribe_FirstDestWithMany */


void Test_Unsubscribe_MiddleDestWithMany(void){

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t  TstPipe1,TstPipe2,TstPipe3;

  fprintf(UT_logfile, "Begin Test for Unsubscribe MiddleDestWithMany\n");  
  SB_ResetUnitTest();
  
  CFE_SB_CreatePipe(&TstPipe1,50,"TstPipe1");
  CFE_SB_CreatePipe(&TstPipe2,50,"TstPipe2");
  CFE_SB_CreatePipe(&TstPipe3,50,"TstPipe3");

  CFE_SB_Subscribe(0x1805,TstPipe1);
  CFE_SB_Subscribe(0x1805,TstPipe2);
  CFE_SB_Subscribe(0x1805,TstPipe3);
  
  ExpRtn = CFE_SUCCESS;
  ActRtn = CFE_SB_Unsubscribe(0x1805,TstPipe2);
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Unsubscribe MiddleDestWithMany,exp=%lx,act=%lx\n",
                                          ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }/* end if */

  ExpRtn = 7;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(TstPipe1);
  CFE_SB_DeletePipe(TstPipe2);
  CFE_SB_DeletePipe(TstPipe3);
  SB_ResetUnitTest();  
  UT_Report(TestStat, "Test_Unsubscribe_API","Unsubscribe MiddleDestWithMany\n");

}/* end Test_Unsubscribe_MiddleDestWithMany */


void Test_Unsubscribe_GetDestPtr(void){

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t  TstPipe1,TstPipe2,TstPipe3;

  fprintf(UT_logfile, "Begin Test for Unsubscribe GetDestPtr\n");  
  SB_ResetUnitTest();
  
  CFE_SB_CreatePipe(&TstPipe1,50,"TstPipe1");
  CFE_SB_CreatePipe(&TstPipe2,50,"TstPipe2");
  CFE_SB_CreatePipe(&TstPipe3,50,"TstPipe3");

  CFE_SB_Subscribe(0x1805,TstPipe1);
  CFE_SB_Subscribe(0x1805,TstPipe2);
  CFE_SB_Subscribe(0x1805,TstPipe3);
  
  if(CFE_SB_GetDestPtr(0x1805,10) != NULL){
      fprintf(UT_logfile, "Unexpected return in Unsubscribe GetDestPtr,exp NULL\n");
      TestStat = CFE_FAIL;
  }/* end if */

  ExpRtn = 6;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(TstPipe1);
  CFE_SB_DeletePipe(TstPipe2);
  CFE_SB_DeletePipe(TstPipe3);
  SB_ResetUnitTest();  
  UT_Report(TestStat, "Test_Unsubscribe_API","Unsubscribe GetDestPtr\n");

}/* end Test_Unsubscribe_ GetDestPtr */











void Test_SendMsg_API(void){

  fprintf(UT_logfile, "\n\n\nBegin Test_SendMsg_API\n\n");

  Test_SendMsg_NullPtr();
  Test_SendMsg_InvalidMsgId();
  Test_SendMsg_NoSubscribers();
  Test_SendMsg_MaxMsgSizePlusOne();
  Test_SendMsg_BasicSend();
  Test_SendMsg_SequenceCount();
  Test_SendMsg_QueuePutError();
  Test_SendMsg_PipeFull();
  Test_SendMsg_MsgIdToPipeLimitExceeded();
  Test_SendMsg_GetPoolBufErr();
  Test_SendMsg_GetPoolCallErr();
  Test_SendMsg_ZeroCopyGetPtr();
  Test_SendMsg_ZeroCopySend();
  Test_SendMsg_ZeroCopyPass();
  Test_SendMsg_ZeroCopyReleasePtr();
  Test_SendMsg_DisabledDestination();

  fprintf(UT_logfile, "End Test_SendMsg_API\n");

}/* end Test_SendMsg_API */


void Test_SendMsg_NullPtr(void){

  int32            ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile, "Begin Test for Null Ptr\n");
  SB_ResetUnitTest();

  ActRtn = CFE_SB_SendMsg(NULL);
  ExpRtn = CFE_SB_BAD_ARGUMENT;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in Null Ptr Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SEND_BAD_ARG_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SEND_BAD_ARG_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","Null Ptr Test\n");

}/* end Test_SendMsg_NullPtr */


void Test_SendMsg_InvalidMsgId(void){

  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  SB_UT_Test_Tlm_t   TlmPkt;
  CFE_SB_MsgPtr_t    TlmPktPtr = (CFE_SB_MsgPtr_t)&TlmPkt;

  fprintf(UT_logfile, "Begin Test for Invalid MsgId\n");
  SB_ResetUnitTest();

  CFE_SB_InitMsg(&TlmPkt,0x3FFF,sizeof(SB_UT_Test_Tlm_t),TRUE);
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SB_BAD_ARGUMENT;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in Invalid MsgId Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SEND_INV_MSGID_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SEND_INV_MSGID_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","Invalid MsgId Test\n");

}/* end Test_SendMsg_InvalidMsgId */


void Test_SendMsg_NoSubscribers(void){

  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  SB_UT_Test_Tlm_t   TlmPkt;
  CFE_SB_MsgPtr_t    TlmPktPtr = (CFE_SB_MsgPtr_t)&TlmPkt;
  
  fprintf(UT_logfile, "Begin Test for No Subscribers\n");
  SB_ResetUnitTest();
  
  CFE_SB_InitMsg(&TlmPkt,0x0815,sizeof(SB_UT_Test_Tlm_t),TRUE);
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in No Subscribers Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SEND_NO_SUBS_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SEND_NO_SUBS_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","No Subscribers Test\n");

}/* end Test_SendMsg_NoSubscribers */


void Test_SendMsg_MaxMsgSizePlusOne(void){

  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  SB_UT_Test_Tlm_t   TlmPkt;
  CFE_SB_MsgPtr_t    TlmPktPtr = (CFE_SB_MsgPtr_t)&TlmPkt;

  fprintf(UT_logfile, "Begin Test for Max Msg Size Plus One\n");
  SB_ResetUnitTest();
  
  CFE_SB_InitMsg(&TlmPkt,0x0809,CFE_SB_MAX_SB_MSG_SIZE + 1,FALSE);
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SB_MSG_TOO_BIG;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in Max Msg Size Plus One Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_MSG_TOO_BIG_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_MSG_TOO_BIG_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","Max Msg Size Plus One Test\n");

}/* end Test_SendMsg_MaxMsgSizePlusOne */


void Test_SendMsg_BasicSend(void){

  int32              PipeDepth,ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t    PipeId;
  SB_UT_Test_Tlm_t   TlmPkt;
  CFE_SB_MsgPtr_t    TlmPktPtr = (CFE_SB_MsgPtr_t)&TlmPkt;

  fprintf(UT_logfile, "Begin Test for Basic Send\n");
  SB_ResetUnitTest();

  PipeDepth = 2;
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"TstPipe1");
  CFE_SB_Subscribe(0x0809,PipeId);

  CFE_SB_InitMsg(&TlmPkt,0x0809,sizeof(SB_UT_Test_Tlm_t),TRUE);
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in Basic Send Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","Basic Send Test\n");

}/* end Test_SendMsg_BasicSend */


void Test_SendMsg_SequenceCount(void){

  int32            TestStat,ExpRtn,ActRtn;
  CFE_SB_MsgPtr_t  PtrToMsg;
  CFE_SB_PipeId_t  PipeId;
  uint32           PipeDepth = 10;
  SB_UT_Test_Tlm_t TlmPkt;
  CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t)&TlmPkt;

  fprintf(UT_logfile, "Begin Test for Telemetry Sequence Count\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"SeqCntTstPipe1");
  CFE_SB_InitMsg(&TlmPkt,0x0809,sizeof(SB_UT_Test_Tlm_t),TRUE);
  CFE_SB_Subscribe(0x0809,PipeId);
  CCSDS_WR_SEQ(TlmPktPtr->Hdr,22); 

  TestStat = CFE_PASS;
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Send in Sequence Count Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ActRtn = CFE_SB_RcvMsg(&PtrToMsg,PipeId,CFE_SB_PEND_FOREVER);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Rcv 1 in Sequence Count Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(CCSDS_RD_SEQ(PtrToMsg->Hdr)!=1){
    fprintf(UT_logfile, "Unexpected sequence count for Send in Sequence Count Test,exp=1,act=%d\n",
                        CCSDS_RD_SEQ(PtrToMsg->Hdr));
    TestStat = CFE_FAIL;
  }

  ActRtn = CFE_SB_PassMsg(TlmPktPtr);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Pass in Sequence Count Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ActRtn = CFE_SB_RcvMsg(&PtrToMsg,PipeId,CFE_SB_PEND_FOREVER);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Rcv 2 in Sequence Count Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(CCSDS_RD_SEQ(PtrToMsg->Hdr)!=22){
    fprintf(UT_logfile, "Unexpected sequence count for Pass in Sequence Count Test,exp=22,act=%d\n",
                        CCSDS_RD_SEQ(PtrToMsg->Hdr));
    TestStat = CFE_FAIL;
  }

  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Send 2 in Sequence Count Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ActRtn = CFE_SB_RcvMsg(&PtrToMsg,PipeId,CFE_SB_PEND_FOREVER);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Rcv 3 in Sequence Count Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(CCSDS_RD_SEQ(PtrToMsg->Hdr)!=2){
    fprintf(UT_logfile, "Unexpected sequence count for Send in Sequence Count Test,exp=2,act=%d\n",
                        CCSDS_RD_SEQ(PtrToMsg->Hdr));
    TestStat = CFE_FAIL;
  }

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","Telemetry Sequence Count Test\n");

}/* end Test_SendMsg_SequenceCount */


void Test_SendMsg_QueuePutError(void){

  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  int32              PipeDepth,RealQueueId;
  CFE_SB_PipeId_t    PipeId4Error;
  SB_UT_Test_Tlm_t   TlmPkt;
  CFE_SB_MsgPtr_t    TlmPktPtr = (CFE_SB_MsgPtr_t)&TlmPkt;

  fprintf(UT_logfile, "Begin Test for QueuePut Error\n");
  SB_ResetUnitTest();
  PipeDepth = 2;
  CFE_SB_CreatePipe(&PipeId4Error,PipeDepth,"TstPipe2");
  CFE_SB_Subscribe(0x0818,PipeId4Error);

  CFE_SB_InitMsg(&TlmPkt,0x0818,sizeof(SB_UT_Test_Tlm_t),FALSE);
  /* Before sending this message, manipulate the queue_id in the pipe table so that */
  /* the QueuePut returns an error. If QueuePut returns an error, an error */
  /* event is sent, but the expected return from SendMsg (by design) is CFE_SUCCESS */
  RealQueueId = CFE_SB.PipeTbl[PipeId4Error].SysQueueId;
  CFE_SB.PipeTbl[PipeId4Error].SysQueueId = 0xFF00;
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in QueuePut Error Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_Q_WR_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_Q_WR_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB.PipeTbl[PipeId4Error].SysQueueId = RealQueueId;
  CFE_SB_DeletePipe(PipeId4Error);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","QueuePut Error Test\n");

}/* end Test_SendMsg_QueuePutError */


void Test_SendMsg_PipeFull(void){

  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  int32              PipeDepth;
  CFE_SB_PipeId_t    PipeId;
  SB_UT_Test_Tlm_t   TlmPkt;
  CFE_SB_MsgPtr_t    TlmPktPtr = (CFE_SB_MsgPtr_t)&TlmPkt;
  
  fprintf(UT_logfile, "Begin Test for Pipe Full\n");
  SB_ResetUnitTest();
  
  PipeDepth = 1;
  CFE_SB_InitMsg(&TlmPkt,0x0809,sizeof(SB_UT_Test_Tlm_t),TRUE);
  CFE_SB_CreatePipe(&PipeId, PipeDepth,"PipeFullTstPipe");
  CFE_SB_Subscribe(0x0809,PipeId);

  /* this send should pass */
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return1 in Pipe Full Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  /* tell the QueuePut stub to return OS_QUEUE_FULL on its next call */
  UT_SetRtnCode (&QueuePutRtn,OS_QUEUE_FULL,1);
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SUCCESS;/* Pipe overflow causes SendMsg to return CFE_SUCCESS*/
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return2 in Pipe Full Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_Q_FULL_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_Q_FULL_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId);

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","Pipe Full Test\n");  

}/* end Test_SendMsg_PipeFull */


void Test_SendMsg_MsgIdToPipeLimitExceeded(void){

  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  int32              PipeDepth;
  CFE_SB_PipeId_t    PipeId;
  SB_UT_Test_Tlm_t   TlmPkt;
  CFE_SB_MsgPtr_t    TlmPktPtr = (CFE_SB_MsgPtr_t)&TlmPkt;

  fprintf(UT_logfile, "Begin Test for MsgId-To-Pipe Limit Exceeded\n");
  SB_ResetUnitTest();
  
  PipeDepth = 5;
  CFE_SB_InitMsg(&TlmPkt,0x0811,sizeof(SB_UT_Test_Tlm_t),FALSE);
  CFE_SB_CreatePipe(&PipeId, PipeDepth,"MsgLimTstPipe");
  CFE_SB_SubscribeEx(0x0811,PipeId,CFE_SB_Default_Qos,1);
  /* this send should pass */
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in MsgId-To-Pipe Limit Test1,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  /* this send should produce a MsgId to Pipe Limit Exceeded message, but return success */
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in MsgId-To-Pipe Limit Test2,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_MSGID_LIM_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_MSGID_LIM_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","MsgId-To-Pipe Limit Exceeded Test\n");

}/* end Test_SendMsg_MsgIdToPipeLimitExceeded */


void Test_SendMsg_GetPoolBufErr(void){
  
  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  int32              PipeDepth;
  CFE_SB_PipeId_t    PipeId;
  SB_UT_Test_Tlm_t   TlmPkt;
  CFE_SB_MsgPtr_t    TlmPktPtr = (CFE_SB_MsgPtr_t)&TlmPkt;

  fprintf(UT_logfile, "Begin Test for GetPoolBufErr\n");
  SB_ResetUnitTest();
  
  PipeDepth = 1;
  CFE_SB_InitMsg(&TlmPkt,0x0809,sizeof(SB_UT_Test_Tlm_t),TRUE);
  CFE_SB_CreatePipe(&PipeId, PipeDepth,"GetPoolErrPipe");
  CFE_SB_Subscribe(0x0809,PipeId);

  /* have GetPoolBuf stub return error on its next call(Buf descriptor allocation failed) */
  UT_SetRtnCode (&GetPoolRtn,CFE_ES_ERR_MEM_BLOCK_SIZE,1);
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SB_BUF_ALOC_ERR;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in GetPoolBufErr1 Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_GET_BUF_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_GET_BUF_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","GetPoolBufErr Test\n");

}/* end Test_SendMsg_GetPoolBufErr */


void Test_SendMsg_GetPoolCallErr(void){

  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  int32              PipeDepth;
  CFE_SB_PipeId_t    PipeId;
  CFE_SB_MsgPtr_t    ZeroCpyMsgPtr = NULL;
  uint32             ZeroCpyBufHndl;

  fprintf(UT_logfile, "Begin Test for GetPoolCallErr\n");
  SB_ResetUnitTest();

  PipeDepth = 1;
  CFE_SB_CreatePipe(&PipeId, PipeDepth,"GetPoolCallPipe");
  CFE_SB_Subscribe(0x0809,PipeId);

  ZeroCpyMsgPtr = CFE_SB_ZeroCopyGetPtr(sizeof(SB_UT_Test_Tlm_t), &ZeroCpyBufHndl);

  CFE_SB_InitMsg(ZeroCpyMsgPtr,0x0809,sizeof(SB_UT_Test_Tlm_t),TRUE);

  /* have GetPoolBuf stub return error on its next call(Buf descriptor allocation failed) */
  UT_SetRtnCode (&GetPoolRtn,CFE_ES_ERR_MEM_BLOCK_SIZE,1);
  ActRtn = CFE_SB_ZeroCopySend(ZeroCpyMsgPtr,ZeroCpyBufHndl);
  ExpRtn = CFE_SB_BUF_ALOC_ERR;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in GetPoolCallErr Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","ZeroCopyGetPtr Test\n");

}/* end Test_SendMsg_GetPoolCallErr */


void Test_SendMsg_ZeroCopyGetPtr(void){

  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  uint32             ZeroCpyBufHndl;

  fprintf(UT_logfile, "Begin Test for ZeroCopyGetPtr\n");
  SB_ResetUnitTest();

  /* have GetPoolBuf stub return error on its next call(Buf descriptor allocation failed) */
  UT_SetRtnCode (&GetPoolRtn,CFE_ES_ERR_MEM_BLOCK_SIZE,1);
  ActRtn = (int32) CFE_SB_ZeroCopyGetPtr(10,&ZeroCpyBufHndl);
  ExpRtn = (int32)NULL;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in ZeroCopyGetPtr1 Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  /* have GetPoolBuf stub return error on its second call(Actual Buffer allocation failed) */
  UT_SetRtnCode (&GetPoolRtn,CFE_ES_ERR_MEM_BLOCK_SIZE,2);
  ActRtn = (int32) CFE_SB_ZeroCopyGetPtr(10,&ZeroCpyBufHndl);
  ExpRtn = (int32) NULL;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in ZeroCpyGetPtr2 Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 0;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","ZeroCopyGetPtr Test\n");

}/* end Test_SendMsg_ZeroCopyGetPtr */


void Test_SendMsg_ZeroCopySend(void){

  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_MsgPtr_t    PtrToMsg;
  CFE_SB_PipeId_t    PipeId;
  uint32             PipeDepth = 10;
  CFE_SB_MsgPtr_t    ZeroCpyMsgPtr = NULL;
  uint32             ZeroCpyBufHndl = 0;

  fprintf(UT_logfile, "Begin Test for CFE_SB_ZeroCopySend\n");
  SB_ResetUnitTest();

  CFE_SB_CreatePipe(&PipeId,PipeDepth,"ZeroCpyTstPipe1");
  CFE_SB_Subscribe(0x0809,PipeId); 

  ZeroCpyMsgPtr = CFE_SB_ZeroCopyGetPtr(sizeof(SB_UT_Test_Tlm_t), &ZeroCpyBufHndl);

  if(ZeroCpyMsgPtr == NULL){
    fprintf(UT_logfile, "Unexpected NULL pointer returned from ZeroCopyGetPtr\n");
    TestStat = CFE_FAIL;
  }
  else{
    CFE_SB_InitMsg(ZeroCpyMsgPtr,0x0809,sizeof(SB_UT_Test_Tlm_t),TRUE);
    CCSDS_WR_SEQ(ZeroCpyMsgPtr->Hdr,22);
  }

  ActRtn = CFE_SB_ZeroCopySend(ZeroCpyMsgPtr,ZeroCpyBufHndl);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Send in Zero Copy Send Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ActRtn = CFE_SB_RcvMsg(&PtrToMsg,PipeId,CFE_SB_PEND_FOREVER);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Rcv in Zero Copy Send Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(CCSDS_RD_SEQ(PtrToMsg->Hdr)!=1){
    fprintf(UT_logfile, "Unexpected sequence count for Send in Sequence Count Test,exp=1,act=%d\n",
                        CCSDS_RD_SEQ(PtrToMsg->Hdr));
    TestStat = CFE_FAIL;
  }
  
  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","CFE_SB_ZeroCopySend Test\n");

}/* end Test_SendMsg_ZeroCopySend */


void Test_SendMsg_ZeroCopyPass(void){

  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_MsgPtr_t    PtrToMsg;
  CFE_SB_PipeId_t    PipeId;
  uint32             PipeDepth = 10;
  CFE_SB_MsgPtr_t    ZeroCpyMsgPtr = NULL;
  uint32             ZeroCpyBufHndl = 0;

  fprintf(UT_logfile, "Begin Test for CFE_SB_ZeroCopyPass\n");
  SB_ResetUnitTest();

  CFE_SB_CreatePipe(&PipeId,PipeDepth,"ZeroCpyPassTstPipe1");
  CFE_SB_Subscribe(0x0809,PipeId);

  ZeroCpyMsgPtr = CFE_SB_ZeroCopyGetPtr(sizeof(SB_UT_Test_Tlm_t), &ZeroCpyBufHndl);

  if(ZeroCpyMsgPtr == NULL){
    fprintf(UT_logfile, "Unexpected NULL pointer returned from ZeroCopyGetPtr\n");
    TestStat = CFE_FAIL;
  }
  else{
    CFE_SB_InitMsg(ZeroCpyMsgPtr,0x0809,sizeof(SB_UT_Test_Tlm_t),TRUE);
    CCSDS_WR_SEQ(ZeroCpyMsgPtr->Hdr,22);
  }

  ActRtn = CFE_SB_ZeroCopyPass(ZeroCpyMsgPtr,ZeroCpyBufHndl);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Send in Zero Copy Pass Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ActRtn = CFE_SB_RcvMsg(&PtrToMsg,PipeId,CFE_SB_PEND_FOREVER);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Rcv in Zero Copy Pass Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(CCSDS_RD_SEQ(PtrToMsg->Hdr)!=22){
    fprintf(UT_logfile, "Unexpected sequence count for Send in Sequence Count Test,exp=22,act=%d\n",
                        CCSDS_RD_SEQ(PtrToMsg->Hdr));
    TestStat = CFE_FAIL;
  }

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","CFE_SB_ZeroCopyPass Test\n");

}/* end Test_SendMsg_ZeroCopyPass */


void Test_SendMsg_ZeroCopyReleasePtr(void){

  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_MsgPtr_t    ZeroCpyMsgPtr1 = NULL;
  CFE_SB_MsgPtr_t    ZeroCpyMsgPtr2 = NULL;
  CFE_SB_MsgPtr_t    ZeroCpyMsgPtr3 = NULL;
  uint32             ZeroCpyBufHndl1 = 0;
  uint32             ZeroCpyBufHndl2 = 0;
  uint32             ZeroCpyBufHndl3 = 0;

  fprintf(UT_logfile, "Begin Test for ZeroCopyReleasePtr\n");
  SB_ResetUnitTest();
 
  ZeroCpyMsgPtr1 = CFE_SB_ZeroCopyGetPtr(10,&ZeroCpyBufHndl1);
  ZeroCpyMsgPtr2 = CFE_SB_ZeroCopyGetPtr(10,&ZeroCpyBufHndl2);
  ZeroCpyMsgPtr3 = CFE_SB_ZeroCopyGetPtr(10,&ZeroCpyBufHndl3);
 
  ActRtn = CFE_SB_ZeroCopyReleasePtr(ZeroCpyMsgPtr2, ZeroCpyBufHndl2);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in ZeroCopyReleasePtr1 Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  UT_SetRtnCode( &GetPoolInfoRtn, -1, 1);
  ActRtn = CFE_SB_ZeroCopyReleasePtr(ZeroCpyMsgPtr2, ZeroCpyBufHndl2);
  ExpRtn = CFE_SB_BUFFER_INVALID;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in ZeroCopyReleasePtr2 Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ActRtn = CFE_SB_ZeroCopyReleasePtr(ZeroCpyMsgPtr3, ZeroCpyBufHndl3);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in ZeroCopyReleasePtr3 Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ActRtn = CFE_SB_ZeroCopyReleasePtr(ZeroCpyMsgPtr1, ZeroCpyBufHndl1);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in ZeroCopyReleasePtr4 Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 0;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","ZeroCopyReleasePtr Test\n");

}/* end Test_SendMsg_ZeroCopyReleasePtr */


void Test_SendMsg_DisabledDestination(void){
  
  int32              ExpRtn,ActRtn,TestStat = CFE_PASS;
  int32              PipeDepth;
  CFE_SB_PipeId_t    PipeId;
  SB_UT_Test_Tlm_t   TlmPkt;
  CFE_SB_MsgPtr_t    TlmPktPtr = (CFE_SB_MsgPtr_t)&TlmPkt;
  CFE_SB_DestinationD_t   *DestPtr;

  fprintf(UT_logfile, "Begin Test for Send w/Disabled Destination\n");
  SB_ResetUnitTest();

  PipeDepth = 2;
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"TstPipe1");
  CFE_SB_Subscribe(0x0809,PipeId);

  DestPtr = CFE_SB_GetDestPtr(0x0809,PipeId);
  DestPtr->Active = CFE_SB_INACTIVE;  

  CFE_SB_InitMsg(&TlmPkt,0x0809,sizeof(SB_UT_Test_Tlm_t),TRUE);
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in DisabledDestination Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_SendMsg_API","DisabledDestination Test\n");  


}/* end Test_SendMsg_DisabledDestination */








void Test_RcvMsg_API(void){

  fprintf(UT_logfile, "\n\n\nBegin Test_RcvMsg_API\n\n");

  Test_RcvMsg_InvalidPipeId();
  Test_RcvMsg_InvalidTimeout();
  Test_RcvMsg_Poll();
  Test_RcvMsg_GetLastSenderNull();
  Test_RcvMsg_GetLastSenderInvalidPipe();
  Test_RcvMsg_GetLastSenderInvalidCaller();
  Test_RcvMsg_GetLastSenderSuccess();
  Test_RcvMsg_Timeout();
  Test_RcvMsg_PipeReadError();
  Test_RcvMsg_PendForever();
  Test_RcvMsg_MsgLimCntrErr();

  fprintf(UT_logfile, "End Test_RcvMsg_API\n");  

}/* end Test_RcvMsg_API */



void Test_RcvMsg_InvalidPipeId(void){

  int32            TestStat,ExpRtn,ActRtn;
  CFE_SB_MsgPtr_t  PtrToMsg;
  CFE_SB_PipeId_t  InvalidPipeId = 20;

  fprintf(UT_logfile, "Begin Test for Rcv - Invalid PipeId Rcv\n");
  SB_ResetUnitTest();
  TestStat = CFE_PASS;
  ActRtn = CFE_SB_RcvMsg(&PtrToMsg,InvalidPipeId,CFE_SB_POLL);
  ExpRtn = CFE_SB_BAD_ARGUMENT;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in Invalid PipeId Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_BAD_PIPEID_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_BAD_PIPEID_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  SB_ResetUnitTest(); 
  UT_Report(TestStat, "Test_RcvMsg_API","Invalid PipeId Test\n");

}/* end Test_RcvMsg_InvalidPipeId */



void Test_RcvMsg_InvalidTimeout(void){

  int32            TestStat,ExpRtn,ActRtn;
  CFE_SB_MsgPtr_t  PtrToMsg;
  CFE_SB_PipeId_t  PipeId;
  uint32           PipeDepth = 10;

  fprintf(UT_logfile, "Begin Test for Rcv - Invalid Timeout Rcv\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"RcvMsgTstPipe1");
  TestStat = CFE_PASS;
  ActRtn = CFE_SB_RcvMsg(&PtrToMsg,PipeId,-5);
  ExpRtn = CFE_SB_BAD_ARGUMENT;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in Invalid Timeout Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_RCV_BAD_ARG_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_RCV_BAD_ARG_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest(); 
  UT_Report(TestStat, "Test_RcvMsg_API","Invalid Timeout Test\n");  
}/* end Test_RcvMsg_InvalidTimeout */



void Test_RcvMsg_Poll(void){

  int32            TestStat,ExpRtn,ActRtn;
  CFE_SB_MsgPtr_t  PtrToMsg;
  CFE_SB_PipeId_t  PipeId;
  uint32           PipeDepth = 10;

  fprintf(UT_logfile, "Begin Test for Poll Rcv\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"RcvMsgTstPipe1");

  TestStat = CFE_PASS;
  ActRtn = CFE_SB_RcvMsg(&PtrToMsg,PipeId,CFE_SB_POLL);
  ExpRtn = CFE_SB_NO_MESSAGE;
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Poll Receive Test,exp=%lx,act=%lx\n",
                                                ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }

  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_RcvMsg_API","Poll Rcv Test\n");  

}/* end Test_RcvMsg_Poll */


void Test_RcvMsg_GetLastSenderNull(void){

  int32            TestStat,ExpRtn,ActRtn;
  CFE_SB_PipeId_t  PipeId;
  uint32           PipeDepth = 10;

  fprintf(UT_logfile, "Begin Test for GetLastSender Null Ptr\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"RcvMsgTstPipe1");

  TestStat = CFE_PASS;
  ActRtn = CFE_SB_GetLastSenderId(NULL,PipeId);
  ExpRtn = CFE_SB_BAD_ARGUMENT;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in GetLastSenderId Null PtrTest,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_LSTSNDER_ERR1_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_LSTSNDER_ERR1_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_RcvMsg_API","GetLastSenderId Null Ptr Test\n");  

}/* end Test_RcvMsg_GetLastSenderNull */


void Test_RcvMsg_GetLastSenderInvalidPipe(void){

  int32            TestStat,ExpRtn,ActRtn;
  CFE_SB_PipeId_t  PipeId;
  CFE_SB_PipeId_t  InvalidPipeId = 250;
  uint32           PipeDepth = 10;
  CFE_SB_SenderId_t *GLSPtr;


  fprintf(UT_logfile, "Begin Test for GetLastSender Invalid Pipe\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"RcvMsgTstPipe1");

  TestStat = CFE_PASS;
  ActRtn = CFE_SB_GetLastSenderId(&GLSPtr, InvalidPipeId);
  ExpRtn = CFE_SB_BAD_ARGUMENT;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in GetLastSenderId Inv Pipe Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_LSTSNDER_ERR2_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_LSTSNDER_ERR2_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_RcvMsg_API","GetLastSenderId Invalid Pipe Test\n");
  
}/* end Test_RcvMsg_GetLastSenderInvalidPipe */


void Test_RcvMsg_GetLastSenderInvalidCaller(void){

  int32            TestStat,ExpRtn,ActRtn;
  CFE_SB_PipeId_t  PipeId;
  uint32           PipeDepth = 10;
  CFE_SB_SenderId_t *GLSPtr;
  uint32            OrigPipeOwner;

  fprintf(UT_logfile, "Begin Test for GetLastSender Invalid Caller\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"RcvMsgTstPipe1");

  /* change pipe owner id to execute 'invalid caller' code */
  OrigPipeOwner = CFE_SB.PipeTbl[PipeId].AppId;
  CFE_SB.PipeTbl[PipeId].AppId = 100;
  TestStat = CFE_PASS;
  ActRtn = CFE_SB_GetLastSenderId(&GLSPtr,PipeId);
  ExpRtn = CFE_SB_BAD_ARGUMENT;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in GetLastSenderId Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_GLS_INV_CALLER_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_GLS_INV_CALLER_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  /* restore original pipe owner apid */
  CFE_SB.PipeTbl[PipeId].AppId = OrigPipeOwner;

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_RcvMsg_API","GetLastSenderId Invalid Caller Test\n");
  
}/* end Test_RcvMsg_GetLastSenderInvalidCaller */


void Test_RcvMsg_GetLastSenderSuccess(void){

  int32            TestStat,ExpRtn,ActRtn;
  CFE_SB_PipeId_t  PipeId;
  uint32           PipeDepth = 10;
  CFE_SB_SenderId_t *GLSPtr;

  fprintf(UT_logfile, "Begin Test for GetLastSender Success\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"RcvMsgTstPipe1");

  TestStat = CFE_PASS;
  ActRtn = CFE_SB_GetLastSenderId(&GLSPtr,PipeId);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in GetLastSenderId Success Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_RcvMsg_API","GetLastSenderId Success Test\n");
  
}/* end Test_RcvMsg_GetLastSenderSuccess */


void Test_RcvMsg_Timeout(void){

  int32            TestStat,ExpRtn,ActRtn;
  CFE_SB_MsgPtr_t  PtrToMsg;
  CFE_SB_PipeId_t  PipeId;
  uint32           PipeDepth = 10;

  fprintf(UT_logfile, "Begin Test for Timeout Rcv\n");
  SB_ResetUnitTest();  
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"RcvMsgTstPipe1");

  TestStat = CFE_PASS;
  ActRtn = CFE_SB_RcvMsg(&PtrToMsg,PipeId,200);
  ExpRtn = CFE_SB_TIME_OUT;
  if(ActRtn != ExpRtn){
      fprintf(UT_logfile, "Unexpected return in Timeout Receive Test,exp=%lx,act=%lx\n",
                                              ExpRtn,ActRtn);
      TestStat = CFE_FAIL;
  }

  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_RcvMsg_API","Timeout Rcv Test\n");
    
}/* end Test_RcvMsg_Timeout */


void Test_RcvMsg_PipeReadError(void){

  int32            TestStat,ExpRtn,ActRtn;
  int32            Val2Restore;
  CFE_SB_MsgPtr_t  PtrToMsg;
  CFE_SB_PipeId_t  PipeId;
  uint32           PipeDepth = 10;

  fprintf(UT_logfile, "Begin Test for Pipe Read Error\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"RcvMsgTstPipe1");
  TestStat = CFE_PASS;
  /* corrupt the system queueID to get OS_QueueGet to return an error */
  Val2Restore = CFE_SB.PipeTbl[PipeId].SysQueueId;
  CFE_SB.PipeTbl[PipeId].SysQueueId = 100;
  ActRtn = CFE_SB_RcvMsg(&PtrToMsg,PipeId,CFE_SB_PEND_FOREVER);
  ExpRtn = CFE_SB_PIPE_RD_ERR;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return in Pipe Read Error Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_Q_RD_ERR_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_Q_RD_ERR_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  /* restore system queue id */
  CFE_SB.PipeTbl[PipeId].SysQueueId = Val2Restore;

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_RcvMsg_API","Pipe Read Error Test\n");
  
}/* end Test_RcvMsg_PipeReadError */


void Test_RcvMsg_PendForever(void){

  int32            TestStat,ExpRtn,ActRtn;
  CFE_SB_MsgPtr_t  PtrToMsg;
  CFE_SB_PipeId_t  PipeId;
  uint32           PipeDepth = 10;
  SB_UT_Test_Tlm_t TlmPkt;
  CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t)&TlmPkt;
  CFE_SB_PipeD_t         *PipeDscPtr;

  fprintf(UT_logfile, "Begin Test for Pend Forever\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"RcvMsgTstPipe1");
  CFE_SB_InitMsg(&TlmPkt,0x0809,sizeof(SB_UT_Test_Tlm_t),TRUE);
  CFE_SB_Subscribe(0x0809,PipeId);

  TestStat = CFE_PASS;
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Send in Pend Forever Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  ActRtn = CFE_SB_RcvMsg(&PtrToMsg,PipeId,CFE_SB_PEND_FOREVER);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Rcv in Pend Forever Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  if(PtrToMsg != NULL)
    fprintf(UT_logfile, "Received Msg 0x%x\n",CFE_SB_GetMsgId(PtrToMsg));

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }  
  
  PipeDscPtr = CFE_SB_GetPipePtr(PipeId);
  PipeDscPtr->ToTrashBuff = PipeDscPtr->CurrentBuff;
  PipeDscPtr->CurrentBuff = NULL;

  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_RcvMsg_API","Pend Forever Test\n");
  
}/* end Test_RcvMsg_PendForever */



void Test_RcvMsg_MsgLimCntrErr(void){
  
  int32            TestStat,ExpRtn,ActRtn;
  CFE_SB_MsgPtr_t  PtrToMsg;
  CFE_SB_PipeId_t  PipeId;
  uint32           PipeDepth = 10;
  SB_UT_Test_Tlm_t TlmPkt;
  CFE_SB_MsgPtr_t  TlmPktPtr = (CFE_SB_MsgPtr_t)&TlmPkt;
  CFE_SB_DestinationD_t  *DestPtr;

  fprintf(UT_logfile, "Begin Test for MsgLimCntrErr\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId,PipeDepth,"RcvMsgTstPipe1");
  CFE_SB_InitMsg(&TlmPkt,0x0809,sizeof(SB_UT_Test_Tlm_t),TRUE);
  CFE_SB_Subscribe(0x0809,PipeId);

  TestStat = CFE_PASS;
  ActRtn = CFE_SB_SendMsg(TlmPktPtr);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Send in MsgLimCntrErr Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  /* corrupt the buffcount to cause the msg limit counter error */
  DestPtr = CFE_SB_GetDestPtr(0x0809,PipeId);
  DestPtr -> BuffCount = 0;

  ActRtn = CFE_SB_RcvMsg(&PtrToMsg,PipeId,CFE_SB_PEND_FOREVER);
  ExpRtn = CFE_SUCCESS;
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected return from Rcv in MsgLimCntrErr Test,exp=%lx,act=%lx\n",
                        ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  if(PtrToMsg != NULL)
    fprintf(UT_logfile, "Received Msg 0x%x\n",CFE_SB_GetMsgId(PtrToMsg));

  ExpRtn = 3;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_MSGCNT_ERR1_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_MSGCNT_ERR1_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_RcvMsg_API","MsgLimCntrErr Test\n");
  
}/* end Test_RcvMsg_MsgLimCntrErr */




void Test_CleanupApp_API(void){
  
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t      PipeId;
  uint32               ZeroCpyBufHndl = 0;

  fprintf(UT_logfile, "\nBegin Test for CleanupApp\n");
  SB_ResetUnitTest();
  CFE_SB_CreatePipe(&PipeId,50,"TestPipe2");

  CFE_SB_ZeroCopyGetPtr(50, &ZeroCpyBufHndl);  
  CFE_SB_ZeroCopyGetPtr(50, &ZeroCpyBufHndl);  

  if(CFE_SB.ZeroCopyTail == NULL){
    fprintf(UT_logfile, "Unexpected ZeroCopyTail UT_GetNumEventsSent,exp = not NULL");
    TestStat = CFE_FAIL;
  }

  CFE_SB_CleanUpApp(0);

  if(CFE_SB.ZeroCopyTail != NULL){
    fprintf(UT_logfile, "Unexpected ZeroCopyTail UT_GetNumEventsSent,exp = NULL");
    TestStat = CFE_FAIL;
  }

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_PIPE_DELETED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_DELETED_EID not sent\n");
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_CleanupApp_API","CleanupApp Test\n");
  
}/* end Test_CleanupApp_API */







/*
** Test SB Utility APIs
*/
void Test_SB_Utils(void)
{

  fprintf(UT_logfile, "\n\nBegin Test for SB Utils");

  Test_CFE_SB_InitMsg();
  Test_CFE_SB_MsgHdrSize();
  Test_CFE_SB_GetUserData();
  Test_CFE_SB_SetGetMsgId();
  Test_CFE_SB_SetGetUserDataLength();
  Test_CFE_SB_SetGetTotalMsgLength();
  Test_CFE_SB_SetGetMsgTime();
  Test_CFE_SB_TimeStampMsg();
  Test_CFE_SB_SetGetCmdCode();
  Test_CFE_SB_ChecksumUtils();
  fprintf(UT_logfile,"\n");

}/* end Test_SB_Utils */


void Test_CFE_SB_InitMsg(void){

    fprintf(UT_logfile, "\n\nStart Test_CFE_SB_InitMsg");
    SB_ResetUnitTest();
    Test_CFE_SB_InitMsg_True();
    Test_CFE_SB_InitMsg_False();
    fprintf(UT_logfile, "End Test_CFE_SB_InitMsg\n\n");

}/* end Test_CFE_SB_InitMsg */

void Test_CFE_SB_InitMsg_True(void)
{
    SB_UT_Test_Cmd_t   SBCmd;
    CFE_SB_MsgPtr_t    SBCmdPtr = (CFE_SB_MsgPtr_t)&SBCmd;

    fprintf(UT_logfile, "\nBegin SB_TestInitMsg_True\n");
    SB_ResetUnitTest();
    /* set entire cmd packet to all F's */
    memset(SBCmdPtr,0xFF,sizeof(SB_UT_Test_Cmd_t));

    /* print out entire packet */
    fprintf(UT_logfile, "After setting pkt to all F's\n");
    UT_DisplayPkt(SBCmdPtr, sizeof(SB_UT_Test_Cmd_t));

    fprintf(UT_logfile,"Calling CFE_SB_InitMsg w/TRUE as 4th param\n");
    CFE_SB_InitMsg(SBCmdPtr,CFE_SB_CMD_MID,sizeof(SB_UT_Test_Cmd_t),TRUE);
    fprintf(UT_logfile, "Manually verify that all data bytes set to zero\n");
    UT_DisplayPkt(SBCmdPtr, sizeof(SB_UT_Test_Cmd_t));

    UT_Report(CFE_PASS, "SB_TestInitMsg_True", "4th Param TRUE");
    SB_ResetUnitTest();
    fprintf(UT_logfile,"End SB_TestInitMsg_True test\n");

}/* end Test_CFE_SB_InitMsg_True */


void Test_CFE_SB_InitMsg_False(void)
{

    SB_UT_Test_Cmd_t   SBCmd;
    CFE_SB_MsgPtr_t    SBCmdPtr = (CFE_SB_MsgPtr_t)&SBCmd;


    fprintf(UT_logfile, "\nBegin SB_TestInitMsg_False\n");
    SB_ResetUnitTest();
    /* set entire cmd packet to all F's */
    memset(SBCmdPtr,0xFF,sizeof(SB_UT_Test_Cmd_t));

    /* print out entire packet */
    fprintf(UT_logfile, "After setting pkt to all F's\n");
    UT_DisplayPkt(SBCmdPtr, sizeof(SB_UT_Test_Cmd_t));

    fprintf(UT_logfile,"Calling CFE_SB_InitMsg w/FALSE as 4th param\n");
    CFE_SB_InitMsg(SBCmdPtr,CFE_SB_CMD_MID,sizeof(SB_UT_Test_Cmd_t),FALSE);
    fprintf(UT_logfile, "Manually verify that all data bytes set to FF\n");
    UT_DisplayPkt(SBCmdPtr, sizeof(SB_UT_Test_Cmd_t));

    UT_Report(CFE_PASS, "SB_TestInitMsg_False", "4th Param FALSE");
    SB_ResetUnitTest();
    fprintf(UT_logfile,"End SB_TestInitMsg_False test\n");

}/* end Test_CFE_SB_InitMsg_False */



void Test_CFE_SB_MsgHdrSize(void){

    int32 TestStat;

    fprintf(UT_logfile, "Begin Test_CFE_SB_MsgHdrSize\n");
    SB_ResetUnitTest();

    /* Test for Cmds w/Sec Hdr */
    TestStat = CFE_PASS;
    if(CFE_SB_MsgHdrSize(CFE_SB_CMD_MID) != 8){
        TestStat = CFE_FAIL;
    }
    UT_Report(TestStat, "Test_CFE_SB_MsgHdrSize", "Cmds w/Sec Hdr");

    /* Test for Cmds wo/Sec Hdr */
    TestStat = CFE_PASS;
    if(CFE_SB_MsgHdrSize(0x1005) != 6){
        TestStat = CFE_FAIL;
    }
    UT_Report(TestStat, "Test_CFE_SB_MsgHdrSize", "Cmds wo/Sec Hdr");

    /* Test for Tlm w/Sec Hdr */
    TestStat = CFE_PASS;
    if(CFE_SB_MsgHdrSize(CFE_ES_HK_TLM_MID) != 12){
        TestStat = CFE_FAIL;
    }
    UT_Report(TestStat, "Test_CFE_SB_MsgHdrSize", "Tlm w/Sec Hdr");

    /* Test for Tlm wo/Sec Hdr */
    TestStat = CFE_PASS;
    if(CFE_SB_MsgHdrSize(0x0005) != 6){
        TestStat = CFE_FAIL;
    }
    UT_Report(TestStat, "Test_CFE_SB_MsgHdrSize", "Tlm wo/Sec Hdr");

    fprintf(UT_logfile, "End Test_CFE_SB_MsgHdrSize\n\n");

}/* end Test_CFE_SB_MsgHdrSize */


void Test_CFE_SB_GetUserData(void){

  SB_UT_Test_Cmd_t        SBCmd;
  CFE_SB_MsgPtr_t         SBCmdPtr = (CFE_SB_MsgPtr_t)&SBCmd;
  SB_UT_Test_Tlm_t        SBTlm;
  CFE_SB_MsgPtr_t         SBTlmPtr = (CFE_SB_MsgPtr_t)&SBTlm;
  SB_UT_TstPktWoSecHdr_t  SBNoSecHdrPkt;
  CFE_SB_MsgPtr_t         SBNoSecHdrPktPtr = (CFE_SB_MsgPtr_t)&SBNoSecHdrPkt;
  int32                   TestStat;
  uint8                   *ActualAdrReturned;
  uint8                   *ExpAdrReturned;

  fprintf(UT_logfile, "Begin Test_CFE_SB_GetUserData\n");
  SB_ResetUnitTest();

  /* Test Address returned for Cmd Pkts w/Sec Hdr */
  TestStat = CFE_PASS;
  CFE_SB_SetMsgId(SBCmdPtr,0x1805);
  ActualAdrReturned = CFE_SB_GetUserData(SBCmdPtr);
  ExpAdrReturned = (uint8 *)SBCmdPtr + 8;
    if(ActualAdrReturned != ExpAdrReturned){
        fprintf(UT_logfile, "Adr of data for Cmds w/Sec hdr is Pkt Adr + 8\n");
        fprintf(UT_logfile, "PktAdr %p,Retrnd %p,Exp %p\n",(void *)SBCmdPtr,
           ActualAdrReturned,ExpAdrReturned);
        TestStat = CFE_FAIL;
    }/* end if */
  UT_Report(TestStat, "Test_CFE_SB_GetUserData","Cmd Pkt w/Sec Hdr Test");

  /* Test Address returned for Cmd Pkts wo/Sec Hdr */
  TestStat = CFE_PASS;
  CFE_SB_SetMsgId(SBNoSecHdrPktPtr,0x1005);
  ActualAdrReturned = CFE_SB_GetUserData(SBNoSecHdrPktPtr);
  ExpAdrReturned = (uint8 *)SBNoSecHdrPktPtr + 6;
    if(ActualAdrReturned != ExpAdrReturned){
        fprintf(UT_logfile, "Adr of data for Cmds wo/Sec hdr is Pkt Adr + 6\n");
        fprintf(UT_logfile, "PktAdr %p,Retrnd %p,Exp %p\n",
           (void *)SBNoSecHdrPktPtr,ActualAdrReturned,ExpAdrReturned);
        TestStat = CFE_FAIL;
    }/* end if */
  UT_Report(TestStat, "Test_CFE_SB_GetUserData", "Cmd Pkt wo/Sec Hdr Test");

  /* Test Address returned for Tlm Pkts w/Sec Hdr */
  TestStat = CFE_PASS;
  CFE_SB_SetMsgId(SBTlmPtr,0x0805);
  ActualAdrReturned = CFE_SB_GetUserData(SBTlmPtr);
  ExpAdrReturned = (uint8 *)SBTlmPtr + 12;
    if(ActualAdrReturned != ExpAdrReturned){
        fprintf(UT_logfile, "Adr of data for Tlm Pkts w/Sec hdr is Pkt Adr+12\n");
        fprintf(UT_logfile, "PktAdr %p,Retrnd %p,Exp %p\n",(void *)SBTlmPtr,
            ActualAdrReturned,ExpAdrReturned);
        TestStat = CFE_FAIL;
    }/* end if */
  UT_Report(TestStat, "Test_CFE_SB_GetUserData","Tlm Pkt w/Sec Hdr Test");

  /* Test Address returned for Tlm Pkts wo/Sec Hdr */
  TestStat = CFE_PASS;
  CFE_SB_SetMsgId(SBNoSecHdrPktPtr,0x0005);
  ActualAdrReturned = CFE_SB_GetUserData(SBNoSecHdrPktPtr);
  ExpAdrReturned = (uint8 *)SBNoSecHdrPktPtr + 6;
    if(ActualAdrReturned != ExpAdrReturned){
        fprintf(UT_logfile, "Adr of data for Tlm Pkts wo/Sec hdr is Pkt Adr+6\n");
        fprintf(UT_logfile, "PktAdr %p,Retrnd %p,Exp %p\n",
            (void *)SBNoSecHdrPktPtr,ActualAdrReturned,ExpAdrReturned);
    TestStat = CFE_FAIL;
    }/* end if */
  UT_Report(TestStat, "Test_CFE_SB_GetUserData","Tlm Pkt wo/Sec Hdr Test");

  fprintf(UT_logfile, "End Test_CFE_SB_GetUserData\n");

}/* end Test_CFE_SB_GetUserData */


void Test_CFE_SB_SetGetMsgId(void){

  SB_UT_Test_Cmd_t      SBCmd;
  CFE_SB_MsgPtr_t       SBCmdPtr = (CFE_SB_MsgPtr_t)&SBCmd;
  CFE_SB_MsgId_t        MsgIdReturned;
  uint32                TestStat = CFE_PASS;
  uint32                i;

  fprintf(UT_logfile, "\nBegin Test_CFE_SB_SetGetMsgId\n");
  SB_ResetUnitTest();
  /* set entire cmd packet to all F's */
  memset(SBCmdPtr,0xFF,sizeof(SB_UT_Test_Cmd_t));
  CFE_SB_SetMsgId(SBCmdPtr,CFE_SB_CMD_MID);
  MsgIdReturned = CFE_SB_GetMsgId(SBCmdPtr);

  if(MsgIdReturned != CFE_SB_CMD_MID){
      TestStat = CFE_FAIL;
      fprintf(UT_logfile,"CFE_SB_GetMsgId Returned 0x%04x, expected 0x%04x\n",
              MsgIdReturned,CFE_SB_CMD_MID);
  }
  UT_Report(TestStat, "Test_CFE_SB_SetGetMsgId", "One SetGet Test");

  /* Looping through every value from 0 to 0xFFFF */
  for(i=0;i<65536;i++){
        CFE_SB_SetMsgId(SBCmdPtr,i);
        if(CFE_SB_GetMsgId(SBCmdPtr)!=i){
            UT_Report(CFE_FAIL, "Test_CFE_SB_SetGetMsgId", "Loop Test");
            fprintf(UT_logfile, "End Test_CFE_SB_SetGetMsgId\n");
            return;
        }/* end if */
  }/* end for */

  UT_Report(CFE_PASS, "Test_CFE_SB_SetGetMsgId", "Loop Test");

  fprintf(UT_logfile, "End Test_CFE_SB_SetGetMsgId\n");

}/* end Test_CFE_SB_SetGetMsgId */


void Test_CFE_SB_SetGetUserDataLength(void){

  SB_UT_Test_Cmd_t        SBCmd;
  CFE_SB_MsgPtr_t         SBCmdPtr = (CFE_SB_MsgPtr_t)&SBCmd;
  SB_UT_Test_Tlm_t        SBTlm;
  CFE_SB_MsgPtr_t         SBTlmPtr = (CFE_SB_MsgPtr_t)&SBTlm;
  SB_UT_TstPktWoSecHdr_t  SBNoSecHdrPkt;
  CFE_SB_MsgPtr_t         SBNoSecHdrPktPtr = (CFE_SB_MsgPtr_t)&SBNoSecHdrPkt;
  int32                   SetSize,TestStat;
  uint16                  SizeReturned;
  int16                   ActualPktLenField;
  int16                   ExpPktLenField;

  fprintf(UT_logfile, "\nBegin Test_CFE_SB_SetGetUserDataLength\n");
  SB_ResetUnitTest();
  /* CCSDS Pkt Length field = SecHdrSize + Data - 1 */

  /* Loop through all pkt length values for Cmd Pkts w/Sec Hdr */
  TestStat = CFE_PASS;
  CFE_SB_SetMsgId(SBCmdPtr,0x1805);
  for (SetSize=0;SetSize<65536;SetSize++){
        CFE_SB_SetUserDataLength(SBCmdPtr,SetSize);
        SizeReturned = CFE_SB_GetUserDataLength(SBCmdPtr);
        ActualPktLenField = UT_GetActualPktLenField(SBCmdPtr);
        ExpPktLenField = (2 + SetSize - 1);/* SecHdrSize + Data - 1 */
        if((SizeReturned != SetSize)||(ActualPktLenField != ExpPktLenField)){
            fprintf(UT_logfile, "SzRet=%d,SetSz=%d,ActPL=%d,ExpPL=%d\n",
                    SizeReturned,(int)SetSize,ActualPktLenField,ExpPktLenField);
            TestStat = CFE_FAIL;
          break;
        }/* end if */
  }/* end for */
  UT_Report(TestStat, "Test_CFE_SB_SetGetUserDataLength",
                            "Cmd Pkt w/Sec Hdr Test");

  /* Loop through all pkt length values for Cmd Pkts wo/Sec Hdr */
  TestStat = CFE_PASS;
  CFE_SB_SetMsgId(SBNoSecHdrPktPtr,0x1005);
  for (SetSize=0;SetSize<65536;SetSize++){
        CFE_SB_SetUserDataLength(SBNoSecHdrPktPtr,SetSize);
        SizeReturned = CFE_SB_GetUserDataLength(SBNoSecHdrPktPtr);
        ActualPktLenField = UT_GetActualPktLenField(SBNoSecHdrPktPtr);
        ExpPktLenField = (0 + SetSize - 1);/* SecHdrSize + Data - 1 */
        if((SizeReturned != SetSize)||(ActualPktLenField != ExpPktLenField)){
            fprintf(UT_logfile, "SzRet=%d,SetSz=%d,ActPL=%dExpPL=%d\n",
                    SizeReturned,(int)SetSize,ActualPktLenField,ExpPktLenField);
            TestStat = CFE_FAIL;
          break;
        }/* end if */
  }/* end for */
  UT_Report(TestStat, "Test_CFE_SB_SetGetUserDataLength",
                          "Cmd Pkt wo/Sec Hdr Test");

  /* Loop through all pkt length values for Tlm Pkts w/Sec Hdr */
  TestStat = CFE_PASS;
  CFE_SB_SetMsgId(SBTlmPtr,0x0805);
  for (SetSize=0;SetSize<65536;SetSize++){
        CFE_SB_SetUserDataLength(SBTlmPtr,SetSize);
        SizeReturned = CFE_SB_GetUserDataLength(SBTlmPtr);
        ActualPktLenField = UT_GetActualPktLenField(SBTlmPtr);
        ExpPktLenField = (6 + SetSize - 1);/* SecHdrSize + Data - 1 */
        if((SizeReturned != SetSize)||(ActualPktLenField != ExpPktLenField)){
            fprintf(UT_logfile, "SzRet=%d,SetSz=%d,ActPL=%dExpPL=%d\n",
                    SizeReturned,(int)SetSize,ActualPktLenField,ExpPktLenField);
            TestStat = CFE_FAIL;
          break;
        }/* end if */
  }/* end for */
  UT_Report(TestStat, "Test_CFE_SB_SetGetUserDataLength",
                            "Tlm Pkt w/Sec Hdr Test");

  /* Loop through all pkt length values for Tlm Pkts wo/Sec Hdr */
  TestStat = CFE_PASS;
  CFE_SB_SetMsgId(SBNoSecHdrPktPtr,0x0005);
  for (SetSize=0;SetSize<65536;SetSize++){
        CFE_SB_SetUserDataLength(SBNoSecHdrPktPtr,SetSize);
        SizeReturned = CFE_SB_GetUserDataLength(SBNoSecHdrPktPtr);
        ActualPktLenField = UT_GetActualPktLenField(SBNoSecHdrPktPtr);
        ExpPktLenField = (0 + SetSize - 1);/* SecHdrSize + Data - 1 */
        if((SizeReturned != SetSize)||(ActualPktLenField != ExpPktLenField)){
            fprintf(UT_logfile, "SzRet=%d,SetSz=%d,ActPL=%dExpPL=%d\n",
                    SizeReturned,(int)SetSize,ActualPktLenField,ExpPktLenField);
            TestStat = CFE_FAIL;
          break;
        }/* end if */
  }/* end for */
  UT_Report(TestStat, "Test_CFE_SB_SetGetUserDataLength",
                          "Tlm Pkt wo/Sec Hdr Test");

  fprintf(UT_logfile, "End Test_CFE_SB_SetGetUserDataLength\n");

}/* end Test_CFE_SB_SetGetUserDataLength */


void Test_CFE_SB_SetGetTotalMsgLength(void){

  SB_UT_Test_Cmd_t        SBCmd;
  CFE_SB_MsgPtr_t         SBCmdPtr = (CFE_SB_MsgPtr_t)&SBCmd;
  SB_UT_Test_Tlm_t        SBTlm;
  CFE_SB_MsgPtr_t         SBTlmPtr = (CFE_SB_MsgPtr_t)&SBTlm;
  SB_UT_TstPktWoSecHdr_t  SBNoSecHdrPkt;
  CFE_SB_MsgPtr_t         SBNoSecHdrPktPtr = (CFE_SB_MsgPtr_t)&SBNoSecHdrPkt;
  int32                   SetSize,TestStat;
  uint16                  TotalMsgSizeReturned;
  int16                   ActualPktLenField;
  int16                   ExpPktLenField;

  fprintf(UT_logfile, "\nBegin Test_CFE_SB_SetGetTotalMsgLength\n");
  SB_ResetUnitTest();
  /* CCSDS Pkt Length field = TotalPktSize - 7 */

  /* Loop through all pkt length values for Cmd Pkts w/Sec Hdr */
  TestStat = CFE_PASS;
  CFE_SB_SetMsgId(SBCmdPtr,0x1805);
  for (SetSize=0;SetSize<65536;SetSize++){
      CFE_SB_SetTotalMsgLength(SBCmdPtr,SetSize);
      TotalMsgSizeReturned = CFE_SB_GetTotalMsgLength(SBCmdPtr);
      ActualPktLenField = UT_GetActualPktLenField(SBCmdPtr);
      ExpPktLenField = (SetSize - 7);/* TotalPktSize - 7 */
      if((TotalMsgSizeReturned != SetSize)||
         (ActualPktLenField != ExpPktLenField)){
          fprintf(UT_logfile, "SzRet=%d,SetSz=%d,ActPL=%dExpPL=%d\n",
           TotalMsgSizeReturned,(int)SetSize,ActualPktLenField,ExpPktLenField);
          TestStat = CFE_FAIL;
        break;
      }/* end if */
  }/* end for */
  UT_Report(TestStat, "Test_CFE_SB_SetGetTotalMsgLength",
                            "Cmd Pkt w/Sec Hdr Test");

  /* Loop through all pkt length values for Cmd Pkts wo/Sec Hdr */
  TestStat = CFE_PASS;
  CFE_SB_SetMsgId(SBNoSecHdrPktPtr,0x1005);
  for (SetSize=0;SetSize<65536;SetSize++){
      CFE_SB_SetTotalMsgLength(SBNoSecHdrPktPtr,SetSize);
      TotalMsgSizeReturned = CFE_SB_GetTotalMsgLength(SBNoSecHdrPktPtr);
      ActualPktLenField = UT_GetActualPktLenField(SBNoSecHdrPktPtr);
      ExpPktLenField = (SetSize - 7);/* TotalPktSize - 7 */
      if((TotalMsgSizeReturned != SetSize)||(ActualPktLenField != ExpPktLenField)){
          fprintf(UT_logfile, "SzRet=%d,SetSz=%d,ActPL=%dExpPL=%d\n",
            TotalMsgSizeReturned,(int)SetSize,ActualPktLenField,ExpPktLenField);
          TestStat = CFE_FAIL;
        break;
      }/* end if */
  }/* end for */
  UT_Report(TestStat, "Test_CFE_SB_SetGetTotalMsgLength",
                          "Cmd Pkt wo/Sec Hdr Test");

  /* Loop through all pkt length values for Tlm Pkts w/Sec Hdr */
  TestStat = CFE_PASS;
  CFE_SB_SetMsgId(SBTlmPtr,0x0805);
  for (SetSize=0;SetSize<65536;SetSize++){
      CFE_SB_SetTotalMsgLength(SBTlmPtr,SetSize);
      TotalMsgSizeReturned = CFE_SB_GetTotalMsgLength(SBTlmPtr);
      ActualPktLenField = UT_GetActualPktLenField(SBTlmPtr);
      ExpPktLenField = (SetSize - 7);/* TotalPktSize - 7 */
      if((TotalMsgSizeReturned != SetSize)||(ActualPktLenField != ExpPktLenField)){
          fprintf(UT_logfile, "SzRet=%d,SetSz=%d,ActPL=%dExpPL=%d\n",
            TotalMsgSizeReturned,(int)SetSize,ActualPktLenField,ExpPktLenField);
          TestStat = CFE_FAIL;
        break;
      }/* end if */
  }/* end for */
  UT_Report(TestStat, "Test_CFE_SB_SetGetTotalMsgLength",
                            "Tlm Pkt w/Sec Hdr Test");

  /* Loop through all pkt length values for Tlm Pkts wo/Sec Hdr */
  TestStat = CFE_PASS;
  CFE_SB_SetMsgId(SBNoSecHdrPktPtr,0x0005);
  for (SetSize=0;SetSize<65536;SetSize++){
      CFE_SB_SetTotalMsgLength(SBNoSecHdrPktPtr,SetSize);
      TotalMsgSizeReturned = CFE_SB_GetTotalMsgLength(SBNoSecHdrPktPtr);
      ActualPktLenField = UT_GetActualPktLenField(SBNoSecHdrPktPtr);
      ExpPktLenField = (SetSize - 7);/* TotalPktSize - 7 */
      if((TotalMsgSizeReturned != SetSize)||(ActualPktLenField != ExpPktLenField)){
          fprintf(UT_logfile, "SzRet=%d,SetSz=%d,ActPL=%dExpPL=%d\n",
            TotalMsgSizeReturned,(int)SetSize,ActualPktLenField,ExpPktLenField);
          TestStat = CFE_FAIL;
        break;
      }/* end if */
  }/* end for */
  UT_Report(TestStat, "Test_CFE_SB_SetGetTotalMsgLength",
                          "Tlm Pkt wo/Sec Hdr Test");

  fprintf(UT_logfile, "End Test_CFE_SB_SetGetTotalMsgLength\n");

}/* end Test_CFE_SB_SetGetTotalMsgLength */


void Test_CFE_SB_SetGetMsgTime(void){

  SB_UT_Test_Cmd_t          SBCmd;
  CFE_SB_MsgPtr_t           SBCmdPtr = (CFE_SB_MsgPtr_t)&SBCmd;
  SB_UT_Test_Tlm_t          SBTlm;
  CFE_SB_MsgPtr_t           SBTlmPtr = (CFE_SB_MsgPtr_t)&SBTlm;
  SB_UT_TstPktWoSecHdr_t    SBNoSecHdrPkt;
  CFE_SB_MsgPtr_t           SBNoSecHdrPktPtr = (CFE_SB_MsgPtr_t)&SBNoSecHdrPkt;
  CFE_TIME_SysTime_t                SetTime,GetTime;
  int32                                     RtnFromSet,TestStat;

  fprintf(UT_logfile, "\nBegin Test_CFE_SB_SetGetMsgTime\n");
  SB_ResetUnitTest();

  /* Begin Test for Cmd Pkts w/Sec Hdr */
  TestStat = CFE_PASS;
  /* Setting pkt to all F's */
  memset(SBCmdPtr,0xFF,sizeof(SB_UT_Test_Cmd_t));
  /* Setting MsgId to 0x1805 */
  CFE_SB_SetMsgId(SBCmdPtr,0x1805);
  SetTime.Seconds = 0x4321;
  SetTime.Subseconds = 0x8765;
  RtnFromSet = CFE_SB_SetMsgTime(SBCmdPtr,SetTime);
  GetTime.Seconds = 0xFFFF;
  GetTime.Subseconds = 0xFFFF;
  GetTime = CFE_SB_GetMsgTime(SBCmdPtr);
  /* Verify CFE_SB_SetMsgTime returns CFE_SB_WRONG_MSG_TYPE for Cmd Pkts w/Sec Hdr */
  if(RtnFromSet != CFE_SB_WRONG_MSG_TYPE){
      fprintf(UT_logfile, "RtnFromSet=%ld, ExpReturn=0x%x\n",RtnFromSet,
        (unsigned int)CFE_SB_WRONG_MSG_TYPE);
      TestStat = CFE_FAIL;
      UT_DisplayPkt(SBCmdPtr, sizeof(SB_UT_Test_Cmd_t));
  /* Verfiy the call to CFE_SB_GetMsgTime returns a Time value of zero */
  }else if((GetTime.Seconds != 0)||(GetTime.Subseconds != 0)){
      fprintf(UT_logfile,"GetTime.Sec=%d,GetTime.Subsec=%d\n",
        (int)GetTime.Seconds,(int)GetTime.Subseconds);
      TestStat = CFE_FAIL;
      UT_DisplayPkt(SBCmdPtr, sizeof(SB_UT_Test_Cmd_t));
  }/* end if */
  UT_Report(TestStat, "Test_CFE_SB_SetGetMsgTime","Cmd Pkt w/Sec Hdr Test");

  /* Begin Test for Cmd Pkts wo/Sec Hdr */
  TestStat = CFE_PASS;
  /* Setting pkt to all F's */
  memset(SBNoSecHdrPktPtr,0xFF,sizeof(SB_UT_TstPktWoSecHdr_t));
  /* Setting MsgId to 0x1005 */
  CFE_SB_SetMsgId(SBNoSecHdrPktPtr,0x1005);
  SetTime.Seconds = 0x4321;
  SetTime.Subseconds = 0x8765;
  RtnFromSet = CFE_SB_SetMsgTime(SBNoSecHdrPktPtr,SetTime);
  GetTime.Seconds = 0xFFFF;
  GetTime.Subseconds = 0xFFFF;
  GetTime = CFE_SB_GetMsgTime(SBNoSecHdrPktPtr);
  /* Verify CFE_SB_SetMsgTime returns CFE_SB_WRONG_MSG_TYPE for Cmd Pkts wo/Sec Hdr */
  if(RtnFromSet != CFE_SB_WRONG_MSG_TYPE){
        fprintf(UT_logfile, "RtnFromSet=%ld, ExpReturn=0x%x\n",RtnFromSet,
            (unsigned int)CFE_SB_WRONG_MSG_TYPE);
    TestStat = CFE_FAIL;
    UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SB_UT_TstPktWoSecHdr_t));
  /* Verfiy the call to CFE_SB_GetMsgTime returns a Time value of zero */
  }else if((GetTime.Seconds != 0)||(GetTime.Subseconds != 0)){
      fprintf(UT_logfile,"GetTime.Sec=%d,GetTime.Subsec=%d\n",(int)GetTime.Seconds,(int)GetTime.Subseconds);
      TestStat = CFE_FAIL;
      UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SB_UT_TstPktWoSecHdr_t));
  }/* end if */
  UT_Report(TestStat, "Test_CFE_SB_SetGetMsgTime","Cmd Pkt wo/Sec Hdr Test");

  /* Begin Test for Tlm Pkts w/Sec Hdr */
  TestStat = CFE_PASS;
  /* Setting pkt to all F's */
  memset(SBTlmPtr,0xFF,sizeof(SB_UT_Test_Tlm_t));
  /* Setting MsgId to 0x0805 */
  CFE_SB_SetMsgId(SBTlmPtr,0x0805);
  SetTime.Seconds = 0x01234567;
  SetTime.Subseconds = 0x89ABCDEF;
  RtnFromSet = CFE_SB_SetMsgTime(SBTlmPtr,SetTime);
  GetTime.Seconds = 0xFFFF;
  GetTime.Subseconds = 0xFFFF;
  GetTime = CFE_SB_GetMsgTime(SBTlmPtr);
  /* Verify CFE_SB_SetMsgTime returns CFE_SUCCESS for Tlm Pkts w/Sec Hdr */
  if(RtnFromSet != CFE_SUCCESS){
        fprintf(UT_logfile, "RtnFromSet=%ld, ExpReturn=0\n",RtnFromSet);
        TestStat = CFE_FAIL;
        UT_DisplayPkt(SBTlmPtr, sizeof(SB_UT_Test_Tlm_t));
  /* Verfiy CFE_SB_GetMsgTime returns the SetTime value w/2 LSBytes of subseconds zeroed out */
  }else if((GetTime.Seconds != SetTime.Seconds)||(GetTime.Subseconds != (SetTime.Subseconds & 0xFFFF0000))){
      fprintf(UT_logfile,"SetTime.Sec=0x%X,GetTime.Sec=0x%X,SetTime.Subsec=0x%X,GetTime.Subsec=0x%X\n",
            (unsigned int)SetTime.Seconds,(unsigned int)GetTime.Seconds,
            (unsigned int)SetTime.Subseconds,(unsigned int)GetTime.Subseconds);
      TestStat = CFE_FAIL;
      UT_DisplayPkt(SBTlmPtr, sizeof(SB_UT_Test_Tlm_t));
  }/* end if */
  UT_Report(TestStat, "Test_CFE_SB_SetGetMsgTime","Tlm Pkt w/Sec Hdr Test");

  /* Begin Test for Tlm Pkts wo/Sec Hdr */
  TestStat = CFE_PASS;
  /* Setting pkt to all F's */
  memset(SBNoSecHdrPktPtr,0xFF,sizeof(SB_UT_TstPktWoSecHdr_t));
  /* Setting MsgId to 0x0005 */
  CFE_SB_SetMsgId(SBNoSecHdrPktPtr,0x0005);
  SetTime.Seconds = 0x01234567;
  SetTime.Subseconds = 0x89ABCDEF;
  RtnFromSet = CFE_SB_SetMsgTime(SBNoSecHdrPktPtr,SetTime);
  GetTime.Seconds = 0xFFFF;
  GetTime.Subseconds = 0xFFFF;
  GetTime = CFE_SB_GetMsgTime(SBNoSecHdrPktPtr);
  /* Verify CFE_SB_SetMsgTime returns CFE_SB_WRONG_MSG_TYPE for Tlm Pkts wo/Sec Hdr */
  if(RtnFromSet != CFE_SB_WRONG_MSG_TYPE){
      fprintf(UT_logfile, "RtnFromSet=%ld, ExpReturn=0x%x\n",RtnFromSet,(unsigned int)CFE_SB_WRONG_MSG_TYPE);
      TestStat = CFE_FAIL;
      UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SB_UT_TstPktWoSecHdr_t));
  /* Verfiy the call to CFE_SB_GetMsgTime returns a Time value of zero */
  }else if((GetTime.Seconds != 0)||(GetTime.Subseconds != 0)){
      fprintf(UT_logfile,"GetTime.Sec=%d,GetTime.Subsec=%d\n",(int)GetTime.Seconds,(int)GetTime.Subseconds);
      TestStat = CFE_FAIL;
      UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SB_UT_TstPktWoSecHdr_t));
  }/* end if */
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_CFE_SB_SetGetMsgTime","Tlm Pkt wo/Sec Hdr Test");

  fprintf(UT_logfile, "End Test_CFE_SB_SetGetMsgTime\n");

}/* end Test_CFE_SB_SetGetMsgTime */


void Test_CFE_SB_TimeStampMsg(void){

  SB_UT_Test_Tlm_t          SBTlm;
  CFE_SB_MsgPtr_t           SBTlmPtr = (CFE_SB_MsgPtr_t)&SBTlm;
  CFE_TIME_SysTime_t                GetTime;
  int32                                     TestStat;

  fprintf(UT_logfile, "\nBegin Test_CFE_SB_TimeStampMsg\n");
  SB_ResetUnitTest();

/* Note : Only Tlm Pkt with Sec Hdr is tested here because this function    */
/* (Test_CFE_SB_TimeStampMsg) is simply a call to Test_CFE_SB_SetGetMsgTime.*/
/* Test_CFE_SB_SetGetMsgTime has covered the other pkt types */

  /* Begin Test for Tlm Pkts w/Sec Hdr */
  TestStat = CFE_PASS;
  /* Setting pkt to all F's */
  memset(SBTlmPtr,0xFF,sizeof(SB_UT_Test_Tlm_t));
  /* Setting MsgId to 0x0805 */
  CFE_SB_SetMsgId(SBTlmPtr,0x0805);

  CFE_SB_TimeStampMsg(SBTlmPtr);
  GetTime.Seconds = 0xFFFF;
  GetTime.Subseconds = 0xFFFF;
  GetTime = CFE_SB_GetMsgTime(SBTlmPtr);
  /* may be helpful to uncomment next line if test fails */
  /* fprintf(UT_logfile,"GetTime.Sec=0x%X,GetTime.Subsec=0x%X\n",
            (unsigned int)GetTime.Seconds,(unsigned int)GetTime.Subseconds); */

  CFE_SB_TimeStampMsg(SBTlmPtr);
  GetTime.Seconds = 0xFFFF;
  GetTime.Subseconds = 0xFFFF;
  GetTime = CFE_SB_GetMsgTime(SBTlmPtr);
  /* may be helpful to uncomment next line if test fails */
  /* fprintf(UT_logfile,"GetTime.Sec=0x%X,GetTime.Subsec=0x%X\n",
            (unsigned int)GetTime.Seconds,(unsigned int)GetTime.Subseconds); */

  /* Verfiy CFE_SB_GetMsgTime returns the Time value expected by CFE_TIME_GetTime */
  /* the stub for CFE_TIME_GetTime simply increments the seconds cnt on each call */
  if((GetTime.Seconds != 3)||(GetTime.Subseconds != 0)){
      fprintf(UT_logfile,"GetTime.Sec=%d,ExpSecs=3,GetTime.Subsec=%d,ExpSubsecs=0\n",
            (unsigned int)GetTime.Seconds,(unsigned int)GetTime.Subseconds);
      TestStat = CFE_FAIL;
      UT_DisplayPkt(SBTlmPtr, sizeof(SB_UT_Test_Tlm_t));
  }/* end if */

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_CFE_SB_SetGetMsgTime","Tlm Pkt w/Sec Hdr Test");
  fprintf(UT_logfile, "End Test_CFE_SB_TimeStampMsg\n");

}/* end Test_CFE_SB_TimeStampMsg */


void Test_CFE_SB_SetGetCmdCode(void){

  SB_UT_Test_Cmd_t        SBCmd;
  CFE_SB_MsgPtr_t         SBCmdPtr = (CFE_SB_MsgPtr_t)&SBCmd;
  SB_UT_Test_Tlm_t        SBTlm;
  CFE_SB_MsgPtr_t         SBTlmPtr = (CFE_SB_MsgPtr_t)&SBTlm;
  SB_UT_TstPktWoSecHdr_t  SBNoSecHdrPkt;
  CFE_SB_MsgPtr_t         SBNoSecHdrPktPtr = (CFE_SB_MsgPtr_t)&SBNoSecHdrPkt;
  int32                   TestStat,RtnFromSet,ExpRtnFrmSet;
  uint16                  CmdCodeSet,CmdCodeReturned;
  uint8                   ActualCmdCodeField;
  int16                   ExpCmdCode;

  fprintf(UT_logfile, "\nBegin Test_CFE_SB_SetGetCmdCode\n");
  SB_ResetUnitTest();

  /* Loop through all cmd code values(plus a few invalid) for Cmd Pkts w/Sec Hdr */
  /* Begin Test for Cmd Pkts w/Sec Hdr */
  TestStat = CFE_PASS;
  /* Setting pkt to all F's */
  memset(SBCmdPtr,0xFF,sizeof(SB_UT_Test_Cmd_t));
  /* Setting MsgId to 0x1805 */
  CFE_SB_SetMsgId(SBCmdPtr,0x1805);
  for (CmdCodeSet=0;CmdCodeSet<256;CmdCodeSet++){
        RtnFromSet = CFE_SB_SetCmdCode(SBCmdPtr,CmdCodeSet);
        ExpRtnFrmSet = CFE_SUCCESS;
        CmdCodeReturned = CFE_SB_GetCmdCode(SBCmdPtr);
        ActualCmdCodeField = UT_GetActualCmdCodeField(SBCmdPtr);
        ExpCmdCode = (CmdCodeSet & 0x007F);/* GSFC CmdCode is the 7 LSBs of the 1st byte of Cmd Sec hdr */
        if((CmdCodeReturned != ExpCmdCode)||(ActualCmdCodeField != ExpCmdCode)||
            (RtnFromSet != ExpRtnFrmSet)){
            fprintf(UT_logfile, "CCRet=%d,CCSet=%d,ActCC=%d,ExpCC=%d,RtnFrmSet=%x,ExpRtFrmSet=%x\n",
                        CmdCodeReturned,(int)CmdCodeSet,ActualCmdCodeField,ExpCmdCode,
                        (unsigned int)RtnFromSet,(unsigned int)ExpRtnFrmSet);
            UT_DisplayPkt(SBCmdPtr, sizeof(SB_UT_Test_Cmd_t));
            TestStat = CFE_FAIL;
          break;
        }/* end if */
  }/* end for */
  UT_Report(TestStat, "Test_CFE_SB_SetGetCmdCode","Cmd Pkt w/Sec Hdr Test");

  /* Loop through all cmd code values(plus a few invalid)for Cmd Pkts wo/Sec Hdr */
  /* Begin Test for Cmd Pkts wo/Sec Hdr */
  TestStat = CFE_PASS;
  /* Setting pkt to all F's */
  memset(SBNoSecHdrPktPtr,0xFF,sizeof(SB_UT_TstPktWoSecHdr_t));
  /* Setting MsgId to 0x1005 */
  CFE_SB_SetMsgId(SBNoSecHdrPktPtr,0x1005);
  for(CmdCodeSet=0;CmdCodeSet<256;CmdCodeSet++){
        RtnFromSet = CFE_SB_SetCmdCode(SBNoSecHdrPktPtr,CmdCodeSet);
        ExpRtnFrmSet = CFE_SB_WRONG_MSG_TYPE;
        CmdCodeReturned = CFE_SB_GetCmdCode(SBNoSecHdrPktPtr);
        ActualCmdCodeField = UT_GetActualCmdCodeField(SBNoSecHdrPktPtr);
        ExpCmdCode = (CmdCodeSet & 0x007F);/* GSFC CmdCode is the 7 LSBs of the 1st byte of Cmd Sec hdr */
        if(RtnFromSet != ExpRtnFrmSet){
            fprintf(UT_logfile, "CCRet=%d,CCSet=%d,ActCC=%d,ExpCC=%d,RtnFrmSet=%x,ExpRtFrmSet=%x\n",
                        CmdCodeReturned,(int)CmdCodeSet,ActualCmdCodeField,ExpCmdCode,
                        (unsigned int)RtnFromSet,(unsigned int)ExpRtnFrmSet);
            UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SB_UT_TstPktWoSecHdr_t));
            TestStat = CFE_FAIL;
          break;
        }/* end if */
  }/* end for */
  UT_Report(TestStat, "Test_CFE_SB_SetGetCmdCode","Cmd Pkt wo/Sec Hdr Test");

  /* Loop through all cmd code values(plus a few invalid)for Tlm Pkts w/Sec Hdr */
  /* Begin Test for Tlm Pkts w/Sec Hdr */
  TestStat = CFE_PASS;
  /* Setting pkt to all F's */
  memset(SBTlmPtr,0xFF,sizeof(SB_UT_Test_Tlm_t));
  /* Setting MsgId to 0x0805 */
  CFE_SB_SetMsgId(SBTlmPtr,0x0805);
  for (CmdCodeSet=0;CmdCodeSet<256;CmdCodeSet++){
        RtnFromSet = CFE_SB_SetCmdCode(SBTlmPtr,CmdCodeSet);
        ExpRtnFrmSet = CFE_SB_WRONG_MSG_TYPE;
        CmdCodeReturned = CFE_SB_GetCmdCode(SBTlmPtr);
        ActualCmdCodeField = UT_GetActualCmdCodeField(SBTlmPtr);
        ExpCmdCode = (CmdCodeSet & 0x007F);/* GSFC CmdCode is the 7 LSBs of the 1st byte of Cmd Sec hdr */
        if(RtnFromSet != ExpRtnFrmSet){
            fprintf(UT_logfile, "CCRet=%d,CCSet=%d,ActCC=%d,ExpCC=%d,RtnFrmSet=%x,ExpRtFrmSet=%x\n",
                        CmdCodeReturned,(int)CmdCodeSet,ActualCmdCodeField,ExpCmdCode,
                        (unsigned int)RtnFromSet,(unsigned int)ExpRtnFrmSet);
            UT_DisplayPkt(SBTlmPtr, sizeof(SB_UT_Test_Tlm_t));
            TestStat = CFE_FAIL;
          break;
        }/* end if */
  }/* end for */
  UT_Report(TestStat, "Test_CFE_SB_SetGetCmdCode","Tlm Pkt w/Sec Hdr Test");

  /* Loop through all cmd code values(plus a few invalid) for Tlm Pkts wo/Sec Hdr */
  /* Begin Test for Tlm Pkts wo/Sec Hdr */
  TestStat = CFE_PASS;
  /* Setting pkt to all F's */
  memset(SBNoSecHdrPktPtr,0xFF,sizeof(SB_UT_TstPktWoSecHdr_t));
  /* Setting MsgId to 0x0005 */
  CFE_SB_SetMsgId(SBNoSecHdrPktPtr,0x0005);
  for (CmdCodeSet=0;CmdCodeSet<256;CmdCodeSet++){
        RtnFromSet = CFE_SB_SetCmdCode(SBNoSecHdrPktPtr,CmdCodeSet);
        ExpRtnFrmSet = CFE_SB_WRONG_MSG_TYPE;
        CmdCodeReturned = CFE_SB_GetCmdCode(SBNoSecHdrPktPtr);
        ActualCmdCodeField = UT_GetActualCmdCodeField(SBNoSecHdrPktPtr);
        ExpCmdCode = (CmdCodeSet & 0x007F);/* GSFC CmdCode is the 7 LSBs of the 1st byte of Cmd Sec hdr */
        if(RtnFromSet != ExpRtnFrmSet){
            fprintf(UT_logfile, "CCRet=%d,CCSet=%d,ActCC=%d,ExpCC=%d,RtnFrmSet=%x,ExpRtFrmSet=%x\n",
                        CmdCodeReturned,(int)CmdCodeSet,ActualCmdCodeField,ExpCmdCode,
                        (unsigned int)RtnFromSet,(unsigned int)ExpRtnFrmSet);
            UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SB_UT_TstPktWoSecHdr_t));
            TestStat = CFE_FAIL;
          break;
        }/* end if */
  }/* end for */
  
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_CFE_SB_SetGetCmdCode","Tlm Pkt wo/Sec Hdr Test");

  fprintf(UT_logfile, "End Test_CFE_SB_SetGetCmdCode\n");

}/* end Test_CFE_SB_SetGetCmdCode */


void Test_CFE_SB_ChecksumUtils(void){

  SB_UT_Test_Cmd_t        SBCmd;
  CFE_SB_MsgPtr_t         SBCmdPtr = (CFE_SB_MsgPtr_t)&SBCmd;
  SB_UT_Test_Tlm_t        SBTlm;
  CFE_SB_MsgPtr_t         SBTlmPtr = (CFE_SB_MsgPtr_t)&SBTlm;
  SB_UT_TstPktWoSecHdr_t  SBNoSecHdrPkt;
  CFE_SB_MsgPtr_t         SBNoSecHdrPktPtr = (CFE_SB_MsgPtr_t)&SBNoSecHdrPkt;
  uint16                  RtnFrmGet,ExpRtnFrmGet;
  boolean                 RtnFrmValidate,ExpRtnFrmVal;
  int32                   TestStat;

  fprintf(UT_logfile, "\nBegin Test_CFE_SB_ChecksumUtils\n");
  SB_ResetUnitTest();

  /* Begin Test for Cmd Pkts w/Sec Hdr */
  TestStat = CFE_PASS;
  /* Initializing pkt, setting data to zero */
  CFE_SB_InitMsg(SBCmdPtr,0x1805,sizeof(SB_UT_Test_Cmd_t),TRUE);
  /* Setting checksum field */
  CFE_SB_GenerateChecksum(SBCmdPtr);
  RtnFrmGet = CFE_SB_GetChecksum(SBCmdPtr);
  ExpRtnFrmGet = 0x2f;
  /* Validation expected to return TRUE */
  RtnFrmValidate = CFE_SB_ValidateChecksum(SBCmdPtr);
  ExpRtnFrmVal = TRUE;

  if((RtnFrmGet != ExpRtnFrmGet)||(RtnFrmValidate != ExpRtnFrmVal)){
      fprintf(UT_logfile, "Unexpected results in 1st check,RtnFrmValidate=%d,ExpRtnFrmVal=%d\n",
                                                RtnFrmValidate,ExpRtnFrmVal);
      fprintf(UT_logfile, "   RtnFrmGet=%x,ExpRtnFrmGet=%x\n",RtnFrmGet,ExpRtnFrmGet);
      UT_DisplayPkt(SBCmdPtr, sizeof(SB_UT_Test_Cmd_t));
      TestStat = CFE_FAIL;
  }/* end if */

  /* change 1 byte in pkt and verify checksum is no longer valid */
  /* Incrementing MsgId by 1 to 0x1806 */
  /* Validation expected to return FALSE */
  CFE_SB_SetMsgId(SBCmdPtr,0x1806);
  RtnFrmValidate = CFE_SB_ValidateChecksum(SBCmdPtr);
  ExpRtnFrmVal = FALSE;

  if((TestStat == CFE_FAIL)||(RtnFrmValidate != ExpRtnFrmVal)){
        fprintf(UT_logfile, "Second validate...RtnFrmValidate=%d,ExpRtnFrmVal=%d\n",RtnFrmValidate,ExpRtnFrmVal);
        UT_DisplayPkt(SBCmdPtr, sizeof(SB_UT_Test_Cmd_t));
        TestStat = CFE_FAIL;
  }/* end if */
  UT_Report(TestStat, "Test_CFE_SB_ChecksumUtils","Cmd Pkt w/Sec Hdr Test");

  /* Begin Test for Cmd Pkts wo/Sec Hdr */
  TestStat = CFE_PASS;
  /* Initializing pkt, setting data to zero */
  CFE_SB_InitMsg(SBNoSecHdrPktPtr,0x1005,sizeof(SB_UT_TstPktWoSecHdr_t),TRUE);
  /* Setting checksum field */
  CFE_SB_GenerateChecksum(SBNoSecHdrPktPtr);
  RtnFrmGet = CFE_SB_GetChecksum(SBNoSecHdrPktPtr);
  ExpRtnFrmGet = 0;
  /* Validation expected to return FALSE */
  RtnFrmValidate = CFE_SB_ValidateChecksum(SBNoSecHdrPktPtr);
  ExpRtnFrmVal = FALSE;

  if((RtnFrmGet != ExpRtnFrmGet)||(RtnFrmValidate != ExpRtnFrmVal)){
      fprintf(UT_logfile, "Unexpected results in 1st check,RtnFrmValidate=%d,ExpRtnFrmVal=%d\n",
                                                RtnFrmValidate,ExpRtnFrmVal);
      fprintf(UT_logfile, "   RtnFrmGet=%x,ExpRtnFrmGet=%x\n",RtnFrmGet,ExpRtnFrmGet);
      UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SB_UT_TstPktWoSecHdr_t));
      TestStat = CFE_FAIL;
  }/* end if */

  /* change 1 byte in pkt and verify checksum is no longer valid */
  /* Incrementing MsgId by 1 to 0x1006 */
  /* Validation expected to return FALSE */
  CFE_SB_SetMsgId(SBNoSecHdrPktPtr,0x1006);
  RtnFrmValidate = CFE_SB_ValidateChecksum(SBNoSecHdrPktPtr);
  ExpRtnFrmVal = FALSE;

  if((TestStat == CFE_FAIL)||(RtnFrmValidate != ExpRtnFrmVal)){
      fprintf(UT_logfile, "Second validate...RtnFrmValidate=%d,ExpRtnFrmVal=%d\n",RtnFrmValidate,ExpRtnFrmVal);
      UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SB_UT_TstPktWoSecHdr_t));
      TestStat = CFE_FAIL;
  }/* end if */
  UT_Report(TestStat, "Test_CFE_SB_ChecksumUtils","Cmd Pkt wo/Sec Hdr Test");

  /* Begin Test for Tlm Pkts w/Sec Hdr */
  TestStat = CFE_PASS;
  /* Initializing pkt, setting data to zero */
  CFE_SB_InitMsg(SBTlmPtr,0x0805,sizeof(SB_UT_Test_Tlm_t),TRUE);
  /* Setting checksum field */
  CFE_SB_GenerateChecksum(SBTlmPtr);
  RtnFrmGet = CFE_SB_GetChecksum(SBTlmPtr);
  ExpRtnFrmGet = 0;
  /* Validation expected to return FALSE */
  RtnFrmValidate = CFE_SB_ValidateChecksum(SBTlmPtr);
  ExpRtnFrmVal = FALSE;

  if((RtnFrmGet != ExpRtnFrmGet)||(RtnFrmValidate != ExpRtnFrmVal)){
      fprintf(UT_logfile, "Unexpected results in 1st check,RtnFrmValidate=%d,ExpRtnFrmVal=%d\n",
                                              RtnFrmValidate,ExpRtnFrmVal);
      fprintf(UT_logfile, "   RtnFrmGet=%x,ExpRtnFrmGet=%x\n",RtnFrmGet,ExpRtnFrmGet);
      UT_DisplayPkt(SBTlmPtr, sizeof(SB_UT_Test_Tlm_t));
      TestStat = CFE_FAIL;
  }/* end if */

  /* change 1 byte in pkt and verify checksum is no longer valid */
  /* Incrementing MsgId by 1 to 0x0806 */
  /* Validation expected to return FALSE */
  CFE_SB_SetMsgId(SBTlmPtr,0x1806);
  RtnFrmValidate = CFE_SB_ValidateChecksum(SBTlmPtr);
  ExpRtnFrmVal = FALSE;

  if((TestStat == CFE_FAIL)||(RtnFrmValidate != ExpRtnFrmVal)){
      fprintf(UT_logfile, "Second validate...RtnFrmValidate=%d,ExpRtnFrmVal=%d\n",RtnFrmValidate,ExpRtnFrmVal);
      UT_DisplayPkt(SBTlmPtr, sizeof(SB_UT_Test_Tlm_t));
      TestStat = CFE_FAIL;
  }/* end if */
  UT_Report(TestStat, "Test_CFE_SB_ChecksumUtils","Tlm Pkt w/Sec Hdr Test");

  /* Begin Test for Tlm Pkts wo/Sec Hdr */
  TestStat = CFE_PASS;
  /* Initializing pkt, setting data to zero */
  CFE_SB_InitMsg(SBNoSecHdrPktPtr,0x0005,sizeof(SB_UT_TstPktWoSecHdr_t),TRUE);
  /* Setting checksum field */
  CFE_SB_GenerateChecksum(SBNoSecHdrPktPtr);
  RtnFrmGet = CFE_SB_GetChecksum(SBNoSecHdrPktPtr);
  ExpRtnFrmGet = 0;
  /* Validation expected to return FALSE */
  RtnFrmValidate = CFE_SB_ValidateChecksum(SBNoSecHdrPktPtr);
  ExpRtnFrmVal = FALSE;

  if((RtnFrmGet != ExpRtnFrmGet)||(RtnFrmValidate != ExpRtnFrmVal)){
      fprintf(UT_logfile, "Unexpected results in 1st check,RtnFrmValidate=%d,ExpRtnFrmVal=%d\n",
                                              RtnFrmValidate,ExpRtnFrmVal);
      fprintf(UT_logfile, "   RtnFrmGet=%x,ExpRtnFrmGet=%x\n",RtnFrmGet,ExpRtnFrmGet);
      UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SB_UT_TstPktWoSecHdr_t));
      TestStat = CFE_FAIL;
  }/* end if */

  /* change 1 byte in pkt and verify checksum is no longer valid */
  /* Incrementing MsgId by 1 to 0x0006 */
  /* Validation expected to return FALSE */
  CFE_SB_SetMsgId(SBNoSecHdrPktPtr,0x0006);
  RtnFrmValidate = CFE_SB_ValidateChecksum(SBNoSecHdrPktPtr);
  ExpRtnFrmVal = FALSE;

  if((TestStat == CFE_FAIL)||(RtnFrmValidate != ExpRtnFrmVal)){
      fprintf(UT_logfile, "Second validate...RtnFrmValidate=%d,ExpRtnFrmVal=%d\n",RtnFrmValidate,ExpRtnFrmVal);
      UT_DisplayPkt(SBNoSecHdrPktPtr, sizeof(SB_UT_TstPktWoSecHdr_t));
      TestStat = CFE_FAIL;
  }/* end if */
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test_CFE_SB_ChecksumUtils","Tlm Pkt wo/Sec Hdr Test");

  fprintf(UT_logfile, "End Test_CFE_SB_ChecksumUtils\n");

}/* end Test_CFE_SB_ChecksumUtils */








/*
** Test SB Special Cases
*/
void Test_SB_SpecialCases(void)
{

  fprintf(UT_logfile,"\nBegin Test Special Cases\n\n");
  
  Test_OS_MutSem_ErrLogic();
  Test_GetPipeName_ErrLogic();
  Test_ReqToSendEvent_ErrLogic();
  Test_PutDestBlk_ErrLogic();

  fprintf(UT_logfile,"End Test Special Cases\n\n");

}/* end Test_SB_SpecialCases */


void Test_OS_MutSem_ErrLogic(void){
  
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t      PipeId;


  fprintf(UT_logfile,"Begin Test Shared Mutex Give/Take Failure\n");
  SB_ResetUnitTest();
  UT_SetRtnCode (&MutSemTakeRtn,CFE_OS_SEM_FAILURE,1);
  UT_SetRtnCode (&MutSemGiveRtn,CFE_OS_SEM_FAILURE,2);

  CFE_SB_CreatePipe(&PipeId,50,"TestPipe2");
  CFE_SB_Subscribe(0x1801,0);

  ExpRtn = 2;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  if(UT_EventIsInHistory(CFE_SB_SUBSCRIPTION_RCVD_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_SUBSCRIPTION_RCVD_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test Special Cases","Give/Take Failure Test\n");
  
}/* end Test_OS_MutSemTake_ErrLogic */



void Test_GetPipeName_ErrLogic(void){
  
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;
  CFE_SB_PipeId_t      PipeId;
  char                 *CharStar;

  fprintf(UT_logfile,"Begin Test GetPipename Error\n");
  SB_ResetUnitTest();

  CFE_SB_CreatePipe(&PipeId,50,"TestPipe2");

  CharStar = CFE_SB_GetPipeName(250);

  ExpRtn = 1;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  if(UT_EventIsInHistory(CFE_SB_PIPE_ADDED_EID) == FALSE){
    fprintf(UT_logfile,"CFE_SB_PIPE_ADDED_EID not sent\n");
    TestStat = CFE_FAIL;
  }
  
  CFE_SB_DeletePipe(PipeId);
  SB_ResetUnitTest();
  UT_Report(TestStat, "Test Special Cases","Get Pipename Error Test\n");
  
}/* end Test_GetPipeName_ErrLogic */



void Test_ReqToSendEvent_ErrLogic(void){
  
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile,"Begin Test ReqToSendEvent Error\n");
  SB_ResetUnitTest();

  CFE_SB.StopRecurseFlags[13] = 0xffffffff;

  ExpRtn = CFE_SB_DENIED;
  ActRtn = CFE_SB_RequestToSendEvent(13, 5);

  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  ExpRtn = 0;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test Special Cases","Req To Send Event Error Test\n");
  
}/* end Test_ReqToSendEvent_ErrLogic */


void Test_PutDestBlk_ErrLogic(void){
  
  int32                ExpRtn,ActRtn,TestStat = CFE_PASS;

  fprintf(UT_logfile,"Begin Test PutDestBlk Error\n");
  SB_ResetUnitTest();

  ExpRtn = CFE_SB_BAD_ARGUMENT;
  ActRtn = CFE_SB_PutDestinationBlk(NULL);

  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }
  
  ExpRtn = 0;
  ActRtn = UT_GetNumEventsSent();
  if(ActRtn != ExpRtn){
    fprintf(UT_logfile, "Unexpected rtn from UT_GetNumEventsSent,exp=%ld,act=%ld\n",
                      ExpRtn,ActRtn);
    TestStat = CFE_FAIL;
  }

  SB_ResetUnitTest();
  UT_Report(TestStat, "Test Special Cases","PutDestBlk Error Test\n");
  
}/* end Test_PutDestBlk_ErrLogic */





