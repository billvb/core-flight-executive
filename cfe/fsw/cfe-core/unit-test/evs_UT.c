/*
** File:
**    $id:$
**
** Purpose:
**    EVS unit test
**
** References:
**    1. cFE Application Developers Guide
**    2. unit test standard 092503
**    3. C Coding Standard 102904
**
** Notes:
**    1. This is unit test code only, not for use in flight
**
** $Date: 2010/11/03 15:15:37EDT $
** $Revision: 1.4 $
**
*/
/*
** Includes
*/

#include <stdio.h>
#include <string.h>

#include "cfe.h"
#include "common_types.h"
#include "cfe_evs.h"
#include "../evs/cfe_evs_log.h"
#include "../evs/cfe_evs_task.h"
#include "../evs/cfe_evs_utils.h"
#include "cfe_sb.h"
#include "cfe_es.h"
#include "cfe_time.h"
#include "osapi.h"
#include "ut_stubs.h"

extern CFE_EVS_GlobalData_t CFE_EVS_GlobalData;

/*
** Function Definitions
*/
extern UT_SetRtn_t MutSemCreateRtn;
extern UT_SetRtn_t ES_RegisterRtn;
extern UT_SetRtn_t SB_CreatePipeRtn;
extern UT_SetRtn_t SB_SubscribeExRtn;
extern UT_SetRtn_t CreatePoolRtn;



void CFE_EVS_TaskMain (void);
void CFE_EVS_TaskInit (void);
int32 EVS_GetApplicationInfo (uint32 *, char *);
void CFE_EVS_ProcessCommandPacket (CFE_SB_MsgPtr_t EVS_MsgPtr);

void Test_Init (void);
void Test_IllegalAppID (void);
void Test_UnregisteredApp (void);
void Test_FilterRegistration (void);
void Test_FilterReset (void);
void Test_Format (void);
void Test_Ports (void);
void Test_Logging (void);
void Test_WriteApp (void);
void Test_BadAppCmd (void);
void Test_EventCmd (void);
void Test_FilterCmd (void);
void Test_Misc (void);

/*
** main function runs all the tests
*/
int main (void)
{

  /* Initialize UnitTest */
  UT_Init("ut_cfe_evs");


  /* Test_Init is a test but also MUST be called 1st and only once */
  Test_Init();

  Test_IllegalAppID();

  Test_UnregisteredApp();

  Test_FilterRegistration();

  Test_FilterReset();
  Test_Format();
  Test_Ports();
  Test_Logging();
  Test_WriteApp();
  Test_BadAppCmd();
  Test_EventCmd();
  Test_FilterCmd();
  Test_Misc();

  /* Final report on number of errors */
  UT_ReportFailures();

  exit(0);

}

/*
** Initialization test, must be called first
*/
void Test_Init (void)
{

  CFE_EVS_BitMaskCmd_t            bitmaskcmd;

  /* Initialize the CFE_EVS */
  CFE_EVS_EarlyInit();

  CFE_EVS_TaskMain();

  /* make the ES register fail */
  UT_SetRtnCode( &ES_RegisterRtn, -1 ,1 );
  CFE_EVS_TaskMain();
 
  /* make EVS_LogPtr fail */
  UT_SetStatusBSPResetArea(-1, CFE_TIME_RESET_SIGNATURE);
  CFE_EVS_EarlyInit();
  UT_SetStatusBSPResetArea(OS_SUCCESS, CFE_TIME_RESET_SIGNATURE);
 
  UT_SetRtnCode(&CreatePoolRtn, -1, 1);
  CFE_EVS_EarlyInit();

  UT_SetRtnCode(&MutSemCreateRtn, -1, 1);
  CFE_EVS_EarlyInit();


  UT_SetRtnCode(&ES_RegisterRtn, -1, 1);
  CFE_EVS_TaskInit();

  UT_SetRtnCode(&SB_CreatePipeRtn, -1, 1);
  CFE_EVS_TaskInit();

  UT_SetRtnCode(&SB_SubscribeExRtn, -1, 1);
  CFE_EVS_TaskInit();

  UT_SetRtnCode(&SB_SubscribeExRtn, -1, 2);
  CFE_EVS_TaskInit();

 /* Get the EVS registration to fail */
  UT_SetAppID(10000);
  CFE_EVS_TaskInit();

  /* TEST: initialize the task WARM */
  UT_SetAppID(0);
  CFE_EVS_TaskInit();


  /* Disable ports */
  bitmaskcmd.BitMask = 0x0F;
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_BitMaskCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&bitmaskcmd, CFE_EVS_CMD_MID,
             CFE_EVS_DISABLE_PORTS_CC);

}

/*
** Test functions using an illegal application ID
*/
void Test_IllegalAppID (void)
{

  CFE_TIME_SysTime_t      time;

  /* Set test up with illegal application ID */

  UT_SetAppID(CFE_ES_MAX_APPLICATIONS + 1);

  /* Call external functions to test behavior with illegal ID */
  UT_Report(CFE_EVS_Register(NULL, 0, 0) == CFE_EVS_APP_ILLEGAL_APP_ID,
            "CFE_EVS_Register", "illegal app id");

  UT_Report(CFE_EVS_Unregister() == CFE_EVS_APP_ILLEGAL_APP_ID,
            "CFE_EVS_Unregister", "illegal app id");

  UT_Report(CFE_EVS_SendEvent(0, 0, "NULL") == CFE_EVS_APP_ILLEGAL_APP_ID,
            "CFE_EVS_SendEvent", "illegal app id");

  UT_Report(CFE_EVS_SendTimedEvent(time, 0, 0,"NULL" ) ==
            CFE_EVS_APP_ILLEGAL_APP_ID,
            "CFE_EVS_SendTimedEvent", "illegal app id");


  UT_Report(CFE_EVS_SendEventWithAppID(0, 0, CFE_ES_MAX_APPLICATIONS + 1, "NULL")
            == CFE_EVS_APP_ILLEGAL_APP_ID,
            "CFE_EVS_SendEventWithAppID", "illegal app id");


  UT_Report(CFE_EVS_ResetFilter(0) == CFE_EVS_APP_ILLEGAL_APP_ID,
            "CFE_EVS_ResetFilter", "illegal app id");

  UT_Report(CFE_EVS_ResetAllFilters() == CFE_EVS_APP_ILLEGAL_APP_ID,
            "CFE_EVS_ResetAllFilters", "illegal app id");


  /* Return application ID to valid value */
  UT_SetAppID(0);


}

/*
** Test functions using an unregistered application
*/
void Test_UnregisteredApp (void)
{

  /* Unregister the application (it gets registered in CFE_EVS_TaskInit) */
  CFE_EVS_Unregister();

  

  /* TEST: application not registered */
  UT_Report(CFE_EVS_Unregister() == CFE_EVS_APP_NOT_REGISTERED,
            "CFE_EVS_Unregister","app not registered");

  UT_Report(CFE_EVS_SendEvent(0, 0, "NULL") == CFE_EVS_APP_NOT_REGISTERED,
            "CFE_EVS_SendEvent", "app not registered");


  UT_Report(CFE_EVS_ResetFilter(0) == CFE_EVS_APP_NOT_REGISTERED,
            "CFE_EVS_ResetFilter", "app not registered");
  
  UT_Report(CFE_EVS_ResetAllFilters() == CFE_EVS_APP_NOT_REGISTERED,
            "CFE_EVS_ResetAllFilters", "app not registered");

  /* Return application to original state, register application */
  CFE_EVS_Register(NULL, 0, CFE_EVS_BINARY_FILTER);

}

/*
** Test filter registration related calls
*/
void Test_FilterRegistration (void)
{

  int i;

  CFE_EVS_BinFilter_t filter[CFE_EVS_MAX_EVENT_FILTERS + 1];
  CFE_EVS_EVSBinFilter_t *FilterPtr = NULL;
  CFE_EVS_Table_t        *TablePtr;
  uint32                 AppID;
  CFE_EVS_BitMaskCmd_t            bitmaskcmd;

  /* TEST: unknown filter */
  UT_Report(CFE_EVS_Register(NULL, 0, -1) == CFE_EVS_UNKNOWN_FILTER,
            "CFE_EVS_Register","illegal filter option");

  /* TEST: valid registration with no filters */
  UT_Report(CFE_EVS_Register(NULL, 0, CFE_EVS_BINARY_FILTER) == CFE_SUCCESS,
            "CFE_EVS_Register", "valid w/ no filters");

  /* TEST: unregistration with failed ES_putPool */
  UT_SetPutPoolFail(1);
  UT_Report(CFE_EVS_Unregister() < 0, "CFE_EVS_Unregister",
            "unregistration with failed ES_putPool");
  UT_SetPutPoolFail(0);

  /* Re-register to test valid unregistration */
  UT_Report(CFE_EVS_Register(NULL, 0, CFE_EVS_BINARY_FILTER) == CFE_SUCCESS,
            "CFE_EVS_Register", "valid w/ no filters (re-registration)");

  /* TEST: valid unregistration */
  UT_Report(CFE_EVS_Unregister() == CFE_SUCCESS,
            "CFE_EVS_Unregister", "valid call");

  /* TEST: valid registration with filter */
  filter[0].EventID = 0;
  filter[0].Mask = 0x0001;

  UT_Report(CFE_EVS_Register(filter, 1, CFE_EVS_BINARY_FILTER) == CFE_SUCCESS,
            "CFE_EVS_Register", "valid w/ filter");

  /* TEST: multiple valid registration with more than MAX filters */
  for (i = 0; i < CFE_EVS_MAX_EVENT_FILTERS + 1; i++){
    filter[i].EventID = i;
    filter[i].Mask = 1;
  }

  UT_Report(CFE_EVS_Register(filter, CFE_EVS_MAX_EVENT_FILTERS + 1,
                     CFE_EVS_BINARY_FILTER) == CFE_EVS_APP_FILTER_OVERLOAD,
            "CFE_EVS_Register", "valid over max filters");

  /* TEST: Send 1st information message, should get through */
  UT_Report(CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, "OK") == CFE_SUCCESS,
            "CFE_EVS_SendEvent", "1st info message should go through");

  /* TEST: Send 2nd information message, should be filtered */
  UT_Report(CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, "FAILED") == CFE_SUCCESS,
            "CFE_EVS_SendEvent", "2nd info message should be filtered");

  /* TEST: Send Last information message, which should cause filtering to lock */
  CFE_ES_GetAppID(&AppID);
  TablePtr = CFE_EVS_GlobalData.EVS_TablePtr[AppID].Address;
  FilterPtr = EVS_FindEventID(0, (CFE_EVS_EVSBinFilter_t *)&TablePtr->BinFilters);
  FilterPtr->Count = CFE_EVS_MAX_FILTER_COUNT - 1;
  UT_Report(CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, "OK") == CFE_SUCCESS,
            "CFE_EVS_SendEvent", "Last info message should go through");
  UT_Report(CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, "FAILED") == CFE_SUCCESS,
            "CFE_EVS_SendEvent", "Locked info message should be filtered");
  UT_Report(CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, "FAILED") == CFE_SUCCESS,
            "CFE_EVS_SendEvent", "Locked info message should still be filtered");

  /* Return application to original state, re-register application */
  CFE_EVS_Register(NULL, 0, CFE_EVS_BINARY_FILTER);

}

/*
** Test reset filter calls
*/
void Test_FilterReset (void)
{

  CFE_EVS_BinFilter_t filter;

  filter.EventID = 1;
  filter.Mask = 0x0001;
  CFE_EVS_Register(&filter, 1, CFE_EVS_BINARY_FILTER);

  /* TEST: invalid ID for filter reset */
  UT_Report(CFE_EVS_ResetFilter(CFE_EVS_MAX_EVENT_FILTERS + 1) == CFE_SUCCESS,
            "CFE_EVS_ResetFilter", "invalid event ID");

  /* TEST: unregistered ID for filter reset */
  UT_Report(CFE_EVS_ResetFilter(-1) == CFE_SUCCESS,
            "CFE_EVS_ResetFilter", "unregistered event ID");

  /* TEST: valid ID for filter reset */
  UT_Report(CFE_EVS_ResetFilter(1) == CFE_SUCCESS,
            "CFE_EVS_ResetFilter", "valid reset filter");

  /* TEST: valid reset of all filters */
  UT_Report(CFE_EVS_ResetAllFilters() == CFE_SUCCESS,
            "CFE_EVS_ResetAllFilters", "valid reset all filters");

  /* Return application to original state, re-register application */
  CFE_EVS_Register(NULL, 0, CFE_EVS_BINARY_FILTER);

}

/*
** Test long and short format, also long string
*/
void Test_Format (void)
{

  int i;
  char long_msg[CFE_EVS_MAX_MESSAGE_LENGTH + 2];

  CFE_TIME_SysTime_t  time;
  CFE_EVS_ModeCmd_t   modecmd;

  /* TEST: invalid command to set format */
  modecmd.Mode = -1;
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_ModeCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_SET_EVENT_FORMAT_MODE_CC);

  /* TEST: valid command to set short format, reports implicitly via event */
  modecmd.Mode = CFE_EVS_SHORT_FORMAT;
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_SET_EVENT_FORMAT_MODE_CC);

  CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, "FAILED : short format selected");

  /* TEST; valid command to set long format, reports implicitly via event */
  modecmd.Mode = CFE_EVS_LONG_FORMAT;
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_SET_EVENT_FORMAT_MODE_CC);

  CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, "PASSED : long format selected");
  CFE_EVS_SendTimedEvent(time, 0, CFE_EVS_INFORMATION,
                         "PASSED : long format selected (SendTimedEvent)");
  CFE_EVS_SendEventWithAppID(0, CFE_EVS_INFORMATION, 0,
                         "PASSED : long format selected (SendEventWithAppID)");

  /* TEST: Send information message with > MAX string length */
  for (i = 0; i <= CFE_EVS_MAX_MESSAGE_LENGTH; i++)
    long_msg[i] = (char)(i%10 + 48);
  long_msg[CFE_EVS_MAX_MESSAGE_LENGTH + 1] = '\0';

  UT_Report(CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, long_msg) == CFE_SUCCESS,
            "CFE_EVS_SendEvent", "sent info msg with > MAX string length");

  UT_Report(CFE_EVS_SendEventWithAppID(0, CFE_EVS_INFORMATION,0, long_msg) == CFE_SUCCESS,
            "CFE_EVS_SendEventWithAppID", "sent info msg with > MAX string length");

  UT_Report(CFE_EVS_SendTimedEvent(time,0, CFE_EVS_INFORMATION, long_msg) == CFE_SUCCESS,
            "CFE_EVS_SendTimedEvent", "sent info msg with > MAX string length");
}

/*
** Test enable/disable of port outputs
*/
void Test_Ports (void)
{

  CFE_EVS_BitMaskCmd_t    bitmaskcmd;

  /* TEST: enable all ports, reports implicitly via port output output */
  bitmaskcmd.BitMask = 0x0F;
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_BitMaskCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&bitmaskcmd, CFE_EVS_CMD_MID,
             CFE_EVS_ENABLE_PORTS_CC);

  /* Send a message to test the ports */
  CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, "Test ports message");

  /* Disable all ports to cut down on unneeded output */
  bitmaskcmd.BitMask = 0x0F;
  UT_SendMsg((CFE_SB_MsgPtr_t)&bitmaskcmd, CFE_EVS_CMD_MID,
             CFE_EVS_DISABLE_PORTS_CC);

  /* Test sending a bitmask that is out of range */

  bitmaskcmd.BitMask = 0xFF;
  UT_SendMsg((CFE_SB_MsgPtr_t)&bitmaskcmd, CFE_EVS_CMD_MID,
             CFE_EVS_ENABLE_PORTS_CC);

  UT_SendMsg((CFE_SB_MsgPtr_t)&bitmaskcmd, CFE_EVS_CMD_MID,
             CFE_EVS_DISABLE_PORTS_CC);



}

/*
** Test logging
*/
void Test_Logging (void)
{

  int i;

  CFE_EVS_ModeCmd_t   modecmd;

  /* TEST: invalid logging mode */
  /* NOTE: consider sending an event for invalid logging mode */
  modecmd.Mode = -1;
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_ModeCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_SET_LOG_MODE_CC);

  /* TEST: set logging mode to DISCARD */
  modecmd.Mode = CFE_EVS_LOG_DISCARD;
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_SET_LOG_MODE_CC);

  /* TEST: overfill log in CFE_EVS_LOG_DISCARD mode */
  /* Ensure log is filled and add one more, implicitly testing EVS_LogFull */
  for(i = 0; !EVS_LogFull(); i++)
    CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, "log fill event %d", i);

  CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, "log overfill event discard");

  /* TEST: write log while full and open failure */
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_LogFileCmd_t));
  UT_SetOSFail(OS_CREAT_FAIL);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_FILE_WRITE_LOG_DATA_CC);

  /* TEST: write log while full and write/close failure */
  UT_SetOSFail(OS_WRITE_FAIL | OS_CLOSE_FAIL);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_FILE_WRITE_LOG_DATA_CC);

  /* TEST: write log while full */
  UT_SetOSFail(0);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_FILE_WRITE_LOG_DATA_CC);

  /* TEST: add to overfull log in CFE_EVS_LOG_OVERWRITE */
  modecmd.Mode = CFE_EVS_LOG_OVERWRITE;
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_ModeCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_SET_LOG_MODE_CC);
  CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, "log overfill event overwrite");

  /* TEST: verify overfull log gets written out in FIFO order */
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_LogFileCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_FILE_WRITE_LOG_DATA_CC);

  /* TEST: clear log */
  UT_SetSBTotalMsgLen(8);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_CLEAR_LOG_CC);

  /* TEST: write empty log */
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_LogFileCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_FILE_WRITE_LOG_DATA_CC);

  /* TEST: no op */
  UT_SetSBTotalMsgLen(8);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_NO_OPERATION_CC);

  /* TEST: write log w/ one message and write failure */
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_LogFileCmd_t));
  UT_SetOSFail(OS_WRITE_FAIL);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID,
             CFE_EVS_FILE_WRITE_LOG_DATA_CC);

}

/*
** Test writing application data
*/
void Test_WriteApp (void)
{

  CFE_EVS_AppDataCmd_t AppDataCmd;

  /* TEST: reset counters */
  UT_SetSBTotalMsgLen(8);
  UT_SendMsg((CFE_SB_MsgPtr_t)&AppDataCmd, CFE_EVS_CMD_MID,
             CFE_EVS_RESET_COUNTERS_CC);

  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppDataCmd_t));
  /* TEST: write application data with open failure */
  UT_SetOSFail(OS_CREAT_FAIL);
  UT_SendMsg((CFE_SB_MsgPtr_t)&AppDataCmd, CFE_EVS_CMD_MID,
             CFE_EVS_FILE_WRITE_APP_DATA_CC);

  /* TEST: write application data with write/close failure */
  UT_SetOSFail(OS_WRITE_FAIL | OS_CLOSE_FAIL);
  UT_SendMsg((CFE_SB_MsgPtr_t)&AppDataCmd, CFE_EVS_CMD_MID,
             CFE_EVS_FILE_WRITE_APP_DATA_CC);

  /* TEST: write application data with no failures */
  UT_SetOSFail(0);
  UT_SendMsg((CFE_SB_MsgPtr_t)&AppDataCmd, CFE_EVS_CMD_MID,
             CFE_EVS_FILE_WRITE_APP_DATA_CC);

  AppDataCmd.AppDataFilename[0] = '\0';
  UT_SendMsg((CFE_SB_MsgPtr_t)&AppDataCmd, CFE_EVS_CMD_MID,
             CFE_EVS_FILE_WRITE_APP_DATA_CC);


}

/*
** Test commands with bad application names (unknown, illegal, unregistered)
*/
void Test_BadAppCmd (void)
{

  int  i;
  char app_name[80];
    
  CFE_EVS_AppNameBitMaskCmd_t     appbitcmd;
  CFE_EVS_AppNameCmd_t            appnamecmd;
  CFE_EVS_AppNameEventIDMaskCmd_t appmaskcmd;
  CFE_EVS_AppNameEventIDCmd_t     appcmdcmd;

  /* Set up event and mask */
  appbitcmd.BitMask = 0;
  appmaskcmd.Mask = 0;
  appmaskcmd.EventID = 0;
  appcmdcmd.EventID = 0;
  
  /* Run tests with unknown name, illegal id, and unregisterd application */
  for (i=0; i<3; i++){
  
    if (i == 0)
      strcpy(app_name,"unknown name");
    else if (i == 1)
      strcpy(app_name,"illegal id");
    else if (i == 2)
      strcpy(app_name,"unregistered app");

    strcpy((char *)appbitcmd.AppName, app_name);
    strcpy((char *)appnamecmd.AppName, app_name);
    strcpy((char *)appmaskcmd.AppName, app_name);
    strcpy((char *)appcmdcmd.AppName, app_name);

    /* TEST: disable/enable app event type */
    UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameBitMaskCmd_t));
    UT_SendMsg((CFE_SB_MsgPtr_t)&appbitcmd, CFE_EVS_CMD_MID,
               CFE_EVS_DISABLE_APP_EVENT_TYPE_CC);
    UT_SendMsg((CFE_SB_MsgPtr_t)&appbitcmd, CFE_EVS_CMD_MID,
               CFE_EVS_ENABLE_APP_EVENT_TYPE_CC);

    /* TEST: disable/enable app events */
    UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameCmd_t));
    UT_SendMsg((CFE_SB_MsgPtr_t)&appnamecmd, CFE_EVS_CMD_MID,
               CFE_EVS_ENABLE_APP_EVENTS_CC);
    UT_SendMsg((CFE_SB_MsgPtr_t)&appnamecmd, CFE_EVS_CMD_MID,
               CFE_EVS_DISABLE_APP_EVENTS_CC);

    /* TEST: reset app event counter */
    UT_SendMsg((CFE_SB_MsgPtr_t)&appnamecmd, CFE_EVS_CMD_MID,
               CFE_EVS_RESET_APP_COUNTER_CC);

    /* TEST: modify filters */
    UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameEventIDMaskCmd_t));
    UT_SendMsg((CFE_SB_MsgPtr_t)&appmaskcmd, CFE_EVS_CMD_MID,
               CFE_EVS_ADD_EVENT_FILTER_CC);
    UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameEventIDCmd_t));
    UT_SendMsg((CFE_SB_MsgPtr_t)&appcmdcmd, CFE_EVS_CMD_MID,
               CFE_EVS_DELETE_EVENT_FILTER_CC);
    UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameEventIDMaskCmd_t));
    UT_SendMsg((CFE_SB_MsgPtr_t)&appcmdcmd, CFE_EVS_CMD_MID,
               CFE_EVS_SET_FILTER_CC);
    UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameEventIDCmd_t));
    UT_SendMsg((CFE_SB_MsgPtr_t)&appcmdcmd, CFE_EVS_CMD_MID,
               CFE_EVS_RESET_FILTER_CC);
    UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameCmd_t));
    UT_SendMsg((CFE_SB_MsgPtr_t)&appnamecmd, CFE_EVS_CMD_MID,
               CFE_EVS_RESET_ALL_FILTERS_CC);
  }

}

/*
** Test event commands (and reset counter)
*/
void Test_EventCmd (void)
{

  CFE_EVS_BitMaskCmd_t            bitmaskcmd;
  CFE_EVS_AppNameBitMaskCmd_t     appbitcmd;
  CFE_EVS_AppNameCmd_t            appnamecmd;

  /* Run the next series of tests with valid, registered application name */
  strcpy((char *)appbitcmd.AppName, "ut_cfe_evs");
  strcpy((char *)appnamecmd.AppName, "ut_cfe_evs");

  /* TEST: disable/enable/send app events */
  appbitcmd.BitMask = 0x0F;
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameBitMaskCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appbitcmd, CFE_EVS_CMD_MID,
             CFE_EVS_DISABLE_APP_EVENT_TYPE_CC);
  CFE_EVS_SendEvent(0, CFE_EVS_DEBUG, "FAIL : Debug message disabled");
  CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, "FAIL : Info message disabled");
  CFE_EVS_SendEvent(0, CFE_EVS_ERROR, "FAIL : Error message disabled");
  CFE_EVS_SendEvent(0, CFE_EVS_CRITICAL, "FAIL : Critical message disabled");

  UT_SendMsg((CFE_SB_MsgPtr_t)&appbitcmd, CFE_EVS_CMD_MID,
             CFE_EVS_ENABLE_APP_EVENT_TYPE_CC);
  CFE_EVS_SendEvent(0, CFE_EVS_DEBUG, "PASS : Debug message enabled");
  CFE_EVS_SendEvent(0, CFE_EVS_INFORMATION, "PASS : Info message enabled");
  CFE_EVS_SendEvent(0, CFE_EVS_ERROR, "PASS : Error message enabled");
  CFE_EVS_SendEvent(0, CFE_EVS_CRITICAL, "PASS : Critical message enabled");

  /* TEST: disable/enable app event illegal type */
  appbitcmd.BitMask = -1;
  UT_SendMsg((CFE_SB_MsgPtr_t)&appbitcmd, CFE_EVS_CMD_MID,
             CFE_EVS_DISABLE_APP_EVENT_TYPE_CC);
  CFE_EVS_SendEvent(0, -1, "FAIL : Illegal type disabled");
  UT_SendMsg((CFE_SB_MsgPtr_t)&appbitcmd, CFE_EVS_CMD_MID,
             CFE_EVS_ENABLE_APP_EVENT_TYPE_CC);

  /* TEST: disable/enable app events for valid app */
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appnamecmd, CFE_EVS_CMD_MID,
             CFE_EVS_ENABLE_APP_EVENTS_CC);
  UT_SendMsg((CFE_SB_MsgPtr_t)&appnamecmd, CFE_EVS_CMD_MID,
             CFE_EVS_DISABLE_APP_EVENTS_CC);

  /* TEST: disable/enable events */
  bitmaskcmd.BitMask = 0x0F;
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_BitMaskCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&bitmaskcmd, CFE_EVS_CMD_MID,
             CFE_EVS_DISABLE_EVENT_TYPE_CC);
  UT_SendMsg((CFE_SB_MsgPtr_t)&bitmaskcmd, CFE_EVS_CMD_MID,
             CFE_EVS_ENABLE_EVENT_TYPE_CC);

  /* TEST: reset app event counter with valid app */
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appnamecmd, CFE_EVS_CMD_MID,
             CFE_EVS_RESET_APP_COUNTER_CC);

  
  /* Test sending a bitmask that is out of range */

  bitmaskcmd.BitMask = 0xFF;
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_BitMaskCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&bitmaskcmd, CFE_EVS_CMD_MID,
             CFE_EVS_DISABLE_EVENT_TYPE_CC);
  UT_SendMsg((CFE_SB_MsgPtr_t)&bitmaskcmd, CFE_EVS_CMD_MID,
             CFE_EVS_ENABLE_EVENT_TYPE_CC);
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameBitMaskCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appbitcmd, CFE_EVS_CMD_MID,
             CFE_EVS_DISABLE_APP_EVENT_TYPE_CC);
  UT_SendMsg((CFE_SB_MsgPtr_t)&appbitcmd, CFE_EVS_CMD_MID,
             CFE_EVS_ENABLE_APP_EVENT_TYPE_CC);

}

/*
** Test filter commands
*/
void Test_FilterCmd (void)
{

  int  i;

  CFE_EVS_AppNameCmd_t            appnamecmd;
  CFE_EVS_AppNameEventIDMaskCmd_t appmaskcmd;
  CFE_EVS_AppNameEventIDCmd_t     appcmdcmd;

  /* Set up event and mask */
  appmaskcmd.Mask = 0;
  appmaskcmd.EventID = 0;
  appcmdcmd.EventID = 0;

  /* Run the next series of tests with valid, registered application name */
  strcpy((char *)appnamecmd.AppName, "ut_cfe_evs");
  strcpy((char *)appmaskcmd.AppName, "ut_cfe_evs");
  strcpy((char *)appcmdcmd.AppName, "ut_cfe_evs");

  /* Ensure there is no filter for the next tests */
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameEventIDCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appcmdcmd, CFE_EVS_CMD_MID,
             CFE_EVS_DELETE_EVENT_FILTER_CC);

  /* TEST: modify filters with valid app but no filter */
  /* NOTE: set filter is the same as add filter when one already exists */
  /*       consider consolidating functionality                         */
  /*       delete event filter is the same as setting the filter to     */
  /*       0xFFFF (I think)... why do this.                             */
  /*       Is space really that big of an issue?  Just store a filter   */
  /*       value with each event... always there, init to 0xFFFF        */
  /*       I guess if you want multiple filters on same event???        */
  /*       MULTIPLE FILTERS ON SAME EVENT IS NOT PROTECTED AGAINST!!!   */
  /*       could cause strange behavior                                 */
  UT_SendMsg((CFE_SB_MsgPtr_t)&appcmdcmd, CFE_EVS_CMD_MID,
             CFE_EVS_DELETE_EVENT_FILTER_CC);
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameEventIDMaskCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appmaskcmd, CFE_EVS_CMD_MID,
             CFE_EVS_SET_FILTER_CC);
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameEventIDCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appcmdcmd, CFE_EVS_CMD_MID,
             CFE_EVS_RESET_FILTER_CC);
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appnamecmd, CFE_EVS_CMD_MID,
             CFE_EVS_RESET_ALL_FILTERS_CC);
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameEventIDMaskCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appmaskcmd, CFE_EVS_CMD_MID,
             CFE_EVS_ADD_EVENT_FILTER_CC);

  /* TEST: modify filters with valid app and filter */
  UT_SendMsg((CFE_SB_MsgPtr_t)&appmaskcmd, CFE_EVS_CMD_MID,
             CFE_EVS_ADD_EVENT_FILTER_CC);
  UT_SendMsg((CFE_SB_MsgPtr_t)&appmaskcmd, CFE_EVS_CMD_MID,
             CFE_EVS_SET_FILTER_CC);
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameEventIDCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appcmdcmd, CFE_EVS_CMD_MID,
             CFE_EVS_RESET_FILTER_CC);
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appnamecmd, CFE_EVS_CMD_MID,
             CFE_EVS_RESET_ALL_FILTERS_CC);
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameEventIDCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appcmdcmd, CFE_EVS_CMD_MID,
             CFE_EVS_DELETE_EVENT_FILTER_CC);


  
  /* TEST: overfill filters */
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameEventIDMaskCmd_t));
  for (i = 0; i <= CFE_EVS_MAX_EVENT_FILTERS; i++)
  {
    UT_SendMsg((CFE_SB_MsgPtr_t)&appmaskcmd, CFE_EVS_CMD_MID,
               CFE_EVS_ADD_EVENT_FILTER_CC);
    appmaskcmd.EventID++;
  }
    appmaskcmd.EventID = 0;

  /* Return application to original state, re-register application */
  CFE_EVS_Register(NULL, 0, CFE_EVS_BINARY_FILTER);

  /* TEST: try filtering the same event twice */
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameEventIDMaskCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appcmdcmd, CFE_EVS_CMD_MID,
             CFE_EVS_ADD_EVENT_FILTER_CC);
  UT_SendMsg((CFE_SB_MsgPtr_t)&appcmdcmd, CFE_EVS_CMD_MID,
             CFE_EVS_ADD_EVENT_FILTER_CC);
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_AppNameEventIDCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&appcmdcmd, CFE_EVS_CMD_MID,
             CFE_EVS_DELETE_EVENT_FILTER_CC);

  /* Return application to original state, re-register application */
  CFE_EVS_Register(NULL, 0, CFE_EVS_BINARY_FILTER);

}

/*
** Test miscellaneous functionality
*/
void Test_Misc (void)
{

  CFE_EVS_ModeCmd_t   modecmd;



  /* TEST: null inputs to exercise EVS_GetApplicationInfo */
  UT_Report(EVS_GetApplicationInfo(NULL,NULL) == CFE_ES_ERR_BUFFER,
            "EVS_GetApplicationInfo","get application info with null inputs");

  /* TEST: send event with an illegal type */
  CFE_EVS_SendEvent(0, -1, "Illegal type no filter");

  /* TEST: Invalid message id */
  UT_SetSBTotalMsgLen(sizeof(CFE_EVS_ModeCmd_t));
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, -1, 0);

  /* TEST: Invalid command */
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, -1);

  /* TEST: Invalid command length */
  UT_SetSBTotalMsgLen(6);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 1);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 2);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 3);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 4);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 5);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 6);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 7);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 8);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 9);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 10);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 11);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 12);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 13);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 14);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 15);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 16);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 17);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 18);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 19);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 20);
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_CMD_MID, 21);


  /* TEST: report housekeeping */
  UT_SendMsg((CFE_SB_MsgPtr_t)&modecmd, CFE_EVS_SEND_HK_MID, 0);


  /* Test failure of getting the reset area */
  UT_SetStatusBSPResetArea(-1,-1);
  EVS_InitLogPtr();

  CFE_EVS_CleanUpApp(0);
}

/*
** Unit specific call to process SB messages
*/
void UT_ProcessSBMsg(CFE_SB_Msg_t *MsgPtr)
{
  CFE_EVS_ProcessCommandPacket(MsgPtr);
}


