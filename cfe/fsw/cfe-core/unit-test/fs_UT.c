/*
**
** Name: fs_UT.c
** Author: Jonathan Oben, Intern to Nicholas Yanchik
** Last Updated: June 28, 2007
**
** Notes: This file is the unit test of the
**			File Services module of the cFE.
**			
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cfe.h"
#include "cfe_fs.h"
#include "../src/fs/cfe_fs_priv.h"
#include "common_types.h"
#include "ut_stubs.h"

#include "../src/fs/cfe_fs_priv.h"

void CFE_FS_ByteSwapCFEHeader(CFE_FS_Header_t *Hdr);
void CFE_FS_ByteSwapUint32(uint32 *Uint32ToSwapPtr);

/* Testing Function Declarations */


void Test_CFE_FS_ByteSwapUint32(void);
void Test_CFE_FS_ByteSwapCFEHeader(void);
void Test_CFE_FS_SetTimestamp(void);
void Test_CFE_FS_WriteHeader(void);
void Test_CFE_FS_ReadHeader(void);
void Test_CFE_FS_ExctractFileNameFromPath(void);
void Test_CFE_FS_IsGzFile(void);
int32 FS_gz_inflate_fixed( void );
int32 FS_gz_inflate_stored( void );
int16 FS_gz_fill_inbuf (void);
void FS_gz_flush_window( void );


void Test_CFE_FS_API(void);
void Test_CFE_FS_PRIV(void);
void Test_CFE_FS_DECOMPRESS(void);

/* Global Variables */
extern int			UT_SB_TotalMsgLen;
extern UT_SetRtn_t	FileWriteRtn;
extern UT_SetRtn_t	OSlseekRtn; 
extern UT_SetRtn_t MutSemGiveRtn;
extern UT_SetRtn_t MutSemTakeRtn;
extern UT_SetRtn_t OSReadRtn; 
extern UT_SetRtn_t OSReadRtn2; 
extern UT_SetRtn_t FileWriteRtn; 

extern uint32      gz_outcnt;


void UT_ProcessSBMsg(CFE_SB_Msg_t *MsgPtr)
{
}

/* All test functions are run in the main function */

int main(void)
{
	/* Initialize Unit Test */
	UT_Init("ut_cfe_fs");
	

	/*cfe_fs_api.c Functions */
	Test_CFE_FS_API();
    Test_CFE_FS_PRIV();
    Test_CFE_FS_DECOMPRESS();
	
    UT_ReportFailures();
    return 0;
}

/*
** Tests for cfe_fs_api.c
*/

void Test_CFE_FS_API()
{

	Test_CFE_FS_ByteSwapUint32();
	Test_CFE_FS_ByteSwapCFEHeader();
	Test_CFE_FS_SetTimestamp();
	Test_CFE_FS_WriteHeader();
	Test_CFE_FS_ReadHeader();
    Test_CFE_FS_ExctractFileNameFromPath();
    Test_CFE_FS_IsGzFile();

}

void Test_CFE_FS_ReadHeader()
{
	
	int32 FileDes;
	CFE_FS_Header_t Hdr;
	
	/* TEST */
	UT_SetRtnCode(&OSlseekRtn, OS_FS_SUCCESS, 1);
	UT_SetOSFail(OS_READ_FAIL);
	UT_Report(CFE_FS_ReadHeader(&Hdr, FileDes) == (sizeof(CFE_FS_Header_t) - 1),
				"CFE_FS_ReadHeader", "Single path through");

}

void Test_CFE_FS_WriteHeader()
{

	int32 FileDes;
	CFE_FS_Header_t Hdr;
	
	/* TEST */
	UT_SetRtnCode(&OSlseekRtn, OS_FS_SUCCESS, 1);
	UT_SetRtnCode(&FileWriteRtn, OS_FS_SUCCESS, 1);
	UT_Report(CFE_FS_WriteHeader(FileDes, &Hdr) == OS_FS_SUCCESS,
				"CFE_FS_WriteHeader", "Single path through");

}

void Test_CFE_FS_SetTimestamp()
{
	int32 FileDes;
	CFE_TIME_SysTime_t NewTimestamp;

	/* TEST1: Failed to lseek time fields */
	UT_SetOSFail(OS_LSEEK_FAIL);
	UT_Report(CFE_FS_SetTimestamp(FileDes, NewTimestamp) == OS_FS_ERROR,
				"CFE_FS_SetTimestamp", "Failed to lseek time fields");
	
	/* TEST2: Failed to write seconds */
	UT_SetOSFail(OS_WRITE_FAIL);
	UT_Report(CFE_FS_SetTimestamp(FileDes, NewTimestamp) != OS_SUCCESS,
				"CFE_FS_SetTimestamp", "Failed to write seconds");
	
	/* TEST3: Failed to write subseconds */
	UT_SetOSFail(0);
	UT_SetRtnCode(&FileWriteRtn, 0, 2);
	UT_Report(CFE_FS_SetTimestamp(FileDes, NewTimestamp) == OS_SUCCESS,
				"CFE_FS_SetTimestamp", "Failed to write subseconds");
	
	/* TEST4: Success Case */
	UT_Report(CFE_FS_SetTimestamp(FileDes, NewTimestamp) == OS_FS_SUCCESS,
				"CFE_FS_SetTimestamp", "Succeeded at writing timestamp");

}

/* function has no return value */
void Test_CFE_FS_ByteSwapCFEHeader()
{
	CFE_FS_Header_t Hdr;

	/* TEST: function has no variable paths */
	CFE_FS_ByteSwapCFEHeader(&Hdr);
}


/* function has no return value */
void Test_CFE_FS_ByteSwapUint32()
{
	uint32 test;
	uint32 *testptr = &test;
	
	/* TEST: function has no variable paths */
	CFE_FS_ByteSwapUint32(testptr);
}

void Test_CFE_FS_ExctractFileNameFromPath()
{
    char Original [50];
    char FileName [20];
    
    strcpy(Original, "/cf/appslibrary.gz");

    UT_Report( CFE_FS_ExtractFilenameFromPath( "name", FileName) == CFE_FS_INVALID_PATH,
              " CFE_FS_ExtractFilenameFromPath", "Null argument");
    
    UT_Report( CFE_FS_ExtractFilenameFromPath( NULL, FileName) == CFE_FS_BAD_ARGUMENT,
              " CFE_FS_ExtractFilenameFromPath", "Null argument");
    
    UT_Report( CFE_FS_ExtractFilenameFromPath( "/cf/1234567890123456789012345678901234567890123456789012345678901234.toolong", FileName) == CFE_FS_FNAME_TOO_LONG,
              " CFE_FS_ExtractFilenameFromPath", "Name Too Long");
    
    
    UT_Report( CFE_FS_ExtractFilenameFromPath( Original, FileName) == CFE_SUCCESS,
              " CFE_FS_ExtractFilenameFromPath", "Successful");
}

void Test_CFE_FS_IsGzFile()
{
    UT_Report(CFE_FS_IsGzFile("a") == FALSE, "CFE_FS_IsGzFile", "Too Short");
    UT_Report(CFE_FS_IsGzFile("tar.gz") == TRUE,"CFE_FS_IsGzFile", "Success");
    UT_Report(CFE_FS_IsGzFile("Normal") == FALSE,"CFE_FS_IsGzFile", "Success");
    UT_Report(CFE_FS_IsGzFile(NULL) == FALSE,"CFE_FS_IsGzFile", "Null Name");    
}






void Test_CFE_FS_PRIV()
{
    

    UT_Report(CFE_FS_EarlyInit() == CFE_SUCCESS,"CFE_FS_EarlyInit", "Success");    
    UT_SetOSFail(OS_MUTCREATE_FAIL);
    UT_Report(CFE_FS_EarlyInit() == -1 ,"CFE_FS_EarlyInit", "Mutex Creation failure");    
    UT_SetOSFail(0); 


    CFE_FS_LockSharedData("FunctionName");

    UT_SetRtnCode (& MutSemTakeRtn, -1,1);
    CFE_FS_LockSharedData("FunctionName");
    
    CFE_FS_UnlockSharedData("FunctionName");
    
    UT_SetRtnCode (& MutSemGiveRtn, -1,1);
    CFE_FS_UnlockSharedData("FunctionName");
    
}

void Test_CFE_FS_DECOMPRESS(void)
{
    
    char Buffer[100000];
    
    int fd;
    int NumBytes;
    
    UT_SetOSFail(OS_OPEN_FAIL);
    UT_Report(CFE_FS_Decompress(" Filename.gz", "Output") == CFE_FS_GZIP_OPEN_INPUT ,"CFE_FS_Decompress", "Open Fail");    
    UT_SetOSFail(0);

    UT_SetOSFail(OS_CREAT_FAIL);
    UT_Report(CFE_FS_Decompress(" Filename.gz", "Output") == CFE_FS_GZIP_OPEN_OUTPUT ,"CFE_FS_Decompress", "creat Fail");    
    UT_SetOSFail(0);
    
    fd = open("../fs_test.gz", O_RDONLY);


    NumBytes = read(fd, Buffer, 100000);

    UT_SetReadBuffer(Buffer,NumBytes);    
    
    UT_Report(CFE_FS_Decompress("fs_test.gz", "Output") == CFE_SUCCESS ,"CFE_FS_Decompress", "read Fail"); 
    
    close (fd);
    
    fd = open("fs_UT", O_RDONLY);

    NumBytes = read(fd, Buffer, 100000);
    
    UT_SetReadBuffer(Buffer,NumBytes);    
    
    UT_Report(CFE_FS_Decompress("fake", "Output") == CFE_FS_GZIP_NON_ZIP_FILE ,"CFE_FS_Decompress", "read Fail"); 
    
    close (fd);
    
    UT_Report(FS_gz_inflate_fixed() > CFE_SUCCESS, "FS_gz_inflate_fixed", "failure");
    UT_Report(FS_gz_inflate_stored() < CFE_SUCCESS, "FS_gz_inflate_stored", "failure");
    
    /* Get len == OS_FS_ERROR */
    UT_SetRtnCode(&OSReadRtn, 4, 1);
    UT_SetRtnCode(&OSReadRtn2, OS_FS_ERROR, 1);
    FS_gz_fill_inbuf();
    
       UT_SetRtnCode(&FileWriteRtn, -1, 1); 
    gz_outcnt = 435;
    FS_gz_flush_window( );
    

    
    
    
    
}


