/*
** File:
** $Id: ut_sb_stubs.c 1.1 2008/04/17 08:05:46EDT ruperera Exp  $
**
** Purpose:
** Unit test stubs for Software Bus routines
** 
** Notes:
** Minimal work is done, only what is required for unit testing
**
** $Data:$
** $Revision: 1.1 $
** $Log: ut_sb_stubs.c  $
** Revision 1.1 2008/04/17 08:05:46EDT ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
** Revision 1.9 2007/05/30 15:15:22EDT njyanchik 
** check in of ES Unit test
** Revision 1.8 2007/05/21 08:17:37EDT njyanchik 
** A check in of ES's Unit test to save my work
** Revision 1.7 2007/05/16 11:14:37EDT njyanchik 
** Update ES's unit test driver to match code for build 4.1
** Revision 1.6 2007/05/10 15:14:25EDT njyanchik 
** Another update of Jonathans UT
** Revision 1.5 2007/05/07 13:45:37EDT njyanchik 
** EVS's Unit test drivers have been updated
** Revision 1.4 2007/05/04 09:10:24EDT njyanchik 
** Check in of Time UT and related changes
** Revision 1.3 2007/05/01 13:28:14EDT njyanchik 
** I updated the ut stubs to get the each of the subsytems to compile under the unit test. I did not
** change the unit tests themselves to cover more of the files, however.
** Revision 1.2 2007/01/17 09:26:07EST njyanchik 
** Check in of ES Unit Test changed files
** Revision 1.1 2006/03/02 15:10:26GMT-05:00 jjhageman 
** Initial revision
** Member added to project d:/mksdata/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
*/

/*
** Include section
*/

#include <stdio.h>
#include <unistd.h>

#include "cfe_sb.h"
#include "common_types.h"
#include "../evs/cfe_evs_task.h"

#include "ut_stubs.h"

/*
** Exported Global Data
*/

extern CFE_EVS_GlobalData_t CFE_EVS_GlobalData;
extern FILE                *UT_logfile;
extern uint32               UT_RestartType;
extern CFE_SB_MsgId_t       UT_RcvMsgId;
extern uint16               UT_RcvMsgCode;
CFE_SB_Qos_t                CFE_SB_Default_Qos;
extern int UT_SB_TotalMsgLen;

extern UT_SetRtn_t  SB_SubscribeRtn;
extern UT_SetRtn_t  SB_SubscribeExRtn;
extern UT_SetRtn_t  SB_SubscribeLocalRtn;
extern UT_SetRtn_t  SB_CreatePipeRtn;
extern UT_SetRtn_t	SBSendMsgRtn;
extern UT_SetRtn_t  SBCleanUpRtn;

/*
** Function definitions
*/

int32 CFE_SB_EarlyInit(void)
{
    return CFE_SUCCESS;
}
    
void CFE_SB_TaskMain(void)
{
}
int32 CFE_SB_CreatePipe (CFE_SB_PipeId_t *PipeIdPtr, uint16 Depth,
                        char *PipeName)
{
    if (SB_CreatePipeRtn.count > 0)
    {
        SB_CreatePipeRtn.count--;

        if(SB_CreatePipeRtn.count == 0)
        {
            fprintf(UT_logfile, "  CFE_SB_CreatePipe: called %d\n",SB_CreatePipeRtn.value);

            return SB_CreatePipeRtn.value;        
        }
    }
  fprintf(UT_logfile, "  CFE_SB_CreatePipe: called\n");
  return CFE_SUCCESS;
}

/* Almost real code */
uint16 CFE_SB_GetCmdCode (CFE_SB_MsgPtr_t MsgPtr)
{
  CFE_SB_CmdHdr_t     *CmdHdrPtr;

  /* if msg type is telemetry, return CFE_SUCCESS */
  if(CCSDS_RD_TYPE(MsgPtr->Hdr) == CCSDS_TLM)
      return CFE_SUCCESS;

  /* Cast the input pointer to a Cmd Msg pointer */
  CmdHdrPtr = (CFE_SB_CmdHdr_t *)MsgPtr;

  return CCSDS_RD_FC(CmdHdrPtr->Sec);
}

/* Almost real code */
CFE_SB_MsgId_t CFE_SB_GetMsgId (CFE_SB_MsgPtr_t MsgPtr)
{
  return CCSDS_RD_SID(MsgPtr->Hdr);
}

void CFE_SB_InitMsg (void *MsgPtr, CFE_SB_MsgId_t MsgId, uint16 Length,
                     boolean Clear)
{
  fprintf(UT_logfile, "  CFE_SB_InitMsg: initialized msg %d\n", MsgId);
  CFE_SB_SetMsgId(MsgPtr, MsgId);
}

/* Returns one successful command, then fails */
int32 CFE_SB_RcvMsg (CFE_SB_MsgPtr_t *BufPtr, CFE_SB_PipeId_t PipeId,
                    int32 TimeOut)
{

  CFE_SB_Msg_t message;
  int32 status = CFE_SUCCESS;
  static int NumRuns = 0;

  if (NumRuns == 0){

    CFE_SB_SetMsgId(&message, UT_RcvMsgId);
    CFE_SB_SetCmdCode(&message, UT_RcvMsgCode);

    *BufPtr = &message;

    NumRuns++;

  }else{ 

            if (NumRuns == 1)
            {
                  status = CFE_SB_TIME_OUT;
              NumRuns++;
            }

            else{
                

                  if (NumRuns == 2)
                  {
                      status = -1;
                      NumRuns = 0;
                  }
            }
  }

  

  return status;
}

/* Cheating here.. only doing subset of total messages */
/* NOTE: Currently does EVS, TIME */
int32 CFE_SB_SendMsg (CFE_SB_Msg_t *MsgPtr)
{

  int i;

  CFE_EVS_Packet_t *packet;
  CFE_EVS_TlmPkt_t *tlmpkt;

  if(SBSendMsgRtn.count > 0)
  {
      SBSendMsgRtn.count--;
      if(SBSendMsgRtn.count == 0)
        return SBSendMsgRtn.value;
  }


  /* Process message per type */
  switch(CFE_SB_GetMsgId(MsgPtr)){

    case CFE_EVS_EVENT_MSG_MID:
      packet = (CFE_EVS_Packet_t *)MsgPtr;

      fprintf(UT_logfile, "  CFE_SB_SendMsg: type CFE_EVS_EVENT_MSG_MID\n");
      fprintf(UT_logfile, "   %d/%d/%s %d:%s\n", 
              (int)packet->PacketID.SpacecraftID,
              (int)packet->PacketID.ProcessorID, packet->PacketID.AppName,
              packet->PacketID.EventID, packet->Message);
      break;

    case CFE_EVS_HK_TLM_MID:

      tlmpkt = (CFE_EVS_TlmPkt_t *)MsgPtr;

      fprintf(UT_logfile, "  CFE_SB_SendMsg: type CFE_EVS_HK_TLM_MID\n");
      fprintf(UT_logfile, "   CommandCounter = %d\n"
                       "   CommandErrCounter = %d\n"
                       "   MessageFormatMode = %d\n"
                       "   MessageSendCounter = %d\n"
                       "   MessageTruncCounter = %d\n"
                       "   UnregisteredAppCounter = %d\n"
                       "   OutputPort = %d\n"
                       "   LogFullFlag = %d\n"
                       "   LogMode = %d\n"
                       "   LogOverflowCounter = %d\n",
                       tlmpkt->CommandCounter, tlmpkt->CommandErrCounter,
                       tlmpkt->MessageFormatMode, tlmpkt->MessageSendCounter,
                       tlmpkt->MessageTruncCounter,
                       tlmpkt->UnregisteredAppCounter, tlmpkt->OutputPort,
                       tlmpkt->LogFullFlag, tlmpkt->LogMode,
                       tlmpkt->LogOverflowCounter);

      for (i = 0; i < CFE_ES_MAX_APPLICATIONS; i++){
        fprintf(UT_logfile, "   AppID = %d\n"
                         "    AppEnableStatus = %d\n"
                         "    AppMessageSentCounter = %d\n",
                         tlmpkt->AppData[i].AppID,
                         tlmpkt->AppData[i].AppEnableStatus,
                         tlmpkt->AppData[i].AppMessageSentCounter);
      }

      break;

    case CFE_TIME_DIAG_TLM_MID:

    case CFE_TIME_HK_TLM_MID:

    case CFE_TIME_1HZ_CMD_MID:

      fprintf(UT_logfile, "Incomplete TIME command: %d\n",
              CFE_SB_GetMsgId(MsgPtr));

      break;

    /* Pass the rest of the commands on */
    default:

      /* Process the message with subsystem specific call */
      UT_ProcessSBMsg(MsgPtr);

      break;

  }

  return CFE_SUCCESS;
}

/* Almost real code */
int32 CFE_SB_SetCmdCode (CFE_SB_MsgPtr_t MsgPtr, uint16 CmdCode)
{

    CFE_SB_CmdHdr_t     *CmdHdrPtr;

    /* if msg type is telemetry, ignore the request */
    if(CCSDS_RD_TYPE(MsgPtr->Hdr) == CCSDS_TLM)
        return CFE_SB_WRONG_MSG_TYPE;

    /* Cast the input pointer to a Cmd Msg pointer */
    CmdHdrPtr = (CFE_SB_CmdHdr_t *)MsgPtr;

    CCSDS_WR_FC(CmdHdrPtr->Sec,CmdCode);

    return CFE_SUCCESS;

}/* end CFE_SB_SetCmdCode */

/* Almost real code */
void  CFE_SB_SetMsgId (CFE_SB_MsgPtr_t MsgPtr, CFE_SB_MsgId_t MsgId)
{
    CCSDS_WR_SID(MsgPtr->Hdr,MsgId);
}


int32 CFE_SB_SetMsgTime (CFE_SB_MsgPtr_t MsgPtr, CFE_TIME_SysTime_t time)
{
  fprintf(UT_logfile, "  CFE_SB_SetMsgTime: called\n");

  return CFE_SUCCESS;
}

int32 CFE_SB_SubscribeEx (CFE_SB_MsgId_t MsgId, CFE_SB_PipeId_t PipeId,
                          CFE_SB_Qos_t Quality, uint16 MsgLim)
{
    if (SB_SubscribeExRtn.count > 0)
    {
        SB_SubscribeExRtn.count--;

        if(SB_SubscribeExRtn.count == 0)
        {
            fprintf(UT_logfile, "  CFE_SB_SubscribeEx: called: %d\n",SB_SubscribeExRtn.value);

            return SB_SubscribeExRtn.value;        
        }
    }
  fprintf(UT_logfile, "  CFE_SB_SubscribeEx: subscribed to msg %d\n", MsgId);
  return CFE_SUCCESS;
}

int32 CFE_SB_Subscribe(CFE_SB_MsgId_t   MsgId, CFE_SB_PipeId_t  PipeId)
{
    if (SB_SubscribeRtn.count > 0)
    {
        SB_SubscribeRtn.count--;

        if(SB_SubscribeRtn.count == 0)
        {
            fprintf(UT_logfile, "  CFE_SB_Subscribe: called: %d\n",SB_SubscribeRtn.value);

            return SB_SubscribeRtn.value;        
        }
    }
  fprintf(UT_logfile, "  CFE_SB_Subscribe: subscribed to msg %d\n", MsgId);
  return CFE_SUCCESS;
}


int32 CFE_SB_SubscribeLocal(CFE_SB_MsgId_t   MsgId, CFE_SB_PipeId_t  PipeId,uint16 MsgLim)
{

    if (SB_SubscribeLocalRtn.count > 0)
    {
        SB_SubscribeLocalRtn.count--;

        if(SB_SubscribeLocalRtn.count == 0)
        {
            fprintf(UT_logfile, "  CFE_SB_Subscribe: called: %d\n",SB_SubscribeLocalRtn.value);

            return SB_SubscribeLocalRtn.value;        
        }
    }
  fprintf(UT_logfile, "  CFE_SB_SubscribeLocal: subscribed to msg %d\n", MsgId);
  return CFE_SUCCESS;
}

void CFE_SB_TimeStampMsg (CFE_SB_MsgPtr_t MsgPtr)
{
  fprintf(UT_logfile, "  CFE_SB_TimeStampMsg: called\n");
}

uint16 CFE_SB_GetTotalMsgLength(CFE_SB_MsgPtr_t MsgPtr)
{
    return UT_SB_TotalMsgLen;
}
int32 CFE_SB_CleanUpApp (uint32 AppId)
{
    if(SBCleanUpRtn.count > 0)
    {
      SBCleanUpRtn.count--;
      if(SBCleanUpRtn.count == 0)
        return SBCleanUpRtn.value;
    }


    return CFE_SUCCESS;
}
