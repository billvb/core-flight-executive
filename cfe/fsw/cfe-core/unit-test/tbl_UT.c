/*
**
** Name: tbl_UT.c
** Author: Jonathan Oben, Intern to Nicholas Yanchik
** Last Updated: May 15, 2007
**
** Notes: This file is the unit test of the
**			Table Services module of the cFE.
**			
*/

#include <stdio.h>
#include <string.h>
#include "cfe.h"
#include "cfe_tbl.h"
#include "common_types.h"
#include "ut_stubs.h"
#include "cfe_tbl_msg.h"
#include "../src/tbl/cfe_tbl_internal.h"
#include "../src/tbl/cfe_tbl_task.h"
#include "../src/tbl/cfe_tbl_task_cmds.h"

/* Functions and Variables from other files not included in '~.h' files */

/* From cfe_tbl_task_cmds.h */
	CFE_TBL_CmdProcRet_t CFE_TBL_DumpToFile( char *DumpFilename, char *TableName, 
											void *DumpDataAddr, uint32 TblSizeInBytes);
	void CFE_TBL_GetTblRegData(void);
	void CFE_TBL_GetHkData(void);

/* From cfe_tbl_task.c */
	#define CFE_TBL_BAD_CMD_CODE  (-1) /**< Command Code found in Message does not match any in #CFE_TBL_CmdHandlerTbl */
	#define CFE_TBL_BAD_MSG_ID    (-2) /**< Message ID found in Message does not match any in #CFE_TBL_CmdHandlerTbl */

	typedef enum
	{
		CFE_TBL_TERM_MSGTYPE=0,   /**< \brief Command Handler Table Terminator Type */
		CFE_TBL_MSG_MSGTYPE,      /**< \brief Message Type (requires Message ID match) */
		CFE_TBL_CMD_MSGTYPE       /**< \brief Command Type (requires Message ID and Command Code match) */
	} CFE_TBL_MsgType_t;

	/**
	** Data structure of a single record in #CFE_TBL_CmdHandlerTbl
	*/
	typedef struct {
		uint32                   MsgId;           /**< \brief Acceptable Message ID */
		uint32                   CmdCode;         /**< \brief Acceptable Command Code (if necessary) */
		uint32                   ExpectedLength;  /**< \brief Expected Message Length (in bytes) including message header */
		CFE_TBL_MsgProcFuncPtr_t MsgProcFuncPtr;  /**< \brief Pointer to function to handle message  */
		CFE_TBL_MsgType_t        MsgTypes;        /**< \brief Message Type (i.e. - with/without Cmd Code)   */
	} CFE_TBL_CmdHandlerTblRec_t;

	void  CFE_TBL_TaskMain(void);
	int32 CFE_TBL_TaskInit(void);
	void CFE_TBL_InitData(void);
	void  CFE_TBL_TaskPipe(CFE_SB_Msg_t *MessagePtr);
	int16 CFE_TBL_SearchCmdHndlrTbl(CFE_SB_MsgId_t MessageID, uint16 CommandCode);

/* Testing Function Declarations */
void Test_CFE_TBL_TlmRegCmd(void);
void Test_CFE_TBL_AbortLoadCmd(void);
void Test_CFE_TBL_ActivateCmd(void);
void Test_CFE_TBL_DumpToFile(void);
void Test_CFE_TBL_ValidateCmd(void);
void Test_CFE_TBL_ResetCmd(void);
void Test_CFE_TBL_NoopCmd(void);
void Test_CFE_TBL_GetTblRegData(void);
void Test_CFE_TBL_GetHkData(void);
void Test_CFE_TBL_DumpRegCmd(void);
void Test_CFE_TBL_DumpCmd(void);
void Test_CFE_TBL_LoadCmd(void);
void Test_CFE_TBL_HousekeepingCmd(void);
void Test_CFE_TBL_DeleteCDSCmd(void);
void Test_CFE_TBL_TaskInit(void);
void Test_CFE_TBL_InitData(void);
void Test_CFE_TBL_SearchCmdHndlrTbl(void);
void Test_CFE_TBL_TaskPipe(void);
void Test_CFE_TBL_ApiInit(void);
void Test_CFE_TBL_Register(void);
void Test_CFE_TBL_Share(void);
void Test_CFE_TBL_Unregister(void);
void Test_CFE_TBL_Load(void);
void Test_CFE_TBL_Update(void);
void Test_CFE_TBL_GetAddress(void);
void Test_CFE_TBL_ReleaseAddress(void);
void Test_CFE_TBL_GetAddresses(void);
void Test_CFE_TBL_ReleaseAddresses(void);
void Test_CFE_TBL_Validate(void);
void Test_CFE_TBL_Manage(void);
void Test_CFE_TBL_GetStatus(void);
void Test_CFE_TBL_GetInfo(void);
void Test_CFE_TBL_Internal(void);
void Test_CFE_TBL_DCRs(void);
int32 Test_CFE_TBL_ValidationFunc(void *TblPtr);

/* Function Declarations for utility functions:
**     Functions written to expidite Table Registry checks */
void UT_SwapArrayforControl(CFE_TBL_RegistryRec_t Registry[], CFE_TBL_RegistryRec_t Desired[]);
void ReturnOriginalValues(CFE_TBL_RegistryRec_t Registry[]);
int32 FindLength(CFE_TBL_RegistryRec_t Registry[]);

extern void CFE_TBL_ByteSwapUint32(uint32 *Uint32ToSwapPtr);

/* Global Variables */
typedef struct
{
    uint32 TblElement1;
    uint32 TblElement2;
} UT_Table1_t;

typedef struct
{
    uint32 TblElement1;
    uint32 TblElement2;
    uint32 TblElement3;
} UT_Table2_t;

typedef struct
{
    CFE_TBL_File_Hdr_t      TblHeader;
    UT_Table1_t             TblData;
} UT_TempFile_t;

CFE_TBL_TaskData_t CFE_TBL_TaskData;
CFE_TBL_Handle_t   App1TblHandle1;
CFE_TBL_Handle_t   App1TblHandle2;
CFE_TBL_Handle_t   App2TblHandle1;
CFE_TBL_Handle_t   App2TblHandle2;
CFE_TBL_Handle_t   ArrayOfHandles[2];
void              *Tbl1Ptr = NULL;
void              *Tbl2Ptr = NULL;
void             **ArrayOfPtrsToTblPtrs[2];

CFE_TBL_RegistryRec_t Original[CFE_TBL_MAX_NUM_TABLES];
UT_SetRtn_t  TBL_ValidationFuncRtn;

extern CFE_TBL_TaskData_t CFE_TBL_TaskData;
extern uint32 UT_OS_Fail;
extern UT_SetRtn_t FileWriteRtn; 
extern UT_SetRtn_t FSWriteHdrRtn;
extern UT_SetRtn_t FSReadHdrRtn;
extern UT_SetRtn_t TBLGetWrkBuffRtn;
extern UT_SetRtn_t OSReadRtn;
extern UT_SetRtn_t OSReadRtn2; 
extern UT_SetRtn_t SBSendMsgRtn;
extern UT_SetRtn_t FSSetTimestampRtn;
extern UT_SetRtn_t ES_DeleteCDSRtn;
extern UT_SetRtn_t ES_RegisterCDSRtn;
extern UT_SetRtn_t ES_CopyToCDSRtn;
extern UT_SetRtn_t ES_RestoreFromCDSRtn;
extern UT_SetRtn_t ES_RegisterRtn;
extern UT_SetRtn_t SB_CreatePipeRtn;
extern UT_SetRtn_t SB_SubscribeRtn;
extern UT_SetRtn_t EVS_SendEventRtn;
extern UT_SetRtn_t EVS_RegisterRtn;
extern UT_SetRtn_t GetAppIDRtn;
extern UT_SetRtn_t GetPoolRtn;
extern UT_SetRtn_t PutPoolRtn;
extern UT_SetRtn_t MutSemCreateRtn;
extern UT_SetRtn_t MutSemTakeRtn;
extern UT_SetRtn_t CreatePoolRtn;
extern uint8       UT_CDS[UT_CDS_SIZE];
extern UT_CDS_Map_t UT_CDS_Map;
extern FILE *UT_logfile;

/* Saves the data inside the Array passed in and sets the data to a controlled set of data */
void UT_SwapRegistryforControl(CFE_TBL_RegistryRec_t Registry[], CFE_TBL_RegistryRec_t Desired[])
{
	int length = FindLength(Registry);
	int i, h;
	for(i = 0; i < length; i++)
		strcpy(Original[i].Name, Registry[i].Name);
	for(h = 0; h < length; h++)
		strcpy(Registry[h].Name, Desired[h].Name);
}

/* Returns the original array's values passed in by the UT_SwapRegistryforControl function */
void UT_ReturnOriginValues(CFE_TBL_RegistryRec_t Registry[])
{
	int j;
	int length = FindLength(Registry);
	for(j = 0; j < length; j++)
		strcpy(Registry[j].Name, Original[j].Name);
}

/* Finds the number of indexes in the array passed into function */
int32 FindLength(CFE_TBL_RegistryRec_t Registry[])
{
	int length = 0;
	if(sizeof(Registry) != 0 && sizeof(Registry[0]) != 0)
		length = (int)(sizeof(Registry) / sizeof(Registry[0]));
	return length;
}



/* All test functions are run in the main function */

void UT_ProcessSBMsg(CFE_SB_Msg_t *MsgPtr)
{
    CFE_TBL_TaskPipe(MsgPtr);
}


void UT_ResetTableRegistry(void)
{
    int32 i=0;
    
    for(i=0; i < CFE_TBL_MAX_NUM_TABLES; i++)
    {
        CFE_TBL_InitRegistryRecord(&CFE_TBL_TaskData.Registry[i]);
    }

    /* Initialize the Table Access Descriptors */
    for (i=0; i<CFE_TBL_MAX_NUM_HANDLES; i++)
    {
        CFE_TBL_TaskData.Handles[i].AppId = CFE_ES_ERR_APPID;
        CFE_TBL_TaskData.Handles[i].RegIndex = 0;
        CFE_TBL_TaskData.Handles[i].PrevLink = CFE_TBL_END_OF_LIST;
        CFE_TBL_TaskData.Handles[i].NextLink = CFE_TBL_END_OF_LIST;
        CFE_TBL_TaskData.Handles[i].UsedFlag = FALSE;
        CFE_TBL_TaskData.Handles[i].LockFlag = FALSE;
        CFE_TBL_TaskData.Handles[i].Updated = FALSE;
        CFE_TBL_TaskData.Handles[i].BufferIndex = 0;
    }

    /* Initialize the Table Validation Results Records */
    for (i=0; i<CFE_TBL_MAX_NUM_VALIDATIONS; i++)
    {
        CFE_TBL_TaskData.ValidationResults[i].State = CFE_TBL_VALIDATION_FREE;
        CFE_TBL_TaskData.ValidationResults[i].CrcOfTable = 0;
        CFE_TBL_TaskData.ValidationResults[i].Result = 0;
        CFE_TBL_TaskData.ValidationResults[i].ActiveBuffer = FALSE;
        CFE_TBL_TaskData.ValidationResults[i].TableName[0] = '\0';
    }

    /* Initialize the Dump-Only Table Dump Control Blocks */
    for (i=0; i<CFE_TBL_MAX_SIMULTANEOUS_LOADS; i++)
    {
        CFE_TBL_TaskData.DumpControlBlocks[i].State = CFE_TBL_DUMP_FREE;
        CFE_TBL_TaskData.DumpControlBlocks[i].DumpBufferPtr = NULL;
        CFE_TBL_TaskData.DumpControlBlocks[i].Size = 0;
        CFE_TBL_TaskData.DumpControlBlocks[i].TableName[0] = '\0';
        
        /* Free all shared buffers */
        CFE_TBL_TaskData.LoadBuffs[i].Taken = FALSE;
    }

    CFE_TBL_TaskData.ValidationCtr = 0;

    CFE_TBL_TaskData.HkTlmTblRegIndex = CFE_TBL_NOT_FOUND;
    CFE_TBL_TaskData.LastTblUpdated = CFE_TBL_NOT_FOUND;
}

int main(void)
{
	/* Initialize Unit Test */
	UT_Init("ut_cfe_tbl");
	TBL_ValidationFuncRtn.count = 0;
    
	/* cfe_tbl_task.c Functions */
	Test_CFE_TBL_TaskInit();
	Test_CFE_TBL_InitData();
	Test_CFE_TBL_SearchCmdHndlrTbl();
	
	/* cfe_tbl_task_cmds.c Functions */
	Test_CFE_TBL_DeleteCDSCmd();   /* This should be done first (it initializes working data structures) */
	Test_CFE_TBL_TlmRegCmd();
	Test_CFE_TBL_AbortLoadCmd();
	Test_CFE_TBL_ActivateCmd();
	Test_CFE_TBL_DumpToFile();
	Test_CFE_TBL_ResetCmd();
	Test_CFE_TBL_ValidateCmd();
	Test_CFE_TBL_NoopCmd();
	Test_CFE_TBL_GetTblRegData();
	Test_CFE_TBL_GetHkData();
	Test_CFE_TBL_DumpRegCmd();
	Test_CFE_TBL_DumpCmd();
	Test_CFE_TBL_LoadCmd();
	Test_CFE_TBL_HousekeepingCmd();
	
	/* cfe_tbl_api.c and cfe_tbl_internal.c Functions */
    Test_CFE_TBL_ApiInit();
    Test_CFE_TBL_Register();
    Test_CFE_TBL_Share();
    Test_CFE_TBL_Unregister();
    Test_CFE_TBL_Load();
    Test_CFE_TBL_GetAddress();
    Test_CFE_TBL_ReleaseAddress();
    Test_CFE_TBL_GetAddresses();
    Test_CFE_TBL_ReleaseAddresses();
    Test_CFE_TBL_Validate();
    Test_CFE_TBL_Manage();
    Test_CFE_TBL_Update();
    Test_CFE_TBL_GetStatus();
    Test_CFE_TBL_GetInfo();

    /* miscellaneous cfe_tbl_internal.c tests */
    Test_CFE_TBL_Internal();
    
    /* Additional tests for DCR verifications */
    Test_CFE_TBL_DCRs();
    
    /* Final report on number of errors */
    UT_ReportFailures();
    
	exit(0);
}


/*
** Tests for cfe_tbl_task.c
*/

void Test_CFE_TBL_TaskPipe(void)
{
	
}

void Test_CFE_TBL_SearchCmdHndlrTbl(void)
{
	
	int16 TblIndex = 1;
	CFE_SB_MsgId_t MsgID;
	uint16 CmdCode;
	
	/* TEST1: Found matching message ID and Command Code */
	MsgID = CFE_TBL_CMD_MID;
	CmdCode = CFE_TBL_NOOP_CC;
	UT_Report(CFE_TBL_SearchCmdHndlrTbl(MsgID, CmdCode) == TblIndex,
		"CFE_TBL_SearchCmdHndlrTbl", "Found matching message ID and Command Code");

	/* TEST2: Message is no a command message with specific command code */
	TblIndex = 0;
	MsgID = CFE_TBL_SEND_HK_MID;
	UT_Report(CFE_TBL_SearchCmdHndlrTbl(MsgID, CmdCode) == TblIndex,
		"CFE_TBL_SearchCmdHndlrTbl", "Message is no a command message with specific command code");
	
	/* TEST3: A matching message ID was found, so the command code must be bad */
	TblIndex = CFE_TBL_BAD_CMD_CODE;
	MsgID = CFE_TBL_CMD_MID;
	CmdCode = -1;
	UT_Report(CFE_TBL_SearchCmdHndlrTbl(MsgID, CmdCode) == TblIndex,
		"CFE_TBL_SearchCmdHndlrTbl", "A matching message ID was found, so the command code must be bad");
	
	/* TEST4: No matching message ID was found */
	TblIndex = CFE_TBL_BAD_MSG_ID;
	MsgID = -1;
	UT_Report(CFE_TBL_SearchCmdHndlrTbl(MsgID, CmdCode) == TblIndex,
		"CFE_TBL_SearchCmdHndlrTbl", "No matching message ID was found");
}

void Test_CFE_TBL_InitData(void)
{
	/* This functions has only one possible path with no return code */
	CFE_TBL_InitData();
}

void Test_CFE_TBL_TaskInit(void)
{
    CFE_SB_MsgPtr_t MsgPtr;
    CFE_TBL_NoArgsCmd_t NoArgsCmd;
    
    MsgPtr = (CFE_SB_MsgPtr_t) &NoArgsCmd;
    
    CFE_TBL_TaskMain();

    /*Make TaskInit Fail */
    UT_SetRtnCode(&ES_RegisterRtn, -1, 1);
    CFE_TBL_TaskMain();


	/* With current setup of stub functions, all functions used inside will return CFE_SUCCESS */
	UT_Report(CFE_TBL_TaskInit() == CFE_SUCCESS,
		"CFE_TBL_TaskInit", "Successful Run");

    UT_SetRtnCode (&SB_CreatePipeRtn, -1, 1);
	UT_Report(CFE_TBL_TaskInit() != CFE_SUCCESS,
		"CFE_TBL_TaskInit", "SB_CreatePipe failed");
    
    UT_SetRtnCode (&SB_SubscribeRtn,-1,1);
	UT_Report(CFE_TBL_TaskInit() != CFE_SUCCESS,
	    "CFE_TBL_TaskInit", "SB_Subscribe failed");
    
    UT_SetRtnCode (&SB_SubscribeRtn,-1,2);
	UT_Report(CFE_TBL_TaskInit() != CFE_SUCCESS,
		"CFE_TBL_TaskInit", "SB_Subscribe 2 failed");

    UT_SetRtnCode (&EVS_SendEventRtn, -1,1);
	UT_Report(CFE_TBL_TaskInit() != CFE_SUCCESS,
		"CFE_TBL_TaskInit", "EVS_SendEvent failed");

    UT_SetRtnCode (&EVS_RegisterRtn, -1, 1);
	UT_Report(CFE_TBL_TaskInit() != CFE_SUCCESS,
		"CFE_TBL_TaskInit", "EVS_Register failed");

    /* successful call */
    UT_SetSBTotalMsgLen(sizeof(CFE_TBL_NoArgsCmd_t));
    CFE_SB_SetMsgId(MsgPtr, CFE_TBL_CMD_MID);
    CFE_SB_SetCmdCode(MsgPtr, CFE_TBL_NOOP_CC);

    CFE_TBL_TaskPipe( MsgPtr);


    /* Bad message length */
    UT_SetSBTotalMsgLen(sizeof(CFE_TBL_NoArgsCmd_t )-1);
    CFE_SB_SetMsgId(MsgPtr, CFE_TBL_CMD_MID);
    CFE_SB_SetCmdCode(MsgPtr, CFE_TBL_NOOP_CC);

    CFE_TBL_TaskPipe( MsgPtr);

    /* Bad command code */
    UT_SetSBTotalMsgLen(sizeof(CFE_TBL_NoArgsCmd_t ));
    CFE_SB_SetMsgId(MsgPtr, CFE_TBL_CMD_MID);
    CFE_SB_SetCmdCode(MsgPtr, 999);

    CFE_TBL_TaskPipe( MsgPtr);
}


/*
** Tests for cfe_tbl_task_cmds.c
*/

/* Tests the delete critical tbl's CDS Cmd message */
void Test_CFE_TBL_DeleteCDSCmd(void)
{
	CFE_TBL_DelCDSCmd_t DelCDSCmd;
	int i, j, k;
	
	for(i = 0; i < CFE_TBL_MAX_NUM_TABLES; i++)
		sprintf(CFE_TBL_TaskData.Registry[i].Name, "%d", i);
	
	/* TEST1: Tablename found in Table Registry */
	sprintf(DelCDSCmd.TableName, "%d", 0);
	UT_Report(CFE_TBL_DeleteCDSCmd((CFE_SB_Msg_t *) &DelCDSCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DeleteCDSCmd", "Tablename found in Table Registry");
	
	/* TEST2: Table not found in Critical Table Registry */
	k = CFE_TBL_MAX_CRITICAL_TABLES + CFE_TBL_MAX_NUM_TABLES;
	for(j = CFE_TBL_MAX_NUM_TABLES; j < k; j++)
	{
		sprintf(CFE_TBL_TaskData.CritReg[j - CFE_TBL_MAX_NUM_TABLES].Name, "%d", j);
	}
	sprintf(DelCDSCmd.TableName, "%d", -1);
	UT_Report(CFE_TBL_DeleteCDSCmd((CFE_SB_Msg_t *) &DelCDSCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DeleteCDSCmd", "Table not found in Critical Table Registry");
	
	/* TEST3a: Table is in Critical Table Registry but CDS is not tagged as a table */
	sprintf(DelCDSCmd.TableName, "%d", CFE_TBL_MAX_CRITICAL_TABLES + CFE_TBL_MAX_NUM_TABLES - 1);
	UT_SetRtnCode(&ES_DeleteCDSRtn, CFE_ES_CDS_WRONG_TYPE_ERR, 1);	
	UT_Report(CFE_TBL_DeleteCDSCmd((CFE_SB_Msg_t *) &DelCDSCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DeleteCDSCmd", "Table is in Critical Table Registry but CDS is not tagged as a table");
	
	/* TEST3b: CDS owning app is still active */
	UT_SetRtnCode(&ES_DeleteCDSRtn, CFE_ES_CDS_OWNER_ACTIVE_ERR, 1);
	UT_Report(CFE_TBL_DeleteCDSCmd((CFE_SB_Msg_t *) &DelCDSCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DeleteCDSCmd", "CDS owning app is still active");
	
	/* TEST3c: Unable to locate table in CDS Registry */
	UT_SetRtnCode(&ES_DeleteCDSRtn, CFE_ES_CDS_NOT_FOUND_ERR, 1);
	UT_Report(CFE_TBL_DeleteCDSCmd((CFE_SB_Msg_t *) &DelCDSCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DeleteCDSCmd", "Unable to locate table in CDS Registry");
	
	/* TEST3d: Error while deleting table from CDS */
	UT_SetRtnCode(&ES_DeleteCDSRtn, CFE_SUCCESS - 1, 1);
	UT_Report(CFE_TBL_DeleteCDSCmd((CFE_SB_Msg_t *) &DelCDSCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DeleteCDSCmd", "Error while deleting table from CDS");
	
	/* TEST3e: Successfully removed table from CDS */
	UT_SetRtnCode(&ES_DeleteCDSRtn, CFE_SUCCESS, 1);
	UT_Report(CFE_TBL_DeleteCDSCmd((CFE_SB_Msg_t *) &DelCDSCmd) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_DeleteCDSCmd", "Successfully removed table from CDS");
}


/* Tests the function that processes Housekeeping request message */
void Test_CFE_TBL_HousekeepingCmd(void)
{
	int i;
	CFE_TBL_LoadBuff_t DumpBuff;
	CFE_TBL_LoadBuff_t *DumpBuffPtr = &DumpBuff;
	CFE_TBL_RegistryRec_t RegRecPtr;
	uint8 Buff;
	uint8 *BuffPtr = &Buff;
	char DataSrc[OS_MAX_PATH_LEN];
	uint32 Secs, SubSecs;
	int32 LoadInProg = 0;
	CFE_TBL_DumpControl_t DumpControl = CFE_TBL_TaskData.DumpControlBlocks[0];
	strcpy(CFE_TBL_TaskData.DumpControlBlocks[0].TableName, "housekeepingtest");
	CFE_TBL_TaskData.DumpControlBlocks[0].Size = 10;
	LoadInProg = CFE_TBL_NO_LOAD_IN_PROGRESS + 1;
	RegRecPtr.LoadInProgress = LoadInProg;
	CFE_TBL_TaskData.DumpControlBlocks[0].RegRecPtr = &RegRecPtr;
	DumpBuffPtr->Taken = TRUE;
	DumpBuffPtr->Validated = TRUE;
	DumpBuffPtr->BufferPtr = BuffPtr;
	DumpBuffPtr->FileCreateTimeSecs = Secs;
	DumpBuffPtr->FileCreateTimeSubSecs = SubSecs;
	strcpy(DumpBuffPtr->DataSource, DataSrc);
	CFE_TBL_TaskData.DumpControlBlocks[0].DumpBufferPtr = DumpBuffPtr;
	CFE_TBL_TaskData.DumpControlBlocks[0].State = CFE_TBL_DUMP_PERFORMED;
	
	for(i = 1; i < CFE_TBL_MAX_SIMULTANEOUS_LOADS; i++)
		CFE_TBL_TaskData.DumpControlBlocks[i].State = CFE_TBL_DUMP_PERFORMED + 1;
	
	/* TEST1: Unable to update timestamp in dump file + Unable to send Hk Packet*/
	UT_SetRtnCode(&SBSendMsgRtn, CFE_SUCCESS - 1, 1);
	UT_SetRtnCode(&FSSetTimestampRtn, OS_FS_SUCCESS - 1, 1);
	CFE_TBL_TaskData.HkTlmTblRegIndex = CFE_TBL_NOT_FOUND + 1;
	UT_Report(CFE_TBL_HousekeepingCmd((CFE_SB_Msg_t *) &DumpControl) == CFE_TBL_DONT_INC_CTR,
		"CFE_TBL_HousekeepingCmd", "Able to open dump file");
		
	for(i = 1; i < CFE_TBL_MAX_SIMULTANEOUS_LOADS; i++)
		CFE_TBL_TaskData.DumpControlBlocks[i].State = CFE_TBL_DUMP_PERFORMED + 1;
	RegRecPtr.LoadInProgress = LoadInProg;
	CFE_TBL_TaskData.DumpControlBlocks[0].RegRecPtr = &RegRecPtr;
		
	/* TEST2: Unable to open dump file */
	CFE_TBL_TaskData.DumpControlBlocks[0].State = CFE_TBL_DUMP_PERFORMED;
	CFE_TBL_TaskData.HkTlmTblRegIndex = CFE_TBL_NOT_FOUND + 1;
	UT_SetOSFail(0x0200);
	UT_Report(CFE_TBL_HousekeepingCmd((CFE_SB_Msg_t *) &DumpControl) == CFE_TBL_DONT_INC_CTR,
		"CFE_TBL_HousekeepingCmd", "Unable to open dump file");
	
}

/* Tests the function that processes load table file to buffer command message. */
void Test_CFE_TBL_LoadCmd(void)
{
	CFE_TBL_LoadCmd_t LoadCmd;
	int i, j;
	CFE_TBL_File_Hdr_t TblFileHeader;
	CFE_FS_Header_t StdFileHeader; /* new */
	CFE_TBL_LoadBuff_t BufferPtr = CFE_TBL_TaskData.LoadBuffs[0];
    int32                       EndianCheck = 0x01020304;
		
	/* TEST1: Unable to open file */
	strcpy(LoadCmd.LoadFilename, "LoadFileName");
	UT_SetOSFail(0x0200);
	UT_Report(CFE_TBL_LoadCmd((CFE_SB_Msg_t *) &LoadCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_LoadCmd", "Unable to open file.");
		
	/* TEST2: Cannot find table in registry */
	UT_SetOSFail(0x0000);
	for (i = 0; i < CFE_TBL_MAX_NUM_TABLES; i++)
    {
		CFE_TBL_TaskData.Registry[i].OwnerAppId = CFE_TBL_NOT_OWNED;
        CFE_TBL_TaskData.Registry[i].LoadPending = FALSE;
    }
	strcpy(TblFileHeader.TableName, CFE_TBL_TaskData.Registry[0].Name);
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
	UT_Report(CFE_TBL_LoadCmd((CFE_SB_Msg_t *) &LoadCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_LoadCmd", "table registry entry doesn't exist");

	/* TEST3: attempting to load a dump only table */
    CFE_TBL_TaskData.Registry[0].OwnerAppId = CFE_TBL_NOT_OWNED + 1;
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
	CFE_TBL_TaskData.Registry[0].Size = sizeof(CFE_TBL_File_Hdr_t) + 1;
	CFE_TBL_TaskData.Registry[0].DumpOnly = TRUE;
	UT_Report(CFE_TBL_LoadCmd((CFE_SB_Msg_t *) &LoadCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_LoadCmd", "attempting to load a dump only table");

	/* TEST3a: attempting to load a table with a load already pending */
    CFE_TBL_TaskData.Registry[0].OwnerAppId = CFE_TBL_NOT_OWNED + 1;
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
	CFE_TBL_TaskData.Registry[0].Size = sizeof(CFE_TBL_File_Hdr_t) + 1;
	CFE_TBL_TaskData.Registry[0].DumpOnly = FALSE;
	CFE_TBL_TaskData.Registry[0].LoadPending = TRUE;
	UT_Report(CFE_TBL_LoadCmd((CFE_SB_Msg_t *) &LoadCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_LoadCmd", "attempting to load a table with load already pending");
	CFE_TBL_TaskData.Registry[0].LoadPending = FALSE;

	/* TEST4: Isn't dump only and passes table checks, get a working buffer, is an extra byte --> more data than header indicates */

        TblFileHeader.Offset = 0;
	CFE_TBL_TaskData.Registry[0].TableLoadedOnce = TRUE;
	TblFileHeader.NumBytes = sizeof(CFE_TBL_File_Hdr_t);

    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
    }

	CFE_TBL_TaskData.Registry[0].Size = sizeof(CFE_TBL_File_Hdr_t);
	CFE_TBL_TaskData.Registry[0].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS + 1;
	CFE_TBL_TaskData.Registry[0].DblBuffered = FALSE;
	CFE_TBL_TaskData.LoadBuffs[CFE_TBL_TaskData.Registry[0].LoadInProgress] = BufferPtr;
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
	CFE_TBL_TaskData.Registry[0].DumpOnly = FALSE;
	UT_Report(CFE_TBL_LoadCmd((CFE_SB_Msg_t *) &LoadCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_LoadCmd", "There is more data than the file indicates");

        /* TEST4b: no extra byte --> successful load */

	TblFileHeader.NumBytes = sizeof(CFE_TBL_File_Hdr_t);

    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
    }

    UT_SetRtnCode(&OSReadRtn, 0, 3);
	strcpy(TblFileHeader.TableName, CFE_TBL_TaskData.Registry[0].Name);
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
	UT_Report(CFE_TBL_LoadCmd((CFE_SB_Msg_t *) &LoadCmd) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_LoadCmd", "successful load");

	/* Test5: differing amount of data from header's claim */
	TblFileHeader.NumBytes = sizeof(CFE_TBL_File_Hdr_t);

    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
    }
	strcpy(TblFileHeader.TableName, CFE_TBL_TaskData.Registry[0].Name);
	UT_SetRtnCode(&OSReadRtn, 0, 2);
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
	UT_Report(CFE_TBL_LoadCmd((CFE_SB_Msg_t *) &LoadCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_LoadCmd", "incomplete load of file into the working buffer");
		
   
	/* TEST6: No working buffers available */

	CFE_TBL_TaskData.Registry[0].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS;
	CFE_TBL_TaskData.Registry[0].TableLoadedOnce = TRUE;
	CFE_TBL_TaskData.Registry[0].DblBuffered = FALSE;
	CFE_TBL_TaskData.Registry[0].Buffers[CFE_TBL_TaskData.Registry[0].ActiveBufferIndex] = BufferPtr;
	for(j = 0; j < CFE_TBL_MAX_SIMULTANEOUS_LOADS; j++)
		CFE_TBL_TaskData.LoadBuffs[j].Taken = TRUE;
	strcpy(TblFileHeader.TableName, CFE_TBL_TaskData.Registry[0].Name);
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
	UT_Report(CFE_TBL_LoadCmd((CFE_SB_Msg_t *) &LoadCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_LoadCmd", "No working buffers available");
	/* TEST7a: Tbl Hdr indicates data beyond size of the table */

	TblFileHeader.NumBytes = sizeof(CFE_TBL_File_Hdr_t);

    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
    }
	CFE_TBL_TaskData.Registry[0].Size = sizeof(CFE_TBL_File_Hdr_t) - 1;
	strcpy(TblFileHeader.TableName, CFE_TBL_TaskData.Registry[0].Name);
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
	UT_Report(CFE_TBL_LoadCmd((CFE_SB_Msg_t *) &LoadCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_LoadCmd", "Tbl Hdr indicates data beyond size of the table");

	/* TEST7b: Tbl Hdr indicates no data in file */

	TblFileHeader.NumBytes = 0;

    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
    }

	strcpy(TblFileHeader.TableName, CFE_TBL_TaskData.Registry[0].Name);
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
	UT_Report(CFE_TBL_LoadCmd((CFE_SB_Msg_t *) &LoadCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_LoadCmd", "Tbl Hdr indicates no data in file");

	/* TEST7c: File has partial load for uninitialized table */
	
TblFileHeader.NumBytes = sizeof(CFE_TBL_File_Hdr_t);
	
        if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
    }

    
    CFE_TBL_TaskData.Registry[0].Size = sizeof(CFE_TBL_File_Hdr_t) + 1;
	TblFileHeader.Offset = 1;
	strcpy(TblFileHeader.TableName, CFE_TBL_TaskData.Registry[0].Name);
	CFE_TBL_TaskData.Registry[0].TableLoadedOnce = FALSE;
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
	UT_Report(CFE_TBL_LoadCmd((CFE_SB_Msg_t *) &LoadCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_LoadCmd", "File has partial load for uninitialized table");


    /* Test 7d: a partial load has taken place */

	TblFileHeader.NumBytes = 1;
    TblFileHeader.Offset = 1;

    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);

    }
	CFE_TBL_TaskData.Registry[0].TableLoadedOnce = FALSE;

	CFE_TBL_TaskData.Registry[0].Size = sizeof(CFE_TBL_File_Hdr_t);
	strcpy(TblFileHeader.TableName, CFE_TBL_TaskData.Registry[0].Name);
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
	UT_Report(CFE_TBL_LoadCmd((CFE_SB_Msg_t *) &LoadCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_LoadCmd", "Partial load");


}


/* Tests the function that processes dump table to file command message */
void Test_CFE_TBL_DumpCmd(void)
{

	CFE_TBL_DumpCmd_t DumpCmd;
	int i, k, u;
	uint8 Buff;
	uint8 *BuffPtr = &Buff;
	CFE_TBL_RegistryRec_t Desired[CFE_TBL_MAX_NUM_TABLES];
	CFE_TBL_LoadBuff_t Load;
	
	/* TEST1: can't find table in registry */
	for(i = 0; i < CFE_TBL_MAX_NUM_TABLES; i++)
		sprintf(Desired[i].Name, "%d", i);
	UT_SwapRegistryforControl(CFE_TBL_TaskData.Registry, Desired);
	sprintf(DumpCmd.TableName, "%d", CFE_TBL_MAX_NUM_TABLES + 1);

	UT_Report(CFE_TBL_DumpCmd((CFE_SB_Msg_t *) &DumpCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DumpCmd", "table registry entry doesn't exist");
	UT_ReturnOriginValues(CFE_TBL_TaskData.Registry);

	/* TEST2a: active buffer, ptr created, validation passes, is a dump only table, no dump already in progress, 
	**         got working buffer: load in progress, single-buffered,
	*/
	strcpy(CFE_TBL_TaskData.Registry[2].Name, "DumpCmdTest");
	
	strcpy(DumpCmd.TableName, CFE_TBL_TaskData.Registry[2].Name);
	
	/*for(i = 0; i < CFE_TBL_MAX_NUM_TABLES; i++)
		printf("%d: Name$$%s$$\n OwnerAppId %d Size %d \n", i, CFE_TBL_TaskData.Registry[i].Name, (int)CFE_TBL_TaskData.Registry[i].OwnerAppId, (int)CFE_TBL_TaskData.Registry[i].Size);*/
	
	DumpCmd.ActiveTblFlag = CFE_TBL_ACTIVE_BUFFER;
	CFE_TBL_TaskData.Registry[2].Buffers[CFE_TBL_TaskData.Registry[2].ActiveBufferIndex].BufferPtr = BuffPtr;
	
	for (i = 0; i< CFE_TBL_MAX_NUM_TABLES; i++)
	{
	
		CFE_TBL_TaskData.Registry[i].DumpOnly = TRUE;
	}
	CFE_TBL_TaskData.DumpControlBlocks[2].State = CFE_TBL_DUMP_FREE + 1;
	CFE_TBL_TaskData.DumpControlBlocks[3].State = CFE_TBL_DUMP_FREE;
	CFE_TBL_TaskData.Registry[2].DumpControlIndex = CFE_TBL_NO_DUMP_PENDING;
	CFE_TBL_TaskData.Registry[2].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS + 1;
	CFE_TBL_TaskData.Registry[2].DblBuffered = FALSE;
	CFE_TBL_TaskData.LoadBuffs[CFE_TBL_TaskData.Registry[2].LoadInProgress] = Load;
	UT_Report(CFE_TBL_DumpCmd((CFE_SB_Msg_t *) &DumpCmd) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_DumpCmd", "active buffer, ptr created, active buffer, ptr created, validation passes, is a dump only table, no dump already in progress, got working buffer");
	
	/* TEST2b: active buffer, ptr created, is a dump only tbl, no dump already in progress,
	**         fails to get a working buffer: no load in progress, TableLoadedOnce = TRUE, single-buffered, no buffer available.
	*/
	CFE_TBL_TaskData.DumpControlBlocks[2].State = CFE_TBL_DUMP_FREE;
	CFE_TBL_TaskData.Registry[2].DumpControlIndex = CFE_TBL_NO_DUMP_PENDING;
	CFE_TBL_TaskData.Registry[2].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS;
	CFE_TBL_TaskData.Registry[2].TableLoadedOnce = TRUE;
	CFE_TBL_TaskData.Registry[2].DblBuffered = FALSE;
	for(u = 0; u < CFE_TBL_MAX_SIMULTANEOUS_LOADS; u++)
		CFE_TBL_TaskData.LoadBuffs[u].Taken = TRUE;
		
	UT_Report(CFE_TBL_DumpCmd((CFE_SB_Msg_t *) &DumpCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DumpCmd", "active buffer, ptr created, is a dump only tbl, no dump already in progress, fails to get a working buffer: No working buffers available");
		
	/* TEST2c: active buffer, ptr created, is dump only tbl, no dump  fails to find a free dump control block: too many dump only tbl dumps have been requested */
	CFE_TBL_TaskData.Registry[2].DumpControlIndex = CFE_TBL_NO_DUMP_PENDING;
	for(k = 0; k < CFE_TBL_MAX_SIMULTANEOUS_LOADS; k++)
		CFE_TBL_TaskData.DumpControlBlocks[k].State = CFE_TBL_DUMP_FREE + 1;
	UT_Report(CFE_TBL_DumpCmd((CFE_SB_Msg_t *) &DumpCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DumpCmd", "active buffer, ptr created, is dump only tbl, fails to find a free dump control block: too many dump only tbl dumps have been requested");
	
	/* TEST3a: inactive buffer, double-buffered, dump already in progress: dump is already pending */
	DumpCmd.ActiveTblFlag = CFE_TBL_INACTIVE_BUFFER;
	CFE_TBL_TaskData.Registry[2].DblBuffered = TRUE;
	CFE_TBL_TaskData.Registry[2].Buffers[(1U-CFE_TBL_TaskData.Registry[2].ActiveBufferIndex)].BufferPtr = BuffPtr;
	CFE_TBL_TaskData.Registry[2].DumpControlIndex = CFE_TBL_NO_DUMP_PENDING + 1;
	UT_Report(CFE_TBL_DumpCmd((CFE_SB_Msg_t *) &DumpCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DumpCmd", "inactive buffer, double-buffered, dump already in progress: dump is already pending");
	
	/* TEST3b: inactive buffer, single-buffered, pointer created, is a dump only table */
	CFE_TBL_TaskData.Registry[2].DblBuffered = FALSE;
	CFE_TBL_TaskData.Registry[2].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS + 1;
	CFE_TBL_TaskData.LoadBuffs[CFE_TBL_TaskData.Registry[2].LoadInProgress].BufferPtr = BuffPtr;
	CFE_TBL_TaskData.Registry[2].DumpOnly = FALSE;
	strcpy(DumpCmd.DumpFilename, CFE_TBL_TaskData.Registry[2].LastFileLoaded);
	UT_Report(CFE_TBL_DumpCmd((CFE_SB_Msg_t *) &DumpCmd) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_DumpCmd", "inactive buffer, single-buffered, pointer created, is a dump only table");
	
	/* TEST4a: inactive buffer, single-buffered: No inactive buffer for table */
	CFE_TBL_TaskData.Registry[2].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS;
	UT_Report(CFE_TBL_DumpCmd((CFE_SB_Msg_t *) &DumpCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DumpCmd", "inactive buffer, single-buffered: No inactive buffer for table");
		
	/* TEST4b: illegal buffer parameter */
	DumpCmd.ActiveTblFlag = CFE_TBL_ACTIVE_BUFFER + 1;
	UT_Report(CFE_TBL_DumpCmd((CFE_SB_Msg_t *) &DumpCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DumpCmd", "illegal buffer parameter");
		
	
}

/* Tests the function that processes dump table registration to file Command message */
void Test_CFE_TBL_DumpRegCmd(void)
{

	CFE_TBL_DumpRegCmd_t DumpRegCmd;
	int q;
	for(q = 0; q < CFE_TBL_MAX_NUM_TABLES; q++)
		CFE_TBL_TaskData.Registry[q].HeadOfAccessList = CFE_TBL_END_OF_LIST;
	
	/* TEST1: error creating dump file*/
	DumpRegCmd.DumpFilename[0] = '\0';
	UT_SetOSFail(OS_CREAT_FAIL);
	UT_Report(CFE_TBL_DumpRegCmd((CFE_SB_Msg_t *) &DumpRegCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DumpRegCmd", "error creating dump file");
	
	/* TEST2: error writing cFE File header */
	UT_SetOSFail(0x0000);
	UT_SetRtnCode(&FSWriteHdrRtn, sizeof(CFE_FS_Header_t) - 1, 1);
	UT_Report(CFE_TBL_DumpRegCmd((CFE_SB_Msg_t *) &DumpRegCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DumpRegCmd", "error writing cFE File header");
	
	/* TEST3a: Table is owned, file didn't exist previously: successfully dumped table */
	UT_SetRtnCode(&FSWriteHdrRtn, sizeof(CFE_FS_Header_t), 10);
	CFE_TBL_TaskData.Registry[0].OwnerAppId = CFE_TBL_NOT_OWNED + 1;
	CFE_TBL_TaskData.Registry[0].HeadOfAccessList = CFE_TBL_END_OF_LIST;
	CFE_TBL_TaskData.Registry[1].OwnerAppId = CFE_TBL_NOT_OWNED;
	CFE_TBL_TaskData.Registry[0].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS + 1;
	CFE_TBL_TaskData.Registry[0].DblBuffered = TRUE;
	UT_SetOSFail(OS_OPEN_FAIL);
	UT_Report(CFE_TBL_DumpRegCmd((CFE_SB_Msg_t *) &DumpRegCmd) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_DumpRegCmd", "Table is owned, file didn't exist previously: successfully dumped table");


	/* TEST3b: File did exist previously: successfully overwritten table */
	UT_SetOSFail(0x0000);
	UT_Report(CFE_TBL_DumpRegCmd((CFE_SB_Msg_t *) &DumpRegCmd) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_DumpRegCmd", "File did exist previously: successfully overwritten table ");
	
	/* TEST3b: Table is not owned, OS_write fails: Error writing Registry */
	UT_SetRtnCode(&FileWriteRtn, sizeof(CFE_TBL_RegDumpRec_t) - 1, 1); 
	CFE_TBL_TaskData.Registry[0].OwnerAppId = CFE_TBL_NOT_OWNED;
	CFE_TBL_TaskData.Registry[0].HeadOfAccessList = 2;
	CFE_TBL_TaskData.Handles[2].NextLink = CFE_TBL_END_OF_LIST;
	UT_Report(CFE_TBL_DumpRegCmd((CFE_SB_Msg_t *) &DumpRegCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DumpRegCmd", "Table is not owned, OS_write fails: Error writing Registry");

}


/* Tests the function that collects data and stores it in the Housekeeping message. */
void Test_CFE_TBL_GetHkData(void)
{
	
	int32 NumLoadPendingIndex = CFE_TBL_MAX_NUM_TABLES - 1;
	int32 FreeSharedBuffIndex = CFE_TBL_MAX_SIMULTANEOUS_LOADS - 1;
	int32 ValTableIndex = CFE_TBL_MAX_NUM_VALIDATIONS - 1;
	
	/* TEST1: raise the count of load pending tables */
	CFE_TBL_TaskData.Registry[NumLoadPendingIndex].LoadPending = TRUE;
	CFE_TBL_GetHkData();
	
	/* TEST2: lower the count of free shared buffers. */
	CFE_TBL_TaskData.LoadBuffs[FreeSharedBuffIndex].Taken = TRUE;
	CFE_TBL_GetHkData();
	
	/* TEST3: make a ValPtr with Result = CFE_SUCCESS */
	CFE_TBL_TaskData.ValidationResults[ValTableIndex].State = CFE_TBL_VALIDATION_PERFORMED;
	CFE_TBL_TaskData.ValidationResults[ValTableIndex].Result = CFE_SUCCESS;
	CFE_TBL_GetHkData();
	
	/* TEST4: make a ValPtr without Result = CFE_SUCCESS */
	CFE_TBL_TaskData.ValidationResults[ValTableIndex].State = CFE_TBL_VALIDATION_PERFORMED;
	CFE_TBL_TaskData.ValidationResults[ValTableIndex].Result = CFE_SUCCESS - 1;
	CFE_TBL_GetHkData();
	
}

/* Tests the function which converts table registry entries for tables into messages. */
void Test_CFE_TBL_GetTblRegData(void)
{
	
	/* TEST1: for a double buffered table */
	CFE_TBL_TaskData.Registry[CFE_TBL_TaskData.HkTlmTblRegIndex].DblBuffered = TRUE;
	CFE_TBL_GetTblRegData();
		
	/* TEST2: single buffered and is an inactive buffer */
	CFE_TBL_TaskData.Registry[CFE_TBL_TaskData.HkTlmTblRegIndex].DblBuffered = FALSE;
	CFE_TBL_TaskData.Registry[CFE_TBL_TaskData.HkTlmTblRegIndex].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS + 1;
	CFE_TBL_GetTblRegData();
	
	/* TEST3: no inactive buffer */
	CFE_TBL_TaskData.Registry[CFE_TBL_TaskData.HkTlmTblRegIndex].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS;
	CFE_TBL_GetTblRegData();
	
}

/* Tests the processing reset counters Command Message function. */
void Test_CFE_TBL_ResetCmd(void)
{
	CFE_SB_Msg_t MessagePtr;
	
	/* TEST1: Run function -> No additional paths. */
	UT_Report(CFE_TBL_ResetCmd(&MessagePtr) == CFE_TBL_DONT_INC_CTR,
				"CFE_TBL_ResetCmd", "function run and completed.");
}

/* Tests the processing NO-Operation Command Message function. */
void Test_CFE_TBL_NoopCmd(void)
{
	CFE_SB_Msg_t MessagePtr;
	
	/* TEST1: Run function -> No additional paths. */
	UT_Report(CFE_TBL_NoopCmd(&MessagePtr) == CFE_TBL_INC_CMD_CTR,
				"CFE_TBL_NoopCmd", "function run and completed.");
}

/* Test the validate table command message function. */
void Test_CFE_TBL_ValidateCmd(void)
{
	int i, k;
	CFE_TBL_RegistryRec_t Desired[CFE_TBL_MAX_NUM_TABLES];
	CFE_TBL_ValidateCmd_t ValidateCmd;
	uint8 Buff;
	uint8 *BuffPtr = &Buff;
	CFE_TBL_CallbackFuncPtr_t ValFuncPtr = (CFE_TBL_CallbackFuncPtr_t)((unsigned long )&UT_SwapRegistryforControl);

	/* TEST1: Table name not found in registry. */
	for(i = 0; i < CFE_TBL_MAX_NUM_TABLES; i++)
		sprintf(Desired[i].Name, "%d", i);
	UT_SwapRegistryforControl(CFE_TBL_TaskData.Registry, Desired);
	sprintf(ValidateCmd.TableName, "%d", CFE_TBL_MAX_NUM_TABLES + 1);
	UT_Report(CFE_TBL_ValidateCmd((CFE_SB_Msg_t *) &ValidateCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_ValidateCmd", "table registry entry doesn't exist");
	UT_ReturnOriginValues(CFE_TBL_TaskData.Registry);
	
	/* TEST2a: active buffer with data: too many table validations have been requested */
	strcpy(ValidateCmd.TableName, CFE_TBL_TaskData.Registry[0].Name);
	ValidateCmd.ActiveTblFlag = CFE_TBL_ACTIVE_BUFFER;
	CFE_TBL_TaskData.Registry[0].Buffers[CFE_TBL_TaskData.Registry[0].ActiveBufferIndex].BufferPtr = BuffPtr;
	for(k = 0; k < CFE_TBL_MAX_NUM_VALIDATIONS; k++)
		CFE_TBL_TaskData.ValidationResults[k].State = CFE_TBL_VALIDATION_FREE + 1;
	UT_Report(CFE_TBL_ValidateCmd((CFE_SB_Msg_t *) &ValidateCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_ValidateCmd", "active buffer with data: too many table validations have been requested");
		
	/* TEST2b: active buffer with data: No validation function ptr */
	CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_FREE;
	CFE_TBL_TaskData.Registry[0].ValidationFuncPtr = NULL;
	UT_Report(CFE_TBL_ValidateCmd((CFE_SB_Msg_t *) &ValidateCmd) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_ValidateCmd", "active buffer with data: No validation function ptr");
		
	/* TEST2c: active buffer with data: validation function ptr and active tbl flag */
	CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_FREE;
	CFE_TBL_TaskData.Registry[0].ValidationFuncPtr = ValFuncPtr;
	ValidateCmd.ActiveTblFlag = TRUE;
	UT_Report(CFE_TBL_ValidateCmd((CFE_SB_Msg_t *) &ValidateCmd) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_ValidateCmd", "active buffer with data: validation function ptr and active tbl flag");	
			
	/* TEST3a: inactive buffer: double-buffered table : validation function ptr*/
	ValidateCmd.ActiveTblFlag = CFE_TBL_INACTIVE_BUFFER;
	CFE_TBL_TaskData.Registry[0].DblBuffered = TRUE;
	CFE_TBL_TaskData.Registry[0].Buffers[(1U-CFE_TBL_TaskData.Registry[0].ActiveBufferIndex)].BufferPtr = BuffPtr;
	CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_FREE;
	CFE_TBL_TaskData.Registry[0].ValidationFuncPtr = ValFuncPtr;
	UT_Report(CFE_TBL_ValidateCmd((CFE_SB_Msg_t *) &ValidateCmd) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_ValidateCmd", "inactive buffer: double buffered table : validation function ptr");
				
	/* TEST3b: inactive buffer: single-buffered table with load in progress: validation function ptr */
	CFE_TBL_TaskData.Registry[0].DblBuffered = FALSE;
	CFE_TBL_TaskData.LoadBuffs[CFE_TBL_TaskData.Registry[0].LoadInProgress].BufferPtr = BuffPtr;
	CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_FREE;
	CFE_TBL_TaskData.Registry[0].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS + 1;

	UT_Report(CFE_TBL_ValidateCmd((CFE_SB_Msg_t *) &ValidateCmd) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_ValidateCmd", "inactive buffer: single buffered table with load in progress: validation function ptr");

	/* TEST3c: no inactive buffer present (single-buffered table without load in progress) */
	CFE_TBL_TaskData.Registry[0].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS;
	UT_Report(CFE_TBL_ValidateCmd((CFE_SB_Msg_t *) &ValidateCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_ValidateCmd", "inactive buffer: single buffered table with load in progress");
	
	/* TEST4: illegal buffer */
	ValidateCmd.ActiveTblFlag = -1;
	UT_Report(CFE_TBL_ValidateCmd((CFE_SB_Msg_t *) &ValidateCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_ValidateCmd", "illegal buffer");
}


/* Test the Write table data to a file function. */
void Test_CFE_TBL_DumpToFile(void)
{
	uint32 TblSizeInBytes = 9;
	uint32 oldFail = UT_OS_Fail;
	uint32 newFail = 0x0001;

	/* TEST1: Error creating dump file. */
	UT_SetOSFail(newFail);
	UT_Report(CFE_TBL_DumpToFile("filename" ,"tablename" ,"dumpaddress", TblSizeInBytes) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DumpToFile", "error creating dump file");
	
	/* TEST2: Error writing CFE file header. */
	UT_SetOSFail(0x0000);
	UT_SetRtnCode(&FSWriteHdrRtn, sizeof(CFE_FS_Header_t) - 1, 1);
	UT_Report(CFE_TBL_DumpToFile("filename" ,"tablename" ,"dumpaddress", TblSizeInBytes) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DumpToFile", "error writing CFE file header");
		
	/* TEST3: Error writing table file header. */
	/* Setting first return code variable to a large enough count to pass through everytime. */
	UT_SetRtnCode(&FSWriteHdrRtn, sizeof(CFE_FS_Header_t), 6);
	UT_SetRtnCode(&FileWriteRtn, sizeof(CFE_TBL_File_Hdr_t) - 1, 1);
	UT_Report(CFE_TBL_DumpToFile("filename" ,"tablename" ,"dumpaddress", TblSizeInBytes) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DumpToFile", "error writing CFE file header");
			
	/* TEST4: Error writing table to file. */
	UT_SetRtnCode(&FileWriteRtn, TblSizeInBytes - 1, 2); 
	UT_Report(CFE_TBL_DumpToFile("filename" ,"tablename" ,"dumpaddress", TblSizeInBytes) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_DumpToFile", "error writing CFE file header");
	
	/* TEST5: File created and dumped data.*/
	UT_SetRtnCode(&FileWriteRtn, 0, 0);
	UT_SetOSFail(0x0200);
	UT_Report(CFE_TBL_DumpToFile("filename" ,"tablename" ,"dumpaddress", TblSizeInBytes) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_DumpToFile", "file created and dumped data");
	
	/* TEST6: File existed previously => data overwritten. */
	UT_SetOSFail(0x0000);
	UT_Report(CFE_TBL_DumpToFile("filename" ,"tablename" ,"dumpaddress", TblSizeInBytes) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_DumpToFile", " File existed previously => data overwritten.");
	UT_SetOSFail(oldFail);
	
}

/* Test the Activate Table Command Message function */
void Test_CFE_TBL_ActivateCmd(void)
{

	/*const CFE_SB_Msg_t MessagePtr; ERROR and DELETING: this type not used as the real variable but the variable type-casted to it when passed into function */
	int load = (int)CFE_TBL_TaskData.Registry[0].LoadInProgress;
	uint8 dump = CFE_TBL_TaskData.Registry[0].DumpOnly;
	int i;
	CFE_TBL_RegistryRec_t Desired[CFE_TBL_MAX_NUM_TABLES]; 
	CFE_TBL_ActivateCmd_t ActivateCmd;
	
	/* Entering the if statement with table name that has to be in the registry. */
	strcpy(ActivateCmd.TableName, CFE_TBL_TaskData.Registry[0].Name);
	
	/* TEST1: Table name exists, attempting to activate a dump-only table. Should fail. */
	CFE_TBL_TaskData.Registry[0].DumpOnly = TRUE;
	UT_Report(CFE_TBL_ActivateCmd((CFE_SB_Msg_t *) &ActivateCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_ActivateCmd", "table registry exists but dump-only table attempted to load");
			
	/* TEST2a: Tbl name exists, not a dump-only, and a load in progress: Table is double-buffered. */
	CFE_TBL_TaskData.Registry[0].DumpOnly = FALSE;
	CFE_TBL_TaskData.Registry[0].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS + 1;
	CFE_TBL_TaskData.Registry[0].DblBuffered = TRUE;
	UT_Report(CFE_TBL_ActivateCmd((CFE_SB_Msg_t *) &ActivateCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_ActivateCmd", "table registry exists, not a dump-only tbl, and a load in progress: Table is double-buffered");
	
	/* TEST2b: Tbl name exists, not a dump-only, and a load in progress: Table isn't double-buffered and ValidationStatus = TRUE  */
	CFE_TBL_TaskData.Registry[0].DblBuffered = FALSE;
	CFE_TBL_TaskData.LoadBuffs[CFE_TBL_TaskData.Registry[0].LoadInProgress].Validated = TRUE;
	UT_Report(CFE_TBL_ActivateCmd((CFE_SB_Msg_t *) &ActivateCmd) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_ActivateCmd", "table registry exists, not a dump-only tbl, and a load in progress: Table isn't double-buffered");
	
	/* TEST3: Tbl name exists, not a dump-only, but no load in progress. */
	CFE_TBL_TaskData.Registry[0].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS;
	UT_Report(CFE_TBL_ActivateCmd((CFE_SB_Msg_t *) &ActivateCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_ActivateCmd", "table registry exists, not a dump-only tbl, no load in progress");
		
	/* TEST4: Tbl name doesn't exist. */
	for(i = 0; i < CFE_TBL_MAX_NUM_TABLES; i++)
		sprintf(Desired[i].Name, "%d", i);
		 
	UT_SwapRegistryforControl(CFE_TBL_TaskData.Registry, Desired);
	sprintf(ActivateCmd.TableName, "%d", CFE_TBL_MAX_NUM_TABLES + 1);
	UT_Report(CFE_TBL_ActivateCmd((CFE_SB_Msg_t *) &ActivateCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_ActivateCmd", "table registry entry doesn't exist");
	
	/* Returning original values. */
	UT_ReturnOriginValues(CFE_TBL_TaskData.Registry);
	CFE_TBL_TaskData.Registry[0].LoadInProgress = load;
	CFE_TBL_TaskData.Registry[0].DumpOnly = dump;
	
}


/* Tests the Processing Abort Load Command Message function */
void Test_CFE_TBL_AbortLoadCmd(void)
{

	CFE_TBL_AbortLdCmd_t AbortLdCmd;
	int load = (int)CFE_TBL_TaskData.Registry[0].LoadInProgress;
	int i;
	CFE_TBL_RegistryRec_t Desired[CFE_TBL_MAX_NUM_TABLES];	
	
	/* Entering the if statement with table name that has to be in the registry. */
	strcpy(AbortLdCmd.TableName, CFE_TBL_TaskData.Registry[0].Name);
	
	/* TEST1: Table name does exist and a table load in progress.*/
	CFE_TBL_TaskData.Registry[0].LoadInProgress = 1;
	UT_Report(CFE_TBL_AbortLoadCmd((CFE_SB_Msg_t *) &AbortLdCmd) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_AbortLoadCmd", "table registry entry exists & load in progress");
		
	/* TEST2: Table name does exist but no table load in progress. */
	CFE_TBL_TaskData.Registry[0].LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS;
	UT_Report(CFE_TBL_AbortLoadCmd((CFE_SB_Msg_t *) &AbortLdCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_AbortLoadCmd", "table registry entry exists but no load in progress");
	
	/* TEST3: Table name not found in registry. */
	CFE_TBL_TaskData.Registry[0].LoadInProgress = load;
	for(i = 0; i < CFE_TBL_MAX_NUM_TABLES; i++)
		sprintf(Desired[i].Name, "%d", i);
	UT_SwapRegistryforControl(CFE_TBL_TaskData.Registry, Desired);	sprintf(AbortLdCmd.TableName, "%d", CFE_TBL_MAX_NUM_TABLES + 1);
	UT_Report(CFE_TBL_AbortLoadCmd((CFE_SB_Msg_t *) &AbortLdCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_AbortLoadCmd", "table registry entry doesn't exist");
		
	/* Returning original values. */
	UT_ReturnOriginValues(CFE_TBL_TaskData.Registry);
	
	
}


/* Test the Proccessing Telemeter Table Registry Command Message function */
void Test_CFE_TBL_TlmRegCmd(void)
{

	int i;
	CFE_TBL_RegistryRec_t Desired[CFE_TBL_MAX_NUM_TABLES];
	CFE_TBL_TlmRegCmd_t TlmRegCmd;
	
	/* OrigName used because it is confirmed to be a registered table name. */
	strcpy(TlmRegCmd.TableName, CFE_TBL_TaskData.Registry[0].Name);
	
	/* TEST: Table name does exist */
	UT_Report(CFE_TBL_TlmRegCmd((CFE_SB_Msg_t *) &TlmRegCmd) == CFE_TBL_INC_CMD_CTR,
		"CFE_TBL_TlmRegCmd", "table registry entry for telemetry does exist"); 
				
	/*
	** Will use swap function to make the whole tbl registry filled with known tbl names
	**	in order to test when a table name is not registered.
	*/
	
	for(i = 0; i < CFE_TBL_MAX_NUM_TABLES; i++)
		sprintf(Desired[i].Name, "%d", i);
	UT_SwapRegistryforControl(CFE_TBL_TaskData.Registry, Desired);
	sprintf(TlmRegCmd.TableName, "%d", CFE_TBL_MAX_NUM_TABLES + 1);
	
	/* TEST: Table name doesn't exist */
	UT_Report(CFE_TBL_TlmRegCmd((CFE_SB_Msg_t *) &TlmRegCmd) == CFE_TBL_INC_ERR_CTR,
		"CFE_TBL_TlmRegCmd", "table registry entry for telemetry doesn't exist");
		
	/* Returning original values. */
	UT_ReturnOriginValues(CFE_TBL_TaskData.Registry);
	
}

/* Prepare to test Table API functions */
void Test_CFE_TBL_ApiInit(void)
{
    UT_SetAppID(1);
    UT_ResetCDS();
    CFE_TBL_EarlyInit();
    CFE_TBL_TaskData.TableTaskAppId = 10;
}

/* Test Cases 1000-1xxx - CFE_TBL_Register */
void Test_CFE_TBL_Register(void)
{
    int32            RtnCode;
    CFE_TBL_Handle_t TblHandle1;
    CFE_TBL_Handle_t TblHandle2;
    CFE_TBL_Handle_t TblHandle3;
    boolean          EventsCorrect;
    char             TblName[15];
    int16            i;
    
    /* Test Case 1001 - CFE_TBL_ValidateAppID returns CFE_ES_ERR_APPID */
    UT_SetRtnCode(&GetAppIDRtn, CFE_ES_ERR_APPID, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", sizeof(UT_Table1_t), CFE_TBL_OPT_DEFAULT, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_ES_ERR_APPID) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1001 - CFE_TBL_ValidateAppID returns CFE_ES_ERR_APPID");
    
    /* Test Case 1002 - CFE_TBL_ValidateAppID returns CFE_TBL_ERR_BAD_APP_ID */
    UT_SetAppID(CFE_ES_MAX_APPLICATIONS);
    UT_SetRtnCode(&GetAppIDRtn, CFE_SUCCESS, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", sizeof(UT_Table1_t), CFE_TBL_OPT_DEFAULT, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_BAD_APP_ID) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1002 - CFE_TBL_ValidateAppID returns CFE_TBL_ERR_BAD_APP_ID");
    
    /* Test Case 1003 - Table Name Too Long */ 
    UT_SetRtnCode(&GetAppIDRtn, CFE_SUCCESS, 0);
    UT_SetAppID(1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_TblNameTooLong", sizeof(UT_Table1_t), CFE_TBL_OPT_DEFAULT, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_NAME) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1003 - Table Name Too Long");
    
    /* Test Case 1004 - Table Name Too Short */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "", sizeof(UT_Table1_t), CFE_TBL_OPT_DEFAULT, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_NAME) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1004 - Table Name Too Short");
    
    /* Test Case 1005 - Size of Table = 0 */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", 0, CFE_TBL_OPT_DEFAULT, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_SIZE) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1005 - Size of Table = 0");
    
    /* Test Case 1006 - Size of Table > CFE_TBL_MAX_SNGL_TABLE_SIZE */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", (CFE_TBL_MAX_SNGL_TABLE_SIZE+1), CFE_TBL_OPT_DEFAULT, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_SIZE) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1006 - Size of Table > CFE_TBL_MAX_SNGL_TABLE_SIZE");
    
    /* Test Case 1007 - Size of Table > CFE_TBL_MAX_DBL_TABLE_SIZE */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", (CFE_TBL_MAX_DBL_TABLE_SIZE+1), CFE_TBL_OPT_DBL_BUFFER, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_SIZE) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1007 - Size of Table > CFE_TBL_MAX_DBL_TABLE_SIZE");
    
    /* Test Case 1008 - Tbl Options = (CFE_TBL_OPT_USR_DEF_ADDR | CFE_TBL_OPT_DBL_BUFFER) */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", sizeof(UT_Table1_t), ((CFE_TBL_OPT_USR_DEF_ADDR & ~CFE_TBL_OPT_LD_DMP_MSK) | CFE_TBL_OPT_DBL_BUFFER), NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_OPTIONS) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1008 - Tbl Options = (CFE_TBL_OPT_USR_DEF_ADDR | CFE_TBL_OPT_DBL_BUFFER)");
    
    /* Test Case 1009 - Tbl Options = (CFE_TBL_OPT_USR_DEF_ADDR | CFE_TBL_OPT_LOAD_DUMP) */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", sizeof(UT_Table1_t), (CFE_TBL_OPT_USR_DEF_ADDR & ~CFE_TBL_OPT_LD_DMP_MSK), NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_OPTIONS) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1009 - Tbl Options = (CFE_TBL_OPT_USR_DEF_ADDR | CFE_TBL_OPT_LOAD_DUMP)");
    
    /* Test Case 1010 - Tbl Options = (CFE_TBL_OPT_USR_DEF_ADDR | CFE_TBL_OPT_CRITICAL) */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", sizeof(UT_Table1_t), ((CFE_TBL_OPT_USR_DEF_ADDR & ~CFE_TBL_OPT_LD_DMP_MSK) | CFE_TBL_OPT_CRITICAL), NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_OPTIONS) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1010 - Tbl Options = (CFE_TBL_OPT_USR_DEF_ADDR | CFE_TBL_OPT_CRITICAL)");
    
    /* Test Case 1011 - Tbl Options = (CFE_TBL_OPT_DUMP_ONLY | CFE_TBL_OPT_DBL_BUFFER) */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", sizeof(UT_Table1_t), (CFE_TBL_OPT_DUMP_ONLY | CFE_TBL_OPT_DBL_BUFFER), NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_OPTIONS) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1011 - Tbl Options = (CFE_TBL_OPT_DUMP_ONLY | CFE_TBL_OPT_DBL_BUFFER)");
    
    /* Test Case 1012 - GetPoolBuf Failure code = CFE_ES_ERR_MEM_HANDLE */ 
    UT_ClearEventHistory();
    UT_SetRtnCode(&GetPoolRtn, CFE_ES_ERR_MEM_HANDLE, 1);
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", sizeof(UT_Table1_t), CFE_TBL_OPT_DEFAULT, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_ES_ERR_MEM_HANDLE) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1012 - GetPoolBuf Failure code = CFE_ES_ERR_MEM_HANDLE");
    
    /* Test Case 1013 - GetPoolBuf Failure code = CFE_ES_ERR_MEM_BLOCK_SIZE */ 
    UT_ClearEventHistory();
    UT_SetRtnCode(&GetPoolRtn, CFE_ES_ERR_MEM_BLOCK_SIZE, 1);
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", sizeof(UT_Table1_t), CFE_TBL_OPT_DEFAULT, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_ES_ERR_MEM_BLOCK_SIZE) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1013 - GetPoolBuf Failure code = CFE_ES_ERR_MEM_BLOCK_SIZE");
    
    /* Test Case 1014 - GetPoolBuf Failure code (for second buffer) = CFE_ES_ERR_MEM_BLOCK_SIZE */ 
    UT_ClearEventHistory();
    UT_SetRtnCode(&GetPoolRtn, CFE_ES_ERR_MEM_BLOCK_SIZE, 2);
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", sizeof(UT_Table1_t), CFE_TBL_OPT_DBL_BUFFER, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_ES_ERR_MEM_BLOCK_SIZE) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1014 - GetPoolBuf Failure code (for second buffer) = CFE_ES_ERR_MEM_BLOCK_SIZE");
    
    /* Test Case 1015 - Successfully get a double buffered table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", sizeof(UT_Table1_t), CFE_TBL_OPT_DBL_BUFFER, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1015 - Successfully get a double buffered table ");
    
    /* Test Case 1016 - Attempt to register table owned by another Application */ 
    UT_ClearEventHistory();
    UT_SetAppID(2);
    RtnCode = CFE_TBL_Register(&TblHandle3, "UT_Table1", sizeof(UT_Table1_t), CFE_TBL_OPT_DBL_BUFFER, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_DUPLICATE_NOT_OWNED) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1016 - Attempt to register table owned by another Application ");
    /* Restore AppID to proper value */
    UT_SetAppID(1);
    
    /* Test Case 1017 - Attempt to register table with Different Size */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle3, "UT_Table1", sizeof(UT_Table2_t), CFE_TBL_OPT_DBL_BUFFER, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_DUPLICATE_DIFF_SIZE) && EventsCorrect), 
              "CFE_TBL_Register", 
              "1017 - Attempt to register table with Different Size ");
    
    /* Test Case 1018 - Register table with same size and name */ 
    UT_ClearEventHistory();
    UT_SetAppID(2);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    RtnCode = CFE_TBL_Share(&TblHandle3, "ut_cfe_tbl.UT_Table1");
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "1018 - Attempt to register table with same size and name Setup");
    /* Restore AppID to proper value */
    UT_SetAppID(1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle2, "UT_Table1", sizeof(UT_Table1_t), CFE_TBL_OPT_DBL_BUFFER, NULL);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_WARN_DUPLICATE) && (EventsCorrect) && (TblHandle1 == TblHandle2)), 
              "CFE_TBL_Register", 
              "1018 - Attempt to register table with same size and name ");
    
    /* Test Case 3001 - Unregister table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(TblHandle2);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister Table ");
    
    /* Test Case 3001 - Unregister table */ 
    UT_SetAppID(2);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(TblHandle3);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister Table ");
    
    /* Test Case 1019 - Register Single Buffered table */ 
    UT_SetAppID(1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", sizeof(UT_Table1_t), CFE_TBL_OPT_DEFAULT, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "1019 - Register Single Buffered table  ");
    
    /* Test Case 3001 - Unregister table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister Table ");
    
    /* Test Case 1020 - Register Single Buffered Dump Only table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table1", sizeof(UT_Table1_t), (CFE_TBL_OPT_SNGL_BUFFER | CFE_TBL_OPT_DUMP_ONLY), NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "1020 - Register Single Buffered Dump-Only table  ");
    
    /* Test Case 3001 - Unregister table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister Table ");
    
    /* Test Case 1021 - Register User Defined Address table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table001", sizeof(UT_Table1_t), CFE_TBL_OPT_USR_DEF_ADDR, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "1021 - Register User Defined Address table ");
    
    /* Test Case 3001 - Unregister table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister Table ");
    
    /* Test Case 1022 - Register Critical table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table001", sizeof(UT_Table1_t), CFE_TBL_OPT_CRITICAL, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "1022 - Register Critical table ");
    
    /* Test Case 3001 - Unregister table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister Table ");
    
    /* Test Case 1023 - Register Critical table that already has an allocated CDS */ 
    UT_SetRtnCode(&ES_RegisterCDSRtn, CFE_ES_CDS_ALREADY_EXISTS, 1);
    UT_ClearEventHistory();
    UT_CDS_Map.NextHandle--;
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table001", sizeof(UT_Table1_t), CFE_TBL_OPT_CRITICAL, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_TBL_INFO_RECOVERED_TBL) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "1023 - Register Critical table that already has an allocated CDS");
    
    /* Test Case 3001 - Unregister table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister Table ");
    
    /* Test Case 1024 - Register Critical table that already has an allocated CDS but fails recovery */ 
    UT_SetRtnCode(&ES_RegisterCDSRtn, CFE_ES_CDS_ALREADY_EXISTS, 1);
    UT_SetRtnCode(&ES_RestoreFromCDSRtn, CFE_ES_ERR_MEM_HANDLE, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table001", sizeof(UT_Table1_t), CFE_TBL_OPT_CRITICAL, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "1024 - Register Critical table that already has an allocated CDS but fails recovery");
    
    /* Test Case 3001 - Unregister table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister Table ");
    
    /* Test Case 1025 - Register Critical table that already has an allocated CDS but no critical table reg entry */ 
    UT_SetRtnCode(&ES_RegisterCDSRtn, CFE_ES_CDS_ALREADY_EXISTS, 1);
    /* Remove all entries from critical table registry */
    for (i=0; i<CFE_TBL_MAX_CRITICAL_TABLES; i++)
    {
        CFE_TBL_TaskData.CritReg[i].CDSHandle = CFE_ES_CDS_BAD_HANDLE;
    }
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table001", sizeof(UT_Table1_t), CFE_TBL_OPT_CRITICAL, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_TBL_INFO_RECOVERED_TBL) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "1025 - Register Critical table that already has an allocated CDS but no critical table reg entry");
    
    /* Test Case 3001 - Unregister table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister Table ");
    
    /* Test Case 1026 - Register Critical table but no critical table reg entry is free*/ 
    UT_SetRtnCode(&ES_RegisterCDSRtn, CFE_SUCCESS, 1);
    /* Remove all entries from critical table registry */
    for (i=0; i<CFE_TBL_MAX_CRITICAL_TABLES; i++)
    {
        CFE_TBL_TaskData.CritReg[i].CDSHandle = 1;
    }
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table001", sizeof(UT_Table1_t), CFE_TBL_OPT_CRITICAL, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "1026 - Register Critical table but no critical table reg entry is free");
    
    /* Test Case 3001 - Unregister table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister Table ");
    
    /* Test Case 1027 - Register Critical table when CDS Registry is Full*/ 
    UT_SetRtnCode(&ES_RegisterCDSRtn, CFE_ES_CDS_REGISTRY_FULL, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&TblHandle1, "UT_Table001", sizeof(UT_Table1_t), CFE_TBL_OPT_CRITICAL, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_TBL_WARN_NOT_CRITICAL) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "1027 - Register Critical table when CDS Registry is Full");
    
    /* Test Case 1028 - Registry Full */
    /* Reset Table Registry */
    UT_ResetTableRegistry();
    UT_ClearEventHistory();
    i = 0;
    RtnCode = CFE_SUCCESS;
    while ((i<CFE_TBL_MAX_NUM_TABLES) && (RtnCode == CFE_SUCCESS))
    {
        sprintf(TblName, "UT_Table%03d", (i+1));
        RtnCode = CFE_TBL_Register(&TblHandle1, TblName, sizeof(UT_Table1_t), CFE_TBL_OPT_DEFAULT, NULL);
        i++;
    }
    if (RtnCode != CFE_SUCCESS)
    {
        UT_Report((RtnCode == CFE_SUCCESS), 
                  "CFE_TBL_Register", 
                  "1028 - Registry Full setup failed");
    }
    else
    {
        sprintf(TblName, "UT_Table%03d", (i+1));
        RtnCode = CFE_TBL_Register(&TblHandle2, TblName, sizeof(UT_Table1_t), CFE_TBL_OPT_DEFAULT, NULL);
        EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
        UT_Report(((RtnCode == CFE_TBL_ERR_REGISTRY_FULL) && (EventsCorrect)), 
                  "CFE_TBL_Register", 
                  "1028 - Registry Full ");
    }      
    
    /* Test Case 3001 - Unregister table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(TblHandle1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister Table ");
    
    /* Test Case 1029 - No More Handles */ 
    UT_ClearEventHistory();
    do
    {
        RtnCode = CFE_TBL_Share(&TblHandle1, "ut_cfe_tbl.UT_Table002");
    } while ((TblHandle1<(CFE_TBL_MAX_NUM_HANDLES-1)) && (RtnCode == CFE_SUCCESS));
    if (RtnCode != CFE_SUCCESS)
    {
        UT_Report((RtnCode == CFE_SUCCESS), 
                  "CFE_TBL_Register", 
                  "1029 - No More Handles setup failed");
    }
    else
    {
        sprintf(TblName, "UT_Table%03d", CFE_TBL_MAX_NUM_TABLES);
        RtnCode = CFE_TBL_Register(&TblHandle1, TblName, sizeof(UT_Table1_t), CFE_TBL_OPT_DEFAULT, NULL);
        EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
        UT_Report(((RtnCode == CFE_TBL_ERR_HANDLES_FULL) && (EventsCorrect)), 
                  "CFE_TBL_Register", 
                  "1029 - No More Handles ");
    }      
}

/* Test Cases 2000-2xxx - CFE_TBL_Share */
void Test_CFE_TBL_Share(void)
{
    int32              RtnCode;
    boolean            EventsCorrect;
	CFE_FS_Header_t    StdFileHeader;
	CFE_TBL_File_Hdr_t TblFileHeader;
    int32              EndianCheck = 0x01020304;

    /* Test Case 2001 - CFE_TBL_ValidateAppID returns CFE_ES_ERR_APPID */
    UT_SetRtnCode(&GetAppIDRtn, CFE_ES_ERR_APPID, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Share(&App2TblHandle1, "ut_cfe_tbl.UT_Table002");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_SHARE_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_ES_ERR_APPID) && EventsCorrect), 
              "CFE_TBL_Share", 
              "2001 - CFE_TBL_ValidateAppID returns CFE_ES_ERR_APPID");
    
    /* Test Case 2002 - CFE_TBL_ValidateAppID returns CFE_TBL_ERR_BAD_APP_ID */ 
    UT_SetAppID(CFE_ES_MAX_APPLICATIONS);
    UT_SetRtnCode(&GetAppIDRtn, CFE_SUCCESS, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Share(&App2TblHandle1, "ut_cfe_tbl.UT_Table002");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_SHARE_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_BAD_APP_ID) && EventsCorrect), 
              "CFE_TBL_Share", 
              "2002 - CFE_TBL_ValidateAppID returns CFE_TBL_ERR_BAD_APP_ID");
    
    /* Test Case 2003 - Table Name Not In Registry */ 
    UT_SetRtnCode(&GetAppIDRtn, CFE_SUCCESS, 0);
    UT_SetAppID(1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Share(&App1TblHandle1, "ut_cfe_tbl.NOT_Table002");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_SHARE_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_NAME) && EventsCorrect), 
              "CFE_TBL_Share", 
              "2003 - Table Name Not In Registry");
    
    /* Test Case 2004 - No more free Table Handles */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Share(&App1TblHandle1, "ut_cfe_tbl.UT_Table003");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_SHARE_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_HANDLES_FULL) && EventsCorrect), 
              "CFE_TBL_Share", 
              "2004 - No more free Table Handles");
    
    /* Test Case 3001 - Unregister tables to free handles*/ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(CFE_TBL_MAX_NUM_HANDLES/2);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister Table ");
    UT_ClearEventHistory();
    /* Unit Test 14008 - Force one of the unregisters to have a PutPoolBuf error */
    UT_SetRtnCode(&PutPoolRtn, CFE_ES_ERR_MEM_HANDLE, 1);
    RtnCode = CFE_TBL_Unregister(CFE_TBL_MAX_NUM_HANDLES/2+1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_RemoveAccessLink", 
              "14008 - Unregister Table with PutPoolBuf error");
    
    /* Test Case 4001 - Successful first load of a table */
	UT_SetOSFail(0x0000);
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table004");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(3, CFE_TBL_SRC_FILE, "TblSrcFileName.dat");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_Load", 
              "4001 - Successful first load of a table");

    /* Test Case 2005 - Successful Share of Table that has not been loaded once */ 
    UT_SetAppID(2);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Share(&App2TblHandle1, "ut_cfe_tbl.UT_Table003");
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_Share", 
              "2005 - Successful Share of Table that has not been loaded");

    /* Test Case 2006 - Successful Share of Table that has been loaded once */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Share(&App2TblHandle2, "ut_cfe_tbl.UT_Table004");
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_Share", 
              "2006 - Successful Share of Table that has been loaded");
    
}

/* Test Cases 3000-3xxx - Test_TBL_Unregister */
void Test_CFE_TBL_Unregister(void)
{
    int32            RtnCode;
    boolean          EventsCorrect;

    /* Test Case 3002 - Try to Unregister Table with invalid handle */ 
    UT_SetAppID(1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(CFE_TBL_MAX_NUM_HANDLES);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_UNREGISTER_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_HANDLE) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3002 - Try to Unregister Table with invalid handle");
              
    /* Test Case 3001 - Unregister shared table to make it unowned */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(3);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister shared table to make it unowned ");
              
    /* Test Case 3003 - Try to Unregister unowned Table */ 
    UT_SetAppID(2);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(App2TblHandle2);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_UNREGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3003 - Try to Unregister unowned Table ");
}

/* Test Cases 4000-4xxx - Test_TBL_Load */
void Test_CFE_TBL_Load(void)
{
    CFE_TBL_Handle_t DumpOnlyTblHandle;
    CFE_TBL_Handle_t UserDefTblHandle;
    UT_Table1_t      TestTable1;
    int32            RtnCode;
    boolean          EventsCorrect;
	CFE_FS_Header_t    StdFileHeader;
	CFE_TBL_File_Hdr_t TblFileHeader;
    int32              EndianCheck = 0x01020304;
    UT_Table1_t     *App2TblPtr;
    
    UT_SetAppID(1);
    UT_ResetTableRegistry();
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&App1TblHandle1, "UT_Table1", sizeof(UT_Table1_t), CFE_TBL_OPT_DEFAULT, Test_CFE_TBL_ValidationFunc);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "1019 - Register Single Buffered table  ");
    
    /* Test Case 4002 - Attempt to perform partial INITIAL load */
	UT_SetOSFail(0x0000);
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t)-1;
    TblFileHeader.Offset = 1;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle1, CFE_TBL_SRC_FILE, "TblSrcFileName.dat");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_TBL_ERR_PARTIAL_LOAD) && EventsCorrect), 
              "CFE_TBL_Load", 
              "4002 - Attempt to perform partial INITIAL load");
    
    /* Test Case 4011 - File for wrong table */
    UT_ClearEventHistory();
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.NotUT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    RtnCode = CFE_TBL_Load(App1TblHandle1, CFE_TBL_SRC_FILE, "TblSrcFileName.dat");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_FILE_FOR_WRONG_TABLE) && (EventsCorrect)), 
              "CFE_TBL_Load", 
              "4011 - File for wrong table");
    
    /* Test Case 4003 - Perform Load from Memory */
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle1, CFE_TBL_SRC_ADDRESS, &TestTable1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_Load", 
              "4003 - Perform Load from Memory");
    
    /* Test Case 4004 - Attempt to load from illegal source type */
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle1, 4, &TestTable1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_TYPE_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_TBL_ERR_ILLEGAL_SRC_TYPE) && EventsCorrect), 
              "CFE_TBL_Load", 
              "4004 - Attempt to load from illegal source type");
    
    /* Test Case 4005 - Fail Validation Function on Table Load */
    UT_SetRtnCode(&TBL_ValidationFuncRtn, -1, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle1, CFE_TBL_SRC_ADDRESS, &TestTable1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == -1) && EventsCorrect), 
              "CFE_TBL_Load", 
              "4005 - Fail Validation Function on Table Load  (negative return code)");

    /* Test Case 4005.1 - Fail Validation Function on Table Load */
    UT_SetRtnCode(&TBL_ValidationFuncRtn, 1, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle1, CFE_TBL_SRC_ADDRESS, &TestTable1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == -1) && EventsCorrect), 
              "CFE_TBL_Load", 
              "4005.1 - Fail Validation Function on Table Load  (positive return code)");

    /* Test Case 4006 - Attempt to load to table with bad handle */
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(CFE_TBL_MAX_NUM_HANDLES, CFE_TBL_SRC_ADDRESS, &TestTable1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_HANDLE) && EventsCorrect), 
              "CFE_TBL_Load", 
              "4006 - Attempt to load to table with bad handle");
    
    /* Test Case 4007 - Attempt to load a dump only table */
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&DumpOnlyTblHandle, "UT_Table2", sizeof(UT_Table1_t), CFE_TBL_OPT_DUMP_ONLY, NULL);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "4007 - Attempt to load a dump only table setup ");
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(DumpOnlyTblHandle, CFE_TBL_SRC_ADDRESS, &TestTable1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_TBL_ERR_DUMP_ONLY) && EventsCorrect), 
              "CFE_TBL_Load", 
              "4007 - Attempt to load a dump only table");
    
    /* Test Case 4008 - Specify Table address for a User Defined Table */
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&UserDefTblHandle, "UT_Table3", sizeof(UT_Table1_t), CFE_TBL_OPT_USR_DEF_ADDR, NULL);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "4008 - Specify Table address for a User Defined Table ");
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(UserDefTblHandle, CFE_TBL_SRC_ADDRESS, &TestTable1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_Load", 
              "4008 - Specify Table address for a User Defined Table");
    
    /* Test Case 4009 - Attempt to load locked shared table */ 
    UT_SetAppID(2);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Share(&App2TblHandle1, "ut_cfe_tbl.UT_Table1");
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_Share", 
              "4009 - Attempt to load locked shared table setup Part 1");
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetAddress((void **)&App2TblPtr, App2TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_INFO_UPDATED) && EventsCorrect), 
              "CFE_TBL_GetAddress", 
              "6001 - Attempt to load locked shared table setup Part 2");
    UT_SetAppID(1);      
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle1, CFE_TBL_SRC_ADDRESS, &TestTable1);
    EventsCorrect = (UT_GetNumEventsSent() == 0); 
    UT_Report(((RtnCode == CFE_TBL_INFO_TABLE_LOCKED) && EventsCorrect), 
              "CFE_TBL_Load", 
              "4010 - Attempt to load locked shared table");
    
    /* Unlock shared table */
    UT_SetAppID(2);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_ReleaseAddress(App2TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_ReleaseAddress", 
              "7001 - Release Address to unlock shared table");
}

/* Test Cases 6000-6xxx - Test_TBL_GetAddress */
void Test_CFE_TBL_GetAddress(void)
{
    int32            RtnCode;
    boolean          EventsCorrect;
    UT_Table1_t     *App3TblPtr;
    UT_Table1_t     *App2TblPtr;
    
    /* Test Case 6002 - Attempt to get address of unowned table */ 
    UT_SetAppID(3);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetAddress((void **)&App3TblPtr, App2TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_ERR_NO_ACCESS) && EventsCorrect), 
              "CFE_TBL_GetAddress", 
              "6002 - Attempt to get address of unowned table");
    
    /* Test Case 6003 - Attempt to get address with invalid Application ID */ 
    UT_SetRtnCode(&GetAppIDRtn, CFE_ES_ERR_APPID, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetAddress((void **)&App3TblPtr, App2TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_ES_ERR_APPID) && EventsCorrect), 
              "CFE_TBL_GetAddress", 
              "6003 - Attempt to get address with invalid Application ID");
    
    /* Test Case 6004 - Attempt to get address with invalid handle */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetAddress((void **)&App3TblPtr, CFE_TBL_MAX_NUM_HANDLES);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_HANDLE) && EventsCorrect), 
              "CFE_TBL_GetAddress", 
              "6004 - Attempt to get address with invalid handle");
    
    /* Test Case 6005 - Attempt to get address of unowned table */ 
    UT_SetAppID(1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(App1TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Unregister", 
              "3001 - Unregister shared table to make it unowned ");
    UT_SetAppID(2);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetAddress((void **)&App2TblPtr, App2TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_ERR_UNREGISTERED) && EventsCorrect), 
              "CFE_TBL_GetAddress", 
              "6005 - Attempt to get address of unowned table");
}

/* Test Cases 7000-7xxx - Test_TBL_ReleaseAddress */
void Test_CFE_TBL_ReleaseAddress(void)
{
    int32            RtnCode;
    boolean          EventsCorrect;
    
    UT_SetAppID(1);
    UT_ResetTableRegistry();
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&App1TblHandle1, "UT_Table1", sizeof(UT_Table1_t), CFE_TBL_OPT_DEFAULT, Test_CFE_TBL_ValidationFunc);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "1019 - Register Single Buffered table  ");
    UT_SetRtnCode(&GetAppIDRtn, CFE_ES_ERR_APPID, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_ReleaseAddress(App1TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_ES_ERR_APPID) && EventsCorrect), 
              "CFE_TBL_GetAddress", 
              "7002 - Attempt to release address with invalid Application ID");
    
    
}

/* Test Cases 8000-8xxx - Test_TBL_GetAddresses */
void Test_CFE_TBL_GetAddresses(void)
{
    int32             RtnCode;
    boolean           EventsCorrect;
    
    UT_SetAppID(1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Register(&App1TblHandle2, "UT_Table2", sizeof(UT_Table1_t), CFE_TBL_OPT_DBL_BUFFER, Test_CFE_TBL_ValidationFunc);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_REGISTER_ERR_EID) == FALSE) && (UT_GetNumEventsSent() == 0));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Register", 
              "1019 - Register Another Table (double buffered)");
    
    /* Initialize array of Handles */
    ArrayOfHandles[0] = App1TblHandle1;
    ArrayOfHandles[1] = App1TblHandle2;
    ArrayOfPtrsToTblPtrs[0] = &Tbl1Ptr;
    ArrayOfPtrsToTblPtrs[1] = &Tbl2Ptr;
    
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetAddresses(ArrayOfPtrsToTblPtrs, 2, ArrayOfHandles);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_ERR_NEVER_LOADED) && (EventsCorrect) && (Tbl1Ptr != NULL) && (Tbl2Ptr != NULL)), 
              "CFE_TBL_GetAddresses", 
              "8001 - Get Addresses of two tables (neither of which have been loaded)");
              
    /* Test Case 8002 - Attempt to get addresses of tables that app is not allowed to see */
    UT_SetAppID(CFE_ES_MAX_APPLICATIONS);      
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetAddresses(ArrayOfPtrsToTblPtrs, 2, ArrayOfHandles);
    EventsCorrect = (UT_GetNumEventsSent() == 0); 
    UT_Report(((RtnCode == CFE_TBL_ERR_BAD_APP_ID) && EventsCorrect), 
              "CFE_TBL_Validate", 
              "8002 - Attempt to get addresses of tables that app is not allowed to see");
}

/* Test Cases 9000-9xxx - Test_TBL_ReleaseAddresses */
void Test_CFE_TBL_ReleaseAddresses(void)
{
    int32             RtnCode;
    boolean           EventsCorrect;
    
    UT_SetAppID(1);      
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_ReleaseAddresses(2, ArrayOfHandles);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_ERR_NEVER_LOADED) && (EventsCorrect)), 
              "CFE_TBL_ReleaseAddresses", 
              "9001 - Release Addresses of two tables (neither of which have been loaded)");
}

/* Test Cases 10000-10xxx - Test_TBL_Validate */
void Test_CFE_TBL_Validate(void)
{
    int32                   RtnCode;
    boolean                 EventsCorrect;

    /* Test Case 10001 - Attempt to validate table that app is not allowed to see */
    UT_SetAppID(CFE_ES_MAX_APPLICATIONS);      
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Validate(App1TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0); 
    UT_Report(((RtnCode == CFE_TBL_ERR_BAD_APP_ID) && EventsCorrect), 
              "CFE_TBL_Validate", 
              "10001 - Attempt to validate table that app is not allowed to see");
              
    /* Test Case 10002 - Attempt to validate table when no validation is pending */
    UT_SetAppID(1);      
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Validate(App1TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0); 
    UT_Report(((RtnCode == CFE_TBL_INFO_NO_VALIDATION_PENDING) && EventsCorrect), 
              "CFE_TBL_Validate", 
              "10002 - Attempt to validate table when no validation is pending");
}

/* Test Cases 11000-11xxx - Test_TBL_Manage */
void Test_CFE_TBL_Manage(void)
{
    int32                   RtnCode;
    boolean                 EventsCorrect;
    int32                   RegIndex;
    CFE_TBL_RegistryRec_t  *RegRecPtr;
    CFE_TBL_LoadBuff_t     *WorkingBufferPtr;
    UT_Table1_t             TestTable1;
    UT_Table1_t            *App2TblPtr;
    
    /* Test Case 11001 - Manage Table that doesn't need managing */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Manage", 
              "11001 - Manage Table that doesn't need managing");
        
    /* "Load" image into inactive buffer for table */
    RegIndex = CFE_TBL_FindTableInRegistry("ut_cfe_tbl.UT_Table1");
    RegRecPtr = &CFE_TBL_TaskData.Registry[RegIndex];
    RtnCode = CFE_TBL_GetWorkingBuffer(&WorkingBufferPtr, RegRecPtr, FALSE);
    
    /* Now is a good time to try to perform an API Load while one is in progress via command */
    /* Test Case 4011 - Attempt to load while a load is in progress */
    UT_SetAppID(1);      
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle1, CFE_TBL_SRC_ADDRESS, &TestTable1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_TBL_ERR_LOAD_IN_PROGRESS) && EventsCorrect), 
              "CFE_TBL_Load", 
              "4011 - Attempt to load while a load is in progress");
        
    /* Test Case 11002 - Process an unsuccessful Validation Request on Inactive Buffer */
    /* Configure table for Validation */
    CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_PENDING;
    CFE_TBL_TaskData.ValidationResults[0].Result = 0;
    strncpy(CFE_TBL_TaskData.ValidationResults[0].TableName, "ut_cfe_tbl.UT_Table1", CFE_TBL_MAX_FULL_NAME_LEN);
    CFE_TBL_TaskData.ValidationResults[0].CrcOfTable = 0;
    CFE_TBL_TaskData.ValidationResults[0].ActiveBuffer = FALSE;
    RegRecPtr->ValidateInactiveIndex = 0;
    /* Perform Validation via Manage call */
    UT_SetRtnCode(&TBL_ValidationFuncRtn, -1, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_VALIDATION_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && (CFE_TBL_TaskData.ValidationResults[0].Result == -1)), 
              "CFE_TBL_Manage", 
              "11002 - Manage Table that has a failed Validation pending on Inactive Buffer");
        
    /* Test Case 11002.1 - Process an unsuccessful Validation Request on Inactive Buffer */
    /* Configure table for Validation */
    CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_PENDING;
    CFE_TBL_TaskData.ValidationResults[0].Result = 0;
    strncpy(CFE_TBL_TaskData.ValidationResults[0].TableName, "ut_cfe_tbl.UT_Table1", CFE_TBL_MAX_FULL_NAME_LEN);
    CFE_TBL_TaskData.ValidationResults[0].CrcOfTable = 0;
    CFE_TBL_TaskData.ValidationResults[0].ActiveBuffer = FALSE;
    RegRecPtr->ValidateInactiveIndex = 0;
    /* Perform Validation via Manage call */
    UT_SetRtnCode(&TBL_ValidationFuncRtn, 1, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_VALIDATION_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && (CFE_TBL_TaskData.ValidationResults[0].Result == -1)), 
              "CFE_TBL_Manage", 
              "11002.1 - Manage Table that has a failed Validation pending on Inactive Buffer");
        
    /* Test Case 11003 - Process a successful Validation Request on Inactive Buffer */
    /* Configure table for Validation */
    CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_PENDING;
    CFE_TBL_TaskData.ValidationResults[0].Result = 1;
    strncpy(CFE_TBL_TaskData.ValidationResults[0].TableName, "ut_cfe_tbl.UT_Table1", CFE_TBL_MAX_FULL_NAME_LEN);
    CFE_TBL_TaskData.ValidationResults[0].CrcOfTable = 0;
    CFE_TBL_TaskData.ValidationResults[0].ActiveBuffer = FALSE;
    RegRecPtr->ValidateInactiveIndex = 0;
    /* Perform Validation via Manage call */
    UT_SetRtnCode(&TBL_ValidationFuncRtn, CFE_SUCCESS, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_VALIDATION_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && (CFE_TBL_TaskData.ValidationResults[0].Result == 0)), 
              "CFE_TBL_Manage", 
              "11003 - Manage Table that has a successful Validation pending on Inactive Buffer");
        
    /* Test Case 11004 - Process an unsuccessful Validation Request on Active Buffer */
    /* Configure table for Validation */
    CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_PENDING;
    CFE_TBL_TaskData.ValidationResults[0].Result = 0;
    strncpy(CFE_TBL_TaskData.ValidationResults[0].TableName, "ut_cfe_tbl.UT_Table1", CFE_TBL_MAX_FULL_NAME_LEN);
    CFE_TBL_TaskData.ValidationResults[0].CrcOfTable = 0;
    CFE_TBL_TaskData.ValidationResults[0].ActiveBuffer = TRUE;
    RegRecPtr->ValidateActiveIndex = 0;
    /* Perform Validation via Manage call */
    UT_SetRtnCode(&TBL_ValidationFuncRtn, -1, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_VALIDATION_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && (CFE_TBL_TaskData.ValidationResults[0].Result == -1)), 
              "CFE_TBL_Manage", 
              "11004 - Manage Table that has an unsuccessful Validation pending on Active Buffer");
        
    /* Test Case 11004.1 - Process an unsuccessful Validation Request on Active Buffer */
    /* Configure table for Validation */
    CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_PENDING;
    CFE_TBL_TaskData.ValidationResults[0].Result = 0;
    strncpy(CFE_TBL_TaskData.ValidationResults[0].TableName, "ut_cfe_tbl.UT_Table1", CFE_TBL_MAX_FULL_NAME_LEN);
    CFE_TBL_TaskData.ValidationResults[0].CrcOfTable = 0;
    CFE_TBL_TaskData.ValidationResults[0].ActiveBuffer = TRUE;
    RegRecPtr->ValidateActiveIndex = 0;
    /* Perform Validation via Manage call */
    UT_SetRtnCode(&TBL_ValidationFuncRtn, 1, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_VALIDATION_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && (CFE_TBL_TaskData.ValidationResults[0].Result == -1)), 
              "CFE_TBL_Manage", 
              "11004.1 - Manage Table that has an unsuccessful Validation pending on Active Buffer");
        
    /* Test Case 11005 - Process a successful Validation Request on Active Buffer */
    /* Configure table for Validation */
    CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_PENDING;
    CFE_TBL_TaskData.ValidationResults[0].Result = 1;
    strncpy(CFE_TBL_TaskData.ValidationResults[0].TableName, "ut_cfe_tbl.UT_Table1", CFE_TBL_MAX_FULL_NAME_LEN);
    CFE_TBL_TaskData.ValidationResults[0].CrcOfTable = 0;
    CFE_TBL_TaskData.ValidationResults[0].ActiveBuffer = TRUE;
    RegRecPtr->ValidateActiveIndex = 0;
    /* Perform Validation via Manage call */
    UT_SetRtnCode(&TBL_ValidationFuncRtn, CFE_SUCCESS, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_VALIDATION_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && (CFE_TBL_TaskData.ValidationResults[0].Result == 0)), 
              "CFE_TBL_Manage", 
              "11005 - Manage Table that has a successful Validation pending on Active Buffer");
        
    /* Test Case 11006 - Process an Update Request on a locked table */
    UT_SetAppID(2);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Share(&App2TblHandle1, "ut_cfe_tbl.UT_Table1");
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_Share", 
              "4009 - Process an Update Request on a locked table setup Part 1");
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetAddress((void **)&App2TblPtr, App2TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_ERR_NEVER_LOADED) && EventsCorrect), 
              "CFE_TBL_GetAddress", 
              "6001 - Process an Update Request on a locked table setup Part 2");
    UT_SetAppID(1);      
    UT_ClearEventHistory();
    /* Configure table for Update */
    RegRecPtr->LoadPending = TRUE;
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_INFO_TABLE_LOCKED) && (EventsCorrect)), 
              "CFE_TBL_Manage", 
              "11006 - Process an Update Request on a locked table");
    /* Unlock table by releasing the address */      
    UT_ClearEventHistory();
    UT_SetAppID(2);
    RtnCode = CFE_TBL_ReleaseAddress(App2TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_ERR_NEVER_LOADED) && EventsCorrect), 
              "CFE_TBL_ReleaseAddress", 
              "7001 - Release address to unlock shared table");

    /* Test Case 11007 - Process an Update Request on a single buffered table */
    UT_SetAppID(1);      
    UT_ClearEventHistory();
    /* Configure table for Update */
    RegRecPtr->LoadPending = TRUE;
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_UPDATE_SUCCESS_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_INFO_UPDATED) && (EventsCorrect)), 
              "CFE_TBL_Manage", 
              "11007 - Process an Update Request on a single buffered table");
        
    /* Test Case 11008 - Process an unsuccessful Validation Request on Inactive Buffer (dbl buff) */
    /* "Load" image into inactive buffer for table */
    RegIndex = CFE_TBL_FindTableInRegistry("ut_cfe_tbl.UT_Table2");
    RegRecPtr = &CFE_TBL_TaskData.Registry[RegIndex];
    RtnCode = CFE_TBL_GetWorkingBuffer(&WorkingBufferPtr, RegRecPtr, FALSE);
    /* Configure table for Validation */
    CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_PENDING;
    CFE_TBL_TaskData.ValidationResults[0].Result = 0;
    strncpy(CFE_TBL_TaskData.ValidationResults[0].TableName, "ut_cfe_tbl.UT_Table2", CFE_TBL_MAX_FULL_NAME_LEN);
    CFE_TBL_TaskData.ValidationResults[0].CrcOfTable = 0;
    CFE_TBL_TaskData.ValidationResults[0].ActiveBuffer = FALSE;
    RegRecPtr->ValidateInactiveIndex = 0;
    /* Perform Validation via Manage call */
    UT_SetRtnCode(&TBL_ValidationFuncRtn, -1, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle2);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_VALIDATION_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && (CFE_TBL_TaskData.ValidationResults[0].Result == -1)), 
              "CFE_TBL_Manage", 
              "11008 - Manage Table that has a failed Validation pending on Inactive Buffer (dbl buff)");
        
    /* Test Case 11009 - Process a successful Validation Request on Inactive Buffer (dbl buff) */
    /* Configure table for Validation */
    CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_PENDING;
    CFE_TBL_TaskData.ValidationResults[0].Result = 1;
    strncpy(CFE_TBL_TaskData.ValidationResults[0].TableName, "ut_cfe_tbl.UT_Table2", CFE_TBL_MAX_FULL_NAME_LEN);
    CFE_TBL_TaskData.ValidationResults[0].CrcOfTable = 0;
    CFE_TBL_TaskData.ValidationResults[0].ActiveBuffer = FALSE;
    RegRecPtr->ValidateInactiveIndex = 0;
    /* Perform Validation via Manage call */
    UT_SetRtnCode(&TBL_ValidationFuncRtn, CFE_SUCCESS, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle2);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_VALIDATION_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && (CFE_TBL_TaskData.ValidationResults[0].Result == 0)), 
              "CFE_TBL_Manage", 
              "11009 - Manage Table that has a successful Validation pending on Inactive Buffer (dbl buff)");
        
    /* Test Case 11010 - Process an unsuccessful Validation Request on Active Buffer (dbl buff) */
    /* Configure table for Validation */
    CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_PENDING;
    CFE_TBL_TaskData.ValidationResults[0].Result = 0;
    strncpy(CFE_TBL_TaskData.ValidationResults[0].TableName, "ut_cfe_tbl.UT_Table2", CFE_TBL_MAX_FULL_NAME_LEN);
    CFE_TBL_TaskData.ValidationResults[0].CrcOfTable = 0;
    CFE_TBL_TaskData.ValidationResults[0].ActiveBuffer = TRUE;
    RegRecPtr->ValidateActiveIndex = 0;
    /* Perform Validation via Manage call */
    UT_SetRtnCode(&TBL_ValidationFuncRtn, -1, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle2);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_VALIDATION_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && (CFE_TBL_TaskData.ValidationResults[0].Result == -1)), 
              "CFE_TBL_Manage", 
              "11010 - Manage Table that has an unsuccessful Validation pending on Active Buffer (dbl buff)");
        
    /* Test Case 11011 - Process a successful Validation Request on Active Buffer (dbl buff) */
    /* Configure table for Validation */
    CFE_TBL_TaskData.ValidationResults[0].State = CFE_TBL_VALIDATION_PENDING;
    CFE_TBL_TaskData.ValidationResults[0].Result = 1;
    strncpy(CFE_TBL_TaskData.ValidationResults[0].TableName, "ut_cfe_tbl.UT_Table2", CFE_TBL_MAX_FULL_NAME_LEN);
    CFE_TBL_TaskData.ValidationResults[0].CrcOfTable = 0;
    CFE_TBL_TaskData.ValidationResults[0].ActiveBuffer = TRUE;
    RegRecPtr->ValidateActiveIndex = 0;
    /* Perform Validation via Manage call */
    UT_SetRtnCode(&TBL_ValidationFuncRtn, CFE_SUCCESS, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle2);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_VALIDATION_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && (CFE_TBL_TaskData.ValidationResults[0].Result == 0)), 
              "CFE_TBL_Manage", 
              "11011 - Manage Table that has a successful Validation pending on Active Buffer (dbl buff)");

    /* Test Case 11012 - Process a Table Dump Request */
    RtnCode = CFE_TBL_GetWorkingBuffer(&WorkingBufferPtr, RegRecPtr, FALSE);
    CFE_TBL_TaskData.DumpControlBlocks[0].State = CFE_TBL_DUMP_PENDING;
    CFE_TBL_TaskData.DumpControlBlocks[0].RegRecPtr = RegRecPtr;
    /* Save the name of the desired dump filename, table name and size for later */
    CFE_TBL_TaskData.DumpControlBlocks[0].DumpBufferPtr = WorkingBufferPtr;
    CFE_PSP_MemCpy(CFE_TBL_TaskData.DumpControlBlocks[0].DumpBufferPtr->DataSource, "MyDumpFilename", OS_MAX_PATH_LEN);
    CFE_PSP_MemCpy(CFE_TBL_TaskData.DumpControlBlocks[0].TableName, "ut_cfe_tbl.UT_Table2", CFE_TBL_MAX_FULL_NAME_LEN);
    CFE_TBL_TaskData.DumpControlBlocks[0].Size = RegRecPtr->Size;
    RegRecPtr->DumpControlIndex = 0;
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Manage(App1TblHandle2);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_Manage", 
              "11012 - Manage Table that has a dump request pending");

}

/* Test Cases 5000-5xxx - Test_TBL_Update */
void Test_CFE_TBL_Update(void)
{
    int32            RtnCode;
    boolean          EventsCorrect;

    /* Test Case 5001 - Process an Update on a single buffered table without priveleges */
    UT_SetAppID(2);      
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Update(App1TblHandle1);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_UPDATE_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_NO_ACCESS) && (EventsCorrect)), 
              "CFE_TBL_Update", 
              "5001 - Process an Update on a single buffered table without priveleges");

    /* Test Case 5002 - Process an Update on a single buffered table when no update is pending */
    UT_SetAppID(1);      
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Update(App1TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_INFO_NO_UPDATE_PENDING) && (EventsCorrect)), 
              "CFE_TBL_Update", 
              "5002 - Process an Update on a single buffered table when no update is pending");
}

/* Test Cases 12000-12xxx - Test_TBL_GetStatus */
void Test_CFE_TBL_GetStatus(void)
{
    int32                   RtnCode;
    boolean                 EventsCorrect;

    /* Test Case 12001 - Attempt to get status on table that app is not allowed to see */
    UT_SetAppID(CFE_ES_MAX_APPLICATIONS);      
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetStatus(App1TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0); 
    UT_Report(((RtnCode == CFE_TBL_ERR_BAD_APP_ID) && EventsCorrect), 
              "CFE_TBL_GetStatus", 
              "12001 - Attempt to get status on table that app is not allowed to see");
}

/* Test Cases 13000-13xxx - Test_TBL_GetInfo */
void Test_CFE_TBL_GetInfo(void)
{
    int32             RtnCode;
    boolean           EventsCorrect;
    CFE_TBL_Info_t    TblInfo;
    
    /* Test Case 13001 - Get Information on UT_Table1 */ 
    UT_SetAppID(1);      
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetInfo(&TblInfo, "ut_cfe_tbl.UT_Table1");
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_GetInfo", 
              "13001 - Get Information on UT_Table1");
    
    /* Test Case 13002 - Get Information on non-existent table */ 
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetInfo(&TblInfo, "ut_cfe_tbl.UT_Table_Not");
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_NAME) && (EventsCorrect)), 
              "CFE_TBL_GetInfo", 
              "13002 - Get Information on non-existent table");
}

void Test_CFE_TBL_Internal(void)
{
    int32                       RtnCode;
    boolean                     EventsCorrect;
    CFE_TBL_LoadBuff_t         *WorkingBufferPtr;
    CFE_TBL_RegistryRec_t      *RegRecPtr;
    CFE_TBL_AccessDescriptor_t *AccessDescPtr;
    char                        Filename[OS_MAX_PATH_LEN+10];
    int32                       i;
	CFE_FS_Header_t             StdFileHeader;
	CFE_TBL_File_Hdr_t          TblFileHeader;
    int32                       EndianCheck = 0x01020304;
    int32                       OldOSFail;
    int32                       FileDescriptor;
    void                       *TblPtr;
    
    /* Test Case 14010 - GetWorkingBuffer - Initial Load of Double Buff Table */
    UT_ClearEventHistory();
    AccessDescPtr = &CFE_TBL_TaskData.Handles[App1TblHandle2];
    RegRecPtr = &CFE_TBL_TaskData.Registry[AccessDescPtr->RegIndex];
    RegRecPtr->TableLoadedOnce = FALSE;
    RegRecPtr->LoadInProgress = CFE_TBL_NO_LOAD_IN_PROGRESS;
    RtnCode = CFE_TBL_GetWorkingBuffer(&WorkingBufferPtr, RegRecPtr, TRUE);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && 
               (WorkingBufferPtr == &RegRecPtr->Buffers[RegRecPtr->ActiveBufferIndex])), 
              "CFE_TBL_GetWorkingBuffer", 
              "14010 - Initial Load of Double Buff Table");

    /* Test Case 14011 - GetWorkingBuffer - Sngl Buff Tbl has Mutex Sem Take failure */
    UT_ClearEventHistory();
    UT_SetRtnCode(&MutSemTakeRtn, OS_ERROR, 1);
    AccessDescPtr = &CFE_TBL_TaskData.Handles[App1TblHandle1];
    RegRecPtr = &CFE_TBL_TaskData.Registry[AccessDescPtr->RegIndex];
    RtnCode = CFE_TBL_GetWorkingBuffer(&WorkingBufferPtr, RegRecPtr, TRUE);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_GetWorkingBuffer", 
              "14011 - Sngl Buff Tbl has Mutex Sem Take failure");
    
    /* Test Case 14012 - LoadFromFile - Filename too long */
    UT_ClearEventHistory();
    for (i=0; i<(OS_MAX_PATH_LEN+10); i++)
    {
        Filename[i] = 'a';
    }
    RtnCode = CFE_TBL_LoadFromFile(WorkingBufferPtr, RegRecPtr, Filename);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_ERR_FILENAME_TOO_LONG) && (EventsCorrect)), 
              "CFE_TBL_LoadFromFile", 
              "14012 - Filename too long");
    
    /* Test Case 14013 - LoadFromFile - File content too large (according to header)*/
    UT_ClearEventHistory();
    Filename[OS_MAX_PATH_LEN-1] = '\0';
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 1;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    RtnCode = CFE_TBL_LoadFromFile(WorkingBufferPtr, RegRecPtr, Filename);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_ERR_FILE_TOO_LARGE) && (EventsCorrect)), 
              "CFE_TBL_LoadFromFile", 
              "14013 - File content too large (according to header)");
    
    /* Test Case 14014 - LoadFromFile - File content too large (too much content)*/
    UT_ClearEventHistory();
    Filename[OS_MAX_PATH_LEN-1] = '\0';
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, sizeof(UT_Table1_t), 2);
    UT_SetRtnCode(&OSReadRtn2, 0, 0);
    RtnCode = CFE_TBL_LoadFromFile(WorkingBufferPtr, RegRecPtr, Filename);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_ERR_FILE_TOO_LARGE) && (EventsCorrect)), 
              "CFE_TBL_LoadFromFile", 
              "14014 - File content too large (too much content)");
    
    /* Test Case 14015 - LoadFromFile - File content too small */
    UT_ClearEventHistory();
    Filename[OS_MAX_PATH_LEN-1] = '\0';
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, sizeof(UT_Table1_t)-1, 2);
    UT_SetRtnCode(&OSReadRtn2, 0, 2);
    RtnCode = CFE_TBL_LoadFromFile(WorkingBufferPtr, RegRecPtr, Filename);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_ERR_LOAD_INCOMPLETE) && (EventsCorrect)), 
              "CFE_TBL_LoadFromFile", 
              "14015 - File content too small");
    
    /* Test Case 14016 - LoadFromFile - File for wrong table */
    UT_ClearEventHistory();
    Filename[OS_MAX_PATH_LEN-1] = '\0';
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.NotUT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    RtnCode = CFE_TBL_LoadFromFile(WorkingBufferPtr, RegRecPtr, Filename);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_ERR_FILE_FOR_WRONG_TABLE) && (EventsCorrect)), 
              "CFE_TBL_LoadFromFile", 
              "14016 - File for wrong table");
    
    /* Test Case 14017 - LoadFromFile - OS_open error */
    UT_ClearEventHistory();
    Filename[OS_MAX_PATH_LEN-1] = '\0';
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    OldOSFail = UT_OS_Fail;
    UT_OS_Fail = OS_OPEN_FAIL;
    RtnCode = CFE_TBL_LoadFromFile(WorkingBufferPtr, RegRecPtr, Filename);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == OS_ERROR) && (EventsCorrect)), 
              "CFE_TBL_LoadFromFile", 
              "14017 - OS_open error");
    UT_OS_Fail = OldOSFail;      
    
    /* Test Case 14018 - LoadFromFile - File too short warning */
    UT_ClearEventHistory();
    Filename[OS_MAX_PATH_LEN-1] = '\0';
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t)-1;
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    RtnCode = CFE_TBL_LoadFromFile(WorkingBufferPtr, RegRecPtr, Filename);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_WARN_SHORT_FILE) && (EventsCorrect)), 
              "CFE_TBL_LoadFromFile", 
              "14018 - File too short warning");
    
    /* Test Case 14019 - ReadHeaders - Failure reading Standard cFE File Header */
    UT_ClearEventHistory();
    Filename[OS_MAX_PATH_LEN-1] = '\0';
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t)-1;
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    strcpy(Filename, "MyTestInputFilename");
    UT_SetRtnCode(&FSReadHdrRtn, sizeof(CFE_FS_Header_t)-1, 1);
    RtnCode = CFE_TBL_ReadHeaders(FileDescriptor, &StdFileHeader, &TblFileHeader, Filename);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_FILE_STD_HDR_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_NO_STD_HEADER) && (EventsCorrect)), 
              "CFE_TBL_ReadHeaders",
              "14019 - Failure reading Standard cFE File Header");
    
    /* Test Case 14020 - ReadHeaders - Bad Magic Number in cFE Standard Header */
    UT_ClearEventHistory();
    Filename[OS_MAX_PATH_LEN-1] = '\0';
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID-1;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t)-1;
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    RtnCode = CFE_TBL_ReadHeaders(FileDescriptor, &StdFileHeader, &TblFileHeader, Filename);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_FILE_TYPE_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_BAD_CONTENT_ID) && (EventsCorrect)), 
              "CFE_TBL_ReadHeaders",
              "14020 - Bad Magic Number in cFE Standard Header");
    
    /* Test Case 14021 - ReadHeaders - Wrong cFE File SubType */
    UT_ClearEventHistory();
    Filename[OS_MAX_PATH_LEN-1] = '\0';
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE-1;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t)-1;
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    RtnCode = CFE_TBL_ReadHeaders(FileDescriptor, &StdFileHeader, &TblFileHeader, Filename);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_FILE_SUBTYPE_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_BAD_SUBTYPE_ID) && (EventsCorrect)), 
              "CFE_TBL_ReadHeaders",
              "14021 - Wrong cFE File SubType");
    
    /* Test Case 14022 - ReadHeaders - Failure Reading cFE Table Header */
    UT_ClearEventHistory();
    Filename[OS_MAX_PATH_LEN-1] = '\0';
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t)-1;
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, sizeof(CFE_TBL_File_Hdr_t)-1, 1);
    RtnCode = CFE_TBL_ReadHeaders(FileDescriptor, &StdFileHeader, &TblFileHeader, Filename);
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_FILE_TBL_HDR_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1));
    UT_Report(((RtnCode == CFE_TBL_ERR_NO_TBL_HEADER) && (EventsCorrect)), 
              "CFE_TBL_ReadHeaders",
              "14022 - Failure Reading cFE Table Header");
    
    /* Test Case 14009 - RemoveAccessLink - Fail to put memory buffer for double buffered tbl back */
    UT_ClearEventHistory();
    UT_SetRtnCode(&PutPoolRtn, CFE_ES_ERR_MEM_HANDLE, 2);
    RtnCode = CFE_TBL_Unregister(App1TblHandle2);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_RemoveAccessLInk", 
              "14009 - Fail to put memory buffer for double buffered tbl back");
    
    /* Test Case 14001 - EarlyInit - Table Registry Mutex Create Failure */
    UT_ClearEventHistory();
    UT_SetRtnCode(&MutSemCreateRtn, OS_ERROR, 1);
    UT_ResetCDS();
    RtnCode = CFE_TBL_EarlyInit();
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == OS_ERROR) && (EventsCorrect)), 
              "CFE_TBL_EarlyInit", 
              "14001 - Table Registry Mutex Create Failure");
    
    /* Test Case 14002 - EarlyInit - Work Buffer Mutex Create Failure */
    UT_ClearEventHistory();
    UT_SetRtnCode(&MutSemCreateRtn, OS_ERROR, 2);
    UT_ResetCDS();
    RtnCode = CFE_TBL_EarlyInit();
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == OS_ERROR) && (EventsCorrect)), 
              "CFE_TBL_EarlyInit", 
              "14002 - Work Buffer Mutex Create Failure");
    
    /* Test Case 14003 - EarlyInit - Memory Pool Create Failure */
    UT_ClearEventHistory();
    UT_SetRtnCode(&CreatePoolRtn, CFE_ES_BAD_ARGUMENT, 1);
    UT_ResetCDS();
    RtnCode = CFE_TBL_EarlyInit();
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_ES_BAD_ARGUMENT) && (EventsCorrect)), 
              "CFE_TBL_EarlyInit", 
              "14003 - Memory Pool Create Failure");
    
    /* Test Case 14004 - EarlyInit - Get Pool Buff Failure */
    UT_ClearEventHistory();
    UT_SetRtnCode(&GetPoolRtn, CFE_ES_ERR_MEM_HANDLE, 1);
    UT_ResetCDS();
    RtnCode = CFE_TBL_EarlyInit();
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_ES_ERR_MEM_HANDLE) && (EventsCorrect)), 
              "CFE_TBL_EarlyInit", 
              "14004 - Get Pool Buff Failure");
    
    /* Test Case 14005 - EarlyInit - CDS Already exists but restore fails */
    UT_ClearEventHistory();
    UT_ResetCDS();
    UT_SetRtnCode(&ES_RegisterCDSRtn, CFE_ES_CDS_ALREADY_EXISTS, 1);
    UT_SetRtnCode(&ES_RestoreFromCDSRtn, CFE_ES_CDS_BLOCK_CRC_ERR, 1);
    RtnCode = CFE_TBL_EarlyInit();
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_EarlyInit", 
              "14005 - CDS Already exists but restore fails");
    
    /* Test Case 14006 - EarlyInit - No CDS Available */
    UT_ClearEventHistory();
    UT_ResetCDS();
    UT_SetRtnCode(&ES_RegisterCDSRtn, CFE_ES_NOT_IMPLEMENTED, 1);
    RtnCode = CFE_TBL_EarlyInit();
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_EarlyInit", 
              "14006 - No CDS Available");
    
    /* Test Case 14007 - EarlyInit - Fail to save Critical Tbl Registry to CDS */
    UT_ClearEventHistory();
    UT_ResetCDS();
    UT_SetRtnCode(&ES_CopyToCDSRtn, CFE_ES_ERR_MEM_HANDLE, 1);
    RtnCode = CFE_TBL_EarlyInit();
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "CFE_TBL_EarlyInit", 
              "14007 - Fail to save Critical Tbl Registry to CDS");
        
    /* Start anew so that tests can be configured correctly */
    UT_ClearEventHistory();
    UT_SetAppID(1);      
    UT_ResetCDS();
    RtnCode = CFE_TBL_EarlyInit();
    UT_Report((RtnCode == CFE_SUCCESS), 
              "CFE_TBL_UpdateInternal",
              "14023 - Test Setup Part 1");
    /* Register Critical Double and Single Buffered Tables */
    RtnCode = CFE_TBL_Register(&App1TblHandle1, "UT_Table1", sizeof(UT_Table1_t), 
                               (CFE_TBL_OPT_DEFAULT | CFE_TBL_OPT_CRITICAL), 
                               Test_CFE_TBL_ValidationFunc);
    UT_Report((RtnCode == CFE_SUCCESS), 
              "CFE_TBL_UpdateInternal",
              "14023 - Test Setup Part 2a");
    RtnCode = CFE_TBL_Register(&App1TblHandle2, "UT_Table2", sizeof(UT_Table1_t), 
                               (CFE_TBL_OPT_DBL_BUFFER | CFE_TBL_OPT_CRITICAL),
                               Test_CFE_TBL_ValidationFunc);
    UT_Report((RtnCode == CFE_SUCCESS), 
              "CFE_TBL_UpdateInternal",
              "14024 - Test Setup Part 2b");
    /* Perform an initial load on both tables */      
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle1, CFE_TBL_SRC_FILE, "TblSrcFileName.dat");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_UpdateInternal", 
              "14023 - Test Setup Part 3a");
    /* Test Case 14023 - UpdateInternal - Update CDS for Single Buffered Table */
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table1");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle1, CFE_TBL_SRC_FILE, "TblSrcFileName.dat");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_UpdateInternal", 
              "14023 - Update CDS for Single Buffered Table");
              
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table2");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle2, CFE_TBL_SRC_FILE, "TblSrcFileName.dat");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_UpdateInternal", 
              "14024 - Test Setup Part 3b");
              
    /* Test Case 14024 - UpdateInternal - Update CDS for Double Buffered Table */
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table2");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle2, CFE_TBL_SRC_FILE, "TblSrcFileName.dat");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_UpdateInternal", 
              "14024 - Update CDS for Double Buffered Table");
              
    /* Test Case 14025 - GetWorkingBuffer - Both double buffered table buffers locked */
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table2");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle2, CFE_TBL_SRC_FILE, "TblSrcFileName.dat");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_TBL_ERR_NO_BUFFER_AVAIL) && EventsCorrect), 
              "CFE_TBL_GetWorkingBuffer", 
              "14025 - Both double buffered table buffers locked");
    
    /* Release buffer */
    /* Test Case 14026 - UpdateCriticalTblCDS - Error writing to CDS */
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetAddress(&TblPtr, App1TblHandle2);      
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_INFO_UPDATED) && EventsCorrect), 
              "CFE_TBL_UpdateInternal", 
              "14026 - Setup Step 1");
    RtnCode = CFE_TBL_ReleaseAddress(App1TblHandle2);      
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_UpdateInternal", 
              "14026 - Setup Step 2");
              
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table2");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    UT_SetRtnCode(&ES_CopyToCDSRtn, CFE_ES_ERR_MEM_HANDLE, 1);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle2, CFE_TBL_SRC_FILE, "TblSrcFileName.dat");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_UpdateCriticalTblCDS", 
              "14026 - Error updating CDS");

    /* Test Case 14027 - UpdateCriticalTblCDS - Error writing to CDS (second time) */
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetAddress(&TblPtr, App1TblHandle2);      
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_INFO_UPDATED) && EventsCorrect), 
              "CFE_TBL_UpdateInternal", 
              "14027 - Setup Step 1");
    RtnCode = CFE_TBL_ReleaseAddress(App1TblHandle2);      
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_UpdateInternal", 
              "14027 - Setup Step 2");
              
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table2");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    UT_SetRtnCode(&ES_CopyToCDSRtn, CFE_ES_ERR_MEM_HANDLE, 2);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle2, CFE_TBL_SRC_FILE, "TblSrcFileName.dat");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_UpdateCriticalTblCDS", 
              "14027 - Error updating CDS (second time)");

    /* Test Case 14028 - UpdateCriticalTblCDS - Failure to find CDS handle in CDS Registry */
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_GetAddress(&TblPtr, App1TblHandle2);      
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_TBL_INFO_UPDATED) && EventsCorrect), 
              "CFE_TBL_UpdateInternal", 
              "14028 - Setup Step 1");
    RtnCode = CFE_TBL_ReleaseAddress(App1TblHandle2);      
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_UpdateInternal", 
              "14028 - Setup Step 2");
              
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table2");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    UT_SetRtnCode(&ES_CopyToCDSRtn, CFE_ES_ERR_MEM_HANDLE, 2);
    UT_ClearEventHistory();
    AccessDescPtr = &CFE_TBL_TaskData.Handles[App1TblHandle2];
    RegRecPtr = &CFE_TBL_TaskData.Registry[AccessDescPtr->RegIndex];
    i = 0;
    while (CFE_TBL_TaskData.CritReg[i].CDSHandle != RegRecPtr->CDSHandle)
    {
        i++;
    }
    CFE_TBL_TaskData.CritReg[i].CDSHandle = CFE_ES_CDS_BAD_HANDLE - 1;
    RtnCode = CFE_TBL_Load(App1TblHandle2, CFE_TBL_SRC_FILE, "TblSrcFileName.dat");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_UpdateCriticalTblCDS", 
              "14028 - Failure to find CDS handle in CDS Registry");
    CFE_TBL_TaskData.CritReg[i].CDSHandle = RegRecPtr->CDSHandle;      

    UT_ClearEventHistory();
    UT_SetAppID(2);      
    RtnCode = CFE_TBL_Share(&App2TblHandle1, "ut_cfe_tbl.UT_Table1");
    UT_Report((RtnCode == CFE_SUCCESS), 
              "CFE_TBL_RemoveAccessLink",
              "14029 - Test Setup Part 1");
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Unregister(App2TblHandle1);
    EventsCorrect = (UT_GetNumEventsSent() == 0); 
    UT_Report(((RtnCode == CFE_SUCCESS) && EventsCorrect), 
              "CFE_TBL_RemoveAccessLink", 
              "14029 - Unregister shared table");
        
    UT_ClearEventHistory();
    UT_SetAppID(1);
    AccessDescPtr = &CFE_TBL_TaskData.Handles[App1TblHandle1];
    RegRecPtr = &CFE_TBL_TaskData.Registry[AccessDescPtr->RegIndex];
    CFE_TBL_TaskData.DumpControlBlocks[3].State = CFE_TBL_DUMP_PENDING;
    CFE_TBL_TaskData.DumpControlBlocks[3].RegRecPtr = RegRecPtr;
    RegRecPtr->LoadInProgress = 1;
    CFE_TBL_TaskData.LoadBuffs[1].Taken = TRUE;
    CFE_TBL_CleanUpApp(1);
    UT_Report(((CFE_TBL_TaskData.DumpControlBlocks[3].State == CFE_TBL_DUMP_FREE) &&
               (RegRecPtr->OwnerAppId == (uint32)CFE_TBL_NOT_OWNED) &&
               (CFE_TBL_TaskData.LoadBuffs[RegRecPtr->LoadInProgress].Taken == FALSE) &&
               (RegRecPtr->LoadInProgress == CFE_TBL_NO_LOAD_IN_PROGRESS)), 
              "CFE_TBL_CleanUpApp",
              "14030 - Execute Clean Up");
              
	StdFileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	StdFileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(TblFileHeader.TableName, "ut_cfe_tbl.UT_Table2");
    TblFileHeader.NumBytes = sizeof(UT_Table1_t);
    TblFileHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &TblFileHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &TblFileHeader.Offset);
    }
	UT_SetReadBuffer(&TblFileHeader, sizeof(CFE_TBL_File_Hdr_t));
	UT_SetReadHeader(&StdFileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    UT_SetRtnCode(&ES_CopyToCDSRtn, CFE_ES_ERR_MEM_HANDLE, 2);
    UT_ClearEventHistory();
    RtnCode = CFE_TBL_Load(App1TblHandle2, CFE_TBL_SRC_FILE, "TblSrcFileName.dat");
    EventsCorrect = ((UT_EventIsInHistory(CFE_TBL_LOAD_ERR_EID) == TRUE) && (UT_GetNumEventsSent() == 1)); 
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_HANDLE) && EventsCorrect), 
              "CFE_TBL_ValidateHandle", 
              "14031 - Try to use an old handle");
}

void Test_CFE_TBL_DCRs(void)
{
    int32                       RtnCode;
    int32                       RtnCode2;
    boolean                     EventsCorrect;
	CFE_FS_Header_t             FileHeader;
    UT_TempFile_t               File;
    int32                       EndianCheck = 0x01020304;
    uint32                      Index;
    CFE_TBL_Info_t              TblInfo1;
    CFE_TBL_Info_t              TblInfo2;
    CFE_TBL_TlmRegCmd_t         TlmRegCmd;
    UT_Table1_t                *TblDataPtr;
    char                        MyFilename[OS_MAX_PATH_LEN];
    
    /* Start with a clean slate */
    UT_InitData();
    UT_SetAppID(1);
    UT_ResetCDS();
    CFE_TBL_EarlyInit();
    
    /* DCR 4646 - Critical Tables do not update CDS on initial load */
    UT_ClearEventHistory();
    /* Register a Critical Table */
    RtnCode = CFE_TBL_Register(&App1TblHandle1, "DCR4646_Tbl", sizeof(UT_Table1_t), 
                               (CFE_TBL_OPT_DEFAULT | CFE_TBL_OPT_CRITICAL), NULL);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "DCR 4646", 
              "Setup - Part 1");
    
    /* Configure for successful file read to initialize table */       
	FileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
	FileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
	strcpy(File.TblHeader.TableName, "ut_cfe_tbl.DCR4646_Tbl");
    File.TblHeader.NumBytes = sizeof(UT_Table1_t);
    File.TblHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &File.TblHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &File.TblHeader.Offset);
    }
    File.TblData.TblElement1 = 0x04030201;
    File.TblData.TblElement2 = 0x08070605;
	UT_SetReadBuffer(&File, sizeof(UT_TempFile_t));
	UT_SetReadHeader(&FileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    
    /* Perform load and verify CDS contents */
    RtnCode = CFE_TBL_Load(App1TblHandle1, CFE_TBL_SRC_FILE, "MyInputFile");
    EventsCorrect = ((UT_GetNumEventsSent() == 1) && 
                     (UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE));
    Index = CFE_TBL_TaskData.Registry[CFE_TBL_TaskData.Handles[App1TblHandle1].RegIndex].CDSHandle;
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && 
               (UT_CDS[Index+0] == 0x01) && (UT_CDS[Index+1]==0x02) &&
               (UT_CDS[Index+2] == 0x03) && (UT_CDS[Index+3]==0x04) && 
               (UT_CDS[Index+4] == 0x05) && (UT_CDS[Index+5]==0x06) &&
               (UT_CDS[Index+6] == 0x07) && (UT_CDS[Index+7]==0x08)), 
              "DCR 4646", 
              "Critical Tables do not update CDS on initial load");
    
    /***********************************************************************/
    /* Start with a clean slate */
    UT_InitData();
    UT_SetAppID(1);
    UT_ResetCDS();
    CFE_TBL_EarlyInit();
    
    /* DCR 1714 - CFE_TBL_GetInfo doesn't set Critical Flag to True for Critical Tables */
    UT_ClearEventHistory();
    /* Register a Critical Table */
    RtnCode = CFE_TBL_Register(&App1TblHandle1, "DCR1714_CTbl", sizeof(UT_Table1_t), 
                               (CFE_TBL_OPT_DEFAULT | CFE_TBL_OPT_CRITICAL), NULL);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "DCR 1714", 
              "Setup - Part 1");

    /* Register a Non Critical Table */
    RtnCode = CFE_TBL_Register(&App1TblHandle2, "DCR1714_NTbl", sizeof(UT_Table1_t), 
                               CFE_TBL_OPT_DEFAULT, NULL);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "DCR 1714", 
              "Setup - Part 2");

    RtnCode  = CFE_TBL_GetInfo(&TblInfo1, "ut_cfe_tbl.DCR1714_CTbl");
    RtnCode2 = CFE_TBL_GetInfo(&TblInfo2, "ut_cfe_tbl.DCR1714_NTbl");
    
    UT_Report(((RtnCode == CFE_SUCCESS) && (RtnCode2 == CFE_SUCCESS) && 
               (EventsCorrect) && (TblInfo1.Critical == TRUE) &&
               (TblInfo2.Critical == FALSE)), 
              "DCR 1714", 
              "CFE_TBL_GetInfo doesn't set Critical Flag to True for Critical Tables");

    /***********************************************************************/
    /* Start with a clean slate */
    UT_InitData();
    UT_SetAppID(1);
    UT_ResetCDS();
    CFE_TBL_EarlyInit();
    
    /* DCR 2846 - Table Registry dumps do not reflect the File Create Times of last load files */
    UT_ClearEventHistory();
    
    /* Register a Non Critical Table */
    RtnCode = CFE_TBL_Register(&App1TblHandle1, "DCR2846_Tbl", sizeof(UT_Table1_t), 
                               CFE_TBL_OPT_DEFAULT, NULL);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "DCR 2846", 
              "Setup - Part 1");
    
    /* Configure for successful file read to initialize table */       
    FileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
    FileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
    FileHeader.TimeSeconds = 2846;
    FileHeader.TimeSubSeconds = 103;
    strcpy(File.TblHeader.TableName, "ut_cfe_tbl.DCR2846_Tbl");
    File.TblHeader.NumBytes = sizeof(UT_Table1_t);
    File.TblHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &File.TblHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &File.TblHeader.Offset);
    }
    File.TblData.TblElement1 = 0x04030201;
    File.TblData.TblElement2 = 0x08070605;
    UT_SetReadBuffer(&File, sizeof(UT_TempFile_t));
    UT_SetReadHeader(&FileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    
    /* Perform load and verify Time Stamp values */
    RtnCode = CFE_TBL_Load(App1TblHandle1, CFE_TBL_SRC_FILE, "MyInputFile");
    EventsCorrect = ((UT_GetNumEventsSent() == 1) && 
                     (UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE));
    CFE_TBL_GetInfo(&TblInfo1, "ut_cfe_tbl.DCR2846_Tbl");
    
    /* Create a Tlm Table Registry Command */
    strcpy(&TlmRegCmd.TableName[0], "ut_cfe_tbl.DCR2846_Tbl");
    RtnCode2 = CFE_TBL_TlmRegCmd((CFE_SB_Msg_t *)&TlmRegCmd);
    CFE_TBL_HousekeepingCmd((CFE_SB_Msg_t *)&TlmRegCmd);
    fprintf(UT_logfile, "TblInfo: %ld.%ld, TblRegPkt: %ld.%ld\n", TblInfo1.FileCreateTimeSecs, TblInfo1.FileCreateTimeSubSecs, 
                                                 CFE_TBL_TaskData.TblRegPacket.FileCreateTimeSecs, 
                                                 CFE_TBL_TaskData.TblRegPacket.FileCreateTimeSubSecs);
    UT_Report(((RtnCode == CFE_SUCCESS) && 
               (RtnCode2 == CFE_TBL_INC_CMD_CTR) && 
               (EventsCorrect) &&
               (TblInfo1.FileCreateTimeSecs == 2846) &&
               (TblInfo1.FileCreateTimeSubSecs == 103) &&
               (CFE_TBL_TaskData.TblRegPacket.FileCreateTimeSecs == 2846) &&
               (CFE_TBL_TaskData.TblRegPacket.FileCreateTimeSubSecs == 103)), 
              "DCR 2846", 
              "Table Registry Load File Creation Times are incorrectly reported");
    

    /***********************************************************************/
    /* Start with a clean slate */
    UT_InitData();
    UT_SetAppID(1);
    UT_ResetCDS();
    CFE_TBL_EarlyInit();
    
    /* DCR 1704 - Adding CFE_TBL_Modified API function call */
    UT_ClearEventHistory();
    
    /* Register a Non Critical Table */
    RtnCode = CFE_TBL_Register(&App1TblHandle1, "DCR1704_Tbl", sizeof(UT_Table1_t), 
                               CFE_TBL_OPT_CRITICAL, NULL);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "DCR 1704 - Part 1", 
              "Setup");
    
    /* Configure for successful file read to initialize table */       
    FileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
    FileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
    FileHeader.TimeSeconds = 1704;
    FileHeader.TimeSubSeconds = 104;
    strcpy(File.TblHeader.TableName, "ut_cfe_tbl.DCR1704_Tbl");
    File.TblHeader.NumBytes = sizeof(UT_Table1_t);
    File.TblHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &File.TblHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &File.TblHeader.Offset);
    }
    File.TblData.TblElement1 = 0x04030201;
    File.TblData.TblElement2 = 0x08070605;
    UT_SetReadBuffer(&File, sizeof(UT_TempFile_t));
    UT_SetReadHeader(&FileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    
    /* Perform load */
    RtnCode = CFE_TBL_Load(App1TblHandle1, CFE_TBL_SRC_FILE, "MyInputFile");
    EventsCorrect = ((UT_GetNumEventsSent() == 1) && 
                     (UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE));
    
    /* Modify the contents of the table */
    CFE_TBL_GetAddress((void **)&TblDataPtr, App1TblHandle1);
    TblDataPtr->TblElement1 = 0x01020304;
    
    /* Notify Table Services that the table has been modified */
    RtnCode = CFE_TBL_Modified(App1TblHandle1);
    RtnCode2 = CFE_TBL_GetInfo(&TblInfo1, "ut_cfe_tbl.DCR1704_Tbl");
    Index = CFE_TBL_TaskData.Registry[CFE_TBL_TaskData.Handles[App1TblHandle1].RegIndex].CDSHandle;
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && 
               (TblInfo1.TimeOfLastUpdate.Seconds == TblInfo1.TimeOfLastUpdate.Subseconds) &&
               (strcmp(&TblInfo1.LastFileLoaded[0], "MyInputFile(*)") == 0) &&
               (UT_CDS[Index+0] == 0x04) && (UT_CDS[Index+1]==0x03) &&
               (UT_CDS[Index+2] == 0x02) && (UT_CDS[Index+3]==0x01) && 
               (UT_CDS[Index+4] == 0x05) && (UT_CDS[Index+5]==0x06) &&
               (UT_CDS[Index+6] == 0x07) && (UT_CDS[Index+7]==0x08)), 
               "DCR 1704 - Part 1",
               "Add TBL API for notifying Table services that table has been updated by Application");

    /* DCR 1704 - Part 2 - LONG Filename */
    UT_ClearEventHistory();
    
    /* Register a Non Critical Table */
    RtnCode = CFE_TBL_Register(&App1TblHandle1, "DCR1704-2_Tbl", sizeof(UT_Table1_t), 
                               CFE_TBL_OPT_DEFAULT, NULL);
    EventsCorrect = (UT_GetNumEventsSent() == 0);
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect)), 
              "DCR 1704 - Part 2", 
              "Setup");
    
    /* Configure for successful file read to initialize table */       
    FileHeader.ContentType = CFE_FS_FILE_CONTENT_ID;
    FileHeader.SubType = CFE_FS_TBL_IMG_SUBTYPE;
    FileHeader.TimeSeconds = 1704;
    FileHeader.TimeSubSeconds = 104;
    strcpy(File.TblHeader.TableName, "ut_cfe_tbl.DCR1704-2_Tbl");
    File.TblHeader.NumBytes = sizeof(UT_Table1_t);
    File.TblHeader.Offset = 0;
    if ((*(char *)&EndianCheck) == 0x04)
    {
        CFE_TBL_ByteSwapUint32( &File.TblHeader.NumBytes);
        CFE_TBL_ByteSwapUint32( &File.TblHeader.Offset);
    }
    File.TblData.TblElement1 = 0x04030201;
    File.TblData.TblElement2 = 0x08070605;
    UT_SetReadBuffer(&File, sizeof(UT_TempFile_t));
    UT_SetReadHeader(&FileHeader, sizeof(CFE_FS_Header_t));
    UT_SetRtnCode(&OSReadRtn, 0, 3);
    
    /* Perform load with extra long filename */
    for (Index = 0; Index<(OS_MAX_PATH_LEN-1); Index++)
    {
        MyFilename[Index] = 'a';
    }
    MyFilename[(OS_MAX_PATH_LEN-1)] = '\0';
    RtnCode = CFE_TBL_Load(App1TblHandle1, CFE_TBL_SRC_FILE, MyFilename);
    EventsCorrect = ((UT_GetNumEventsSent() == 1) && 
                     (UT_EventIsInHistory(CFE_TBL_LOAD_SUCCESS_INF_EID) == TRUE));
    
    /* Notify Table Services that the table has been modified */
    RtnCode = CFE_TBL_Modified(App1TblHandle1);
    RtnCode2 = CFE_TBL_GetInfo(&TblInfo1, "ut_cfe_tbl.DCR1704-2_Tbl");
    UT_Report(((RtnCode == CFE_SUCCESS) && (EventsCorrect) && 
               (TblInfo1.TimeOfLastUpdate.Seconds == TblInfo1.TimeOfLastUpdate.Subseconds) &&
               (strncmp(&TblInfo1.LastFileLoaded[0], MyFilename, (OS_MAX_PATH_LEN-4)) == 0) &&
               (strncmp(&TblInfo1.LastFileLoaded[(OS_MAX_PATH_LEN-4)], "(*)", 3) == 0)),
               "DCR 1704 - Part 2",
               "Add TBL API for notifying Table services that table has been updated by Application");


    /* DCR 1704 - Part 3 - Invalid Handle */
    RtnCode = CFE_TBL_Modified(CFE_TBL_BAD_TABLE_HANDLE);
    UT_Report(((RtnCode == CFE_TBL_ERR_INVALID_HANDLE) && (EventsCorrect)),
               "DCR 1704 - Part 3",
               "Add TBL API for notifying Table services that table has been updated by Application");
}

int32 Test_CFE_TBL_ValidationFunc(void *TblPtr)
{
    if(TBL_ValidationFuncRtn.count > 0)
    {
      TBL_ValidationFuncRtn.count--;
      if(TBL_ValidationFuncRtn.count == 0)
        return TBL_ValidationFuncRtn.value;
    }
    return 0;
}
