/*
** File:
** $Id: ut_bsp_stubs.c 1.6 2010/11/23 13:17:48EST jmdagost Exp  $
**
** Purpose:
** Unit test stubs for BSP routines
** 
** Notes:
** Minimal work is done, only what is required for unit testing
**
** $Data:$
** $Revision: 1.6 $
** $Log: ut_bsp_stubs.c  $
** Revision 1.6 2010/11/23 13:17:48EST jmdagost 
** Added CFE_PSP_MemRead8() stub.
** Revision 1.5 2010/09/09 15:02:57EDT jmdagost 
** Updated funciton CFE_PSP_GetCFETextSegmentInfo() to avoid segment fault in CentOS VM environment.
** Also made dummy CFE segment CfeData a non-zero value.
** Revision 1.4 2009/06/25 14:14:47EDT rmcgraw 
** DCR8290:5 Removed #include os_bsp.h
** Revision 1.3 2009/06/25 12:46:02EDT rmcgraw 
** DCR8290:5 cFE API changes to unit test files
** Revision 1.2 2008/08/15 11:27:02EDT njyanchik 
** Check in of ES Unit Test
** Revision 1.1 2008/04/17 13:05:44BST ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
** Revision 1.15 2007/09/26 15:00:47EDT njyanchik 
** This is the first update of the ES unit tests for 5.0
** Revision 1.14 2007/05/30 15:15:18EDT njyanchik 
** check in of ES Unit test
** Revision 1.13 2007/05/30 08:56:41EDT njyanchik 
** Update
** Revision 1.12 2007/05/24 15:48:08EDT njyanchik 
** Update for ES Unit Test
** Revision 1.11 2007/05/22 13:03:57EDT njyanchik 
** Updated changes to not lose my work
** Revision 1.10 2007/05/16 11:14:36EDT njyanchik 
** Update ES's unit test driver to match code for build 4.1
** Revision 1.9 2007/05/04 09:10:32EDT njyanchik 
** Check in of Time UT and related changes
** Revision 1.8 2007/05/01 13:28:24EDT njyanchik 
** I updated the ut stubs to get the each of the subsytems to compile under the unit test. I did not
** change the unit tests themselves to cover more of the files, however.
** Revision 1.7 2007/02/27 15:17:10EST njyanchik 
** There was some code in ut_bsp_stubs.c that would cause linking errors on all OS's but linux.
** This has been resolved
** Revision 1.6 2007/01/17 10:04:20EST njyanchik 
** This CP Checks in cleaned-up versions of the files checked in for the ES unit test
** Revision 1.5 2007/01/17 09:26:08GMT-05:00 njyanchik 
** Check in of ES Unit Test changed files
** Revision 1.4 2006/11/06 15:10:06GMT-05:00 apcudmore 
** Removed OS_BSPGetRestartType. The routine is obsolete.
** Revision 1.3 2006/11/02 13:53:57EST njyanchik 
** Unit test for TIME was updated to match the changes made for this DCR
** Revision 1.2 2006/11/01 12:46:53GMT-05:00 njyanchik 
** Changed the Unit test to reflect the changes from removing the CDS functionality from TIME
** Revision 1.1 2006/03/02 15:10:24GMT-05:00 jjhageman 
** Initial revision
** Member added to project d:/mksdata/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
*/
#include "cfe.h"
#include "osapi.h"
#include "common_types.h"

#include "ut_stubs.h"
#include <string.h>

/*
** Global variables
*/
uint8 *CFE_PSP_CDSPtr = 0;
uint8 *CFE_PSP_ResetAreaPtr = 0;
uint8 *CFE_PSP_UserReservedAreaPtr = 0;
                                          
extern FILE *UT_logfile;
#define CFE_PSP_CDS_SIZE            CFE_ES_CDS_SIZE
/*
** Exported Global Data
*/

extern uint32 UT_RestartType;
extern uint32 UT_StatusBSP;
extern CFE_ES_ResetData_t   UT_CFE_ES_ResetData;
extern CFE_ES_ResetData_t * UT_CFE_ES_ResetDataPtr;
extern int  UT_DummyFuncRtn;
extern int UT_BSPLoadGoodFile;
extern int UT_SizeofESResetArea;
extern int UT_BSP_Fail;
extern int UT_CDS_Size;
extern UT_SetRtn_t BSPWriteCDSRtn;
extern UT_SetRtn_t BSPReadCDSRtn;
extern UT_SetRtn_t BSPUnloadAppFileRtn;
extern boolean UT_CDS_GoodEnd;
extern boolean UT_BSPCheckValidity;
extern boolean UT_CDSReadBlock;

extern UT_SetRtn_t BSPGetCFETextRtn;
extern OS_time_t   BSP_Time;

int CfeData = 1234;

/*extern CFE_ES_Global_t CFE_ES_Global;*/ 



int32 dummy_function(void);

/*
** Function Definitions
*/
uint32 CFE_PSP_GetProcessorId (void)
{
  return 0;
}

uint32 CFE_PSP_GetSpacecraftId (void)
{
  return 0;
}
void CFE_PSP_GetTime( OS_time_t *LocalTime)
{
    LocalTime ->seconds = BSP_Time.seconds;
    LocalTime -> microsecs = BSP_Time.microsecs;
}

int32 CFE_PSP_WriteToCDS(void *PtrToDataToWrite, uint32 CDSOffset, uint32 NumBytes)
{
    int status;

  if(BSPWriteCDSRtn.count > 0)
  {
      BSPWriteCDSRtn.count--;
      if(BSPWriteCDSRtn.count == 0)
        return BSPWriteCDSRtn.value;
  }        

      
    if (UT_BSP_Fail & BSP_WRITECDS_FAIL)
    {
        fprintf(UT_logfile, "  CFE_PSP_WriteToCDS called: (FAILURE)\n");
        status = OS_ERROR;
    } 
    else 
    {
      /*  fprintf(UT_logfile, "  CFE_PSP_WriteToCDS called: (SUCCESS)\n");*/
        status = OS_SUCCESS;
    }
    return status;
}

int32 CFE_PSP_ReadFromCDS(void *PtrToDataToRead, uint32 CDSOffset, uint32 NumBytes)
{
    int status;
    if(BSPReadCDSRtn.count > 0)
    {
        BSPReadCDSRtn.count--;
        if(BSPReadCDSRtn.count == 0)
        {
            if(UT_BSPCheckValidity == TRUE)
            {
                memcpy(PtrToDataToRead, "_CDSBeg_",NumBytes);
            }
           
            return BSPReadCDSRtn.value;
        }
    }    
  
    if (UT_BSP_Fail & BSP_READCDS_FAIL)
    {
        fprintf(UT_logfile, "  CFE_PSP_ReadFromCDS called: (FAILURE)\n");
        status = OS_ERROR;
    } 
    else 
    {
        fprintf(UT_logfile, "  CFE_PSP_ReadFromCDS called: (SUCCESS)\n");
        status = OS_SUCCESS;
        if (UT_CDS_GoodEnd == TRUE)
        {
            if (UT_BSPCheckValidity == TRUE)
            {
            memcpy(PtrToDataToRead,"_CDSEnd_",NumBytes);
            }
        }
        else
        {
            if( UT_BSPCheckValidity == TRUE)
            {
                memcpy(PtrToDataToRead,"gibberis",NumBytes);
            }
        }
        
    }

        return status;
}

int32 CFE_PSP_GetCDSSize(uint32 *SizeOfCDS)
{
    int status;
    if (UT_BSP_Fail & BSP_GETCDSSIZE_FAIL)
    {
        fprintf(UT_logfile, "  CFE_PSP_GetCDSSize called: (FAILURE)\n");
        status = OS_ERROR;
    } 
    else 
    {
        fprintf(UT_logfile, "  CFE_PSP_GEtCDSSize called: (SUCCESS)\n");
        status = OS_SUCCESS;
    }

    *SizeOfCDS = UT_CDS_Size;

    return status;   
}

int32 CFE_PSP_GetVolatileDiskMem(void *PtrToVolDisk, uint32 *SizeOfVolDisk )
{
   int32 return_code;
  

    if (UT_BSP_Fail & BSP_GETVOLDISKMEM_FAIL)
    {
        fprintf(UT_logfile, "  CFE_PSP_GetVolatileDiskMem called: (FAILURE)\n");
        return OS_ERROR;
    } 

   if ( SizeOfVolDisk == NULL )
   {
      return_code = OS_ERROR;
   }
   else
   {
      memset(&(PtrToVolDisk),0,sizeof(PtrToVolDisk));
      *SizeOfVolDisk = 0;
      return_code = OS_SUCCESS;
   }
   
   return(return_code);

}
void CFE_PSP_Restart(uint32 reset_type)
{
    printf("CFE_PSP__RestartCalled\n");
}

int32 dummy_function(void)
{
    printf("dummy function\n");
    return UT_DummyFuncRtn;
}

int32 CFE_PSP_loadAppFile(uint32 *ModuleId, uint32 *StartAddress,
                               char   *FileName, char   *EntryPoint,
                               uint32  LoadAddress)
{

  
   if (( FileName == NULL ) || (EntryPoint == NULL )  || (ModuleId == NULL) || (StartAddress == NULL))
   {
      OS_printf("BSP: Error, invalid parameters to BSPloadAppFile\n");
      return(OS_ERROR);
   }
   
   if (UT_BSPLoadGoodFile ==TRUE)
   {
       *StartAddress = & dummy_function;
       *ModuleId =  &dummy_function; /* this looks like its not that important for the UT */
       return OS_SUCCESS;
   }
   else
   {
       return(OS_ERROR);
   }
}

int32 CFE_PSP_UnloadAppFile(uint32 ModuleId)
{
    int status;

  if(BSPUnloadAppFileRtn.count > 0)
  {
      BSPUnloadAppFileRtn.count--;
      if(BSPUnloadAppFileRtn.count == 0)
        return BSPUnloadAppFileRtn.value;
  }       

  return OS_SUCCESS;

}

void CFE_PSP_Get_Timebase(uint32 *Tbu, uint32* Tbl)
{
   /* SUB -add function call code*/
}

int32 CFE_PSP_GetKernelTextSegmentInfo(void *PtrToKernelSegment, uint32 *SizeOfKernelSegment)
{
   int32 return_code;
   uint32 Address;
   
   if ( SizeOfKernelSegment == NULL )
   {
      return_code = OS_ERROR;
   }
   else
   {
      /* Just use any area of memory for the "kernel" */
      Address = (uint32)(&UT_CFE_ES_ResetData);
      memcpy(PtrToKernelSegment,&Address,sizeof(PtrToKernelSegment));
      *SizeOfKernelSegment = sizeof(UT_CFE_ES_ResetData);
      
      return_code = OS_SUCCESS;
   }
   
   return(return_code);
}

int32 CFE_PSP_GetResetArea (void *PtrToResetArea, uint32 *SizeOfResetArea)
{
    UT_CFE_ES_ResetDataPtr = &(UT_CFE_ES_ResetData); 
    memcpy(PtrToResetArea,&(UT_CFE_ES_ResetDataPtr),sizeof(PtrToResetArea));
    *SizeOfResetArea = UT_SizeofESResetArea;
    
    return UT_StatusBSP;
}

void  CFE_PSP_AttachExceptions(void)
{
}

void CFE_PSP_SetDefaultExceptionEnvironment(void)
{
}

uint32 CFE_PSP_GetTimerTicksPerSecond(void)
{
    return(2000);
}


uint32 CFE_PSP_GetTimerLow32Rollover(void)
{
    return(100000);
}

/*njy */
int32 CFE_PSP_GetCFETextSegmentInfo(void *PtrToCFESegment, uint32 *SizeOfCFESegment)
{
    uint32 Address;
    
    if(BSPGetCFETextRtn.count > 0)
    {
        BSPGetCFETextRtn.count--;
        if(BSPGetCFETextRtn.count == 0)
            return BSPGetCFETextRtn.value;
    }   
    
    /* just set the pointer and size to anything */
    Address = (uint32)(&CfeData);
    memcpy(PtrToCFESegment, &Address,sizeof(PtrToCFESegment));
/*    PtrToCFESegment = & CfeData; */
    *SizeOfCFESegment = sizeof (CfeData);
    return OS_SUCCESS;
}

int32 CFE_PSP_MemRead8(uint32 Address, uint8 *Data)
{
/*    boolean Status;

    Status = UTF_read_sim_address(Address, 1, Data);

    if(Status == FALSE)
        return(OS_ERROR);
*/
    *Data = 0x01;

    return(OS_SUCCESS);
}
