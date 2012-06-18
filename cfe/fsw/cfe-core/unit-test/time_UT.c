/*
** File:
**    $Id: time_UT.c 1.2 2008/08/06 22:43:48EDT dkobe Exp  $
**
** Purpose:
**    TIME unit test
**
** References:
**    1. cFE Application Developers Guide
**    2. unit test standard 092503
**    3. C Coding Standard 102904
**
** Notes:
**    1. This is unit test code only, not for use in flight
**
** $Date: 2008/08/06 22:43:48EDT $
** $Revision: 1.2 $
** $Log: time_UT.c  $
** Revision 1.2 2008/08/06 22:43:48EDT dkobe 
** Updated Unit Tests for CFE_TIME_RegisterSynchCallback, CFE_TIME_UnregisterSynchCallback and CFE_TIME_CleanUpApp
** Revision 1.1 2008/04/17 08:05:44EDT ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
** Revision 1.9 2007/05/15 15:16:31EDT njyanchik 
** Updated unit test for new code
** Revision 1.8 2007/05/04 09:10:25EDT njyanchik 
** Check in of Time UT and related changes
** Revision 1.7 2007/05/01 13:28:14EDT njyanchik 
** I updated the ut stubs to get the each of the subsytems to compile under the unit test. I did not
** change the unit tests themselves to cover more of the files, however.
** Revision 1.6 2006/11/02 13:53:58EST njyanchik 
** Unit test for TIME was updated to match the changes made for this DCR
** Revision 1.5 2006/11/01 12:46:54GMT-05:00 njyanchik 
** Changed the Unit test to reflect the changes from removing the CDS functionality from TIME
** Revision 1.4 2006/10/30 14:09:28GMT-05:00 njyanchik 
** I changed TIME to use the new ES_CDS implementation. This involved using the 
** new functions, as well as modifying the CFE_TIME_TaskData structure as well as
** the CFE_TIME_DiagPacket structure. They both made reference to the address
** of the TIME CDS (the old implementation). Now they both use the new
** CFE_ES_CDSHandle_t. Also, the Unit Test for Time was updated. No new paths
** through the code were created, but since return codes from the CDS functions
** changed, there needed to be updates to the UT.
** Revision 1.3 2006/05/31 08:33:15GMT-05:00 jjhageman 
** Addition of CFE_TIME_MET2SCTime testing and added case to CFE_TIME_Sub2MicroSec for full coverage. 
** Revision 1.2 2006/05/31 08:29:37EDT jjhageman 
** Additions for code coverage with different defines
**
*/

/*
** Includes
*/

#include <stdio.h>
#include <string.h>

#include "cfe_platform_cfg.h"

#include "cfe_sb.h"
#include "cfe_time.h"
#include "cfe_time_msg.h"
#include "../time/cfe_time_utils.h"
#include "common_types.h"
#include "ut_stubs.h"

/*
** Exported Data
*/
extern CFE_TIME_TaskData_t CFE_TIME_TaskData;

extern FILE *UT_logfile;
extern UT_SetRtn_t ES_RegisterRtn;
extern UT_SetRtn_t ES_CreateChildRtn;
extern UT_SetRtn_t  SB_SubscribeRtn;
extern UT_SetRtn_t  SB_SubscribeLocalRtn;
extern UT_SetRtn_t  SB_CreatePipeRtn;
extern UT_SetRtn_t OS_BinSemCreateRtn;
extern UT_SetRtn_t  EVS_SendEventRtn;
extern UT_SetRtn_t  EVS_RegisterRtn;
extern uint32 UT_AppID;


int32 ut_time_CallbackCalled = 0;



/*
** Function Definitions
*/

int32 CFE_TIME_EarlyInit (void);
int32 CFE_TIME_TaskInit (void);
int32 CFE_TIME_CleanUpApp(uint32 AppID);

void Test_Init (void);
void Test_GetTime (void);
void Test_TimeOp (void);
void Test_ConvertTime (void);
void Test_ConvertCFEFS (void);
void Test_Print (void);
void Test_RegisterSynchCallback(void);
void Test_ExternalTone (void);
void Test_External (void);
void Test_PipeCmds (void);
void Test_ResetArea(void);
void Test_State (void);
void Test_GetReference (void);
void Test_Tone (void);
void Test_UnregisterSynchCallback(void);
void Test_CleanUpApp(void);

void CFE_TIME_TaskPipe(CFE_SB_MsgPtr_t MessagePtr);
void CFE_TIME_TaskMain(void);

/*
** main function runs all the tests
*/
int main (void)
{

  /* Initialize UnitTest */
  UT_Init("ut_cfe_time");

  CFE_TIME_TaskMain();

  /* Return a failure from TIME_TaskInit */

  UT_SetRtnCode( &ES_RegisterRtn, -1 ,1 );
  CFE_TIME_TaskMain();

  /* Test_Init is a test but also MUST be called 1st and only once */
  Test_Init();
  Test_GetTime();
  Test_TimeOp();
  Test_ConvertTime();
  Test_ConvertCFEFS();
  Test_Print();
  Test_RegisterSynchCallback();
  Test_ExternalTone();
  Test_External();

  Test_PipeCmds();

  Test_ResetArea();
  Test_State();
  Test_GetReference();

  Test_Tone();
  Test_UnregisterSynchCallback();
  Test_CleanUpApp();

  /* Final report on number of errors */
  UT_ReportFailures();

  exit(0);

}

/*
** Initialization test, must be called first
*/
void Test_Init (void)
{

  CFE_SB_Msg_t message;

  /* Initialize */
  CFE_TIME_EarlyInit();
  /* Test a good run through TimeTaskInit */
  UT_Report(CFE_TIME_TaskInit() == CFE_SUCCESS,"CFE_TIME_Task_Init","Clean run though");



  /* Test CFE_ES_RegisterApp Failing */
  UT_SetRtnCode( &ES_RegisterRtn, -1, 1);
  UT_Report(CFE_TIME_TaskInit() == -1,"CFE_TIME_Task_Init","Register App Failure");


  /* Test first CFE ES_CreateChildTask Failing */
  UT_SetRtnCode( &ES_CreateChildRtn, -1, 1);
  UT_Report(CFE_TIME_TaskInit() == -1,"CFE_TIME_Task_Init","Child Task 1 Create failure");

  /* Test second CFE ES_CreateChildTask Failing */
  UT_SetRtnCode( &ES_CreateChildRtn, -3, 2);
  UT_Report(CFE_TIME_TaskInit() == -3,"CFE_TIME_Task_Init","Child Task 2 Create failure");


  UT_SetRtnCode( &SB_CreatePipeRtn, -1, 1);
  UT_Report(CFE_TIME_TaskInit() == -1,"CFE_TIME_Task_Init","SB Create Pipe failure");




  /* Test first SB_Subscribe Failing */
  UT_SetRtnCode( &SB_SubscribeRtn, -1, 1);
  UT_Report(CFE_TIME_TaskInit() == -1,"CFE_TIME_Task_Init"," SB Subscribe 1 failure");

  /* Test second SB_Subscribe Failing */
  UT_SetRtnCode( &SB_SubscribeRtn, -2, 2);
  UT_Report(CFE_TIME_TaskInit() == -2,"CFE_TIME_Task_Init"," SB Subscribe 2 failure");
  

  UT_SetRtnCode( &SB_SubscribeLocalRtn, -3, 1);
  UT_Report(CFE_TIME_TaskInit() == -3,"CFE_TIME_Task_Init"," SB SubscribeLocal 1 failure");
  
  UT_SetRtnCode( &SB_SubscribeLocalRtn, -4, 2);
  UT_Report(CFE_TIME_TaskInit() == -4,"CFE_TIME_Task_Init"," SB SubscribeLocal 2 failure");

  UT_SetRtnCode( &SB_SubscribeLocalRtn, -5, 3);
  UT_Report(CFE_TIME_TaskInit() == -5,"CFE_TIME_Task_Init"," SB SubscribeLocal 3 failure");

  UT_SetRtnCode( &OS_BinSemCreateRtn, -1, 1);
  UT_Report(CFE_TIME_TaskInit() == -1,"CFE_TIME_Task_Init"," OS_BinSemCreate 1 failure");

  UT_SetRtnCode( &OS_BinSemCreateRtn, -2, 2);
  UT_Report(CFE_TIME_TaskInit() == -2,"CFE_TIME_Task_Init"," OS_BinSemCreate 2 failure");

  UT_SetRtnCode( &EVS_RegisterRtn, -1, 1);
  UT_Report(CFE_TIME_TaskInit() == -1,"CFE_TIME_Task_Init"," CFE_EVS_Register failure");

  UT_SetRtnCode( &EVS_SendEventRtn, -1, 1);
  UT_Report(CFE_TIME_TaskInit() == -1,"CFE_TIME_Task_Init"," CFE_EVS_SendEvent failure");




  /* Set the receive message to noop command */
  UT_SetRcvMsg(CFE_TIME_CMD_MID, CFE_TIME_NOOP_CC);


  /* Send noop */
  UT_SendMsg(&message, CFE_TIME_CMD_MID, CFE_TIME_NOOP_CC);

}

/*
** Test TIME get functions
*/
void Test_GetTime (void)
{

  char buf[160];

  CFE_TIME_Print(buf, CFE_TIME_GetTime());
  fprintf(UT_logfile, "  CFE_TIME_GetTime: %s\n", buf);

  CFE_TIME_Print(buf, CFE_TIME_GetTAI());
  fprintf(UT_logfile, "  CFE_TIME_GetTAI: %s\n", buf);

  CFE_TIME_Print(buf, CFE_TIME_GetUTC());
  fprintf(UT_logfile, "  CFE_TIME_GetUTC: %s\n", buf);

  fprintf(UT_logfile, "  CFE_TIME_GetClockState: %d\n",
          CFE_TIME_GetClockState());


  CFE_TIME_TaskData.ClockSetState = CFE_TIME_WAS_SET;
  CFE_TIME_TaskData.IsToneGood = TRUE;
  CFE_TIME_TaskData.Forced2Fly = TRUE;

  fprintf(UT_logfile, "  CFE_TIME_GetClockInfo: %d\n",
          CFE_TIME_GetClockInfo());



  fprintf(UT_logfile, "  CFE_TIME_GetLeapSeconds: %d\n",
          CFE_TIME_GetLeapSeconds());

  CFE_TIME_Print(buf, CFE_TIME_GetSTCF());
  fprintf(UT_logfile, "  CFE_TIME_GetSTCF: %s\n", buf);

  CFE_TIME_Print(buf, CFE_TIME_GetMET());
  fprintf(UT_logfile, "  CFE_TIME_GetMET: %s\n", buf);

  fprintf(UT_logfile, "  CFE_TIME_GetMETseconds: %d\n",
          (int)CFE_TIME_GetMETseconds());

  fprintf(UT_logfile, "  CFE_TIME_GetMETsubsecs: %d\n",
          (int)CFE_TIME_GetMETsubsecs());
}

/*
** Test operations on time (add, subtract, compare)
*/
void Test_TimeOp (void)
{

  CFE_TIME_SysTime_t time1, time2, result, exp_result;

  /* Test with zero times */
  time1.Subseconds = 0;
  time1.Seconds = 0;
  time2.Subseconds = 0;
  time2.Seconds = 0;

  exp_result.Subseconds = 0;
  exp_result.Seconds = 0;

  result = CFE_TIME_Add(time1, time2);
  UT_Report(memcmp(&result, &exp_result, sizeof(CFE_TIME_SysTime_t)) == 0,
            "CFE_TIME_Add", "Add zero times");

  result = CFE_TIME_Subtract(time1, time2);
  UT_Report(memcmp(&result, &exp_result, sizeof(CFE_TIME_SysTime_t)) == 0,
            "CFE_TIME_Subtract", "Subtract zero times");

  UT_Report(CFE_TIME_Compare(time1, time2) == CFE_TIME_EQUAL,
            "CFE_TIME_Compare", "Compare zero times");

  /* Test with max times */
  time1.Subseconds = 0xFFFFFFFF;
  time1.Seconds = 0xFFFFFFFF;
  time2.Subseconds = 0xFFFFFFFF;
  time2.Seconds = 0xFFFFFFFF;

  exp_result.Subseconds = 0xFFFFFFFE;
  exp_result.Seconds = 0xFFFFFFFF;

  /* Case with extreme time rollover */
  result = CFE_TIME_Add(time1, time2);
  UT_Report(memcmp(&result, &exp_result, sizeof(CFE_TIME_SysTime_t)) == 0,
            "CFE_TIME_Add", "Add maximum times");

  exp_result.Subseconds = 0;
  exp_result.Seconds = 0;

  result = CFE_TIME_Subtract(time1, time2);
  UT_Report(memcmp(&result, &exp_result, sizeof(CFE_TIME_SysTime_t)) == 0,
            "CFE_TIME_Subtract", "Subtract maximum times");

  UT_Report(CFE_TIME_Compare(time1, time2) == CFE_TIME_EQUAL,
            "CFE_TIME_Compare", "Compare maximum times");

  /* Test with A greater than B */
  time1.Subseconds = 0xFFFFFFFF;
  time1.Seconds = 0xFFFF0000;
  time2.Subseconds = 0x00000001;
  time2.Seconds = 0x0000FFFF;

  exp_result.Subseconds = 0;
  exp_result.Seconds = 0;

  /* Case with minimal time rollover */
  result = CFE_TIME_Add(time1, time2);
  UT_Report(memcmp(&result, &exp_result, sizeof(CFE_TIME_SysTime_t)) == 0,
            "CFE_TIME_Add", "Add A >> B times");

  exp_result.Subseconds = 0xFFFFFFFE;
  exp_result.Seconds = 0xFFFE0001;

  result = CFE_TIME_Subtract(time1, time2);
  UT_Report(memcmp(&result, &exp_result, sizeof(CFE_TIME_SysTime_t)) == 0,
            "CFE_TIME_Subtract", "Subtract A >> B times");

  /* A >> B assumes time has rolled over */
  UT_Report(CFE_TIME_Compare(time1, time2) == CFE_TIME_A_LT_B,
            "CFE_TIME_Compare", "Compare A >> B times");
 
  /* Test with A << B */
  exp_result.Subseconds = 0x00000002;
  exp_result.Seconds = 0x0001FFFE;

  result = CFE_TIME_Subtract(time2, time1);
  UT_Report(memcmp(&result, &exp_result, sizeof(CFE_TIME_SysTime_t)) == 0,
            "CFE_TIME_Subtract", "Subtract A << B times");

  /* A << B assumes time has rolled over */
  UT_Report(CFE_TIME_Compare(time2, time1) == CFE_TIME_A_GT_B,
            "CFE_TIME_Compare", "Compare A << B times");

  /* Test with A > B */
  time1.Subseconds = 30;
  time1.Seconds = 3;
  time2.Subseconds = 29;
  time2.Seconds = 3;

  exp_result.Subseconds = 59;
  exp_result.Seconds = 6;

  /* Case with minimal time rollover */
  result = CFE_TIME_Add(time1, time2);
  UT_Report(memcmp(&result, &exp_result, sizeof(CFE_TIME_SysTime_t)) == 0,
            "CFE_TIME_Add", "Add A > B times");

  exp_result.Subseconds = 1;
  exp_result.Seconds = 0;

  result = CFE_TIME_Subtract(time1, time2);
  UT_Report(memcmp(&result, &exp_result, sizeof(CFE_TIME_SysTime_t)) == 0,
            "CFE_TIME_Subtract", "Subtract A > B times");

  UT_Report(CFE_TIME_Compare(time1, time2) == CFE_TIME_A_GT_B,
            "CFE_TIME_Compare", "Compare A > B times");

  /* Test with A < B */
  exp_result.Subseconds = 0xFFFFFFFF;
  exp_result.Seconds = 0xFFFFFFFF;

  result = CFE_TIME_Subtract(time2, time1);
  UT_Report(memcmp(&result, &exp_result, sizeof(CFE_TIME_SysTime_t)) == 0,
            "CFE_TIME_Subtract", "Subtract A < B (subseconds) times");

  UT_Report(CFE_TIME_Compare(time2, time1) == CFE_TIME_A_LT_B,
            "CFE_TIME_Compare", "Compare A < B (subseconds) times");

  /* Test with just seconds different, A > B */
  time1.Subseconds = 18;
  time1.Seconds = 8;
  time2.Subseconds = 18;
  time2.Seconds = 7;

  exp_result.Subseconds = 0;
  exp_result.Seconds = 1;

  result = CFE_TIME_Subtract(time1, time2);
  UT_Report(memcmp(&result, &exp_result, sizeof(CFE_TIME_SysTime_t)) == 0,
            "CFE_TIME_Subtract", "Subtract A > B (seconds) times");

  UT_Report(CFE_TIME_Compare(time1, time2) == CFE_TIME_A_GT_B,
            "CFE_TIME_Compare", "Compare A > B (seconds) times");

  /* Test with just seconds different, A < B */
  exp_result.Subseconds = 0;
  exp_result.Seconds = 0xFFFFFFFF;

  result = CFE_TIME_Subtract(time2, time1);
  UT_Report(memcmp(&result, &exp_result, sizeof(CFE_TIME_SysTime_t)) == 0,
            "CFE_TIME_Subtract", "Subtract A < B (seconds) times");

  UT_Report(CFE_TIME_Compare(time2, time1) == CFE_TIME_A_LT_B,
            "CFE_TIME_Compare", "Compare A < B (seconds) times");

}

/*
** Test time converting functions
*/
void Test_ConvertTime (void)
{

  CFE_TIME_SysTime_t METTime;
  char buf[160];

  METTime.Seconds = 0;
  METTime.Subseconds = 0;

  CFE_TIME_Print(buf, CFE_TIME_MET2SCTime(METTime));
  fprintf(UT_logfile, "  CFE_TIME_MET2SCTime: %s\n", buf);

  UT_Report(CFE_TIME_Sub2MicroSecs(0) == 0, "CFE_TIME_Sub2MicroSecs",
            "Convert 0 subseconds to 0 microseconds");

  UT_Report(CFE_TIME_Sub2MicroSecs(0xFFFF) == 16, "CFE_TIME_Sub2MicroSecs",
            "Convert 0xFFFF subseconds to 16 microseconds");

  UT_Report(CFE_TIME_Sub2MicroSecs(0x80000000) == 500000,
            "CFE_TIME_Sub2MicroSecs",
            "Convert 0x80000000 subseconds to 500000 microseconds");

  UT_Report(CFE_TIME_Sub2MicroSecs(0x80002000) == 500001,
            "CFE_TIME_Sub2MicroSecs",
            "Convert 0x80000002 subseconds to 500001 microseconds");

  UT_Report(CFE_TIME_Sub2MicroSecs(0xFFFFFFFF) == 999999,
            "CFE_TIME_Sub2MicroSecs",
            "Convert 0xFFFFFFFF subseconds to 999999 microseconds");

  UT_Report(CFE_TIME_Micro2SubSecs(0) == 0, "CFE_TIME_Micro2SubSecs",
            "Convert 0 microseconds to 0 subseconds");

  UT_Report(CFE_TIME_Micro2SubSecs(0xFFFF) == 281468928,
            "CFE_TIME_Micro2SubSecs",
            "Convert 0xFFFF microseconds to 281468928 subseconds");

  UT_Report(CFE_TIME_Micro2SubSecs(999998) == 0xFFFFE000lu,
            "CFE_TIME_Micro2SubSecs",
            "Convert 999998 microseconds to 0xFFFFE000 subseconds");

  UT_Report(CFE_TIME_Micro2SubSecs(999999) == 0xFFFFF000lu,
            "CFE_TIME_Micro2SubSecs",
            "Convert 999999 microseconds to 0XFFFFF000 subseconds");

  UT_Report(CFE_TIME_Micro2SubSecs(0xFFFFFFFF) == 0xFFFFFFFF,
            "CFE_TIME_Micro2SubSecs",
            "Convert 0xFFFFFFFF microseconds to 0xFFFFFFFF subseconds");
}

/*
** Test CFE and FS second conversions
**
** NOTE: May not get full coverage, needs CFE_TIME_FS_FACTOR < 0
*/
void Test_ConvertCFEFS (void)
{

  uint32 result;

  /* Calculate expected result */
  if (CFE_TIME_FS_FACTOR < 0 && -CFE_TIME_FS_FACTOR > 0){
    result = 0;
  } else {
    result = CFE_TIME_FS_FACTOR;
  }

  UT_Report(CFE_TIME_CFE2FSSeconds(0) == result,
            "CFE_TIME_CFE2FSSeconds", "Convert 0 CFE seconds to FS seconds");

  /* Calculate expected result */
  if (CFE_TIME_FS_FACTOR < 0 && -CFE_TIME_FS_FACTOR > 0xFFFF){ 
    result = 0;
  } else {
    result = CFE_TIME_FS_FACTOR + 0xFFFF;
  }

  UT_Report(CFE_TIME_CFE2FSSeconds(0xFFFF) == result,
            "CFE_TIME_CFE2FSSeconds",
            "Convert 0xFFFF CFE seconds to FS seconds");

  UT_Report(CFE_TIME_CFE2FSSeconds(0xFFFFFFFF) ==
            (uint32)CFE_TIME_FS_FACTOR - 1, "CFE_TIME_CFE2FSSeconds",
            "Convert 0xFFFFFFFF CFE seconds to FS seconds");

  if (CFE_TIME_FS_FACTOR > (uint32)0){
    result = 0;
  } else {
    result = -(uint32)CFE_TIME_FS_FACTOR;
  }

  UT_Report(CFE_TIME_FS2CFESeconds(0) == result, "CFE_TIME_FS2CFESeconds",
            "Convert 0 FS seconds to CFE seconds");

  UT_Report(CFE_TIME_FS2CFESeconds(CFE_TIME_FS_FACTOR - 1) == 0,
            "CFE_TIME_FS2CFESeconds",
            "Convert CFE_TIME_FS_FACTOR - 1 FS seconds to CFE seconds");

  if (CFE_TIME_FS_FACTOR > (uint32)(CFE_TIME_FS_FACTOR + 1)){
    result = 0;
  } else {
    result = 1;
  }

  UT_Report(CFE_TIME_FS2CFESeconds(CFE_TIME_FS_FACTOR + 1) == result,
            "CFE_TIME_FS2CFESeconds",
            "Convert CFE_TIME_FS_FACTOR + 1 FS seconds to CFE seconds");

  UT_Report(CFE_TIME_FS2CFESeconds(0xFFFFFFFF) ==
            0xFFFFFFFF - CFE_TIME_FS_FACTOR, "CFE_TIME_FS2CFESeconds",
            "Convert 0xFFFFFFFF FS seconds to CFE seconds");

}

/*
** Test print of time
**
** NOTE: Will only get full coverage if CFE_TIME_EPOCH_SECOND > 0
**       Since outputs depend on system defines, to verifiy
**       the outputs should be checked in the log file
**       
*/
void Test_Print (void)
{

  char buf[160];
  CFE_TIME_SysTime_t time;

  /* Test with zeros */
  time.Subseconds = 0;
  time.Seconds = 0;

  CFE_TIME_Print(buf, time);
  UT_Report(TRUE, "CFE_TIME_Print", strcat(buf," Time with zero inputs"));

  /* Test with value that will cause seconds >=60 case when  */
  /* CFE_TIME_EPOCH_SECOND > 0                               */
  time.Subseconds = 0;
  time.Seconds = 59;

  CFE_TIME_Print(buf, time);
  UT_Report(TRUE, "CFE_TIME_Print",
            strcat(buf," Sec overflow if CFE_TIME_EPOCH_SECOND > 0"));

  /* Test with mission representitive numbers */
  time.Subseconds = 215000;
  time.Seconds = 883620184;

  CFE_TIME_Print(buf, time);
  UT_Report(TRUE, "CFE_TIME_Print", strcat(buf," Mission representative time"));

  /* Test with maximum integers */
  time.Subseconds = 0xFFFFFFFF;
  time.Seconds = 0xFFFFFFFF;

  CFE_TIME_Print(buf, time);
  UT_Report(TRUE, "CFE_TIME_Print", strcat(buf," Max integer inputs"));

}

/*
** Test Register and Unregister Synch Callback API
*/
void ut_time_MyCallbackFunc(void)
{
    ut_time_CallbackCalled++;
    fprintf(UT_logfile, "  Time Synch Callback Called: %ld\n", ut_time_CallbackCalled);
    return;
}

void Test_RegisterSynchCallback(void)
{
    uint32  i = 0;
    int32   Result = CFE_SUCCESS;
    boolean SuccessfulTestResult = TRUE;
    boolean UnsuccessfulTestResult = TRUE;
    
    /* Register Callback function one too many times to test all cases */
    for (i=0; i<=CFE_TIME_MAX_NUM_SYNCH_FUNCS; i++)
    {
        Result = CFE_TIME_RegisterSynchCallback((CFE_TIME_SynchCallbackPtr_t)&ut_time_MyCallbackFunc);
        
        if ((i<CFE_TIME_MAX_NUM_SYNCH_FUNCS) && (Result != CFE_SUCCESS))
        {
            SuccessfulTestResult = FALSE;
        }
        else if ((i==CFE_TIME_MAX_NUM_SYNCH_FUNCS) && 
                 (Result != CFE_TIME_TOO_MANY_SYNCH_CALLBACKS))
        {
            UnsuccessfulTestResult = FALSE;
        }
    }
    
    UT_Report(SuccessfulTestResult, "CFE_TIME_RegisterSynchCallback", "Successfully Registered Callbacks");
    UT_Report(UnsuccessfulTestResult, "CFE_TIME_RegisterSynchCallback", "Unsuccessful Registered Callback");
    return;
}

/*
** Test external tone
*/
void Test_ExternalTone (void)
{
  CFE_TIME_ExternalTone();
}

/*
** Test external set of time
*/
void Test_External (void)
{

  CFE_TIME_SysTime_t settime;

  settime.Seconds = 5;
  settime.Subseconds = 4;

#ifdef CFE_TIME_CFG_SRC_MET
  CFE_TIME_ExternalMET(settime);
#endif

#ifdef CFE_TIME_CFG_SRC_GPS
  CFE_TIME_ExternalGPS(settime, 0);
#endif

#ifdef CFE_TIME_CFG_SRC_TIME
  CFE_TIME_ExternalTime(settime);
#endif
}

/*
** Test commands using the task pipe
*/
void Test_PipeCmds (void)
{

  CFE_SB_MsgPtr_t msgptr;
  CFE_SB_Msg_t message;
  CFE_TIME_ToneDataCmd_t tonedatacmd;
  CFE_TIME_StateCmd_t statecmd;
  CFE_TIME_SourceCmd_t sourcecmd;
  CFE_TIME_SignalCmd_t signalcmd;
  CFE_TIME_TimeCmd_t timecmd;
  CFE_TIME_LeapsCmd_t leapscmd;

  /* Send one of each command type */
  UT_SendMsg(&message, CFE_TIME_SEND_HK_MID, 0);
  UT_SendMsg(&message, CFE_TIME_TONE_CMD_MID, 0);

  msgptr = (CFE_SB_MsgPtr_t)&tonedatacmd;
  UT_SendMsg(msgptr, CFE_TIME_DATA_CMD_MID, 0);

  UT_SendMsg(&message, CFE_TIME_FAKE_CMD_MID, 0);
  UT_SendMsg(&message, CFE_TIME_SEND_CMD_MID, 0);
  UT_SendMsg(&message, CFE_TIME_CMD_MID, CFE_TIME_NOOP_CC);
  UT_SendMsg(&message, CFE_TIME_CMD_MID, CFE_TIME_RESET_CC);
  UT_SendMsg(&message, CFE_TIME_CMD_MID, CFE_TIME_DIAG_TLM_CC);

  /* Send all state command possibilities */
  msgptr = (CFE_SB_MsgPtr_t)&statecmd;
  statecmd.ClockState = CFE_TIME_INVALID;
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_STATE_CC);
  statecmd.ClockState = CFE_TIME_VALID;
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_STATE_CC);
  statecmd.ClockState = CFE_TIME_FLYWHEEL;
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_STATE_CC);
  statecmd.ClockState = 10;
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_STATE_CC);

  /* Send all source command possibilities */
  msgptr = (CFE_SB_MsgPtr_t)&sourcecmd;
  sourcecmd.TimeSource = CFE_TIME_USE_INTERN;
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_SOURCE_CC);
  sourcecmd.TimeSource = CFE_TIME_USE_EXTERN;
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_SOURCE_CC);
  sourcecmd.TimeSource = -1;
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_SOURCE_CC);

  /* Send all signal command possibilities */
  msgptr = (CFE_SB_MsgPtr_t)&signalcmd;
  signalcmd.ToneSource = CFE_TIME_TONE_PRI;
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_SIGNAL_CC);
  signalcmd.ToneSource = CFE_TIME_TONE_RED;
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_SIGNAL_CC);
  signalcmd.ToneSource = -1;
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_SIGNAL_CC);

  /* Send time modification commands */
  msgptr = (CFE_SB_MsgPtr_t)&timecmd;
  timecmd.MicroSeconds = 0;
  timecmd.Seconds = 0;
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_ADD_DELAY_CC);
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SUB_DELAY_CC);
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_TIME_CC);
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_MET_CC);
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_STCF_CC);
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_ADD_ADJUST_CC);
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SUB_ADJUST_CC);
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_ADD_1HZADJ_CC);
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SUB_1HZADJ_CC);
  timecmd.MicroSeconds = 1000001;
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_ADD_DELAY_CC);
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_TIME_CC);
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_MET_CC);
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_STCF_CC);
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_ADD_ADJUST_CC);
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_ADD_1HZADJ_CC);

  /* Send set leaps commands */
  msgptr = (CFE_SB_MsgPtr_t)&leapscmd;
  leapscmd.LeapSeconds = 0;
  UT_SendMsg(msgptr, CFE_TIME_CMD_MID, CFE_TIME_SET_LEAPS_CC);

  /* Invalid command */
  UT_SendMsg(&message, CFE_TIME_CMD_MID, -1);

  /* Invalid message ID */
  UT_SendMsg(&message, -1, 0);

}

/*
** Test the  Reset Area functionality
*/
void Test_ResetArea (void)
{

  CFE_TIME_Reference_t Reference;

  /* Tests exisiting and good Reset Area */
  UT_SetStatusBSPResetArea(OS_SUCCESS, CFE_TIME_RESET_SIGNATURE);
  CFE_TIME_QueryResetVars();
  CFE_TIME_UpdateResetVars(&Reference);

  /* Tests bad reset area */
  UT_SetStatusBSPResetArea(OS_ERROR, CFE_TIME_RESET_SIGNATURE);
  CFE_TIME_QueryResetVars();

  /* New CDS with all 0's */
  UT_SetStatusBSPResetArea(OS_SUCCESS, -1);
  CFE_TIME_QueryResetVars();
}

/*
** Exersize time state calls
*/
void Test_State (void)
{

  uint16 flag = 0;
  CFE_TIME_Reference_t Reference;

  Reference.ClockSetState = CFE_TIME_WAS_SET;
  Reference.ClockFlyState = CFE_TIME_NO_FLY;

  UT_Report(CFE_TIME_CalculateState(&Reference) == CFE_TIME_VALID,
            "CFE_TIME_CalculateState","Valid time state");

  Reference.ClockFlyState = CFE_TIME_IS_FLY;

  UT_Report(CFE_TIME_CalculateState(&Reference) == CFE_TIME_FLYWHEEL,
            "CFE_TIME_CalculateState","Flywheel time state");

  Reference.ClockSetState = CFE_TIME_INVALID;

  UT_Report(CFE_TIME_CalculateState(&Reference) == CFE_TIME_INVALID,
            "CFE_TIME_CalculateState","Invalid time state");
  /* Set up state flags */
  CFE_TIME_SetState(CFE_TIME_VALID);
  CFE_TIME_SetState(CFE_TIME_FLYWHEEL);

  flag = CFE_TIME_FLAG_CLKSET | CFE_TIME_FLAG_FLYING |
         CFE_TIME_FLAG_SRCINT | CFE_TIME_FLAG_SIGPRI |
         CFE_TIME_FLAG_CMDFLY;

  /* Add server or client flag depending on configuration */
#ifdef CFE_TIME_CFG_SERVER
  flag |= CFE_TIME_FLAG_SERVER | CFE_TIME_FLAG_SRVFLY |
          CFE_TIME_FLAG_ADDTCL;
#else
  flag |= CFE_TIME_FLAG_ADDADJ | CFE_TIME_FLAG_ADD1HZ;
#endif

  UT_Report(CFE_TIME_GetStateFlags() == flag, "CFE_TIME_GetStateFlags",
            "State flags");
 
}

/*
** Hit wraparound state
*/
void Test_GetReference (void)
{

  CFE_TIME_Reference_t Reference;

  UT_SetBSP_Time(0, 0);   
  CFE_TIME_GetReference(&Reference);

}

/*
** Test the CFE_TIME_Tone routines
*/
void Test_Tone (void)
{

  CFE_TIME_SysTime_t time;
  CFE_TIME_SysTime_t time1;
  CFE_TIME_ToneDataCmd_t tonedatacmd;

  time.Seconds = 0;
  time1.Seconds = 1;
  time.Subseconds = 0;
  time1.Subseconds = 0;

  /* Set state to flywheel and call tone routines */
  CFE_TIME_SetState(CFE_TIME_FLYWHEEL);

#ifdef CFE_TIME_CFG_SERVER
  CFE_TIME_ToneSend();

  CFE_TIME_TaskData.ClockFlyState = CFE_TIME_NO_FLY;

  CFE_TIME_ToneSend();
#endif

  CFE_TIME_Local1HzISR();
  CFE_TIME_ToneUpdate();
  CFE_TIME_ToneVerify(time, time);

  /* Set state to no fly and call tone routines */
  CFE_TIME_SetState(CFE_TIME_NO_FLY);
  CFE_TIME_Local1HzISR();

  /* Set state to valid and call tone routines */
  CFE_TIME_SetState(CFE_TIME_VALID);
  CFE_TIME_ToneVerify(time, time);

  /* Verify tone with time A > B, and B > A */
  CFE_TIME_ToneVerify(time1, time);
  CFE_TIME_ToneVerify(time, time1);

  /* Update time with CFE_TIME_INVALID state pending */
  tonedatacmd.AtToneState = CFE_TIME_INVALID;
  CFE_TIME_ToneData(&tonedatacmd);
  CFE_TIME_ToneUpdate();

  /* Update time with CFE_TIME_FLYWHEEL state pending */
  tonedatacmd.AtToneState = CFE_TIME_FLYWHEEL;
  CFE_TIME_ToneData(&tonedatacmd);
  CFE_TIME_ToneUpdate();

#ifdef CFE_TIME_CFG_SERVER
  /* Apply adjustments */
  time.Subseconds = 5;
  CFE_TIME_Set1HzAdj(time, CFE_TIME_ADD_ADJUST);
  CFE_TIME_Local1HzISR();
  CFE_TIME_Set1HzAdj(time, CFE_TIME_SUB_ADJUST);
  CFE_TIME_Local1HzISR();

  CFE_TIME_TaskData.AtToneLatch.Seconds = 1;
  CFE_TIME_TaskData.AtToneLatch.Subseconds =0;

  UT_SetBSP_Time(0, 0);   

  CFE_TIME_TaskData.MaxLocalClock.Seconds = CFE_TIME_CFG_START_FLY+ 3;
  CFE_TIME_TaskData.MaxLocalClock.Subseconds =0;
  
  
  CFE_TIME_Local1HzISR();



  CFE_TIME_TaskData.AtToneLatch.Seconds = 1;
  CFE_TIME_TaskData.AtToneLatch.Subseconds =0;

  UT_SetBSP_Time(0, 0);   

  CFE_TIME_TaskData.MaxLocalClock.Seconds = CFE_TIME_CFG_LATCH_FLY+ 3;
  CFE_TIME_TaskData.MaxLocalClock.Subseconds =0;

  CFE_TIME_TaskData.ClockFlyState = CFE_TIME_IS_FLY;

  CFE_TIME_Local1HzISR();
#endif

  /* Fake a valid 1Hz signal to get coverage of CFE_TIME_Tone1HzISR */
  
  UT_SetBSP_Time(0, 0);

  CFE_TIME_TaskData.ToneSignalLatch.Seconds = 1;
  CFE_TIME_TaskData.ToneSignalLatch.Subseconds = 0;
    
  CFE_TIME_Tone1HzISR();
  UT_SetBSP_Time(1, 0);
  CFE_TIME_Tone1HzISR();
  UT_Report((ut_time_CallbackCalled==CFE_TIME_MAX_NUM_SYNCH_FUNCS), "Synch Callbacks", "Proper number of callbacks made");

  /* Call local 1Hz task and have BinSem fail on the second call */
  UT_SetBinSemFail(2);
  CFE_TIME_Local1HzTask();




  /* Call tone 1Hz task and have BinSem fail on the second call */ 
  UT_SetBinSemFail(2);
  CFE_TIME_Tone1HzTask();


}

void Test_UnregisterSynchCallback(void)
{
    uint32  i = 0;
    int32   Result = CFE_SUCCESS;
    boolean SuccessfulTestResult = TRUE;
    boolean UnsuccessfulTestResult = TRUE;
    
    /* Unregister Callback function one too many times to test all cases */
    for (i=0; i<=CFE_TIME_MAX_NUM_SYNCH_FUNCS; i++)
    {
        Result = CFE_TIME_UnregisterSynchCallback((CFE_TIME_SynchCallbackPtr_t)&ut_time_MyCallbackFunc);
        
        if ((i<CFE_TIME_MAX_NUM_SYNCH_FUNCS) && (Result != CFE_SUCCESS))
        {
            SuccessfulTestResult = FALSE;
        }
        else if ((i==CFE_TIME_MAX_NUM_SYNCH_FUNCS) && 
                 (Result != CFE_TIME_CALLBACK_NOT_REGISTERED))
        {
            UnsuccessfulTestResult = FALSE;
        }
    }
    
    UT_Report(SuccessfulTestResult, "CFE_TIME_UnregisterSynchCallback", "Successfully Unregister Callback");
    UT_Report(UnsuccessfulTestResult, "CFE_TIME_UnregisterSynchCallback", "Unsuccessful Unregister Callback");
    return;
}

void Test_CleanUpApp(void)
{
    int32 Status1 = CFE_SUCCESS;
    int32 Status2 = CFE_SUCCESS;
    
    /* Add multiple entries into callback registry table */
    Test_RegisterSynchCallback();
    
    /* Clean up the mess */
    Status1 = CFE_TIME_CleanUpApp(UT_AppID);
    
    /* Try to unregister an entry again to make sure they are all gone */
    Status2 = CFE_TIME_UnregisterSynchCallback((CFE_TIME_SynchCallbackPtr_t) &ut_time_MyCallbackFunc);
    
    UT_Report(((Status1 == CFE_SUCCESS) && (Status2 == CFE_TIME_CALLBACK_NOT_REGISTERED)), 
               "CFE_TIME_CleanUpApp", "Removed all callback entries for app");
}

/*
** Unit specifi call to process SB messages
*/
void UT_ProcessSBMsg(CFE_SB_Msg_t *MsgPtr)
{
  CFE_TIME_TaskPipe(MsgPtr);
}

