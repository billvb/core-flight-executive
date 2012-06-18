/*
** File:
** $Id: ut_fs_stubs.c 1.4 2010/10/25 18:21:50EDT jmdagost Exp  $
**
** Purpose:
** Unit test stubs for File Service routines
** 
** Notes:
** Minimal work is done, only what is required for unit testing
**
** $Data:$
** $Revision: 1.4 $
** $Log: ut_fs_stubs.c  $
** Revision 1.4 2010/10/25 18:21:50EDT jmdagost 
** Updated CFE_FS_ExtractFilenameFromPath().
** Revision 1.3 2008/08/28 08:52:19EDT apcudmore 
** changed CFS_ call back to CFE_
** Revision 1.2 2008/08/15 11:26:53EDT njyanchik 
** Check in of ES Unit Test
** Revision 1.1 2008/04/17 13:05:45BST ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
** Revision 1.10 2007/07/05 15:04:28EDT David Kobe (dlkobe) 
** Baseline following complete TBL unit testing
** Revision 1.9 2007/05/10 15:14:25EDT njyanchik 
** Another update of Jonathans UT
** Revision 1.8 2007/05/01 13:28:15EDT njyanchik 
** I updated the ut stubs to get the each of the subsytems to compile under the unit test. I did not
** change the unit tests themselves to cover more of the files, however.
** Revision 1.7 2007/03/16 15:12:12EST dlkobe 
** Swapped parameter list for CFE_FS_ReadHeader API.
** Revision 1.6 2007/03/02 15:18:05EST njyanchik 
** Jonathan provided me with a newer tbl unit test to check in
** Revision 1.4 2006/11/08 15:09:27GMT-05:00 rjmcgraw 
** Changed return in WriteHeader API to sizeof(CFE_FS_Header_t)
*/

/*
** Include section
*/

#include "stdio.h"
#include <string.h>

#include "cfe_fs.h"
#include "common_types.h"
#include "ut_stubs.h"

/*
** Exported Global Data
*/

extern FILE         *UT_logfile;
extern UT_SetRtn_t  FSWriteHdrRtn;
extern UT_SetRtn_t  FSReadHdrRtn;
extern int			UT_ReadHdrOffset;
extern UT_SetRtn_t	FSSetTimestampRtn;
extern UT_SetRtn_t FSIsGzFileRtn;
extern UT_SetRtn_t FSDecompressRtn;
extern UT_SetRtn_t FSExtractRtn;

/*
** Function definitions
*/

int32 CFE_FS_WriteHeader (int32 filedes, CFE_FS_Header_t *hdr)
{
  if(FSWriteHdrRtn.count > 0)
  {
      FSWriteHdrRtn.count--;
      if(FSWriteHdrRtn.count == 0)
        return FSWriteHdrRtn.value;
  }

  fprintf(UT_logfile, "  CFE_FS_WriteHeader(%ld): %s\n",
          filedes, hdr->Description);

  return sizeof(CFE_FS_Header_t);
}

int32 CFE_FS_ReadHeader(CFE_FS_Header_t *Hdr, int32 FileDes)
{
    if(FSReadHdrRtn.count > 0)
    {
        FSReadHdrRtn.count--;
        if(FSReadHdrRtn.count == 0)
          return FSReadHdrRtn.value;
    }

	memcpy(Hdr, &UT_ReadHdr[UT_ReadHdrOffset], sizeof(CFE_FS_Header_t));
	UT_ReadHdrOffset += sizeof(CFE_FS_Header_t);
	
  fprintf(UT_logfile, "  CFE_FS_ReadHeader(%ld): %s\n",
          FileDes, Hdr->Description);

     return sizeof(CFE_FS_Header_t);
}

int32 CFE_FS_SetTimestamp(int32 FileDes, CFE_TIME_SysTime_t NewTimestamp)
{
	if(FSSetTimestampRtn.count > 0)
	{
      FSSetTimestampRtn.count--;
      if(FSSetTimestampRtn.count == 0)
        return FSSetTimestampRtn.value;
	}
	
      fprintf(UT_logfile, "  CFE_FS_SetTimeStamp: Called \n");

    return OS_FS_SUCCESS;
}

/*njy*/
int32 CFE_FS_EarlyInit (void) 
{
    return 0;
}


int32 CFE_FS_ExtractFilenameFromPath(char *OriginalPath, char *FileNameOnly)
{
    int    i,j;
    int    StringLength;
    int    DirMarkIdx;
    int32  ReturnCode;
    
    if(FSExtractRtn.count > 0)
	{
        FSExtractRtn.count--;
        if(FSExtractRtn.count == 0)
            return FSExtractRtn.value;
	}
    else
    {
        if ( OriginalPath == NULL || FileNameOnly == NULL )
        {
           ReturnCode = CFE_FS_BAD_ARGUMENT;
        }
        else
        {
      
            /*
            ** Get the string length of the original file path
            */
            StringLength = strlen(OriginalPath);
   
            /*
            ** Extract the filename from the Path
            */
    
            /* 
            ** Find the last '/' Character 
            */
            DirMarkIdx = -1;
            for ( i = 0; i < StringLength; i++ )
            {
               if ( OriginalPath[i] == '/' )
               {
                  DirMarkIdx = i;
               }
            }
    
           /*
           ** Verify the filename isn't too long
           */
           if ((StringLength - (DirMarkIdx + 1)) < OS_MAX_PATH_LEN)
           {       
              /* 
              ** Extract the filename portion 
              */
              if ( DirMarkIdx > 0 )
              {    
                 /* 
                 ** Extract the filename portion 
                 */
                 j = 0;
                 for ( i = DirMarkIdx + 1; i < StringLength; i++ )
                 {
                    FileNameOnly[j] = OriginalPath[i];
                    j++;
                 }
                 FileNameOnly[j] = '\0';
    
                 ReturnCode = CFE_SUCCESS;       
              }
              else
              { 
                 ReturnCode = CFE_FS_INVALID_PATH;
              }
           }
           else 
           {
               ReturnCode = CFE_FS_FNAME_TOO_LONG;
           }
        }
    }
   
    return(ReturnCode);
}

int32 CFE_FS_Decompress( char * SourceFile, char * DestinationFile )
{    
    if(FSDecompressRtn.count > 0)
    {
        FSDecompressRtn.count--;
        if(FSDecompressRtn.count == 0)
            return FSDecompressRtn.value;
    }
    return CFE_SUCCESS;
}

boolean CFE_FS_IsGzFile(char *FileName)
{
    if(FSIsGzFileRtn.count > 0)
	{
        FSIsGzFileRtn.count--;
        if(FSIsGzFileRtn.count == 0)
            return FSIsGzFileRtn.value;
	}
    
    return FALSE;
}


