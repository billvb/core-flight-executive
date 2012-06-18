/*
** File:
** $Id: ut_stubs.h 1.6 2010/10/20 12:40:44EDT jmdagost Exp  $
**
** Purpose:
** Unit specification for Unit Test Stubs
**
** Notes:
** These routines contain a minimum amount of functionality required for
** unit testing full path coverage
**
** $Date: 2010/10/20 12:40:44EDT $
** $Revision: 1.6 $
** $Log: ut_stubs.h  $
** Revision 1.6 2010/10/20 12:40:44EDT jmdagost 
** Added UseMutex element to Pool_t structure.
** Revision 1.5 2009/05/06 09:57:03EDT rmcgraw 
** DCR7366:1 Increased event history from 10 to 64
** Revision 1.4 2009/04/28 16:25:15EDT rmcgraw 
** DCR7366:1 Added utility UT_CheckForOpenSockets
** Revision 1.3 2009/04/01 16:15:16EDT rmcgraw 
** DCR7366:1 Added 'getActual...' and DisplayPkt utilities
** Revision 1.2 2008/08/15 11:26:53EDT njyanchik 
** Check in of ES Unit Test
** Revision 1.1 2008/04/17 13:05:47BST ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
** Revision 1.20 2007/09/21 19:02:57EDT David Kobe (dlkobe) 
** Updated table unit tests to stop segmentation fault
** Revision 1.19 2007/07/08 22:31:27EDT dlkobe 
** Enhanced CDS Simulation
** Revision 1.18 2007/07/05 15:04:31EDT dlkobe 
** Baseline following complete TBL unit testing
** Revision 1.17 2007/05/25 09:17:56EDT njyanchik 
** I added the rmfs call to the OSAL and updated the unit test stubs to match
** Revision 1.16 2007/05/24 15:48:09EDT njyanchik 
** Update for ES Unit Test
** Revision 1.15 2007/05/23 10:54:59EDT njyanchik 
** Update again to prevent loss of work
** Revision 1.14 2007/05/22 13:03:57EDT njyanchik 
** Updated changes to not lose my work
** Revision 1.13 2007/05/16 11:14:36EDT njyanchik 
** Update ES's unit test driver to match code for build 4.1
** Revision 1.10 2007/05/01 13:28:24EDT njyanchik 
** I updated the ut stubs to get the each of the subsytems to compile under the unit test. I did not
** change the unit tests themselves to cover more of the files, however.
** Revision 1.9 2007/03/02 15:18:04EST njyanchik 
** Jonathan provided me with a newer tbl unit test to check in
** Revision 1.8 2007/01/18 13:16:10GMT-05:00 njyanchik 
** I fixed an errant "." (dot)
** Revision 1.7 2007/01/17 10:04:20GMT-05:00 njyanchik 
** This CP Checks in cleaned-up versions of the files checked in for the ES unit test
** Revision 1.6 2007/01/17 09:26:09GMT-05:00 njyanchik 
** Check in of ES Unit Test changed files
** Revision 1.5 2006/11/02 13:53:57GMT-05:00 njyanchik 
** Unit test for TIME was updated to match the changes made for this DCR
** Revision 1.4 2006/11/01 12:46:54GMT-05:00 njyanchik 
** Changed the Unit test to reflect the changes from removing the CDS functionality from TIME
** Revision 1.3 2006/06/08 14:11:20GMT-05:00 rjmcgraw 
** Added support for function UT_SetRtnCode
** Revision 1.2 2006/05/17 13:56:45GMT jjhageman 
** Added UT_Queue for use by the osapi stubs (OS_Queue create, get, set)
** Revision 1.1 2006/03/02 15:10:26EST jjhageman 
** Initial revision
** Member added to project d:/mksdata/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
*/
#ifndef _ut_stubs_
#define _ut_stubs_

/*
** Macro Definitions
*/

#define OS_CREAT_FAIL           0x0001
#define OS_WRITE_FAIL           0x0002
#define OS_CLOSE_FAIL           0x0004
#define OS_MKFS_FAIL            0x0008
#define OS_INITFS_FAIL          0x0010
#define OS_MOUNT_FAIL           0x0020
#define OS_TASKCREATE_FAIL      0x0040
#define OS_SEMCREATE_FAIL       0x0080
#define OS_MUTCREATE_FAIL       0x0100
#define OS_OPEN_FAIL            0x0200
#define OS_TASKDELAY_FAIL       0x0400
#define OS_TASKREGISTER_FAIL    0x0800
#define OS_READ_FAIL		    0x1000 
#define OS_LSEEK_FAIL           0x2000
#define OS_TASKDELETE_FAIL      0x4000
#define OS_RMFS_FAIL            0x8000

#define BSP_WRITECDS_FAIL       0x0001
#define BSP_READCDS_FAIL        0x0002
#define BSP_GETCDSSIZE_FAIL     0x0004
#define BSP_GETVOLDISKMEM_FAIL  0x0008

/*
** Includes
*/


#include "cfe_sb.h"
#include "cfe_es.h"
#include "common_types.h"
#include "../evs/cfe_evs_task.h"
#include "../es/cfe_es_global.h"
#include "../es/cfe_es_cds.h"
#include "../time/cfe_time_utils.h"

#define UT_EVENT_HISTORY_SIZE 64
#define UT_CDS_SIZE 20000
#define UT_MAX_NUM_CDS 20

/*
** Type Definitions
*/

typedef struct
{
  char name[OS_MAX_API_NAME];
  int id;
  boolean free;
  int stack_size;
  int priority;
} UT_Task_t;

typedef struct
{
  char name[OS_MAX_API_NAME];
  int id;
  boolean free;
} UT_Queue_t;

typedef struct
{
  uint32 count;
  uint32 value;
}UT_SetRtn_t;

typedef struct
{
  uint32             NextHandle;
  CFE_ES_CDSHandle_t Handles[UT_MAX_NUM_CDS];
} UT_CDS_Map_t;

char UT_ReadBuf [100000];
char UT_ReadHdr [10000];
int  UT_DummyFuncRtn;
int UT_BSPLoadGoodFile;
int UT_SizeofESResetArea;
int UT_ReadBufOffset;
int UT_ReadHdrOffset;
int UT_CDS_Size;
int UT_SB_TotalMsgLen;




/**************************************************************************/    
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* CDS type declarations */
#define CFE_ES_CDS_CHECK_PATTERN  0x5a5a
    #define CFE_ES_CDS_BLOCK_USED     0xaaaa
    #define CFE_ES_CDS_BLOCK_UNUSED   0xdddd
    /*#define CFE_ES_CDS_MAX_BLOCK_SIZE CFE_SB_MAX_SB_MSG_SIZE*/
    #define CFE_ES_CDS_NUM_BLOCK_SIZES 17
    
    /* MIN_BLOCK_SIZE must be < 16 bytes */
    #define CFE_ES_CDS_MIN_BLOCK_SIZE 8
/*
** Type Definitions
*/



    typedef struct
    {
      uint16    CheckBits;
      uint16    AllocatedFlag;
      uint32    SizeUsed;
      uint32    ActualSize;
      uint32    CRC;
      uint32    Next;
    } CFE_ES_CDSBlockDesc_t;

    typedef struct
    {
       uint32   Top;
       uint32   Cntr;
       uint32   MaxSize;
    } CFE_ES_CDSBlockSizeDesc_t;
/*
** Memory Pool Type
*/
    typedef struct {
       uint32   Start;
       uint32   Size;
       uint32   End;
       uint32   Current;
       int32    SizeIndex;
       uint16   CheckErrCntr;
       uint16   RequestCntr;
       uint32   MutexId;
       CFE_ES_CDSBlockSizeDesc_t SizeDesc[CFE_ES_CDS_NUM_BLOCK_SIZES];
    } CFE_ES_CDSPool_t;

typedef struct
{
  uint16    CheckBits;
  uint16    Allocated;
  uint32    Size;
  uint32   *Next;
} OS_PACK BD_t;

typedef struct
{
  BD_t    *Top;
  uint32   NumCreated;
  uint32   NumFree;
  uint32   MaxSize;
} BlockSizeDesc_t;
typedef struct {
   uint32          *Start;
   uint32           Size;
   uint32           End;
   uint32          *Current;
   BlockSizeDesc_t *SizeDescPtr;
   uint16           CheckErrCntr;
   uint16           RequestCntr;
   uint32           MutexId;
   uint32           UseMutex;
   BlockSizeDesc_t  SizeDesc[CFE_ES_MAX_MEMPOOL_BLOCK_SIZES];
} OS_PACK Pool_t;



/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/******************************************************************/





/*
** Exported Functions
*/

/*
** Initialize unit test - sets up logging file and initialized utilities
**
** Note: includes possible OS dependent calls
*/
void UT_Init(char *);

/*
** Initialize unit test variables.  Called by UT_Init, normally
**
*/
void UT_InitData(void);

/*
** Unit specific call to process SB messages
**
** Note: this gets defined in subsystem_UT.c file since it is
**       specific to each subsystem
*/
void UT_ProcessSBMsg(CFE_SB_Msg_t *MsgPtr);

/*
** Does the required work to send a software bus message
*/
void UT_SendMsg(CFE_SB_MsgPtr_t, CFE_SB_MsgId_t, uint16);

/*
** Set the application ID returned by unit test stubs
*/
void UT_SetAppID(int32);

/*
** Set the OS fail flag
*/
void UT_SetOSFail(uint32);

/*
** Set putpool fail flag
*/
void UT_SetPutPoolFail(uint32);


/* 
** Generic function used to tell stubs to return 'rtnVal' when call 'cnt' = 0
*/
void UT_SetRtnCode (UT_SetRtn_t *varPtr,int32 rtnVal,int32 cnt);

/*
** Set the recieve message failure
*/
void UT_SetRcvMsg(CFE_SB_MsgId_t, uint16);

/*
** Set CDS signature
*/
void UT_SetSignatureCDS(uint32);

/*
** Set CDS status
*/
void UT_SetStatusCDS(uint32);

/*
** Set time counter
*/
void UT_SetLocTime (uint32, uint32);

/*
** Set BinSemFail
*/
void UT_SetBinSemFail (uint32);

/*
** Assert function which writes to log vile
*/
void UT_Report(boolean, char *, char *);

/*
** Final report on pass/fails to be called at the end of testing
*/
void UT_ReportFailures(void);

/* Sets the return code of  OS_BSPGetResetArea and the signature */
void UT_SetStatusBSPResetArea(int32, uint32 );

void OS_BSPGetTime( OS_time_t *LocalTime);
#endif /* _ut_stubs_ */


/* Used to set the reading from file (suchas the startup script */
void UT_SetReadBuffer(void *Buff,int NumBytes);

void UT_SetReadHeader(void *Hdr, int NumBytes);

/* Used to Get return code when ES is starting tasks by pointers
 * The dummy function is started instead of actualy tasks */
void UT_SetDummyFuncRtn(int Return);

/* Sets the return for BSP_LoadAppFromFile */
void UT_SetBSPloadAppFileResult (int Result);

/* Sets the size of the ES reset area gotten from the BSP */
void UT_SetSizeofESResetArea(int32 Size);

/* A multipurpose function used to set return codes of various BSP calls */
void UT_SetBSPFail(uint32 Fail);

/* Sets the CDS size as returned by the BSP */
void UT_SetCDSSize(int32 Size);

/* Sets the SB Total Message length from the SB call */
void UT_SetSBTotalMsgLen(int Size);

/* When reading from the BSP, sets the reading of the end 
 * Validity String to a correct value or not 
 *  Used by CFE_ES_ValidateCDS 
 *  */
void UT_SetCDSReadGoodEnd(boolean Truth);

/* Sets whether or not we are trying to check the 
 * validity of the BSP, to not upset other functions
 * trying to use OS_BSPReadFromCDS */
void UT_SetCDSBSPCheckValidity(boolean Truth);

/* Clears the Event History so that one test's event messages
 * won't be confused with the next
 * */
void UT_ClearEventHistory(void);

/* Adds an event ID to the Event History */
void UT_AddEventToHistory(uint16 EventID);

/* Searches Event History to see if specified Event ID is present */
boolean UT_EventIsInHistory(uint16 EventIDToSearchFor);

/* Provides number of events issued since last UT_ClearEventHistory call */
uint16 UT_GetNumEventsSent(void);

/* Reset CDS so that it is a clean slate */
void UT_ResetCDS(void);

/* Get actual Pkt Length Field */
int16 UT_GetActualPktLenField(CFE_SB_MsgPtr_t MsgPtr);

/* Get Actual Cmd Code from Pkt */
uint16 UT_GetActualCmdCodeField(CFE_SB_MsgPtr_t MsgPtr);

/* Display Pkt */
void UT_DisplayPkt(CFE_SB_MsgPtr_t ptr,uint32 size);

/* Report and close any sockets left open */
void UT_CheckForOpenSockets(void);



