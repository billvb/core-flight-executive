/*
** File:
**    es_UT.c
**
** Purpose:
**    ES unit test
**
** References:
**    1. cFE Application Developers Guide
**    2. unit test standard 092503
**    3. C Coding Standard 102904
**
** Notes:
**    1. This is unit test code only, not for use in flight
**
** $Date: 2010/11/05 15:54:32EDT $
** $Revision: 1.9 $
**
*/

/*
** Includes
*/
#include <stdio.h>
#include <string.h>
#include "cfe.h"
#include "common_types.h"
#include "osapi.h"
#include "osapi-os-loader.h" 

#include "../src/es/cfe_es_cds.h"
#include "../src/es/cfe_es_cds_mempool.h"
#include "../src/es/cfe_es_global.h"
#include "../src/es/cfe_es_log.h"
#include "../src/es/cfe_es_perf.h"
#include "../src/es/cfe_es_shell.h"
#include "../src/es/cfe_es_task.h"
#include "../src/es/cfe_es_verify.h"
#include "../src/es/cfe_es_version.h"
#include "../src/es/cfe_es_start.h"
#include "ut_stubs.h"

/*
** Externally declared variables
*/

/**************************************************/
int32 CFE_ES_ValidateCDS(void);
int32 CFE_ES_InitializeCDS(uint32 CDSSize);
int32 CFE_ES_InitCDSRegistry(void);
int32 CFE_ES_RebuildCDS(void);
int32 CFE_ES_ListResourcesDebug(void);



#define CFE_ES_DBIT(x) (1L << (x))                                    /* Places a one at bit positions 0 thru 31 */
#define CFE_ES_DTEST(i,x) (((i) & CFE_ES_DBIT(x)) != 0)               /* TRUE iff bit x of i is set */
#define CFE_ES_TEST_LONG_MASK(m,s)  (CFE_ES_DTEST(m[(s)/32],(s)%32))  /* Test a bit within an array of 32-bit integers. */


void CFE_ES_OSPerfLog_Add(uint32 Address, uint32 EntryExit);
void CFE_ES_WriteToFile(char * FileName);
void CFE_ES_ProcessCoreException(uint32  HostTaskId,     uint8 *ReasonString, 
                                 uint32 *ContextPointer, uint32 ContextSize);                                   


/********************************************************************/


extern FILE             *UT_logfile;
extern UT_SetRtn_t      QueuePutRtn;
extern UT_SetRtn_t      QueueGetRtn;
extern UT_SetRtn_t      CreatePoolRtn;
extern UT_SetRtn_t      GetPoolRtn;
extern UT_SetRtn_t      PutPoolRtn;
extern UT_SetRtn_t      MutSemGiveRtn;
extern UT_SetRtn_t      MutSemTakeRtn;
extern UT_SetRtn_t      BinSemGiveRtn;
extern UT_SetRtn_t      FSWriteHdrRtn;
extern UT_SetRtn_t OSReadRtn; 
extern UT_SetRtn_t OSlseekRtn; 
extern UT_SetRtn_t  EVS_RegisterRtn;
extern UT_SetRtn_t  EVS_SendEventRtn;
extern UT_SetRtn_t BSPWriteCDSRtn;
extern UT_SetRtn_t BSPReadCDSRtn;
extern UT_SetRtn_t BSPUnloadAppFileRtn;
extern UT_SetRtn_t BlocksFreeRtn;
extern UT_SetRtn_t UnmountRtn;
extern UT_SetRtn_t SBCleanUpRtn;
extern UT_SetRtn_t EVSCleanUpRtn;
extern UT_SetRtn_t TIMECleanUpRtn;


extern UT_SetRtn_t  SB_SubscribeRtn;
extern UT_SetRtn_t  SB_SubscribeExRtn;
extern UT_SetRtn_t  SB_SubscribeLocalRtn;
extern UT_SetRtn_t  SB_CreatePipeRtn;
extern UT_SetRtn_t	SBSendMsgRtn;
extern UT_SetRtn_t CountSemDelRtn;
extern UT_SetRtn_t MutSemDelRtn;
extern UT_SetRtn_t BinSemDelRtn;
extern UT_SetRtn_t QueueDelRtn;
extern UT_SetRtn_t FileWriteRtn;
extern UT_SetRtn_t HeapGetInfoRtn;

extern UT_SetRtn_t BSPGetCFETextRtn;
extern UT_SetRtn_t FSIsGzFileRtn;

extern UT_SetRtn_t ModuleLoadRtn;
extern UT_SetRtn_t ModuleUnloadRtn;
extern UT_SetRtn_t ModuleInfoRtn;

extern UT_SetRtn_t FSDecompressRtn;
extern UT_SetRtn_t FSExtractRtn;
extern UT_SetRtn_t SymbolLookupRtn;


extern UT_Task_t        UT_Task      [OS_MAX_TASKS];
extern CFE_ES_PerfData_t      *Perf;
extern CFE_ES_Global_t CFE_ES_Global;
extern CFE_ES_CDSPool_t      CFE_ES_CDSMemPool;
extern CFE_ES_CDSBlockDesc_t CFE_ES_CDSBlockDesc;





char StartupScript [70 * 4 + 25 +1000];

void TestInit(void);
void TestStartupErrorPaths(void);
void TestApps(void);
void TestERLog(void);
void TestCDS(void);
void TestShell(void);
void TestTask(void);
void TestPerf(void);

void TestAPI(void);
void TestCDSMempool(void);
void TestESMempool(void);

void UT_ProcessSBMsg(CFE_SB_Msg_t *MsgPtr)
{
    CFE_ES_TaskPipe(MsgPtr);
}


int main(void)
{

    /* set up the performance logging variable */

    Perf = (CFE_ES_PerfData_t *)&(CFE_ES_ResetDataPtr->Perf); 
    TestInit();
    
    
    TestStartupErrorPaths(); 
    /* done testing cfe_es_start.c */ 

    TestApps();   
    /* NOT  done testing cfe_es_apps.c */
    TestERLog();
    /* done testing cfe_es_erlog.c*/
    TestShell();
    /*done testing cfe_es_shell.c */
    TestTask();
    /* done testing the cfe_es_task.c */
    
    TestPerf();
    /* done testing cfe_es_perf.c */
    TestAPI();

    /* done testing cfe_es_api.c */
    TestCDS();
    /* done testing cfe_es_cds.c */

    TestCDSMempool();
    /* done testing cfe_es_cds_mempool.c */
   TestESMempool();

    
    UT_ReportFailures();
    return 0;
}

/* This function goes through the main ES Startup */
void TestInit(void)
{
    int i;
    
    int NumBytes;

    UT_Init("ut_cfe_es");
    UT_SetCDSSize(128* 1024); 
    UT_SetSizeofESResetArea(sizeof(CFE_ES_ResetData_t));
    /* initialize the OS Tasks table */
    for (i = 0; i < OS_MAX_TASKS; i++) UT_Task[i].free = TRUE;
    /* set up the reset area */
    UT_SetStatusBSPResetArea(OS_SUCCESS, CFE_TIME_RESET_SIGNATURE);
    /* set up the startup script for reading*/
       
    strcpy(StartupScript, "CFE_LIB, /cf/apps/tst_lib.bundle,  TST_LIB_Init,  TST_LIB,    0,   0,    0x0, 1; CFE_APP, /cf/apps/ci.bundle,       CI_task_main,  CI_APP,    70,   4096, 0x0, 1; CFE_APP, /cf/apps/sch.bundle,      SCH_TaskMain,  SCH_APP,   120,  4096, 0x0, 1; CFE_APP, /cf/apps/to.bundle,       TO_task_main,  TO_APP,    74,   4096, 0x0, 1; !");
    
    NumBytes = strlen(StartupScript);
    UT_SetReadBuffer(StartupScript,NumBytes);
    /* go through ES_Main and try to cover all of the main paths */
    UT_SetDummyFuncRtn(OS_SUCCESS);
    UT_SetBSPloadAppFileResult (TRUE);
    CFE_ES_Main(CFE_ES_POWERON_RESET,1,1, (uint8 *)CFE_ES_NONVOL_STARTUP_FILE);
    /* Reset startup script for future tests */
    UT_SetReadBuffer(StartupScript,NumBytes);
}




/* This function works on getting the rest of path coverage for cfe_es_start.c */
void TestStartupErrorPaths(void)
{
    int j;

    /* get the mutex creation to fail */

    UT_SetOSFail(OS_MUTCREATE_FAIL);
    CFE_ES_Main(CFE_ES_POWERON_RESET,1,1,(uint8 *)CFE_ES_NONVOL_STARTUP_FILE);


    /* try and get the rest of the startup fail paths */
    UT_SetOSFail(0);


    /* get the mutex creation to fail */
    
    UT_SetOSFail(OS_SEMCREATE_FAIL);
    CFE_ES_Main(CFE_ES_POWERON_RESET,1,1,(uint8 *)CFE_ES_NONVOL_STARTUP_FILE);
    
    
    /* try and get the rest of the startup fail paths */
    UT_SetOSFail(0);
    
    
    
    /*get ES to try a power on restart after doing max number of processor restarts*/

    CFE_ES_ResetDataPtr->ResetVars.ES_CausedReset = FALSE;

    CFE_ES_SetupResetVariables(CFE_ES_PROCESSOR_RESET, 1, 1 );
    CFE_ES_SetupResetVariables(CFE_ES_PROCESSOR_RESET, 1, 1 );
    CFE_ES_SetupResetVariables(CFE_ES_PROCESSOR_RESET, 1, 1 );
    CFE_ES_SetupResetVariables(CFE_ES_PROCESSOR_RESET, 1, 1 );
    CFE_ES_SetupResetVariables(CFE_ES_PROCESSOR_RESET, 1, 1 );
    

    /* tell ES That a processor reset was commanded */
    UT_SetStatusBSPResetArea(OS_ERROR, 0 );
    CFE_ES_ResetDataPtr->ResetVars.ES_CausedReset = TRUE;
    CFE_ES_SetupResetVariables(CFE_ES_PROCESSOR_RESET, 1, 1 );

    /* test error condition where the sizeof the area gotten is too small */
    UT_SetSizeofESResetArea(0);
    UT_SetStatusBSPResetArea(OS_SUCCESS, 0 );
    CFE_ES_SetupResetVariables(CFE_ES_PROCESSOR_RESET, 1, 1 );
    UT_SetOSFail(OS_INITFS_FAIL | OS_MOUNT_FAIL | OS_MKFS_FAIL);
    CFE_ES_InitializeFileSystems(CFE_ES_POWERON_RESET);
    UT_SetOSFail(OS_INITFS_FAIL | OS_MOUNT_FAIL | OS_MKFS_FAIL);
    CFE_ES_InitializeFileSystems(CFE_ES_PROCESSOR_RESET);
   
   /* Make the BSP call fail to get to the error code */
    UT_SetBSPFail(BSP_GETVOLDISKMEM_FAIL);
    CFE_ES_InitializeFileSystems(CFE_ES_PROCESSOR_RESET);
 UT_SetBSPFail(0);
 UT_SetOSFail(0);

    UT_SetOSFail(OS_MOUNT_FAIL);
    UT_SetRtnCode( &BlocksFreeRtn, CFE_ES_RAM_DISK_NUM_SECTORS+ 1, 1);
    CFE_ES_InitializeFileSystems(CFE_ES_PROCESSOR_RESET);

    UT_SetOSFail(OS_RMFS_FAIL);
    CFE_ES_InitializeFileSystems(CFE_ES_PROCESSOR_RESET);

    UT_SetRtnCode( &UnmountRtn, -1, 1); 
    CFE_ES_InitializeFileSystems(CFE_ES_PROCESSOR_RESET);

    UT_SetOSFail(0);
    CFE_ES_InitializeFileSystems(4564564);

    UT_SetOSFail(OS_MOUNT_FAIL);
    CFE_ES_InitializeFileSystems(CFE_ES_PROCESSOR_RESET);


    UT_SetOSFail(0);
    UT_SetRtnCode( &BlocksFreeRtn, -1, 1);

    CFE_ES_InitializeFileSystems(CFE_ES_PROCESSOR_RESET);

   /* Get thea run though to cover the if CFE_ES_Global.TaskTable[xxx].RecordUsed == TRUE path covered */
    CFE_ES_CreateObjects();


    /* Cover the paths of failing OS_TaskCreate and OS_BinSemCreate*/
   UT_SetOSFail(OS_TASKCREATE_FAIL | OS_SEMCREATE_FAIL);
    CFE_ES_CreateObjects();

    /* cover the path were all app slots are taken */
    UT_SetOSFail(0);    
    for ( j = 0; j < CFE_ES_MAX_APPLICATIONS; j++ )
    {
       CFE_ES_Global.AppTable[j].RecordUsed = TRUE;
    }
    CFE_ES_CreateObjects();
    
    /* cover the path were a mutex is created sucessfully on startup */
    CFE_ES_ObjectTable[0].ObjectType = CFE_ES_MUTEX_SEM;
    /* Cover the path were a function pointer is null on startup */
    CFE_ES_ObjectTable[1].ObjectType = CFE_ES_FUNCTION_CALL;
    /*CFE_ES_ObjectTable[1].FuncPtrUnion = 0;    */
    CFE_ES_CreateObjects();
    
    /* cover the path of a mutex created at the start failing */
    UT_SetOSFail(OS_MUTCREATE_FAIL);
    CFE_ES_CreateObjects();
    UT_SetOSFail(0);
}
void TestApps(void)
{
    int NumBytes;
    uint32 Id;
    int Return;
    int j;
    CFE_ES_AppInfo_t AppInfo;




    /*make a startupscript that is too long. Filled with jibberish to make it long */
    strcpy(StartupScript, "CFE_LIB, /cf/apps/tst_lib.bundle,  TST_LIB_Init,  TST_fghfghfghfghfghfghfghfghfghfghfghfghfghfghfghfghfghfghfghfghfghfghfghfghfghLIB,    0,   0,    0x0, 1; CFE_APP, /cf/apps/ci.bundle,       CI_task_main,  CI_APP,    70,   4096, 0x0, 1; CFE_APP, /cf/apps/sch.bundle,      SCH_TaskMain,  SCH_APP,   120,  4096, 0x0, 1; CFE_APP, /cf/apps/to.bundle,       TO_task_main,  TO_APP,    74,   4096, 0x0, 1; !");
    NumBytes = strlen(StartupScript);
    UT_SetReadBuffer(StartupScript,NumBytes);

    /* Test path of the OS_read's failing in ES_StartApps */
    CFE_ES_StartApplications(CFE_ES_PROCESSOR_RESET,(uint8 *)CFE_ES_NONVOL_STARTUP_FILE);

    /* set up the startup script for reading*/
    strcpy(StartupScript, "CFE_LIB, /cf/apps/tst_lib.bundle,  TST_LIB_Init,  TST_LIB,    0,   0,    0x0, 1; CFE_APP, /cf/apps/ci.bundle,       CI_task_main,  CI_APP,    70,   4096, 0x0, 1; CFE_APP, /cf/apps/sch.bundle,      SCH_TaskMain,  SCH_APP,   120,  4096, 0x0, 1; CFE_APP, /cf/apps/to.bundle,       TO_task_main,  TO_APP,    74,   4096, 0x0, 1; !");
    NumBytes = strlen(StartupScript);
    UT_SetReadBuffer(StartupScript,NumBytes);


    /* Test path of the OS_read's failing in ES_StartApps */
    UT_SetRtnCode( &OSReadRtn, -1, 1); 
    CFE_ES_StartApplications(CFE_ES_PROCESSOR_RESET,(uint8 *)CFE_ES_NONVOL_STARTUP_FILE);


    /* Test path of the OS_read's return of EOF in ES_StartApps */
    UT_SetRtnCode( &OSReadRtn, 0, 1); 
    CFE_ES_StartApplications(CFE_ES_PROCESSOR_RESET,(uint8 *)CFE_ES_NONVOL_STARTUP_FILE);


    /* Test path of the OS_open's failing in ES_StartApps */
    UT_SetOSFail(OS_OPEN_FAIL);
    CFE_ES_StartApplications(CFE_ES_PROCESSOR_RESET,(uint8 *)CFE_ES_NONVOL_STARTUP_FILE);
    
    /* Test paths of the OS_opens working in ES_StartApps */
    UT_SetOSFail(0);
    UT_SetReadBuffer(StartupScript,NumBytes);
    CFE_ES_StartApplications(CFE_ES_PROCESSOR_RESET,(uint8 *)CFE_ES_NONVOL_STARTUP_FILE);


   /* set the startup script for a correctly parsed file, but with a CFE Driver Type*/
    strcpy(StartupScript, "CFE_DRV /cf/apps/tst_lib.bundle TST_LIB_Init TST_LIB 0 0 0x0 1");
    NumBytes = strlen(StartupScript);
    UT_SetReadBuffer(StartupScript,NumBytes);
 
    UT_Report (CFE_ES_ParseFileEntry(StartupScript) == CFE_ES_ERR_APP_CREATE, 
               "CFE_ES_ParseFileEntry", "unimplemented CFE Driver type");
   
    /* set the startup script for a correctly parsed file, but with a CFE Driver Type*/
    strcpy(StartupScript, "UNKNOWN /cf/apps/tst_lib.bundle TST_LIB_Init TST_LIB 0 0 0x0 1");
    NumBytes = strlen(StartupScript);
    UT_SetReadBuffer(StartupScript,NumBytes);

    
    UT_Report (CFE_ES_ParseFileEntry(StartupScript) == CFE_ES_ERR_APP_CREATE, 
               "CFE_ES_ParseFileEntry", "uknown type of entry");


    /* set up the startup script for reading badly parsed file*/
    strcpy(StartupScript, " ");
    NumBytes = strlen(StartupScript);
    UT_SetReadBuffer(StartupScript,NumBytes);

    UT_Report (CFE_ES_ParseFileEntry(StartupScript) == CFE_ES_ERR_APP_CREATE, 
               "CFE_ES_ParseFileEntry", "sscanf failed");

    for ( j = 0; j < CFE_ES_MAX_APPLICATIONS; j++ )
    {
       CFE_ES_Global.AppTable[j].RecordUsed = FALSE;
    }

    /* Cover path with a good load, but a failed Task Create */
    UT_SetBSPloadAppFileResult(TRUE);
    UT_SetOSFail(OS_TASKCREATE_FAIL);
    Return = CFE_ES_AppCreate(&Id,"ut/filename","EntryPoint","AppName",170,4096,1);
    UT_Report(Return == CFE_ES_ERR_APP_CREATE,"CFE_ES_AppCreate","TaskCreate Fail");

    
    /* Cover path with a good load, good create, and there are free task slots left */
    for (j= 0; j< OS_MAX_TASKS; j++)
    {
        CFE_ES_Global.TaskTable[j].RecordUsed = FALSE;
    }
    
    UT_SetBSPloadAppFileResult(TRUE);
    UT_SetOSFail(0);
    Return = CFE_ES_AppCreate(&Id,"ut/filename.x","EntryPoint","AppName",170,8192,1);
    UT_Report(Return == CFE_SUCCESS,"CFE_ES_AppCreate","Sucessful Run");



    
    UT_SetRtnCode(&FSIsGzFileRtn, TRUE, 1);
    UT_SetRtnCode(&FSDecompressRtn, -1, 1);
    Return = CFE_ES_AppCreate(&Id,"ut/filename.gz","EntryPoint","AppName",170,8192,1);
    UT_Report(Return == CFE_ES_ERR_APP_CREATE,"CFE_ES_AppCreate","Decompression Fail");

    

    UT_SetRtnCode(&FSIsGzFileRtn, TRUE, 1);
        UT_SetRtnCode(&FSDecompressRtn, 0, 1);
    Return = CFE_ES_AppCreate(&Id,"ut/filename.gz","EntryPoint","AppName",170,8192,1);
    UT_Report(Return == CFE_SUCCESS,"CFE_ES_AppCreate","Decompression passed");
    

    UT_SetRtnCode(&FSIsGzFileRtn, TRUE, 1);
    UT_SetRtnCode(&FSExtractRtn, -1, 1);
    Return = CFE_ES_AppCreate(&Id,"ut/filename.gz","EntryPoint","AppName",170,8192,1);
    UT_Report(Return == CFE_ES_ERR_APP_CREATE,"CFE_ES_AppCreate","Extraction failed");
    
    UT_SetRtnCode(&ModuleLoadRtn, -1, 1);
    Return = CFE_ES_AppCreate(&Id,"ut/filename.x","EntryPoint","AppName",170,8192,1);
    UT_Report(Return == CFE_ES_ERR_APP_CREATE,"CFE_ES_AppCreate","Module Load failed");
    
    UT_SetRtnCode(&SymbolLookupRtn, -1, 1);
    Return = CFE_ES_AppCreate(&Id,"ut/filename.x","EntryPoint","AppName",170,8192,1);
    UT_Report(Return == CFE_ES_ERR_APP_CREATE,"CFE_ES_AppCreate","Symbol Lookup failed");
    
    UT_SetRtnCode(&FSIsGzFileRtn, TRUE, 1);
    Return = CFE_ES_AppCreate(&Id,"ut/this_is_a_filename_that_exceeds_the_maximum_allowed_length_when_added_to_path.gz","EntryPoint","AppName",170,8192,1);
    UT_Report(Return == CFE_ES_ERR_APP_CREATE,"CFE_ES_AppCreate","App Name and Path too long");

    UT_SetRtnCode(&FSIsGzFileRtn, TRUE, 1);
    Return = CFE_ES_AppCreate(&Id,"ut/123456789012345678901234567890123456789012345678901234567890.gz","EntryPoint","AppName",170,8192,1);
    UT_Report(Return == CFE_ES_ERR_APP_CREATE,"CFE_ES_AppCreate","App Name and Path too long");

   
    
    /* Test path of a good library load, but the initialization routine returns an error*/
    UT_SetBSPloadAppFileResult(TRUE);
    UT_SetDummyFuncRtn(OS_ERROR);
    UT_SetOSFail(0);
    Return = CFE_ES_LoadLibrary(&Id,"filename","EntryPoint","LibName");
    UT_Report(Return == CFE_ES_ERR_LOAD_LIB,"CFE_ES_LoadLibrary","Library Init routine failed");


    /* test with a gzip'd library */
    UT_SetBSPloadAppFileResult(TRUE);
    
    UT_SetRtnCode(&FSIsGzFileRtn, TRUE, 1);
    UT_SetDummyFuncRtn(OS_SUCCESS);
    UT_SetOSFail(0);
    Return = CFE_ES_LoadLibrary(&Id,"/cf/apps/tst_lib.bundle.gz","TST_LIB_Init","TST_LIB");
    UT_Report(Return == CFE_SUCCESS,"CFE_ES_LoadLibrary","Gziped Library");
    
    
    
    /* Make the decompression fail */
        
    UT_SetBSPloadAppFileResult(TRUE);
    UT_SetRtnCode(&FSIsGzFileRtn, TRUE, 1);

 
    UT_SetRtnCode(&FSDecompressRtn, -1, 1);

    UT_SetDummyFuncRtn(OS_SUCCESS);

    Return = CFE_ES_LoadLibrary(&Id,"/cf/apps/tst_lib.bundle.gz","TST_LIB_Init","TST_LIB");
    UT_Report(Return == CFE_ES_ERR_LOAD_LIB,"CFE_ES_LoadLibrary","Library Init routine failed");
    

    UT_SetRtnCode(&FSIsGzFileRtn, TRUE, 1);
    UT_SetRtnCode(&FSExtractRtn, -1, 1);
    UT_SetDummyFuncRtn(OS_SUCCESS);
    Return = CFE_ES_LoadLibrary(&Id,"/cf/apps/tst_lib.bundle.gz","TST_LIB_Init","TST_LIB");
    UT_Report(Return == CFE_ES_ERR_LOAD_LIB,"CFE_ES_LoadLibrary","Library Init routine failed");
    
    UT_SetRtnCode(&FSIsGzFileRtn, TRUE, 1);
    UT_SetDummyFuncRtn(OS_SUCCESS);
    Return = CFE_ES_LoadLibrary(&Id,"/cf/apps/this_is_a_filename_that_exceeds_the_maximum_allowed_length_when_added_to_path.gz","TST_LIB_Init","TST_LIB");
    UT_Report(Return == CFE_ES_ERR_LOAD_LIB,"CFE_ES_LoadLibrary","Library Name and Path too long");
    

    UT_SetRtnCode(&FSIsGzFileRtn, TRUE, 1);
    UT_SetDummyFuncRtn(OS_SUCCESS);
    Return = CFE_ES_LoadLibrary(&Id,"/cf/apps/123456789012345678901234567890123456789012345678901234567890.gz","TST_LIB_Init","TST_LIB");
    UT_Report(Return == CFE_ES_ERR_LOAD_LIB,"CFE_ES_LoadLibrary","Library Name and Path too long");
    

    
    UT_SetRtnCode(&ModuleLoadRtn, -1, 1);
    Return = CFE_ES_LoadLibrary(&Id,"/cf/apps/tst_lib.bundle.gz","TST_LIB_Init","TST_LIB");
    UT_Report(Return == CFE_ES_ERR_LOAD_LIB,"CFE_ES_LoadLibrary","Library Init routine failed");
    

    UT_SetRtnCode(&SymbolLookupRtn, -1, 1);
    Return = CFE_ES_LoadLibrary(&Id,"/cf/apps/tst_lib.bundle.gz","TST_LIB_Init","TST_LIB");
    UT_Report(Return == CFE_ES_ERR_LOAD_LIB,"CFE_ES_LoadLibrary","Library Init routine failed");
    
    
    
    /* Test path of no more library slots left*/
    
    for (j= 0; j < CFE_ES_MAX_LIBRARIES; j++)
    {
        CFE_ES_Global.LibTable[j].RecordUsed = TRUE;
    }
    Return = CFE_ES_LoadLibrary(&Id,"filename","EntryPoint","LibName");
    UT_Report(Return == CFE_ES_ERR_LOAD_LIB,"CFE_ES_LoadLibrary","No free library slots");
    
    
    
    /* New functions */
    #define ES_APP_DEBUG
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_EXTERNAL;
    CFE_ES_Global.AppTable[0].StateRecord.AppState = CFE_ES_APP_STATE_WAITING;
    CFE_ES_Global.AppTable[0].StateRecord.AppTimer = 0;
    
    CFE_ES_ScanAppTable();

    /* deduct from timer */
    CFE_ES_Global.AppTable[0].StateRecord.AppTimer = 5;
    CFE_ES_ScanAppTable();

    CFE_ES_Global.AppTable[0].StateRecord.AppState = CFE_ES_APP_STATE_STOPPED;
    CFE_ES_ScanAppTable();

    
    
    /* Default Case for switch */
        CFE_ES_ProcessControlRequest(0);
    
    

    
    
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_EXTERNAL;
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.FileName), "/ram/Filename");
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.EntryPoint), "NotNULL");
    CFE_ES_Global.AppTable[0].StartParams.Priority = 255;
    CFE_ES_Global.AppTable[0].StartParams.StackSize = 8192;
    CFE_ES_Global.AppTable[0].StartParams.ExceptionAction =0;
    UT_SetOSFail(0);
    UT_SetBSPloadAppFileResult(TRUE);
    
    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_APP_EXIT;
    CFE_ES_ProcessControlRequest(0);


    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_APP_EXIT;
    UT_SetRtnCode(&EVSCleanUpRtn, -1, 1);
    CFE_ES_ProcessControlRequest(0);
    

    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_APP_ERROR;
    UT_SetRtnCode(&EVSCleanUpRtn, -1, 1);
    CFE_ES_ProcessControlRequest(0);
    
    
    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_DELETE;
    UT_SetRtnCode(&EVSCleanUpRtn, -1, 1);
    CFE_ES_ProcessControlRequest(0);

    
    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_RESTART;
    UT_SetRtnCode(&EVSCleanUpRtn, -1, 1);
    CFE_ES_ProcessControlRequest(0);
    
    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_RESTART;
    UT_SetOSFail(OS_TASKCREATE_FAIL);
    CFE_ES_ProcessControlRequest(0);
    UT_SetOSFail(0);

    
    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_RELOAD;
    UT_SetRtnCode(&EVSCleanUpRtn, -1, 1);
    CFE_ES_ProcessControlRequest(0);
    
    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_RELOAD;
    UT_SetOSFail(OS_TASKCREATE_FAIL);
    CFE_ES_ProcessControlRequest(0);
    UT_SetOSFail(0);
    
    
    UT_SetRtnCode(& BSPUnloadAppFileRtn, -1, 1);
    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_APP_EXIT;
    CFE_ES_ProcessControlRequest(0);


    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_EXTERNAL;
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.FileName), "/ram/FileName");
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.EntryPoint), "NULL");
    CFE_ES_Global.AppTable[0].StartParams.Priority = 255;
    CFE_ES_Global.AppTable[0].StartParams.StackSize = 8192;
   /* njy  CFE_ES_Global.AppTable[0].StartParams.LoadAddress = 0; */
    CFE_ES_Global.AppTable[0].StartParams.ExceptionAction =0;
    UT_SetOSFail(0);

    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_APP_ERROR;
    CFE_ES_ProcessControlRequest(0);
    UT_SetRtnCode(& BSPUnloadAppFileRtn, -1, 1);
    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_APP_ERROR;
    CFE_ES_ProcessControlRequest(0);


    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_EXTERNAL;
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.FileName), "/ram/FileName");
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.EntryPoint), "NULL");
    CFE_ES_Global.AppTable[0].StartParams.Priority = 255;
    CFE_ES_Global.AppTable[0].StartParams.StackSize = 8192;
  /*njy   CFE_ES_Global.AppTable[0].StartParams.LoadAddress = 0; */
    CFE_ES_Global.AppTable[0].StartParams.ExceptionAction =0;
    UT_SetOSFail(0);
    
    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_DELETE;
    CFE_ES_ProcessControlRequest(0);
    UT_SetRtnCode(& BSPUnloadAppFileRtn, -1, 1);

    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_DELETE;
    CFE_ES_ProcessControlRequest(0);
   
   
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_EXTERNAL;
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.FileName), "/ram/FileName");
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.EntryPoint), "NULL");
    CFE_ES_Global.AppTable[0].StartParams.Priority = 255;
    CFE_ES_Global.AppTable[0].StartParams.StackSize = 8192;
/* njy    CFE_ES_Global.AppTable[0].StartParams.LoadAddress = 0; */
    CFE_ES_Global.AppTable[0].StartParams.ExceptionAction =0;
    UT_SetOSFail(0);

    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_RESTART;
    CFE_ES_ProcessControlRequest(0);
    UT_SetRtnCode(& BSPUnloadAppFileRtn, -1, 1);

    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_RESTART;
    CFE_ES_ProcessControlRequest(0);

    UT_SetBSPloadAppFileResult(FALSE);
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_EXTERNAL;
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.FileName), "/ram/FileName");
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.EntryPoint), "NULL");
    CFE_ES_Global.AppTable[0].StartParams.Priority = 255;
    CFE_ES_Global.AppTable[0].StartParams.StackSize = 8192;
 /* njy   CFE_ES_Global.AppTable[0].StartParams.LoadAddress = 0; */
    CFE_ES_Global.AppTable[0].StartParams.ExceptionAction =0;
    UT_SetOSFail(0);

    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_RESTART;
    CFE_ES_ProcessControlRequest(0);

    UT_SetBSPloadAppFileResult(TRUE);


    

    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_EXTERNAL;
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.FileName), "/ram/FileName");
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.EntryPoint), "NULL");
    CFE_ES_Global.AppTable[0].StartParams.Priority = 255;
    CFE_ES_Global.AppTable[0].StartParams.StackSize = 8192;
    /* njy CFE_ES_Global.AppTable[0].StartParams.LoadAddress = 0; */
    CFE_ES_Global.AppTable[0].StartParams.ExceptionAction =0;
    UT_SetOSFail(0);
    
    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_RELOAD;
    CFE_ES_ProcessControlRequest(0);
    UT_SetRtnCode(& BSPUnloadAppFileRtn, -1, 1);

    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_RELOAD;
    CFE_ES_ProcessControlRequest(0);


    UT_SetBSPloadAppFileResult(FALSE);
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_EXTERNAL;
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.FileName), "/ram/FileName");
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.EntryPoint), "NULL");
    CFE_ES_Global.AppTable[0].StartParams.Priority = 255;
    CFE_ES_Global.AppTable[0].StartParams.StackSize = 8192;
 /* njy   CFE_ES_Global.AppTable[0].StartParams.LoadAddress = 0; */
    CFE_ES_Global.AppTable[0].StartParams.ExceptionAction =0;
    UT_SetOSFail(0);

    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_RELOAD;
    CFE_ES_ProcessControlRequest(0);


    UT_SetBSPloadAppFileResult(TRUE);

    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_EXTERNAL;
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.FileName), "/ram/FileName");
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.EntryPoint), "NULL");
    CFE_ES_Global.AppTable[0].StartParams.Priority = 255;
    CFE_ES_Global.AppTable[0].StartParams.StackSize = 8192;
    /* njy CFE_ES_Global.AppTable[0].StartParams.LoadAddress = 0; */
    CFE_ES_Global.AppTable[0].StartParams.ExceptionAction =0;
    UT_SetOSFail(0);

    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_EXCEPTION;
    CFE_ES_ProcessControlRequest(0);
    UT_SetRtnCode(& BSPUnloadAppFileRtn, -1, 1);

    CFE_ES_Global.AppTable[0].StateRecord.AppControlRequest = CFE_ES_SYS_EXCEPTION;
    CFE_ES_ProcessControlRequest(0);
    
    
    
    

    CFE_ES_ListResourcesDebug();

    /* Test CFE_ES_GetAppInfo */
    UT_Report(CFE_ES_GetAppInfo(&AppInfo, 1) == CFE_SUCCESS, "CFE_ES_GetAppInfo", "Sucessful"); 
    UT_Report(CFE_ES_GetAppInfo(NULL, 1) == CFE_ES_ERR_BUFFER, "CFE_ES_GetAppInfo", "Null AppInfo Pointer"); 
    
    CFE_ES_Global.AppTable[5].RecordUsed = FALSE;
    UT_Report(CFE_ES_GetAppInfo(&AppInfo, 5) == CFE_ES_ERR_APPID, "CFE_ES_GetAppInfo", "App record not true"); 

    UT_Report(CFE_ES_GetAppInfo(&AppInfo, CFE_ES_MAX_APPLICATIONS) == CFE_ES_ERR_APPID, "CFE_ES_GetAppInfo", "App ID too high"); 

    
    UT_SetRtnCode(& ModuleInfoRtn, -1, 1);
    UT_Report(CFE_ES_GetAppInfo(&AppInfo, 1) == CFE_SUCCESS, "CFE_ES_GetAppInfo", "Module not found");

    
    
    
    CFE_ES_Global.TaskTable[0].RecordUsed = TRUE;
    CFE_ES_Global.TaskTable[0].AppId = 0;
    UT_SetOSFail(OS_TASKDELETE_FAIL | OS_CLOSE_FAIL);
    UT_SetBSPloadAppFileResult(FALSE);

    UT_Report(CFE_ES_CleanUpApp(0) == CFE_ES_APP_CLEANUP_ERR,"CFE_ES_CleanUpApp", " Error loading app file");
    UT_SetOSFail(0);
    UT_SetBSPloadAppFileResult(TRUE);

    UT_SetRtnCode(&MutSemDelRtn,-1, 1);
    UT_Report(CFE_ES_CleanUpApp(0) == CFE_ES_APP_CLEANUP_ERR,"CFE_ES_CleanUpApp", "Cleanup OS Resources failure");
    UT_SetOSFail(0);

    
    UT_SetRtnCode(&ModuleUnloadRtn,-1, 1);
    UT_Report(CFE_ES_CleanUpApp(0) == CFE_ES_APP_CLEANUP_ERR,"CFE_ES_CleanUpApp", "Cleanup OS Modlue failure");
    
    
    UT_SetRtnCode(&EVSCleanUpRtn,-1, 1);
    UT_Report(CFE_ES_CleanUpApp(0) == CFE_ES_APP_CLEANUP_ERR,"CFE_ES_CleanUpApp", "EVS Cleanup failed");

    UT_SetRtnCode(&SBCleanUpRtn,-1, 1);
    UT_Report(CFE_ES_CleanUpApp(0) == CFE_ES_APP_CLEANUP_ERR,"CFE_ES_CleanUpApp", "EVS Cleanup failed");

    UT_SetRtnCode(&TIMECleanUpRtn,-1, 1);
    UT_Report(CFE_ES_CleanUpApp(0) == CFE_ES_APP_CLEANUP_ERR,"CFE_ES_CleanUpApp", "EVS Cleanup failed");
    
    UT_SetRtnCode(&BinSemDelRtn, -1,1);
    UT_SetRtnCode(&MutSemDelRtn, -1,1);
    UT_SetRtnCode(&CountSemDelRtn, -1,1);
    UT_SetRtnCode(&QueueDelRtn, -1,1);
    UT_SetOSFail(OS_TASKDELETE_FAIL | OS_CLOSE_FAIL);

    UT_Report(CFE_ES_CleanupTaskResources(0) < 0,"CFE_ES_CleanupTaskResources", " OS failures");
    UT_SetOSFail(0);
    UT_Report(CFE_ES_CleanupTaskResources(0) == CFE_SUCCESS,"CFE_ES_CleanupTaskResources", " Successful ");



}
void TestERLog(void)
{
    int Return;
    uint32 Context = 4;
    char Context2 [1000];
    /* Test initial rolling over log entry,
     * Null Description,
     * and non null context with small size */

    CFE_ES_ResetDataPtr->ERLogIndex = CFE_ES_ER_LOG_ENTRIES + 1;
    Return = CFE_ES_WriteToERLog( CFE_ES_CORE_LOG_ENTRY, CFE_ES_POWERON_RESET, 1,NULL, &Context , sizeof(int) );
    UT_Report(Return == CFE_SUCCESS,"CFE_ES_WriteToERLog","Always Successful Run");

    /* Test rolling over log entry at end ,
     * Null Description,
     * and non null context with large size */

    CFE_ES_ResetDataPtr->ERLogIndex = CFE_ES_ER_LOG_ENTRIES - 1;
    Return = CFE_ES_WriteToERLog( CFE_ES_CORE_LOG_ENTRY, CFE_ES_POWERON_RESET, 1,NULL, (uint32*) &Context2 ,  9999999);
    UT_Report(Return == CFE_SUCCESS,"CFE_ES_WriteToERLog","Always Sucessful Run");

    Return = CFE_ES_WriteToERLog( CFE_ES_CORE_LOG_ENTRY, CFE_ES_POWERON_RESET, 1,NULL, &Context , 9999999 );
    UT_Report(Return == CFE_SUCCESS,"CFE_ES_WriteToERLog","Always Successful Run");


}

void TestShell()
{
    UT_SetOSFail(0);
    UT_Report( CFE_ES_ShellOutputCommand("ES_NoSuchThing") == CFE_ES_ERR_SHELL_CMD, 
            "ES_ShellCommand", "No such ES Command");
    UT_Report( CFE_ES_ShellOutputCommand("ls") == CFE_SUCCESS, 
            "ES_ShellCommand", "Send Normal Command");

    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    UT_Report( CFE_ES_ShellOutputCommand("ES_ListApps") == CFE_SUCCESS, 
            "ES_ShellCommand", "Send Normal ES Command 1");  

    UT_Report( CFE_ES_ShellOutputCommand("ES_ListResources") == CFE_SUCCESS, 
            "ES_ShellCommand", "Send Normal ES Command 2"); 

    UT_SetOSFail(0);
    UT_SetRtnCode(&FileWriteRtn , 0, 1);

    CFE_ES_Global.TaskTable[1].RecordUsed = TRUE;
    UT_Report( CFE_ES_ShellOutputCommand("ES_ListTasks") == CFE_SUCCESS, 
            "ES_ShellCommand", "Send Normal ES Command 3"); 
    CFE_ES_Global.TaskTable[1].RecordUsed = FALSE;

    UT_SetRtnCode(&OSlseekRtn , CFE_ES_MAX_SHELL_PKT -2, 1);
    UT_SetOSFail(OS_LSEEK_FAIL);

    UT_Report( CFE_ES_ShellOutputCommand("ls") == CFE_ES_ERR_SHELL_CMD, 
            "ES_ShellCommand", "Failure on OS_lseek");  

    /* test to start sending chunks down */
    UT_SetRtnCode(&OSlseekRtn , CFE_ES_MAX_SHELL_PKT * 2 + 1, 2);
    UT_SetOSFail(0);

    UT_Report( CFE_ES_ShellOutputCommand("ls") == CFE_SUCCESS, 
            "ES_ShellCommand", "Multiple packets to send down");  

    UT_SetOSFail(OS_CREAT_FAIL);
    UT_Report( CFE_ES_ShellOutputCommand("ES_ListApps") == CFE_ES_ERR_SHELL_CMD, 
            "ES_ShellCommand", "OS_creat failed"); 
    UT_SetOSFail(OS_LSEEK_FAIL);
    UT_Report( CFE_ES_ShellOutputCommand("ES_ListApps") == CFE_ES_ERR_SHELL_CMD, 
            "ES_ShellCommand", "Lseek failed"); 
    UT_SetOSFail(0);


}

void TestTask()
{
    CFE_SB_MsgPtr_t msgptr;

    CFE_ES_NoArgsCmd_t NoArgsCmd;
    CFE_ES_RestartCmd_t RestartCmd;
    CFE_ES_ShellCmd_t  ShellCmd;
    CFE_ES_StartAppCmd_t StartAppCmd;
    CFE_ES_AppNameCmd_t AppNameCmd;
    CFE_ES_AppReloadCmd_t   ReloadAppCmd;
    CFE_ES_QueryAllCmd_t QueryAllCmd;
    CFE_ES_OverWriteSysLogCmd_t OverwriteSysLogCmd;
    CFE_ES_WriteSyslogCmd_t WriteSyslogCmd;
    CFE_ES_WriteERlogCmd_t WriteERlogCmd;
    CFE_ES_SetMaxPRCountCmd_t SetMaxPRCountCmd;
    CFE_ES_DeleteCDSCmd_t DeleteCDSCmd;
    CFE_ES_TlmPoolStatsCmd_t    TlmPoolStatsCmd;
    CFE_ES_DumpCDSRegCmd_t      DumpCDSRegCmd;
    CFE_ES_QueryAllTasksCmd_t   QueryAllTasksCmd;
    
    Pool_t *PoolPtr;

    CFE_ES_TaskMain();

    /* Get ES_TaskInit to fail; */

    UT_SetOSFail(OS_TASKREGISTER_FAIL);
    CFE_ES_TaskMain();
    UT_SetOSFail(0);

    /* Get Task Initi to fail on the checksum information*/
    
    UT_SetRtnCode(&BSPGetCFETextRtn, -1, 1);
    CFE_ES_TaskInit();
    
    
    /* Get a good run through */
    CFE_ES_TaskInit();

    /* test ES_AppREgister Failure */
    UT_SetOSFail(OS_TASKREGISTER_FAIL);
    CFE_ES_TaskInit();
    UT_SetOSFail(0);

    /* Test EVS Register Failure */
    UT_SetRtnCode(&EVS_RegisterRtn, -1, 1);
    CFE_ES_TaskInit();

    UT_SetRtnCode(&SB_CreatePipeRtn, -1, 1);
    CFE_ES_TaskInit();

    UT_SetRtnCode(&SB_SubscribeExRtn, -1, 1);
    CFE_ES_TaskInit();

    UT_SetRtnCode(&SB_SubscribeExRtn, -1, 2);
    CFE_ES_TaskInit();

    UT_SetRtnCode(&EVS_SendEventRtn, -1, 1);
    CFE_ES_TaskInit();
    UT_SetRtnCode(&EVS_SendEventRtn, -1, 2);
    CFE_ES_TaskInit();

    UT_SetSBTotalMsgLen(sizeof(CFE_ES_NoArgsCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &NoArgsCmd; 
    
    /* Send a HK request */
    UT_SendMsg(msgptr, CFE_ES_SEND_HK_MID, 0);
    
    UT_SetRtnCode(&HeapGetInfoRtn, -1, 1);
    UT_SendMsg(msgptr, CFE_ES_SEND_HK_MID, 0);

    /* Send messages to tets the command functions */
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_NOOP_CC);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_RESET_CC);

    /* Test Restarting the cFE */
    
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_RestartCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &RestartCmd; 
    /* send Bad Restart Type */
    RestartCmd.RestartType = CFE_ES_PROCESSOR_RESET;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_RESTART_CC);

    /* send Bad Restart Type */
    RestartCmd.RestartType = 4524;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_RESTART_CC);
    
    /* Test Shell Interface */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_ShellCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &ShellCmd; 
    
    strcpy((char *)(ShellCmd.CmdString),"ES_NOAPP");
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_SHELL_CMD_CC);

    strcpy((char *)ShellCmd.CmdString,"ls");
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_SHELL_CMD_CC);

    /* Test App Create */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_StartAppCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &StartAppCmd;

    strcpy ((char *)(StartAppCmd.AppFileName),"filename");
    strcpy ((char *) StartAppCmd.AppEntryPoint,"entrypoint");
    strcpy((char *)StartAppCmd.Application,"appName");
    StartAppCmd.Priority = 160;
    StartAppCmd.StackSize = 8192;
    StartAppCmd.ExceptionAction = CFE_ES_APP_EXCEPTION_RESTART_APP;
    UT_SetOSFail(0);
    UT_SetBSPloadAppFileResult(TRUE);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_START_APP_CC);
    UT_SetBSPloadAppFileResult(FALSE);

    UT_SetOSFail(OS_TASKCREATE_FAIL);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_START_APP_CC);
    UT_SetOSFail(0);

    UT_SetBinSemFail(OS_ERROR);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_START_APP_CC);
    UT_SetBinSemFail(OS_SUCCESS);

    UT_SetOSFail(OS_TASKDELAY_FAIL);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_START_APP_CC);
    UT_SetOSFail(0);

    /* Test too short of file name */
    strcpy ((char *)(StartAppCmd.AppFileName),"123");
    strcpy ((char *) StartAppCmd.AppEntryPoint,"entrypoint");
    strcpy((char *)StartAppCmd.Application,"appName");
    StartAppCmd.Priority = 160;
    StartAppCmd.StackSize = 12096;
    StartAppCmd.ExceptionAction = CFE_ES_APP_EXCEPTION_RESTART_APP;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_START_APP_CC);

    strcpy ((char *)(StartAppCmd.AppFileName),"filename");
    strcpy ((char *) StartAppCmd.AppEntryPoint,"entrypoint");
    strcpy((char *)StartAppCmd.Application,"appName");
    StartAppCmd.Priority = 160;
    StartAppCmd.StackSize = 12096;
    StartAppCmd.ExceptionAction =65534;   /* was 67567 jmd 2010-09-08 */
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_START_APP_CC);



    /* test NULL AppEntryPoint NJY DOESNT WORK fixme*/
    bzero (&StartAppCmd, sizeof (CFE_ES_StartAppCmd_t)); 

    strcpy ((char *)(StartAppCmd.AppFileName),"filename");

    /* strcpy ((char *) StartAppCmd.AppEntryPoint,"entrypoint");*/
    /* memcpy(StartAppCmd.AppEntryPoint,NULL, sizeof(StartAppCmd.AppEntryPoint)); */
    


    strcpy((char *)StartAppCmd.Application,"appName");
    StartAppCmd.Priority = 160;
    StartAppCmd.StackSize = 12096;
    StartAppCmd.ExceptionAction = CFE_ES_APP_EXCEPTION_RESTART_APP; 
    /*UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_START_APP_CC);*/

    /* Test Null Application Name NJY DOESNT WORK fixme*/
    strcpy ((char *)(StartAppCmd.AppFileName),"filename");
    strcpy ((char *) StartAppCmd.AppEntryPoint,"entrypoint");

    bzero((StartAppCmd.Application), OS_MAX_API_NAME);

    StartAppCmd.Priority = 160;
    StartAppCmd.StackSize = 12096;
    StartAppCmd.ExceptionAction = CFE_ES_APP_EXCEPTION_RESTART_APP;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_START_APP_CC);


    /* Test successful run */
    UT_SetBSPloadAppFileResult(TRUE);    
    strcpy ((char *)(StartAppCmd.AppFileName),"/ram/filename");
    strcpy ((char *) StartAppCmd.AppEntryPoint,"entrypoint");
    strcpy((char *)StartAppCmd.Application,"appName");
    StartAppCmd.Priority = 160;
    StartAppCmd.StackSize = 12096;
  /* njy  StartAppCmd.LoadAddress = 4542; */
    StartAppCmd.ExceptionAction = CFE_ES_APP_EXCEPTION_RESTART_APP;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_START_APP_CC);

    UT_SetBSPloadAppFileResult(FALSE);    

    /* test bad stack size */
    strcpy ((char *)(StartAppCmd.AppFileName),"filename");
    strcpy ((char *) StartAppCmd.AppEntryPoint,"entrypoint");
    strcpy((char *)StartAppCmd.Application,"appName");
    StartAppCmd.Priority = 160;
    StartAppCmd.StackSize = 0;
    StartAppCmd.ExceptionAction = CFE_ES_APP_EXCEPTION_RESTART_APP;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_START_APP_CC);

    /* test bad priority */
    strcpy ((char *)(StartAppCmd.AppFileName),"filename");
    strcpy ((char *) StartAppCmd.AppEntryPoint,"entrypoint");
    strcpy((char *)StartAppCmd.Application,"appName");
    StartAppCmd.Priority = 1000;
    StartAppCmd.StackSize = 12096;
    StartAppCmd.ExceptionAction = CFE_ES_APP_EXCEPTION_RESTART_APP;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_START_APP_CC);

    /* Test CFE_ES_StopAppCmd */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_AppNameCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &AppNameCmd;

    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_EXTERNAL; 
    CFE_ES_Global.AppTable[0].StateRecord.AppState = CFE_ES_APP_STATE_RUNNING;
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.Name), "CFE_ES");
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;

    strcpy((char *)AppNameCmd.Application,"CFE_ES");
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_STOP_APP_CC);

    /* test a delete app failure */
    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_CORE; 
    CFE_ES_Global.AppTable[0].StateRecord.AppState = CFE_ES_APP_STATE_WAITING;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_STOP_APP_CC);

    /* tst bad app name */

    strcpy((char *)AppNameCmd.Application,"BAD_APP_NAME");    
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_STOP_APP_CC);

    /* Test CFE_ES_RestartAppCmd */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_AppNameCmd_t));
    strcpy((char *)AppNameCmd.Application,"CFE_ES");

    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_EXTERNAL;
    CFE_ES_Global.AppTable[0].StateRecord.AppState = CFE_ES_APP_STATE_RUNNING;
    strncpy((char *)(CFE_ES_Global.AppTable[0].StartParams.Name), "CFE_ES",OS_MAX_API_NAME);

    msgptr = (CFE_SB_MsgPtr_t) &AppNameCmd;

    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_RESTART_APP_CC);

    strcpy((char *)AppNameCmd.Application,"BAD_APP_NAME");
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_RESTART_APP_CC);

    /* restart fails */   
    strcpy((char *)AppNameCmd.Application,"CFE_ES");
    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_CORE;
    CFE_ES_Global.AppTable[0].StateRecord.AppState = CFE_ES_APP_STATE_WAITING;
    strncpy((char *)(CFE_ES_Global.AppTable[0].StartParams.Name), "CFE_ES",OS_MAX_API_NAME);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_RESTART_APP_CC);

    /* Test CFE_ES_ReloadAppCmd */

    strcpy((char *)AppNameCmd.Application,"CFE_ES");
    strncpy((char *)(CFE_ES_Global.AppTable[0].StartParams.Name), "CFE_ES",OS_MAX_API_NAME);
    msgptr = (CFE_SB_MsgPtr_t) &AppNameCmd;
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_AppReloadCmd_t));  
   
    /* Test good command */
    
    strcpy((char *)ReloadAppCmd.Application,"CFE_ES");
    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_EXTERNAL;
    CFE_ES_Global.AppTable[0].StateRecord.AppState = CFE_ES_APP_STATE_RUNNING;

    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_RELOAD_APP_CC);
    
    /*Test bad app name */
    CFE_ES_Global.AppTable[0].RecordUsed = FALSE;
    strcpy((char *)ReloadAppCmd.Application,"BAD_APP_NAME");
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_RELOAD_APP_CC);
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;

    /* test reload failure */
    CFE_ES_Global.AppTable[0].Type = CFE_ES_APP_TYPE_CORE;
    CFE_ES_Global.AppTable[0].StateRecord.AppState = CFE_ES_APP_STATE_WAITING;
    strcpy((char *)ReloadAppCmd.Application,"CFE_ES");

    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_RELOAD_APP_CC);

    /* Test CFE_ES_QueryOneCmd */

    msgptr = (CFE_SB_MsgPtr_t) &AppNameCmd;
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_AppNameCmd_t));
    strcpy((char *)AppNameCmd.Application,"CFE_ES");
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_QUERY_ONE_CC);

        /*Test failure of SB_SendMsg */
   
   
    /* count must be 2 because UT_SendMsg calls SBSendMmessage */
    UT_SetRtnCode( & SBSendMsgRtn, -1, 2);
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.Name), "CFE_ES");
    strcpy((char *)AppNameCmd.Application,"CFE_ES");
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_QUERY_ONE_CC);
    
    
    /*Test bad app name */
  
    strcpy((char *)AppNameCmd.Application,"BAD_APP_NAME");
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_QUERY_ONE_CC);




    /* Test CFE_ES_QueryAllCmd */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_QueryAllCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &QueryAllCmd;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_QUERY_ALL_CC);

    QueryAllCmd.QueryAllFileName[0] = '\0';
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_QUERY_ALL_CC); 

    /* test failure of writing the header */
    UT_SetRtnCode(&FSWriteHdrRtn,OS_FS_ERROR, 1); 
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_QUERY_ALL_CC); 

    /*Test failure of writing the file */
    UT_SetOSFail(OS_WRITE_FAIL);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_QUERY_ALL_CC);
    UT_SetOSFail(0);

    /*Test failure of creating the file */
    UT_SetOSFail(OS_CREAT_FAIL);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_QUERY_ALL_CC);
    UT_SetOSFail(0);

    CFE_ES_Global.AppTable[0].RecordUsed = FALSE;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_QUERY_ALL_CC);


    /* Query all tasks command */

    msgptr = (CFE_SB_MsgPtr_t) &QueryAllTasksCmd;
    CFE_ES_Global.TaskTable[0].RecordUsed = TRUE;
    CFE_ES_Global.TaskTable[1].RecordUsed = FALSE;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_QUERY_ALL_TASKS_CC);
    CFE_ES_Global.TaskTable[0].RecordUsed = FALSE;
   
    UT_SetRtnCode(&FSWriteHdrRtn, -1, 1);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_QUERY_ALL_TASKS_CC);

    UT_SetOSFail(OS_WRITE_FAIL);
    CFE_ES_Global.TaskTable[0].RecordUsed = TRUE;
    CFE_ES_Global.TaskTable[1].RecordUsed = FALSE;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_QUERY_ALL_TASKS_CC);
    CFE_ES_Global.TaskTable[0].RecordUsed = FALSE;

    UT_SetOSFail(OS_CREAT_FAIL);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_QUERY_ALL_TASKS_CC);
    UT_SetOSFail(0);


    /* Test Clearing the System Log */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_NoArgsCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &NoArgsCmd; 
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_CLEAR_SYSLOG_CC);

    /* Test Overwriting the system log */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_OverWriteSysLogCmd_t ));
    msgptr = (CFE_SB_MsgPtr_t) &OverwriteSysLogCmd;
    OverwriteSysLogCmd.Mode = CFE_ES_LOG_OVERWRITE; 
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_OVERWRITE_SYSLOG_CC);

    /* Test gibberish for mode */
    OverwriteSysLogCmd.Mode = 9342; 
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_OVERWRITE_SYSLOG_CC);

    /* Test Dumping the system log */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_WriteSyslogCmd_t ));
    
    msgptr = (CFE_SB_MsgPtr_t) &WriteSyslogCmd; 
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_WRITE_SYSLOG_CC);

    UT_SetOSFail(0);    
    WriteSyslogCmd.SysLogFileName[0] = 'n';
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_WRITE_SYSLOG_CC);

    UT_SetOSFail(OS_CREAT_FAIL);
    WriteSyslogCmd.SysLogFileName[0] = '\0';
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_WRITE_SYSLOG_CC);
    UT_SetOSFail(0);

    UT_SetOSFail(OS_WRITE_FAIL);
    WriteSyslogCmd.SysLogFileName[0] = '\0';
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_WRITE_SYSLOG_CC);
    UT_SetOSFail(0);

    UT_SetRtnCode(&FSWriteHdrRtn,OS_FS_ERROR, 1);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_WRITE_SYSLOG_CC);

    /* Test Clearing the ER Log */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_NoArgsCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &NoArgsCmd; 
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_CLEAR_ERLOG_CC);

    /* Test Dumping the ER log */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_WriteERlogCmd_t ));
    msgptr = (CFE_SB_MsgPtr_t) &WriteERlogCmd; 
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_WRITE_ERLOG_CC);
    
    UT_SetOSFail(OS_CREAT_FAIL);
    WriteERlogCmd.ERLogFileName[0] = '\0';
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_WRITE_ERLOG_CC);
    UT_SetOSFail(0);

    UT_SetOSFail(OS_WRITE_FAIL);
    WriteERlogCmd.ERLogFileName[0] = 'n';
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_WRITE_ERLOG_CC);
    UT_SetOSFail(0);

    UT_SetRtnCode(&FSWriteHdrRtn,OS_FS_ERROR, 1);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_WRITE_ERLOG_CC);

    UT_SetStatusBSPResetArea(OS_ERROR, 0 );
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_WRITE_ERLOG_CC);
    UT_SetStatusBSPResetArea(OS_SUCCESS, 0 );

    /* Test Getting a bad size in the verify command length call */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_SetMaxPRCountCmd_t ));
    msgptr = (CFE_SB_MsgPtr_t) &NoArgsCmd; 
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_CLEAR_ERLOG_CC);

    /* To get the Odd byte check in Compute CheckSum (function deleted per DCR 4687) */


    /* Test reseting and seting the max for the processor reset count */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_NoArgsCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &NoArgsCmd; 
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_RESET_PR_COUNT_CC);
    
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_SetMaxPRCountCmd_t ));
    msgptr = (CFE_SB_MsgPtr_t) &SetMaxPRCountCmd;
    SetMaxPRCountCmd.MaxPRCount = 3; 
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_SET_MAX_PR_COUNT_CC);

   /* Test CDS commands */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_DeleteCDSCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &DeleteCDSCmd;

    /* find the CDS in the registry */
    strncpy(DeleteCDSCmd.CdsName,"CFE_ES.CDS_NAME",OS_MAX_API_NAME);
    CFE_ES_Global.CDSVars.Registry[0].Taken = TRUE;
    





    strncpy(CFE_ES_Global.CDSVars.Registry[0].Name,"CFE_ES.CDS_NAME",OS_MAX_API_NAME);
    
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_DELETE_CDS_CC);

    CFE_ES_Global.CDSVars.Registry[0].Table = TRUE;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_DELETE_CDS_CC);

    /* successful run through */
    CFE_ES_Global.CDSVars.Registry[0].MemHandle = sizeof(CFE_ES_Global.CDSVars.ValidityField);
    UT_SetRtnCode( & BSPReadCDSRtn, 0,1);
    

    /* Set up the block to read what we need to from the CDS */

    CFE_ES_CDSBlockDesc.CheckBits = CFE_ES_CDS_CHECK_PATTERN;
    CFE_ES_CDSBlockDesc.AllocatedFlag = CFE_ES_CDS_BLOCK_USED;
    CFE_ES_CDSBlockDesc.ActualSize =  512;
    CFE_ES_CDSBlockDesc.SizeUsed =  512;
    
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_DELETE_CDS_CC);

    /* Get GetAppIdByName to pass */
    strncpy(CFE_ES_Global.CDSVars.Registry[0].Name,"CFE_ES.CDS_NAME",OS_MAX_API_NAME);
    CFE_ES_Global.CDSVars.Registry[0].MemHandle = sizeof(CFE_ES_Global.CDSVars.ValidityField);
    UT_SetRtnCode( & BSPReadCDSRtn, 0,1);
 
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.Name), "CFE_ES");
    CFE_ES_Global.CDSVars.Registry[0].Table = TRUE;
    CFE_ES_Global.CDSVars.Registry[0].Taken = TRUE;
    strncpy(CFE_ES_Global.CDSVars.Registry[0].Name,"CFE_ES.CDS_NAME",OS_MAX_API_NAME);

    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_DELETE_CDS_CC);


    /* Get CDS find to fail */
    
    CFE_ES_Global.CDSVars.Registry[0].MemHandle = sizeof(CFE_ES_Global.CDSVars.ValidityField);
    UT_SetRtnCode( & BSPReadCDSRtn, 0,1);
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.Name), "CFE_BAD");
    CFE_ES_Global.AppTable[0].RecordUsed = FALSE;
    CFE_ES_Global.CDSVars.Registry[0].Taken = FALSE;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_DELETE_CDS_CC);
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.Name), "CFE_ES");
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;


    /* Test Dump CDS */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_DumpCDSRegCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &DumpCDSRegCmd;

    /* good run through */
    CFE_ES_Global.CDSVars.Registry[0]. Taken = TRUE;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_DUMP_CDS_REG_CC);

    /* bad FS write header */
    UT_SetRtnCode(&FSWriteHdrRtn, -1, 1);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_DUMP_CDS_REG_CC);

    UT_SetOSFail(OS_CREAT_FAIL);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_DUMP_CDS_REG_CC);
    UT_SetOSFail(0);

    UT_SetOSFail(OS_WRITE_FAIL);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_DUMP_CDS_REG_CC);
    UT_SetOSFail(0);

    /* test telm pool stats */

    UT_SetSBTotalMsgLen(sizeof(CFE_ES_TlmPoolStatsCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &TlmPoolStatsCmd;

    /* invalid handle */

    PoolPtr = (Pool_t *) &TlmPoolStatsCmd.PoolHandle ;
    PoolPtr -> Start = &(TlmPoolStatsCmd.PoolHandle);
    PoolPtr-> Size = 64;
    PoolPtr -> End = *PoolPtr->Start + 0;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_TLM_POOL_STATS_CC);

    PoolPtr = (Pool_t *) &TlmPoolStatsCmd.PoolHandle ;
    PoolPtr -> Start = &(TlmPoolStatsCmd.PoolHandle);
    PoolPtr-> Size = 64;
    PoolPtr -> End = *PoolPtr->Start + PoolPtr->Size;

    /* valid handle */
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_TLM_POOL_STATS_CC);

    /* Test Default Case */

    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_DUMP_CDS_REG_CC + 2);


/* Test CDS Delete again */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_DeleteCDSCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &DeleteCDSCmd;

    CFE_ES_Global.CDSVars.Registry[0].Taken = TRUE;
    CFE_ES_Global.CDSVars.Registry[0].Table = FALSE;
    CFE_ES_Global.CDSVars.Registry[0].MemHandle = 200;

    strncpy(CFE_ES_Global.CDSVars.Registry[0].Name,"CFE_ES.CDS_NAME",OS_MAX_API_NAME);
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.Name), "CFE_ES");
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    strncpy(DeleteCDSCmd.CdsName,"CFE_ES.CDS_NAME",OS_MAX_API_NAME);

    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_DELETE_CDS_CC);

        CFE_ES_Global.AppTable[0].RecordUsed = FALSE;
    strncpy(DeleteCDSCmd.CdsName,"CFE_ES.CDS_NAME",OS_MAX_API_NAME);

    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_DELETE_CDS_CC);


} /* end TestTask */

void TestPerf(void)
{    
    CFE_SB_MsgPtr_t msgptr;    
    CFE_ES_PerfStartCmd_t PerfStartCmd;
    CFE_ES_PerfStopCmd_t PerfStopCmd;
    CFE_ES_PerfSetFilterMaskCmd_t PerfSetFilterMaskCmd;
    CFE_ES_PerfSetTrigMaskCmd_t PerfSetTrigMaskCmd;
    
    extern CFE_ES_PerfLogDump_t    CFE_ES_PerfLogDumpStatus;

    CFE_ES_Global.TaskTable[1].RecordUsed = TRUE;
    CFE_ES_Global.TaskTable[1].AppId = 0;
    CFE_ES_Global.AppTable[0].TaskInfo.MainTaskId = 1;

    CFE_ES_SetupPerfVariables(CFE_ES_PROCESSOR_RESET);

  /* Test PerfStartDataCmd */
    PerfStartCmd.TriggerMode = CFE_ES_PERF_TRIGGER_START;
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_PerfStartCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &PerfStartCmd;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_STARTDATA_CC);

    PerfStartCmd.TriggerMode = CFE_ES_PERF_TRIGGER_CENTER;
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_PerfStartCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &PerfStartCmd;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_STARTDATA_CC);

    PerfStartCmd.TriggerMode = CFE_ES_PERF_TRIGGER_END;
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_PerfStartCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &PerfStartCmd;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_STARTDATA_CC);

    /* Check for a bad trigger mode */
    PerfStartCmd.TriggerMode = (CFE_ES_PERF_TRIGGER_END + 1);
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_PerfStartCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &PerfStartCmd;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_STARTDATA_CC);

    PerfStartCmd.TriggerMode = (CFE_ES_PERF_TRIGGER_START - 1);
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_PerfStartCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &PerfStartCmd;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_STARTDATA_CC);

    /* test to see if there is a file write in progress */
    /* NJY */
    CFE_ES_PerfLogDumpStatus.DataToWrite = 1;
    PerfStartCmd.TriggerMode = CFE_ES_PERF_TRIGGER_START;
    msgptr = (CFE_SB_MsgPtr_t) &PerfStartCmd;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_STARTDATA_CC);
    CFE_ES_PerfLogDumpStatus.DataToWrite = 0;

    /* Send another valid start command to get the rest of the code working */
    PerfStartCmd.TriggerMode = CFE_ES_PERF_TRIGGER_START;
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_PerfStartCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &PerfStartCmd;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_STARTDATA_CC);



    /* Test PerfStopDataCmd */

    PerfStopCmd.DataFileName[0] = '\0';
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_PerfStopCmd_t));
    UT_SetBSPloadAppFileResult (TRUE); 

    CFE_ES_PerfLogDumpStatus.DataToWrite = 0;
    msgptr = (CFE_SB_MsgPtr_t) &PerfStopCmd;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_STOPDATA_CC);
    UT_SetBSPloadAppFileResult (FALSE); 

    /* Test with OS_creat failing */
    UT_SetOSFail(OS_CREAT_FAIL);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_STOPDATA_CC);
    UT_SetOSFail(0);
    
    /* Test with ES_CreateChildTask failing */
    UT_SetOSFail(OS_TASKCREATE_FAIL);
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_STOPDATA_CC);
    UT_SetOSFail(0);

    /* Name is not default */
    PerfStopCmd.DataFileName[0] = 'n';
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_STOPDATA_CC);

    /* test to see if there is a file write in progress */
   /* NJY */
    CFE_ES_PerfLogDumpStatus.DataToWrite = 1;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_STOPDATA_CC);

      /* Test PerfSetFilterMaskCmd */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_PerfSetFilterMaskCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &PerfSetFilterMaskCmd;

    
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_SETFILTERMASK_CC);
    
    
    PerfSetFilterMaskCmd.FilterMaskNum = CFE_ES_PERF_32BIT_WORDS_IN_MASK -1;

    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_SETFILTERMASK_CC);
    
    /* Test PerfSetTriggerMaskCmd */
    UT_SetSBTotalMsgLen(sizeof(CFE_ES_PerfSetTrigMaskCmd_t));
    msgptr = (CFE_SB_MsgPtr_t) &PerfSetTrigMaskCmd;

    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_SETTRIGMASK_CC);
    
    
    PerfSetTrigMaskCmd.TriggerMaskNum = CFE_ES_PERF_32BIT_WORDS_IN_MASK -1;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_SETTRIGMASK_CC);
   
    PerfSetTrigMaskCmd.TriggerMaskNum = CFE_ES_PERF_32BIT_WORDS_IN_MASK + 1;
    UT_SendMsg(msgptr, CFE_ES_CMD_MID, CFE_ES_PERF_SETTRIGMASK_CC);
   

    /* test CFE_ES_PerfLogDump(void)*/

    CFE_ES_PerfLogDump();

    Perf->MetaData.DataCount = 7;
    UT_SetRtnCode( & FSWriteHdrRtn, -1, 1);
    UT_SetOSFail(OS_CREAT_FAIL| OS_WRITE_FAIL);
    CFE_ES_PerfLogDump();
    UT_SetOSFail(0);


    /* test CFE_ES_PerfLogAdd */
Perf->MetaData.State = CFE_ES_PERF_WAITING_FOR_TRIGGER; 
Perf->MetaData.TriggerCount = CFE_ES_PERF_DATA_BUFFER_SIZE +1;
    
    Perf->MetaData.InvalidMarkerReported = FALSE;
    Perf->MetaData.DataEnd = CFE_ES_PERF_DATA_BUFFER_SIZE +1 ;
    CFE_ES_PerfLogAdd(CFE_ES_PERF_MAX_IDS,0);

    Perf->MetaData.InvalidMarkerReported = TRUE;
    Perf->MetaData.DataCount = CFE_ES_PERF_DATA_BUFFER_SIZE + 1;
    Perf->MetaData.TriggerMask[0] = 0xFFFF;
    CFE_ES_PerfLogAdd(1,0);

    Perf->MetaData.State = CFE_ES_PERF_TRIGGERED;
    Perf->MetaData.Mode = CFE_ES_PERF_TRIGGER_CENTER;
    Perf->MetaData.TriggerCount = CFE_ES_PERF_DATA_BUFFER_SIZE / 2 +1 ;

    CFE_ES_PerfLogAdd(1,0);

    Perf->MetaData.State = CFE_ES_PERF_TRIGGERED;
    Perf->MetaData.Mode = CFE_ES_PERF_TRIGGER_END;

    CFE_ES_PerfLogAdd(1,0);
}

void TestAPI(void)
{
    uint32 ResetType;
    uint32 AppId;
    uint32 TaskId;
    char AppName [32];
    int32 Return;
    uint8 Data[12];
    CFE_ES_CDSHandle_t  CDSHandle;
    uint32 TempSize;
    int i;
    uint32 RunStatus;
    CFE_ES_TaskInfo_t TaskInfo;
    uint32 CounterId;
    uint32 CounterCount;

    /* Test Getting the reset type */
    CFE_ES_GetResetType(& ResetType);

    /* Test Reseting the cFE */
    ResetType = CFE_ES_PROCESSOR_RESET;
 
    CFE_ES_ResetDataPtr->ResetVars.ProcessorResetCount = 
        CFE_ES_ResetDataPtr->ResetVars.MaxProcessorResetCount- 1;
    CFE_ES_ResetCFE(ResetType);
    CFE_ES_ResetDataPtr->ResetVars.ProcessorResetCount = 
        CFE_ES_ResetDataPtr->ResetVars.MaxProcessorResetCount;
    CFE_ES_ResetCFE(ResetType);
    
    ResetType = CFE_ES_POWERON_RESET;
    CFE_ES_ResetCFE(ResetType);

    /* Test Bad Argument */
    ResetType = CFE_ES_POWERON_RESET + 3;
    UT_Report (CFE_ES_ResetCFE(ResetType) == CFE_ES_BAD_ARGUMENT,"CFE_ES_ResetCFE", "Bad Reset Type ");

    /* test RestartApp */
    CFE_ES_Global.AppTable[1].Type = CFE_ES_APP_TYPE_EXTERNAL;
    CFE_ES_Global.AppTable[1].StateRecord.AppState = CFE_ES_APP_STATE_STOPPED;
    UT_Report (CFE_ES_RestartApp(1) == CFE_ES_ERR_APPID ,"CFE_ES_RestartApp", "Bad App Id");

    UT_Report (CFE_ES_RestartApp(40000) == CFE_ES_ERR_APPID ,"CFE_ES_RestartApp", "App Id too large ");

    /* test ReloadApp */
    CFE_ES_Global.AppTable[1].Type = CFE_ES_APP_TYPE_EXTERNAL;
    CFE_ES_Global.AppTable[1].StateRecord.AppState = CFE_ES_APP_STATE_STOPPED;
    UT_Report (CFE_ES_ReloadApp(1, "filename") == CFE_ES_ERR_APPID ,"CFE_ES_ReloadApp", "Bad App Id");

    /* test DeleteApp */
    CFE_ES_Global.AppTable[1].Type = CFE_ES_APP_TYPE_EXTERNAL;
    CFE_ES_Global.AppTable[1].StateRecord.AppState = CFE_ES_APP_STATE_STOPPED;
    UT_Report (CFE_ES_DeleteApp(1) == CFE_ES_ERR_APPID ,"CFE_ES_DeleteApp", "Bad App Id");


    /* test ExitApp */

    
    CFE_ES_Global.TaskTable[1].RecordUsed =TRUE;
    CFE_ES_Global.AppTable[1].StateRecord.AppState = CFE_ES_APP_STATE_STOPPED;
    CFE_ES_Global.AppTable[1].Type = CFE_ES_APP_TYPE_CORE;
    CFE_ES_ExitApp(CFE_ES_CORE_APP_INIT_ERROR);


    CFE_ES_Global.TaskTable[1].RecordUsed =TRUE;
    CFE_ES_Global.AppTable[1].StateRecord.AppState = CFE_ES_APP_STATE_STOPPED;
    CFE_ES_Global.AppTable[1].Type = CFE_ES_APP_TYPE_CORE;
    CFE_ES_ExitApp(CFE_ES_CORE_APP_RUNTIME_ERROR);


    CFE_ES_Global.TaskTable[1].RecordUsed =TRUE;
    CFE_ES_Global.AppTable[1].Type = CFE_ES_APP_TYPE_EXTERNAL;
    CFE_ES_Global.AppTable[1].StateRecord.AppState = CFE_ES_APP_STATE_STOPPED;
    CFE_ES_Global.AppTable[1].Type = CFE_ES_APP_TYPE_CORE;
    CFE_ES_ExitApp(1000);

    /* I can't cover this path, because there is a while(1) loop */
#if 0
    CFE_ES_Global.TaskTable[1].RecordUsed =TRUE;
    CFE_ES_Global.TaskTable[1].AppId = 1;
    CFE_ES_Global.AppTable[1].Type = CFE_ES_APP_TYPE_EXTERNAL;
    CFE_ES_Global.AppTable[1].StateRecord.AppState = CFE_ES_APP_STATE_RUNNING;
    CFE_ES_ExitApp(CFE_ES_CORE_APP_RUNTIME_ERROR);
#endif

    /* test CFE_ES_RunLoop */
    CFE_ES_Global.TaskTable[1].RecordUsed =TRUE;
    CFE_ES_Global.TaskTable[1].AppId = 1;
    RunStatus = CFE_ES_APP_RUN;
    CFE_ES_Global.AppTable[1].StateRecord.AppControlRequest = CFE_ES_APP_RUN;
    UT_Report(CFE_ES_RunLoop(& RunStatus) == TRUE,"CFE_ES_RunLoop"," Successful");


    CFE_ES_Global.TaskTable[1].RecordUsed =TRUE;
    CFE_ES_Global.TaskTable[1].AppId = 1;
    RunStatus = CFE_ES_APP_RUN;
    CFE_ES_Global.AppTable[1].StateRecord.AppControlRequest = CFE_ES_APP_EXIT;
    UT_Report(CFE_ES_RunLoop(& RunStatus) == FALSE,"CFE_ES_RunLoop"," Successful");


    CFE_ES_Global.TaskTable[1].RecordUsed =TRUE;
    CFE_ES_Global.TaskTable[1].AppId = 1;
    RunStatus = CFE_ES_APP_EXIT;
    CFE_ES_Global.AppTable[1].StateRecord.AppControlRequest = CFE_ES_APP_EXIT;
    UT_Report(CFE_ES_RunLoop(& RunStatus) == FALSE,"CFE_ES_RunLoop","App needs to exit");

    CFE_ES_Global.TaskTable[1].RecordUsed =FALSE;
    CFE_ES_Global.TaskTable[1].AppId = 1;
    RunStatus = CFE_ES_APP_EXIT;
    CFE_ES_Global.AppTable[1].StateRecord.AppControlRequest = CFE_ES_APP_EXIT;
    UT_Report(CFE_ES_RunLoop(& RunStatus) == FALSE,"CFE_ES_RunLoop","GetAppIDInternal failed");
    
    
    CFE_ES_Global.TaskTable[1].RecordUsed =TRUE;
    CFE_ES_Global.TaskTable[1].AppId = 1;
    RunStatus = 1000;
    CFE_ES_Global.AppTable[1].StateRecord.AppControlRequest = CFE_ES_APP_EXIT;
    UT_Report(CFE_ES_RunLoop(& RunStatus) == FALSE,"CFE_ES_RunLoop"," Bad RunStatus");

    /* test startup sync code */
    CFE_ES_Global.TaskTable[1].RecordUsed =TRUE;
    CFE_ES_Global.TaskTable[1].AppId = 1;
    RunStatus = CFE_ES_APP_RUN;
    CFE_ES_Global.AppTable[1].StateRecord.AppControlRequest = CFE_ES_APP_RUN;
    CFE_ES_Global.AppTable[1].StateRecord.AppState = CFE_ES_APP_STATE_INITIALIZING;
    CFE_ES_Global.StartupSemaphoreReleased = FALSE;
    CFE_ES_Global.StartupFileComplete = TRUE; 
    CFE_ES_Global.AppStartupCounter = -1;
    UT_Report(CFE_ES_RunLoop(& RunStatus) == TRUE,"CFE_ES_RunLoop"," Successful");
    
    

    /* Test Registering a CFE application */
    UT_Report(CFE_ES_RegisterApp() == CFE_SUCCESS,"CFE_ES_RegisterApp"," Sucessful");

    /* Test Getting the cFE Application ID by its name */



    CFE_ES_Global.TaskTable[1].RecordUsed = TRUE;
    UT_Report(CFE_ES_GetAppID(&AppId) == CFE_SUCCESS ,"CFE_ES_GetAppID","Successful Run");

    /* Test Getting the App Name */
    CFE_ES_Global.AppTable[4].RecordUsed = FALSE;
    UT_Report(CFE_ES_GetAppName(AppName, 4, 32) == CFE_ES_ERR_APPID ,"CFE_ES_GetAppName" ,"Bad App Id" );
    UT_Report(CFE_ES_GetAppName(AppName, CFE_ES_MAX_APPLICATIONS + 2, 32) == CFE_ES_ERR_APPID,"CFE_ES_GetAppName" ,"AppID too big" );
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    UT_Report(CFE_ES_GetAppName(AppName, 0, 32) == CFE_SUCCESS ,"CFE_ES_GetAppName" ,"Successful" );



    /* Test GetTaskInfo */

    CFE_ES_Global.TaskTable[1].AppId = 1;
    CFE_ES_Global.AppTable[1].RecordUsed = TRUE;

    UT_Report(CFE_ES_GetTaskInfo( &TaskInfo, 1) == CFE_SUCCESS ,"CFE_ES_GetTaskInfo" ,"Successful" );

    CFE_ES_Global.TaskTable[1].AppId = 1;
    CFE_ES_Global.AppTable[1].RecordUsed = FALSE;
    UT_Report(CFE_ES_GetTaskInfo( &TaskInfo, 1) == CFE_ES_ERR_TASKID ,"CFE_ES_GetTaskInfo" ,"Parent app not active" );

    CFE_ES_Global.TaskTable[1].AppId = 1;
    CFE_ES_Global.AppTable[1].RecordUsed = FALSE;
    CFE_ES_Global.TaskTable[1].RecordUsed = FALSE;
    UT_Report(CFE_ES_GetTaskInfo( &TaskInfo, 1) == CFE_ES_ERR_TASKID ,"CFE_ES_GetTaskInfo" ,"task not active" );

    UT_Report(CFE_ES_GetTaskInfo( &TaskInfo, 1000) == CFE_ES_ERR_TASKID ,"CFE_ES_GetTaskInfo" ,"Invalid Task Id" );


    /* Test Creating a Child Task */
    CFE_ES_Global.TaskTable[1].RecordUsed = FALSE;
 /*   CFE_ES_Global.TaskTable[0].RecordUsed = FALSE;*/

    Return = CFE_ES_CreateChildTask(&TaskId,"TaskName" , TestAPI, (const uint32*)AppName, 32, 400,0);
    UT_Report(Return == CFE_ES_ERR_APPID,"CFE_ES_ChildTaskCreate", "Bad App Id");


    UT_SetOSFail(OS_TASKCREATE_FAIL);
    UT_Task[1].free = TRUE;
    CFE_ES_Global.TaskTable[1].RecordUsed = TRUE;
    Return = CFE_ES_CreateChildTask(&TaskId,"TaskName" ,TestAPI, (const uint32*)AppName, 32, 400,0);
    UT_SetOSFail(0);
    UT_Report(Return == CFE_ES_ERR_CHILD_TASK_CREATE,"CFE_ES_ChildTaskCreate", "Task Create Failed");
    

    UT_SetOSFail(OS_TASKCREATE_FAIL);
    Return = CFE_ES_CreateChildTask(&TaskId,"TaskName" ,TestAPI, (const uint32*)AppName, 32, 400,0);
    UT_SetOSFail(0);
    UT_Report(Return == CFE_ES_ERR_CHILD_TASK_CREATE,"CFE_ES_ChildTaskCreate", "Task Create Failed");


    Return = CFE_ES_CreateChildTask(NULL,"TaskName" ,TestAPI, (const uint32*)AppName, 32, 400,0);
    UT_SetOSFail(0);
    UT_Report(Return == CFE_ES_BAD_ARGUMENT,"CFE_ES_ChildTaskCreate", "TaskIdPtr == NULL");

    Return = CFE_ES_CreateChildTask(&TaskId,NULL ,TestAPI, (const uint32*)AppName, 32, 400,0);
    UT_SetOSFail(0);
    UT_Report(Return == CFE_ES_BAD_ARGUMENT,"CFE_ES_ChildTaskCreate", "Task Name == NULL");

    Return = CFE_ES_CreateChildTask(NULL,NULL ,TestAPI, (const uint32*)AppName, 32, 400,0);
    UT_SetOSFail(0);
    UT_Report(Return == CFE_ES_BAD_ARGUMENT,"CFE_ES_ChildTaskCreate", "Task Name and ID == NULL");

    Return = CFE_ES_CreateChildTask(&TaskId,"TaskName" ,NULL, (const uint32*)AppName, 32, 2,0);
    UT_SetOSFail(0);
    UT_Report(Return == CFE_ES_BAD_ARGUMENT,"CFE_ES_ChildTaskCreate", "Function Ptr == NULL");

    CFE_ES_Global.TaskTable[1].RecordUsed = TRUE;

    CFE_ES_Global.AppTable[1].RecordUsed = TRUE;
    CFE_ES_Global.TaskTable[1].AppId = 1;
    CFE_ES_Global.AppTable[1].TaskInfo.MainTaskId = 3;
    CFE_ES_CreateChildTask(&TaskId,"TaskName" ,TestAPI, (const uint32*)AppName, 32, 400,0);
    UT_SetOSFail(0);

    CFE_ES_Global.AppTable[1].RecordUsed = TRUE;
    CFE_ES_Global.TaskTable[1].AppId = 1;
    CFE_ES_Global.AppTable[1].TaskInfo.MainTaskId = 1;
    CFE_ES_CreateChildTask(&TaskId,"TaskName" ,TestAPI, (const uint32*)AppName, 32, 400,0);
    UT_SetOSFail(0);


    /* Test Delete Child Task */

    CFE_ES_Global.TaskTable[0].RecordUsed = FALSE;
    CFE_ES_Global.AppTable[0].RecordUsed = FALSE;

    CFE_ES_Global.TaskTable[1].RecordUsed = FALSE;
    UT_Report(CFE_ES_DeleteChildTask(1) == CFE_ES_ERR_TASKID ,"CFE_ES_DeleteChilTask" ,"TaskId not in use" );
 


    CFE_ES_Global.TaskTable[1].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[1].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[1].TaskInfo.MainTaskId = 1;
    UT_Report(CFE_ES_DeleteChildTask(1) == CFE_ES_ERR_CHILD_TASK_DELETE_MAIN_TASK ,"CFE_ES_DeleteChilTask" ,"OS_TaskDelete Failed" );


    CFE_ES_Global.TaskTable[0].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[0].TaskInfo.MainTaskId = 15;

    UT_Report(CFE_ES_DeleteChildTask(0) == CFE_SUCCESS ,"CFE_ES_DeleteChilTask" ,"passed" );
    
    
    
    CFE_ES_Global.TaskTable[0].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[0].TaskInfo.MainTaskId = 15;
    UT_SetOSFail(OS_TASKDELETE_FAIL);
    UT_Report(CFE_ES_DeleteChildTask(0) <= 0 ,"CFE_ES_DeleteChilTask" ," task delete failed" );
    UT_SetOSFail(0);

    UT_Report(CFE_ES_DeleteChildTask(1000) == CFE_ES_ERR_TASKID ,"CFE_ES_DeleteChilTask" ,"TaskId too large" );


    /* Test Exit ChildTask */
    CFE_ES_Global.TaskTable[0].RecordUsed = FALSE;
    CFE_ES_Global.AppTable[0].RecordUsed = FALSE;

    CFE_ES_Global.TaskTable[1].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[1].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[1].TaskInfo.MainTaskId = 3;
    CFE_ES_ExitChildTask();

    CFE_ES_Global.TaskTable[1].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[1].RecordUsed = TRUE;
    CFE_ES_Global.AppTable[1].TaskInfo.MainTaskId = 1;
    CFE_ES_ExitChildTask();


    CFE_ES_Global.TaskTable[1].RecordUsed = FALSE;
    CFE_ES_ExitChildTask();

    /* Test Registering the Child Task */

    UT_SetOSFail(OS_TASKREGISTER_FAIL);
    UT_Report(CFE_ES_RegisterChildTask() == CFE_ES_ERR_CHILD_TASK_REGISTER,"CFE_ES_RegisterChildTask","OS_TaskRegister Failed");
/*
    UT_SetOSFail(OS_TASKDELAY_FAIL);
    UT_Report(CFE_ES_RegisterChildTask() == CFE_ES_ERR_CHILD_TASK_REGISTER,"CFE_ES_RegisterChildTask","OS_TaskDelay Failed");
*/    UT_SetOSFail(0);

    UT_Report(CFE_ES_RegisterChildTask() == CFE_SUCCESS,"CFE_ES_RegisterChildTask","Successful");


    /* Test Deleting a child task */

    CFE_ES_ResetDataPtr->SystemLogIndex = CFE_ES_SYSTEM_LOG_SIZE -10;
    CFE_ES_ResetDataPtr->SystemLogMode = CFE_ES_LOG_DISCARD;
    UT_Report (CFE_ES_WriteToSysLog("SysLogText") == CFE_SUCCESS,"CFE_ES_WriteToSysLog","Successful, but truncated");
    
    /* Test Drivers*/
    CFE_ES_RegisterDriver(NULL, NULL);
    CFE_ES_UnloadDriver(0);

    /* Test Calculating CRC */
    /*CFE_ES_CalculateCRC(void *DataPtr, uint32 DataLength, uint32 InputCRC, uint32 TypeCRC)*/
    CFE_ES_CalculateCRC(&Data,12, 345353, CFE_ES_CRC_8);
    CFE_ES_CalculateCRC(&Data,12, 345353, CFE_ES_CRC_16);
    CFE_ES_CalculateCRC(&Data,12, 345353, CFE_ES_CRC_32);

    /* Test CDS Registering */

CFE_ES_Global.TaskTable[1].RecordUsed = TRUE;
CFE_ES_Global.TaskTable[1].AppId =1;

   
    UT_SetRtnCode (&BSPWriteCDSRtn,OS_SUCCESS, 1);
    UT_SetBSPFail(BSP_WRITECDS_FAIL);
    UT_Report( CFE_ES_RegisterCDS(&CDSHandle, 4, "Name3") == OS_ERROR ,"CFE_ES_RegisterCDS", "Writing to BSP CDS Failed");
    UT_SetBSPFail(0);


    
    /* regular run through */
    UT_Report( CFE_ES_RegisterCDS(&CDSHandle, 4, "Name") == CFE_SUCCESS ,"CFE_ES_RegisterCDS", "Successful");
    
    /* Getting back a previously allocated CDS */
    UT_Report( CFE_ES_RegisterCDS(&CDSHandle, 4, "Name") == CFE_ES_CDS_ALREADY_EXISTS ,"CFE_ES_RegisterCDS", "Get back old CDS");

    /* New CDS with same name, but a different size */
    UT_Report( CFE_ES_RegisterCDS(&CDSHandle, 6, "Name") == CFE_SUCCESS,"CFE_ES_RegisterCDS", "Get CDS of same name, but new size");

    UT_Report( CFE_ES_RegisterCDS(&CDSHandle, 4, "") == CFE_ES_CDS_INVALID_NAME ,"CFE_ES_RegisterCDS", "Invalid Name Size");


    /* No memory pool available */
    TempSize = CFE_ES_Global.CDSVars.MemPoolSize;
    CFE_ES_Global.CDSVars.MemPoolSize = 0;


    UT_Report( CFE_ES_RegisterCDS(&CDSHandle, 4, "Name") ==CFE_ES_NOT_IMPLEMENTED ,"CFE_ES_RegisterCDS", " Mempool Size = 0");
    CFE_ES_Global.CDSVars.MemPoolSize = TempSize;

    /* Block Size of Zero */
    UT_Report( CFE_ES_RegisterCDS(&CDSHandle, 0, "Name") == CFE_ES_CDS_INVALID_SIZE ,"CFE_ES_RegisterCDS", "Block Size Zero");

    /* Set all the CDS Registries to Taken */
    for (i = 0; i < CFE_ES_Global.CDSVars.MaxNumRegEntries; i++)
    {
        CFE_ES_Global.CDSVars.Registry[i].Taken = TRUE;
    }

    UT_Report( CFE_ES_RegisterCDS(&CDSHandle, 4, "Name2") == CFE_ES_CDS_REGISTRY_FULL,"CFE_ES_RegisterCDS", "No Free Entries");

    /* Set all the CDS Registries back to not taken */
    for (i = 0; i < CFE_ES_Global.CDSVars.MaxNumRegEntries; i++)
    {
        CFE_ES_Global.CDSVars.Registry[i].Taken = FALSE;
    }

    
    CFE_ES_Global.TaskTable[1].RecordUsed = FALSE;
    
    UT_Report( CFE_ES_RegisterCDS(&CDSHandle, 4, "Name2") == CFE_ES_ERR_APPID,"CFE_ES_RegisterCDS", "Bad App Id");
    CFE_ES_Global.TaskTable[1].RecordUsed = TRUE;


    /* Regular Run again, to set up for the copy */
    UT_SetBSPFail(0);
    UT_Report( CFE_ES_RegisterCDS(&CDSHandle, 4, "Name") ==CFE_SUCCESS ,"CFE_ES_RegisterCDS", "Successful");
    UT_SetRtnCode(&BSPReadCDSRtn,OS_SUCCESS, 1);
    UT_Report( CFE_ES_CopyToCDS(CDSHandle, &TempSize) == CFE_SUCCESS, "CFE_ES_CopyToCDS","Successful");
    
    UT_Report( CFE_ES_RestoreFromCDS(&TempSize, CDSHandle) == CFE_SUCCESS, "CFE_ES_RestoreFromCDS", "Successful");

    
    UT_SetRtnCode( &MutSemTakeRtn, -1,1);
    CFE_ES_LockSharedData(__func__, __LINE__);

    UT_SetRtnCode( &MutSemGiveRtn, -1,1);
    CFE_ES_UnlockSharedData(__func__, __LINE__);

    /* Test Generic Counter Interface */

    UT_Report( CFE_ES_RegisterGenCounter(&CounterId, "Counter1") == CFE_SUCCESS ,"CFE_ES_RegisterGenCounter", "Successful");
    UT_Report( CFE_ES_RegisterGenCounter(&CounterId, "Counter1") == CFE_ES_BAD_ARGUMENT ,"CFE_ES_RegisterGenCounter", "Successful");
    CFE_ES_RegisterGenCounter(&CounterId, "Counter2");
    CFE_ES_RegisterGenCounter(&CounterId, "Counter3");
    CFE_ES_RegisterGenCounter(&CounterId, "Counter4");
    CFE_ES_RegisterGenCounter(&CounterId, "Counter5");
    CFE_ES_RegisterGenCounter(&CounterId, "Counter6");
    CFE_ES_RegisterGenCounter(&CounterId, "Counter7");
    CFE_ES_RegisterGenCounter(&CounterId, "Counter8");
    UT_Report( CFE_ES_RegisterGenCounter(&CounterId, "Counter9") == CFE_ES_BAD_ARGUMENT ,"CFE_ES_RegisterGenCounter", "Successful");
    UT_Report( CFE_ES_GetGenCounterIDByName(&CounterId, "Counter9") == CFE_ES_BAD_ARGUMENT ,"CFE_ES_GetGenCounterIDByName", "Successful");
    UT_Report( CFE_ES_GetGenCounterIDByName(&CounterId, "Counter5") == CFE_SUCCESS ,"CFE_ES_GetGenCounterIDByName", "Successful");
    UT_Report( CFE_ES_DeleteGenCounter(123456) == CFE_ES_BAD_ARGUMENT ,"CFE_ES_DeleteGenCounter", "Successful");
    UT_Report( CFE_ES_DeleteGenCounter(CounterId) == CFE_SUCCESS ,"CFE_ES_DeleteGenCounter", "Successful");
    UT_Report( CFE_ES_RegisterGenCounter(&CounterId, "CounterX") == CFE_SUCCESS ,"CFE_ES_RegisterGenCounter", "Successful");
    UT_Report( CFE_ES_IncrementGenCounter(123456) == CFE_ES_BAD_ARGUMENT ,"CFE_ES_IncrementGenCounter", "Successful");
    UT_Report( CFE_ES_IncrementGenCounter(CounterId) == CFE_SUCCESS ,"CFE_ES_IncrementGenCounter", "Successful");
    UT_Report( CFE_ES_GetGenCount(123456, &CounterCount) == CFE_ES_BAD_ARGUMENT ,"CFE_ES_GetGenCount", "Successful");
    UT_Report( CFE_ES_GetGenCount(CounterId, &CounterCount) == CFE_SUCCESS ,"CFE_ES_GetGenCount", "Successful");
    UT_Report( (CounterCount == 1),"Counter Compare", "Successful");
    UT_Report( CFE_ES_SetGenCount(123456, 5) == CFE_ES_BAD_ARGUMENT ,"CFE_ES_SetGenCount", "Successful");
    UT_Report( CFE_ES_SetGenCount(CounterId, 5) == CFE_SUCCESS ,"CFE_ES_SetGenCount", "Successful");
    CFE_ES_GetGenCount(CounterId, &CounterCount);
    UT_Report( (CounterCount == 5),"Counter Compare", "Successful");

    /* Test ProcessCoreException */
    
    CFE_ES_Global.TaskTable[63].RecordUsed = TRUE;
    CFE_ES_Global.TaskTable[63].AppId = 3;
    CFE_ES_Global.AppTable[3].RecordUsed = TRUE;
        
    CFE_ES_Global.AppTable[3].StartParams.ExceptionAction = CFE_ES_APP_EXCEPTION_RESTART_APP;
    
    CFE_ES_ProcessCoreException(63,(uint8 *)("Reason String"), (uint32 *)&i, sizeof(i)); 


    CFE_ES_ResetDataPtr->ResetVars.ProcessorResetCount = 0;
    CFE_ES_ResetDataPtr->ResetVars.MaxProcessorResetCount = 5;
    CFE_ES_Global.AppTable[3].StartParams.ExceptionAction = CFE_ES_APP_EXCEPTION_RESTART_APP -1;
    
    CFE_ES_ProcessCoreException(63,(uint8 *)("Reason String"), (uint32 *)&i, sizeof(i)); 
    

    CFE_ES_ResetDataPtr->ResetVars.ProcessorResetCount = 100;
    CFE_ES_ResetDataPtr->ResetVars.MaxProcessorResetCount = 5;
    CFE_ES_Global.AppTable[3].StartParams.ExceptionAction = CFE_ES_APP_EXCEPTION_RESTART_APP -1;
    
    CFE_ES_ProcessCoreException(63,(uint8 *)("Reason String"), (uint32 *)&i, sizeof(i)); 
    
    
    /* Test Wait for Startup Sync */
    CFE_ES_Global.StartupSemaphoreReleased = FALSE;
    CFE_ES_Global.StartupFileComplete = TRUE;
    CFE_ES_Global.AppStartupCounter = 0;
    CFE_ES_Global.AppTable[1].StateRecord.AppState = CFE_ES_APP_STATE_INITIALIZING;
    CFE_ES_WaitForStartupSync(0);

    
    CFE_ES_Global.StartupFileComplete = FALSE;
    CFE_ES_WaitForStartupSync(99);

    CFE_ES_Global.StartupSemaphoreReleased = FALSE;
    CFE_ES_Global.StartupFileComplete = FALSE;
    CFE_ES_WaitForStartupSync(99);
    
    
    
}/* end testAPI */

void TestCDS()
{
   
    uint32 Temp;
    uint32 TempSize;

    /* Test the ValidateAppID function */  
    CFE_ES_Global.TaskTable[1].RecordUsed = TRUE;
    CFE_ES_Global.TaskTable[1].AppId = CFE_ES_MAX_APPLICATIONS +1;
    UT_Report (CFE_ES_CDS_ValidateAppID(&Temp) == CFE_ES_ERR_APPID, "CFE_ES_CDS_ValidateAppID", "Bad App ID");

    CFE_ES_Global.TaskTable[1].AppId = 1;

    CFE_ES_Global.CDSVars.MaxNumRegEntries = CFE_ES_CDS_MAX_NUM_ENTRIES +2;
    UT_SetBSPFail(BSP_READCDS_FAIL);
    
    UT_Report(CFE_ES_RebuildCDS() == CFE_ES_CDS_INVALID ,"CFE_ES_RebuildCDS","Second Read from CDS Bad");
    UT_SetBSPFail(0);

    UT_SetRtnCode(&BSPWriteCDSRtn,-1, 1);
    CFE_ES_InitCDSRegistry();


    /* CDS_EarlyInit Seems to kill the system log, and the next WRiteToSysLog  segfaults  */

    TempSize = CFE_ES_Global.CDSVars.CDSSize;
    /* A successful run */
    UT_SetBSPFail(0);
    UT_SetCDSSize(128*1024); 
    UT_SetRtnCode(&BSPReadCDSRtn,OS_SUCCESS, 1);
    CFE_ES_CDS_EarlyInit();

    /* Above max possible entries */
    Temp = CFE_ES_Global.CDSVars.MaxNumRegEntries;
    CFE_ES_Global.CDSVars.MaxNumRegEntries = CFE_ES_CDS_MAX_NUM_ENTRIES +1;
    UT_SetRtnCode(&BSPReadCDSRtn,OS_SUCCESS, 1);
    CFE_ES_CDS_EarlyInit();
    CFE_ES_Global.CDSVars.MaxNumRegEntries = Temp;

   UT_SetBSPFail(0);
    UT_SetCDSSize(1024);
    CFE_ES_CDS_EarlyInit();
    UT_SetCDSSize(128*1024);

    /* Cannot retrieve CDS size */   
    UT_SetBSPFail(BSP_GETCDSSIZE_FAIL); 
    UT_SetCDSSize(128*1024);   
    CFE_ES_CDS_EarlyInit();
    UT_SetBSPFail(0);

   UT_SetRtnCode(&BSPReadCDSRtn,OS_SUCCESS, 1);

    UT_SetCDSBSPCheckValidity(TRUE);
    CFE_ES_CDS_EarlyInit();
    UT_SetCDSBSPCheckValidity(FALSE);


    /* Test ValidateCDS */

    UT_SetCDSBSPCheckValidity(TRUE);

    printf ("G\n");
    UT_SetRtnCode(&BSPReadCDSRtn,OS_SUCCESS, 1);
    UT_SetBSPFail(BSP_READCDS_FAIL);
    UT_Report(CFE_ES_ValidateCDS() == OS_ERROR,"CFE_ES_ValidateCDS","Reading From BSP Failed");
    UT_SetBSPFail(0);

    UT_SetCDSReadGoodEnd(FALSE);
    UT_SetRtnCode(&BSPReadCDSRtn,OS_SUCCESS, 1);

    UT_Report(CFE_ES_ValidateCDS() == CFE_ES_CDS_INVALID,"CFE_ES_ValidateCDS","Reading From BSP Failed end check");
    UT_SetCDSReadGoodEnd(TRUE);

    UT_SetCDSBSPCheckValidity(FALSE);
   
    UT_SetRtnCode(&BSPReadCDSRtn, -1, 1);

    UT_Report(CFE_ES_ValidateCDS() < 0,"CFE_ES_ValidateCDS","Reading From BSP Failed end check");

/* pre 4.1 test */

    TempSize = CFE_ES_Global.CDSVars.CDSSize;
    /* A successful run */
    UT_SetBSPFail(0);
    UT_SetCDSSize(128*1024); 
    UT_SetRtnCode(&BSPReadCDSRtn,OS_SUCCESS, 1);
    CFE_ES_CDS_EarlyInit();


    /* Test path where first Write to the CDS fails */
    UT_SetBSPFail(BSP_WRITECDS_FAIL);
    UT_Report( CFE_ES_InitializeCDS(128 * 1024) == OS_ERROR,"CFE_ES_InitializeCDS","Writing to BSP Failed");
    UT_SetBSPFail(0);

    UT_SetBSPFail(BSP_WRITECDS_FAIL);
    UT_Report( CFE_ES_InitializeCDS(0) == OS_ERROR,"CFE_ES_InitializeCDS","Writing to BSP Failed");
    UT_SetBSPFail(0);

    UT_SetRtnCode(&BSPWriteCDSRtn,OS_SUCCESS, 1);
    
    /* Test initialing the CDS */
    UT_SetBSPFail(BSP_WRITECDS_FAIL);
    UT_Report( CFE_ES_InitializeCDS(0) == OS_ERROR,"CFE_ES_InitializeCDS","Writing to BSP Failed");
    UT_SetBSPFail(0);



    /* Test CFE_ES_RebuildCDS(void) */
    

    UT_SetRtnCode(&BSPReadCDSRtn,OS_SUCCESS, 1);

 /*   CFE_ES_Global.CDSVars.MaxNumRegEntries = CFE_ES_CDS_MAX_NUM_ENTRIES -4;*/
    CFE_ES_Global.CDSVars.MaxNumRegEntries = CFE_ES_CDS_MAX_NUM_ENTRIES +1;
    UT_SetBSPFail(BSP_READCDS_FAIL);
    UT_Report(CFE_ES_RebuildCDS() == CFE_ES_CDS_INVALID ,"CFE_ES_RebuildCDS","Second Read from CDS Bad");
    UT_SetBSPFail(0);

   CFE_ES_Global.CDSVars.MaxNumRegEntries = CFE_ES_CDS_MAX_NUM_ENTRIES -4;
    UT_Report(CFE_ES_RebuildCDS() == CFE_SUCCESS ,"CFE_ES_RebuildCDS","Success");

    UT_SetRtnCode(&BSPReadCDSRtn,OS_SUCCESS, 1);
    CFE_ES_Global.CDSVars.MaxNumRegEntries = CFE_ES_CDS_MAX_NUM_ENTRIES -4;
    UT_SetBSPFail(BSP_READCDS_FAIL);
    UT_Report(CFE_ES_RebuildCDS() == CFE_ES_CDS_INVALID ,"CFE_ES_RebuildCDS","Second Read from CDS Bad");
    UT_SetBSPFail(0);

    /* Test where PutCDSBlock fails */
    CFE_ES_CDSBlockDesc.CheckBits = CFE_ES_CDS_CHECK_PATTERN;
    CFE_ES_CDSBlockDesc.AllocatedFlag = CFE_ES_CDS_BLOCK_USED;
    CFE_ES_CDSBlockDesc.ActualSize =  512;



    CFE_ES_Global.CDSVars.Registry[0].Taken = TRUE;
    CFE_ES_Global.CDSVars.Registry[0].Table = TRUE;
    CFE_ES_Global.CDSVars.Registry[0].MemHandle = 200;

    strncpy(CFE_ES_Global.CDSVars.Registry[0].Name,"NO_APP.CDS_NAME",OS_MAX_API_NAME);

    UT_SetRtnCode(&BSPWriteCDSRtn, -1, 2);
    UT_Report (CFE_ES_DeleteCDS("NO_APP.CDS_NAME",TRUE)  < 0, "CFE_ES_DeleteCDS", "PuutCDSBlock Failed");
    
    /* Test where the owner app is still active */
    CFE_ES_Global.CDSVars.Registry[0].Taken = TRUE;
    CFE_ES_Global.CDSVars.Registry[0].Table = TRUE;
    CFE_ES_Global.CDSVars.Registry[0].MemHandle = 200;

    strncpy(CFE_ES_Global.CDSVars.Registry[0].Name,"CFE_ES.CDS_NAME",OS_MAX_API_NAME);
    strcpy((char *)(CFE_ES_Global.AppTable[0].StartParams.Name), "CFE_ES");
    CFE_ES_Global.AppTable[0].RecordUsed = TRUE;
    UT_Report (CFE_ES_DeleteCDS("CFE_ES.CDS_NAME",TRUE)  == CFE_ES_CDS_OWNER_ACTIVE_ERR, "CFE_ES_DeleteCDS", "Owner App still active");


}

void TestCDSMempool(void)
{


    uint32 MinCDSSize = CFE_ES_CDS_MIN_BLOCK_SIZE + sizeof(CFE_ES_CDSBlockDesc_t);
    CFE_ES_CDSBlockHandle_t BlockHandle;
/*    int i; */ /* unused -- jmd 2010-09-08 */
    int Data;


    /* Set up the block to read what we need to from the CDS */

    CFE_ES_CDSBlockDesc.CheckBits = CFE_ES_CDS_CHECK_PATTERN;
    CFE_ES_CDSBlockDesc.AllocatedFlag = CFE_ES_CDS_BLOCK_USED;
    CFE_ES_CDSBlockDesc.ActualSize =  512;
    CFE_ES_CDSBlockDesc.SizeUsed =  512;

    /* Test Creating the CDS pool */

    UT_Report (CFE_ES_CreateCDSPool(0,0) == CFE_ES_BAD_ARGUMENT, "CFE_ES_CreateCDSPool","CDS PoolSize too small");

    /* Test Rebuilding the CDS pool */
    UT_Report (CFE_ES_RebuildCDSPool(0,0) == CFE_ES_BAD_ARGUMENT, "CFE_ES_RebuildCDSPool","CDS PoolSize too small");

    UT_SetRtnCode(&BSPReadCDSRtn,OS_SUCCESS, 1);
    UT_SetBSPFail(BSP_READCDS_FAIL);
    CFE_ES_CDSBlockDesc.AllocatedFlag = CFE_ES_CDS_BLOCK_UNUSED;
    UT_Report (CFE_ES_RebuildCDSPool(MinCDSSize,0) == CFE_SUCCESS, "CFE_ES_RebuildCDSPool","CDS Block Not used");
    UT_SetBSPFail(0);

   
    UT_SetBSPFail(BSP_READCDS_FAIL);
    UT_Report (CFE_ES_RebuildCDSPool(MinCDSSize,0) == CFE_ES_CDS_ACCESS_ERROR, "CFE_ES_RebuildCDSPool","Reading CDS Failed");
    UT_SetBSPFail(0);

    UT_SetRtnCode(&BSPReadCDSRtn,OS_SUCCESS, 1);
    UT_SetBSPFail(BSP_READCDS_FAIL | BSP_WRITECDS_FAIL);
    UT_Report (CFE_ES_RebuildCDSPool(MinCDSSize,0) == CFE_ES_CDS_ACCESS_ERROR, "CFE_ES_RebuildCDSPool","Writing CDS Failed");
    UT_SetBSPFail(0);
    
    CFE_ES_CDSBlockDesc.CheckBits = 0;
    UT_SetRtnCode(&BSPReadCDSRtn,OS_SUCCESS, 1);
    UT_SetBSPFail(BSP_READCDS_FAIL);
    UT_Report (CFE_ES_RebuildCDSPool(MinCDSSize,0) == CFE_ES_CDS_ACCESS_ERROR, "CFE_ES_RebuildCDSPool","CDS Block Not Used Before");
    UT_SetBSPFail(0);
    CFE_ES_CDSBlockDesc.CheckBits = CFE_ES_CDS_CHECK_PATTERN;

    /* Test GetCDSBlock */



    /* Create a pool where the offset  = 0;*/
    CFE_ES_CreateCDSPool(1000000,0);
    CFE_ES_GetCDSBlock(&BlockHandle,800);
    CFE_ES_PutCDSBlock( BlockHandle);


    /* create a new pool and  set up for getting a premade block */
    BlockHandle = 0;
    CFE_ES_CreateCDSPool(1000000,8);
    CFE_ES_GetCDSBlock(&BlockHandle,800);
    CFE_ES_PutCDSBlock( BlockHandle);


     UT_SetRtnCode (&BSPReadCDSRtn, -1, 1);  

    UT_Report(CFE_ES_GetCDSBlock(&BlockHandle,800) == CFE_ES_CDS_ACCESS_ERROR ,"CFE_ES_GetCDSBlock","Reading CDS Failed");


    UT_SetBSPFail(BSP_WRITECDS_FAIL);  
    UT_SetRtnCode (&BSPWriteCDSRtn, -1, 1);  
    UT_Report(CFE_ES_GetCDSBlock(&BlockHandle,800) == CFE_ES_CDS_ACCESS_ERROR ,"CFE_ES_GetCDSBlock","Writing CDS Failed");
    UT_SetBSPFail(0);
    
    UT_Report(CFE_ES_GetCDSBlock(&BlockHandle,CFE_ES_CDS_MAX_BLOCK_SIZE + 1) == CFE_ES_ERR_MEM_BLOCK_SIZE ,"CFE_ES_GetCDSBlock","Block Size Too Big");

    /* Test Putting a CDS Block */

    /* make blockhandle invalid ( <8) */
    BlockHandle = 7;
    UT_Report( CFE_ES_PutCDSBlock( BlockHandle)==CFE_ES_ERR_MEM_HANDLE ,"CFE_ES_PutCDSBlock","Invalid CDS Handle");

    UT_SetBSPFail(BSP_READCDS_FAIL); 
    BlockHandle = 10;
    UT_Report( CFE_ES_PutCDSBlock( BlockHandle)==CFE_ES_CDS_ACCESS_ERROR,"CFE_ES_PutCDSBlock","Reading CDS Failed");
    UT_SetBSPFail (0);

    CFE_ES_CDSBlockDesc.AllocatedFlag = CFE_ES_CDS_BLOCK_UNUSED;
    UT_Report( CFE_ES_PutCDSBlock( BlockHandle)== CFE_ES_ERR_MEM_HANDLE,"CFE_ES_PutCDSBlock","Invalid Block descriptor");

    CFE_ES_CDSBlockDesc.ActualSize  = CFE_ES_CDS_MAX_BLOCK_SIZE + 1;
    CFE_ES_CDSBlockDesc.AllocatedFlag = CFE_ES_CDS_BLOCK_USED;
    UT_Report( CFE_ES_PutCDSBlock( BlockHandle)== CFE_ES_ERR_MEM_HANDLE,"CFE_ES_PutCDSBlock","Invalid Mem Handle");

    CFE_ES_CDSBlockDesc.ActualSize  = 452;
    UT_SetBSPFail(BSP_WRITECDS_FAIL);
    UT_Report( CFE_ES_PutCDSBlock( BlockHandle)== CFE_ES_CDS_ACCESS_ERROR,"CFE_ES_PutCDSBlock","Error Writing CDS");
    UT_SetBSPFail(0);

    /* Test CDS Block Write */
    BlockHandle = 7;
    UT_Report( CFE_ES_CDSBlockWrite(BlockHandle, &Data)== CFE_ES_ERR_MEM_HANDLE ,"CFE_ES_CDSBlockWrite","Invalid CDS Handle");
    BlockHandle = 10;

    CFE_ES_CDSBlockDesc.ActualSize  = CFE_ES_CDS_MAX_BLOCK_SIZE + 1;
    CFE_ES_CDSBlockDesc.CheckBits = CFE_ES_CDS_CHECK_PATTERN;
    CFE_ES_CDSBlockDesc.AllocatedFlag = CFE_ES_CDS_BLOCK_USED;
    UT_Report( CFE_ES_CDSBlockWrite(BlockHandle, &Data)== CFE_ES_ERR_MEM_HANDLE ,"CFE_ES_CDSBlockWrite","Actual Size too big");
    CFE_ES_CDSBlockDesc.ActualSize  = 452;

    CFE_ES_CDSBlockDesc.AllocatedFlag = CFE_ES_CDS_BLOCK_UNUSED;
    UT_Report( CFE_ES_CDSBlockWrite(BlockHandle, &Data)== CFE_ES_ERR_MEM_HANDLE ,"CFE_ES_CDSBlockWrite","CDS Block Invalid");
    CFE_ES_CDSBlockDesc.AllocatedFlag = CFE_ES_CDS_BLOCK_USED;    
    
    UT_SetBSPFail(BSP_WRITECDS_FAIL);
    UT_Report( CFE_ES_CDSBlockWrite(BlockHandle, &Data)== OS_ERROR ,"CFE_ES_CDSBlockWrite","Error Writing CDS");
    UT_SetBSPFail (0);

    UT_SetBSPFail(BSP_WRITECDS_FAIL);
    UT_SetRtnCode(&BSPWriteCDSRtn,OS_SUCCESS, 1);
    UT_Report( CFE_ES_CDSBlockWrite(BlockHandle, &Data)== OS_ERROR,"CFE_ES_CDSBlockWrite","Error Writing CDS");
    UT_SetBSPFail (0);

  
    UT_SetBSPFail(BSP_READCDS_FAIL);
    UT_Report( CFE_ES_CDSBlockWrite(BlockHandle, &Data)== OS_ERROR,"CFE_ES_CDSBlockWrite","Error Reading CDS");
    UT_SetBSPFail (0);

    /* Test CDS Block Read */
    BlockHandle = 7;
    UT_Report( CFE_ES_CDSBlockRead(&Data, BlockHandle)== CFE_ES_ERR_MEM_HANDLE ,"CFE_ES_CDSBlockRead","Invalid CDS Handle");
    BlockHandle = 10;

    CFE_ES_CDSBlockDesc.ActualSize  = CFE_ES_CDS_MAX_BLOCK_SIZE + 1;
    CFE_ES_CDSBlockDesc.CheckBits = CFE_ES_CDS_CHECK_PATTERN;
    CFE_ES_CDSBlockDesc.AllocatedFlag = CFE_ES_CDS_BLOCK_USED;
    UT_Report( CFE_ES_CDSBlockRead(&Data, BlockHandle)== CFE_ES_ERR_MEM_HANDLE ,"CFE_ES_CDSBlockRead","Actual Size too big");
    CFE_ES_CDSBlockDesc.ActualSize  = 452;


    CFE_ES_CDSBlockDesc.AllocatedFlag = CFE_ES_CDS_BLOCK_UNUSED;
    UT_Report( CFE_ES_CDSBlockRead(&Data, BlockHandle)== CFE_ES_ERR_MEM_HANDLE ,"CFE_ES_CDSBlockRead","CDS Block Invalid");
    CFE_ES_CDSBlockDesc.AllocatedFlag = CFE_ES_CDS_BLOCK_USED;

    Data = CFE_ES_CDSBlockDesc.CRC;
    CFE_ES_CDSBlockDesc.CRC =56456464;
    UT_Report( CFE_ES_CDSBlockRead(&Data, BlockHandle)== CFE_ES_CDS_BLOCK_CRC_ERR,"CFE_ES_CDSBlockRead","CRC Doesn't match");
    CFE_ES_CDSBlockDesc.CRC = Data;

  
    UT_SetBSPFail(BSP_READCDS_FAIL);
    UT_Report( CFE_ES_CDSBlockRead(&Data, BlockHandle)== OS_ERROR ,"CFE_ES_CDSBlockRead","Error Reading CDS");
    UT_SetBSPFail (0);

    UT_SetBSPFail(BSP_READCDS_FAIL);
    UT_SetRtnCode(&BSPReadCDSRtn,OS_SUCCESS, 1);

    UT_Report( CFE_ES_CDSBlockRead(&Data, BlockHandle)== OS_ERROR,"CFE_ES_CDSBlockRead","Error Reading CDS");
    UT_SetBSPFail (0);

}

void TestESMempool(void)
{



    CFE_ES_MemHandle_t  HandlePtr;
    uint8               Buffer[CFE_ES_MAX_BLOCK_SIZE];
    uint8               *address = NULL;
    uint8               *address2 = NULL;
    Pool_t              *PoolPtr;
    CFE_ES_MemPoolStats_t Stats;
    uint32              BlockSizes[4];
    BD_t                *BdPtr;
    CFE_ES_MemHandle_t  HandlePtr2;
    CFE_ES_MemHandle_t  HandlePtrSave;
    
    /* Test Pool Create */

    UT_Report(CFE_ES_PoolCreateNoSem(&HandlePtr, Buffer, 0) == CFE_ES_BAD_ARGUMENT ,"CFE_ES_PoolCreateNoSem","Size too small");
    UT_Report(CFE_ES_PoolCreateNoSem(&HandlePtr, Buffer, CFE_ES_MAX_BLOCK_SIZE) == CFE_SUCCESS ,"CFE_ES_PoolCreateNoSem","Successful");

    UT_Report(CFE_ES_PoolCreate(&HandlePtr, Buffer, 0) == CFE_ES_BAD_ARGUMENT ,"CFE_ES_PoolCreate","Size too small");
    UT_Report(CFE_ES_PoolCreate(&HandlePtr, Buffer, CFE_ES_MAX_BLOCK_SIZE) == CFE_SUCCESS ,"CFE_ES_PoolCreate","Successful");

    /* Test GetPoolBuf */

    /* Test a normal run through Getting and putting a Buffer. This will allocated the first block of a
     * certain size */

    UT_Report(CFE_ES_GetPoolBuf((uint32 **)(&address), HandlePtr, 256) > 0, "CFE_ES_GetPoolBuf","Successful");

    UT_Report(CFE_ES_GetPoolBufInfo(HandlePtr, (uint32 *)(address) ) > 0, "CFE_ES_GetPoolBufInfo","Successful");
   
    UT_Report(CFE_ES_PutPoolBuf(HandlePtr, (uint32 *)(address) ) > 0, "CFE_ES_PutPoolBuf","Successful");
   






    /* Now run through another successful run. Since this block size has a block free, it will take that path */


    UT_Report(CFE_ES_GetPoolBuf((uint32 **)(&address), HandlePtr, 256) > 0, "CFE_ES_GetPoolBuf","Successful, second run");


    UT_Report(CFE_ES_PutPoolBuf(HandlePtr, (uint32 *)(address) ) > 0, "CFE_ES_PutPoolBuf","Successful, second run");





   
    /* Test Validate Handle */


    UT_Report(CFE_ES_ValidateHandle(3) == FALSE, "CFE_ES_ValidateHandle", "Invalid Handle");

    PoolPtr = (Pool_t *) &HandlePtr2;
    PoolPtr -> Start = &(HandlePtrSave);
    PoolPtr-> Size = 64;

    /* test not having enough memory */
/*    UT_Report(CFE_ES_GetPoolBuf((uint32 *)(&address), HandlePtr, 768) > 0, "CFE_ES_GetPoolBuf","n");*/


    UT_Report(CFE_ES_ValidateHandle(HandlePtr2) == FALSE, "CFE_ES_ValidateHandle", "Invalid Handle");

    UT_Report(CFE_ES_GetPoolBuf((uint32 **)(&address), HandlePtr2, 256)  == CFE_ES_ERR_MEM_HANDLE, "CFE_ES_GetPoolBuf","Bad Mem Handle");

    UT_Report(CFE_ES_GetMemPoolStats(&Stats,HandlePtr2)  == CFE_ES_ERR_MEM_HANDLE, "CFE_ES_GetMemPoolStats","Invalid Handle"); 



    PoolPtr = (Pool_t *) &HandlePtr2;
    PoolPtr -> Start = &(HandlePtr2);  
    PoolPtr-> Size = 63;
   
    UT_Report(CFE_ES_ValidateHandle(HandlePtr2) == FALSE, "CFE_ES_ValidateHandle", "Invalid Handle");

   

/* Test taking up too much memory in a request */
    UT_Report(CFE_ES_PoolCreate(&HandlePtr, Buffer, CFE_ES_MAX_BLOCK_SIZE) == CFE_SUCCESS ,"CFE_ES_PoolCreate","Successful");    



    UT_Report(CFE_ES_GetPoolBuf((uint32 **)(&address), HandlePtr, 75000) == CFE_ES_ERR_MEM_BLOCK_SIZE, "CFE_ES_GetPoolBuf"," Size too big to fit ");

    /* test invalid handle in GetPoolBufInfo and PutPoolBuf */
    UT_Report(CFE_ES_GetPoolBufInfo(HandlePtr, (uint32 *)(address) ) < 0, "CFE_ES_GetPoolBufInfo","Invalid Mem Handle");

    UT_Report(CFE_ES_PutPoolBuf(HandlePtr, (uint32 *)(address) ) == CFE_ES_ERR_MEM_HANDLE, "CFE_ES_PutPoolBuf","Invalid Mem Handle");



    UT_Report(CFE_ES_GetPoolBuf((uint32 **)(&address), HandlePtr, 100) > 0, "CFE_ES_GetPoolBuf","Successful, third run"); 





    /* test ES_PoolCreate Ex */
    UT_Report (CFE_ES_PoolCreateEx(&HandlePtr,(uint8 *)&address,0,CFE_ES_MAX_MEMPOOL_BLOCK_SIZES + 2, BlockSizes, CFE_ES_USE_MUTEX)== CFE_ES_BAD_ARGUMENT, "CFE_ES_PoolCreateEx"," Bad NumBlockSizes" );

    UT_Report (CFE_ES_PoolCreateEx(&HandlePtr,(uint8 *)&address,0,CFE_ES_MAX_MEMPOOL_BLOCK_SIZES -2, NULL, CFE_ES_USE_MUTEX)== CFE_ES_BAD_ARGUMENT, "CFE_ES_PoolCreateEx"," Bad block size pointer" );


    UT_Report (CFE_ES_PoolCreateEx(&HandlePtr,(uint8 *)( 145453),0,CFE_ES_MAX_MEMPOOL_BLOCK_SIZES -2, BlockSizes, CFE_ES_USE_MUTEX)== CFE_ES_BAD_ARGUMENT, "CFE_ES_PoolCreateEx"," Bad address" );

    UT_Report (CFE_ES_PoolCreateEx(&HandlePtr,(uint8 *)&address,0,CFE_ES_MAX_MEMPOOL_BLOCK_SIZES -2, BlockSizes, 2)== CFE_ES_BAD_ARGUMENT, "CFE_ES_PoolCreateEx"," Bad mutex option" );


    BlockSizes[0] = 10 ;
    BlockSizes[1] = 50 ;
    BlockSizes[2] = 100;
    BlockSizes[3] = 1000 ;



    UT_Report (CFE_ES_PoolCreateEx(&HandlePtr,(uint8 *)&address,0,4, BlockSizes, CFE_ES_USE_MUTEX)== CFE_ES_BAD_ARGUMENT, "CFE_ES_PoolCreateEx"," Bad NumBlockSizes" );

    
/* Finish testing GetPoolBufInfo and PutPoolBuf */

/*    uint32 array[50]; */  /* unused -- jmd 2010-09-08 */


    UT_Report(CFE_ES_PoolCreateNoSem(&HandlePtr, Buffer, CFE_ES_MAX_BLOCK_SIZE) == CFE_SUCCESS ,"CFE_ES_PoolCreateNoSem","Successful");

    UT_Report(CFE_ES_PoolCreate(&HandlePtr, Buffer, CFE_ES_MAX_BLOCK_SIZE) == CFE_SUCCESS ,"CFE_ES_PoolCreate","Successful");

    UT_Report(CFE_ES_GetPoolBuf((uint32 **)&address, HandlePtr, 256) > 0, "CFE_ES_GetPoolBuf","Successful");

    BdPtr    = (BD_t *) ((uint8 *)address - sizeof(BD_t));

    BdPtr -> Allocated = 717;


    UT_Report(CFE_ES_GetPoolBufInfo(HandlePtr, (uint32 *)address ) == CFE_ES_ERR_MEM_HANDLE, "CFE_ES_GetPoolBufInfo","Invalid Mem Handle");
    UT_Report(CFE_ES_PutPoolBuf(HandlePtr, (uint32 *)address ) == CFE_ES_ERR_MEM_HANDLE, "CFE_ES_PutPoolBuf","Invalid Mem Handle");
    
    BdPtr -> Allocated = 0xaaaa;
    BdPtr -> CheckBits = 717;


    UT_Report(CFE_ES_GetPoolBufInfo(HandlePtr, (uint32 *)address ) == CFE_ES_ERR_MEM_HANDLE, "CFE_ES_GetPoolBufInfo","Invalid Mem Handle");
    UT_Report(CFE_ES_PutPoolBuf(HandlePtr, (uint32 *)address ) == CFE_ES_ERR_MEM_HANDLE, "CFE_ES_PutPoolBuf","Invalid Mem Handle");



    UT_Report(CFE_ES_PoolCreate(&HandlePtr, Buffer, CFE_ES_MAX_BLOCK_SIZE) == CFE_SUCCESS ,"CFE_ES_PoolCreate","Successful");
    UT_Report(CFE_ES_GetPoolBuf((uint32 **)&address, HandlePtr, 256) > 0, "CFE_ES_GetPoolBuf","Successful");

    
    BdPtr -> CheckBits = 0x5a5a;
    BdPtr-> Size =CFE_ES_MAX_BLOCK_SIZE +1;

    UT_Report(CFE_ES_PutPoolBuf(HandlePtr, (uint32 *)address ) == CFE_ES_ERR_MEM_HANDLE, "CFE_ES_PutPoolBuf","Invalid Mem Handle");

    /* Test too big of a Buff size */
    UT_Report(CFE_ES_GetPoolBuf((uint32 **)(&address2), HandlePtr, 99000)  == CFE_ES_ERR_MEM_BLOCK_SIZE, "CFE_ES_GetPoolBuf","Size Too Big"); 

    /* NULL Pointer tests */
    UT_Report(CFE_ES_ValidateHandle(0) == FALSE, "CFE_ES_ValidateHandle", "NULL Handle");
    UT_Report(CFE_ES_PutPoolBuf(0, (uint32 *)address ) == CFE_ES_ERR_MEM_HANDLE, "CFE_ES_PutPoolBuf","NULL Mem Handle");
    UT_Report(CFE_ES_GetPoolBuf((uint32 **)&address, 0, 256)  == CFE_ES_ERR_MEM_HANDLE, "CFE_ES_GetPoolBuf","NULL Mem Handle");
    UT_Report(CFE_ES_GetPoolBufInfo(0, (uint32 *)address ) == CFE_ES_ERR_MEM_HANDLE, "CFE_ES_GetPoolBufInfo","NULL Mem Handle");

}
        
