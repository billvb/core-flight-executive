/*
** File:
** $Id: ut_osapi_stubs.c 1.8 2010/10/25 18:22:41EDT jmdagost Exp  $
**
** Purpose:
** Unit test stubs for Operating System routines
** 
** Notes:
** Minimal work is done, only what is required for unit testing
**
** $Data:$
** $Revision: 1.8 $
** $Log: ut_osapi_stubs.c  $
** Revision 1.8 2010/10/25 18:22:41EDT jmdagost 
** Updated OS_TimerGetInfo() to return a success status.
** Revision 1.7 2010/09/09 15:06:07EDT jmdagost 
** Update argument definition for OS_TaskCreate().
** Added dummy functions for OS_TimerGetInfo() and OS_TimerDelete() that currently only return an error.
** Revision 1.6 2009/08/04 13:34:38EDT aschoeni 
** Added heap information to ES telemetry
** Revision 1.5 2009/06/25 12:46:02EDT rmcgraw 
** DCR8290:5 cFE API changes to unit test files
** Revision 1.4 2009/04/27 10:38:32EDT rmcgraw 
** DCR7366:1 Commented out debug messages to prevent flooding output file
** Revision 1.3 2009/04/01 16:12:03EDT rmcgraw 
** DCR7366:1 Removed the printout of OS_TaskGetId called - it was flooding the output file
** Revision 1.2 2008/08/15 11:26:56EDT njyanchik 
** Check in of ES Unit Test
** Revision 1.1 2008/04/17 13:05:46BST ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/unit-test/project.pj
** Revision 1.27 2007/09/26 15:00:47EDT njyanchik 
** This is the first update of the ES unit tests for 5.0
** Revision 1.26 2007/07/05 15:04:31EDT David Kobe (dlkobe) 
** Baseline following complete TBL unit testing
** Revision 1.25 2007/06/04 13:31:57EDT njyanchik 
** Update of the TBL unit test
** Revision 1.24 2007/05/30 15:15:23EDT njyanchik 
** check in of ES Unit test
** Revision 1.23 2007/05/30 08:56:41EDT njyanchik 
** Update
** Revision 1.22 2007/05/25 13:13:43EDT njyanchik 
** update of ES unit test
** Revision 1.21 2007/05/25 09:17:57EDT njyanchik 
** I added the rmfs call to the OSAL and updated the unit test stubs to match
** Revision 1.20 2007/05/24 15:48:10EDT njyanchik 
** Update for ES Unit Test
** Revision 1.19 2007/05/23 10:54:59EDT njyanchik 
** Update again to prevent loss of work
** Revision 1.18 2007/05/16 11:14:37EDT njyanchik 
** Update ES's unit test driver to match code for build 4.1
** Revision 1.17 2007/05/04 09:10:33EDT njyanchik 
** Check in of Time UT and related changes
** Revision 1.15 2007/05/01 13:28:25EDT njyanchik 
** I updated the ut stubs to get the each of the subsytems to compile under the unit test. I did not
** change the unit tests themselves to cover more of the files, however.
** Revision 1.14 2007/04/04 09:28:47EDT njyanchik 
** This CP updates the unit test for the changes made to the OS (OS_IntLock/OS_IntUnlock) under
** this DCR
** Revision 1.13 2007/03/19 10:49:48EST njyanchik 
** the UT needs to be updated because the API for OS_IntEnableAll changed
** Revision 1.12 2007/03/02 15:18:06EST njyanchik 
** Jonathan provided me with a newer tbl unit test to check in
** Revision 1.11 2007/01/17 09:26:09GMT-05:00 njyanchik 
** Check in of ES Unit Test changed files
** Revision 1.10 2006/11/28 13:01:02GMT-05:00 rjmcgraw 
** Changed OS_write stub to return bytes written
** Revision 1.9 2006/11/08 09:55:19EST rjmcgraw 
** Added OS_open
** Revision 1.8 2006/07/28 07:33:03EDT njyanchik 
** These changes change the osapi functions OS_slose, OS_write, and OS_creat to
** int32 parameters instead of uint32.
** Revision 1.7 2006/07/26 08:08:27EDT rjmcgraw 
** Changed name MutSemGivRtn to MutSemGiveRtn
** Revision 1.6 2006/06/08 19:13:59GMT rjmcgraw 
** Added support for function UT_SetRtnCode
** Revision 1.5 2006/05/22 19:22:22GMT jjhageman 
** Addition of OS_QueueDelete
*/

/*
** Include section
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#include "cfe_error.h"
#include "osapi.h"
#include "common_types.h"

#include "ut_stubs.h"

/*
** Exported Global Data
*/

extern FILE *UT_logfile;
extern uint32 UT_OS_Fail;
extern int UT_ReadBufOffset;
extern int32 dummy_function(void);


/*OS */
#define MAX_PRIORITY 255


UT_Task_t    UT_Task      [OS_MAX_TASKS];

extern FILE   *UT_logfile;
extern char    UT_appname[80];
extern int     UT_lognum;
extern uint32  UT_OS_Fail;
extern uint32  UT_AppID;
extern uint32  UT_LocTimeSec;
extern uint32  UT_LocTimeMSec;
extern uint32  UT_BinSemFail;
extern UT_Queue_t UT_Queue[OS_MAX_QUEUES];
extern UT_SetRtn_t MutSemCreateRtn;
extern UT_SetRtn_t MutSemGiveRtn;
extern UT_SetRtn_t MutSemTakeRtn;
extern UT_SetRtn_t QueueCreateRtn;
extern UT_SetRtn_t QueueDelRtn;
extern UT_SetRtn_t QueueGetRtn;
extern UT_SetRtn_t QueuePutRtn;
extern UT_SetRtn_t FileWriteRtn;
extern UT_SetRtn_t OSReadRtn; 
extern UT_SetRtn_t OSReadRtn2; 
extern UT_SetRtn_t OS_BinSemCreateRtn;
extern UT_SetRtn_t OSlseekRtn; 
extern UT_SetRtn_t CountSemDelRtn;
extern UT_SetRtn_t MutSemDelRtn;
extern UT_SetRtn_t BinSemDelRtn;
extern UT_SetRtn_t BlocksFreeRtn;
extern UT_SetRtn_t UnmountRtn;
extern UT_SetRtn_t RmfsRtn;
extern UT_SetRtn_t ModuleLoadRtn;
extern UT_SetRtn_t ModuleUnloadRtn;
extern UT_SetRtn_t ModuleInfoRtn;
extern UT_SetRtn_t SymbolLookupRtn;
extern UT_SetRtn_t HeapGetInfoRtn;

/*
** Defines
*/
#define OS_BASE_PORT 43000

/*
** Function definitions
*/

/* Not doing anything now for semaphores, simply call task explicitly */


int32 OS_BinSemTake (uint32 sem_id)
{
  /* fprintf(UT_logfile, "  OS_BinSemTake(%d): called\n", (int)sem_id);*/

    /* Enable a failure return on 1st or 2nd second call */
    /* Used to call CFE_TIME_Local1HzTask and get coverage */
    if(UT_BinSemFail == 2)
    {
        UT_BinSemFail = 1;
    }
    else
    {
        if (UT_BinSemFail == 1)
        {
            UT_BinSemFail = 0;
            return -1;
        }
    }
  return OS_SUCCESS;
}


int32 OS_close (int32 filedes)
{
  int32 status;

  if (UT_OS_Fail & OS_CLOSE_FAIL)
  {
    /*fprintf(UT_logfile, "  OS_close(%d) (FAILURE)\n", (int)filedes);*/
    status = OS_FS_ERROR;
  } 
  else 
  {
    /*fprintf(UT_logfile, "  OS_close(%d) (SUCCESS)\n", (int)filedes);*/
    status = OS_FS_SUCCESS;
  }

  return status;
}

/* Open local log file, increment reporting number on each call */
int32 OS_creat (const char *path, int32 access)
{

  int32 status;

  UT_lognum++;

  if (UT_OS_Fail & OS_CREAT_FAIL){
    /*fprintf(UT_logfile, "  OS_creat(%d): Call to open %s (FAILURE)\n",
            UT_lognum, path);*/
    status = OS_FS_ERROR;
  } else {
    /* fprintf(UT_logfile, "  OS_creat(%d): Call to open %s (SUCCESS)\n",
            UT_lognum, path);*/
    status = UT_lognum;
  }

  return status;
}

/* Fake time increments each time it's called */
int32 OS_GetLocalTime (OS_time_t *time_struct)
{

  time_struct->seconds = UT_LocTimeSec;
  time_struct->microsecs = UT_LocTimeMSec;

  UT_LocTimeSec++;

  return 0;
}

int32 OS_IntLock (void)
{

#if 0
#ifndef _ix86_
#  error "IntDisableAll unit test stub may not work for your OS"
#endif
#endif

  return OS_SUCCESS;
}

int32 OS_IntUnlock (int32 IntLevel)
{

#if 0
#ifndef _ix86_
#  error "IntEnableAll unit test stub may not work for your OS"
#endif
#endif

  return OS_SUCCESS;
}

/* Simple memcpy */
int32 CFE_PSP_MemCpy (void *dst, void *src, uint32 size)
{
  memcpy(dst, src, size);
  return OS_SUCCESS;
}

/* Simple memset */
int32 CFE_PSP_MemSet (void *dst, uint8 value , uint32 size)
{
  memset(dst, (int)value, (size_t)size);
  return OS_SUCCESS;
}

int32 OS_MutSemGive (uint32 sem_id)
{
  if(MutSemGiveRtn.count > 0)
  {
      MutSemGiveRtn.count--;
      if(MutSemGiveRtn.count == 0)
        return MutSemGiveRtn.value;
  }

  return OS_SUCCESS;
}

int32 OS_MutSemTake (uint32 sem_id)
{
  if(MutSemTakeRtn.count > 0)
  {
      MutSemTakeRtn.count--;
      if(MutSemTakeRtn.count == 0)
        return MutSemTakeRtn.value;
  }

  return OS_SUCCESS;
}

void OS_printf(const char *String, ...)
{
  va_list ptr;

  va_start(ptr, String);
  vprintf(String, ptr);
  va_end(ptr);
}

/*
** Similar to the real call, doesn't care about creator or do any
** mutex locking
*/
int32 OS_QueueCreate(uint32 *queue_id, const char *queue_name,
                     uint32 queue_depth, uint32 data_size, uint32 flags)
{

    int                     tmpSkt;
    int                     returnStat;
    struct sockaddr_in      servaddr;
    int                     i;
    uint32                  possible_qid;

    if(QueueCreateRtn.count > 0)
    {
        QueueCreateRtn.count--;
        if(QueueCreateRtn.count == 0)
          return QueueCreateRtn.value;
    }

    if ( queue_id == NULL || queue_name == NULL)
        return OS_INVALID_POINTER;

    /* we don't want to allow names too long*/
    /* if truncated, two names might be the same */

    if (strlen(queue_name) > OS_MAX_API_NAME)
            return OS_ERR_NAME_TOO_LONG;

    /* Look for open queue */
    for(possible_qid = 0; possible_qid < OS_MAX_QUEUES; possible_qid++)
    {
        if (UT_Queue[possible_qid].free == TRUE)
            break;
    }

    if( possible_qid >= OS_MAX_QUEUES || UT_Queue[possible_qid].free != TRUE)
        return OS_ERR_NO_FREE_IDS;

    /* See if name is already taken */
    for (i = 0; i < OS_MAX_QUEUES; i++)
    {
        if (strcmp ((char*) queue_name, UT_Queue[i].name) == 0)
        {
            return OS_ERR_NAME_TAKEN;
        }
    } 

    tmpSkt = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(OS_BASE_PORT + possible_qid);
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    /* 
    ** bind the input socket to a pipe
    ** port numbers are OS_BASE_PORT + queue_id
    */
    returnStat = bind(tmpSkt,(struct sockaddr *)&servaddr, sizeof(servaddr));
  
    if ( returnStat == -1 )
    {
       printf("bind failed on OS_QueueCreate. errno = %d\n",errno);
       return OS_ERROR;
    }

    /*
    ** store socket handle
    */
    *queue_id = possible_qid;

    UT_Queue[*queue_id].id = tmpSkt;
    UT_Queue[*queue_id].free = FALSE;
    strcpy( UT_Queue[*queue_id].name, (char*) queue_name);

    return OS_SUCCESS;
}


/* Similar to real code without mutex locking */
int32 OS_QueueDelete (uint32 queue_id)
{

    if(QueueDelRtn.count > 0)
    {
        QueueDelRtn.count--;
        if(QueueDelRtn.count == 0)
          return QueueDelRtn.value;
    }

    /* Check to see if the queue_id given is valid */

    if (queue_id >= OS_MAX_QUEUES || UT_Queue[queue_id].free == TRUE)
            return OS_ERR_INVALID_ID;

    /* Try to delete the queue */

    if(close(UT_Queue[queue_id].id) !=0)
        {
            return OS_ERROR;
        }

    /* 
     * Now that the queue is deleted, remove its "presence"
     * in OS_message_q_table and OS_message_q_name_table 
    */

    UT_Queue[queue_id].free = TRUE;
    strcpy(UT_Queue[queue_id].name, "");
    UT_Queue[queue_id].id = 0;

    return OS_SUCCESS;

} /* end OS_QueueDelete */


/* Works similar to real fcn, note that pend on empty queue doesn't block */
int32 OS_QueueGet(uint32 queue_id, void *data, uint32 size, 
                  uint32 *size_copied, int32 timeout)
{

    int sizeCopied;
    int flags;

    if(QueueGetRtn.count > 0)
    {
        QueueGetRtn.count--;
        if(QueueGetRtn.count == 0)
          return QueueGetRtn.value;
    }

    /* Check parameters */
    if(queue_id >= OS_MAX_QUEUES || UT_Queue[queue_id].free == TRUE)
        return OS_ERR_INVALID_ID;
    else
    {
        if( (data == NULL) || (size_copied == NULL) )
            return OS_INVALID_POINTER;
    }

   /*
   ** Read the socket for data
   */
   if (timeout == OS_PEND)
   {

      flags = fcntl(UT_Queue[queue_id].id, F_GETFL, 0);
      fcntl(UT_Queue[queue_id].id,F_SETFL,flags|O_NONBLOCK);

      sizeCopied = recvfrom(UT_Queue[queue_id].id, data, size, 0, NULL, NULL);

      fcntl(UT_Queue[queue_id].id,F_SETFL,flags);

      if (sizeCopied == -1 && errno == EWOULDBLOCK )
      {
         *size_copied = 0;
         printf("WARNING: OS_QueueGet called as pend on an empty queue"
                " normally would block but in UT environment isn't valid\n");

         return OS_QUEUE_EMPTY;
      }
      else if(sizeCopied != size )
      {
         *size_copied = 0;
         return(OS_QUEUE_INVALID_SIZE);
      }
   }
   else if (timeout == OS_CHECK)
   {
      flags = fcntl(UT_Queue[queue_id].id, F_GETFL, 0);
      fcntl(UT_Queue[queue_id].id,F_SETFL,flags|O_NONBLOCK);

      sizeCopied = recvfrom(UT_Queue[queue_id].id, data, size, 0, NULL, NULL);

      fcntl(UT_Queue[queue_id].id,F_SETFL,flags);

      if (sizeCopied == -1 && errno == EWOULDBLOCK )
      {
         *size_copied = 0;
         return OS_QUEUE_EMPTY;
      }
      else if(sizeCopied != size )
      {
         *size_copied = 0;
         return(OS_QUEUE_INVALID_SIZE);
      }

   }
   else /* timeout */
   {
      int timeloop;

      flags = fcntl(UT_Queue[queue_id].id, F_GETFL, 0);
      fcntl(UT_Queue[queue_id].id,F_SETFL,flags|O_NONBLOCK);

      /*
      ** This "timeout" will check the socket for data every 100 milliseconds
      ** up until the timeout value. Although this works fine for a desktop environment,
      ** it should be written more efficiently for a flight system.
      ** The proper way will be to use select or poll with a timeout
      */
      for ( timeloop = timeout; timeloop > 0; timeloop = timeloop - 100 )
      {
         sizeCopied = recvfrom(UT_Queue[queue_id].id, data, size, 0, NULL, NULL);

         if ( sizeCopied == size )
         {
             *size_copied = sizeCopied;
             fcntl(UT_Queue[queue_id].id,F_SETFL,flags);
             return OS_SUCCESS;

         }
         else if (sizeCopied == -1 && errno == EWOULDBLOCK )
         {
            /*
            ** Sleep for 100 milliseconds
            */
            usleep(100 * 1000);
         }
         else
         {
             *size_copied = 0;
             fcntl(UT_Queue[queue_id].id,F_SETFL,flags);
             return OS_QUEUE_INVALID_SIZE;
         }
      }
      fcntl(UT_Queue[queue_id].id,F_SETFL,flags);
      return(OS_QUEUE_TIMEOUT);

   } /* END timeout */

   /*
   ** Should never really get here.
   */
   return OS_SUCCESS;
}

/* Same as real function */
int32 OS_QueuePut(uint32 queue_id, void *data, uint32 size, 
                  uint32 flags)
{

    struct sockaddr_in serva;
    static int socketFlags = 0;
    int bytesSent    = 0;
    int tempSkt      = 0;

    if(QueuePutRtn.count > 0)
    {
        QueuePutRtn.count--;
        if(QueuePutRtn.count == 0)
          return QueuePutRtn.value;
    }

    /*
    ** Check Parameters 
    */
    if(queue_id >= OS_MAX_QUEUES || UT_Queue[queue_id].free == TRUE)
        return OS_ERR_INVALID_ID;

    if (data == NULL)
        return OS_INVALID_POINTER;
    /* 
    ** specify the IP addres and port number of destination
    */
    memset(&serva, 0, sizeof(serva));
    serva.sin_family      = AF_INET;
    serva.sin_port        = htons(OS_BASE_PORT + queue_id);
    serva.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    /*
    ** open a temporary socket to transfer the packet to MR
    */
    tempSkt = socket(AF_INET, SOCK_DGRAM, 0);

    /* 
    ** send the packet to the message router task (MR)
    */
    bytesSent = sendto(tempSkt,(char *)data, size, socketFlags,
                     (struct sockaddr *)&serva, sizeof(serva));
    if( bytesSent != size )
    {
       return(OS_QUEUE_FULL);
    }

    /* 
    ** close socket
    */
    close(tempSkt);

    return OS_SUCCESS;

}


int32 OS_remove (const char *path)
{
  /* fprintf(UT_logfile, "  OS_remove: %s\n", path);*/
  return 0;
}

/* Only outputs SUCCESS or FAILURE since packet structure isn't known */
int32 OS_write (int32 filedes, void *buffer, uint32 nbytes)
{
  int32 status;

  if(FileWriteRtn.count > 0)
  {
      FileWriteRtn.count--;
      if(FileWriteRtn.count == 0)
        return FileWriteRtn.value;
  }

  if (UT_OS_Fail & OS_WRITE_FAIL){
    /*fprintf(UT_logfile, "  OS_write(%d): (FAILURE)\n", (int)filedes);*/
    status = OS_FS_ERROR;
  } else {
    /* fprintf(UT_logfile, "  OS_write(%d): (SUCCESS)\n", (int)filedes);*/
    status = nbytes;
  }

  return status;
}

int32 OS_BinSemFlush (uint32 sem_id)
{
  /* fprintf(UT_logfile, "  OS_BinSemFlush(%d): called\n", (int)sem_id);*/
  return OS_SUCCESS;
}
int32 OS_TaskRegister (void)
{
      int status;

    if (UT_OS_Fail & OS_TASKREGISTER_FAIL)
    {
        /* fprintf(UT_logfile, "  OS_TaskRegister: called (FAILURE)\n");*/
        status = OS_ERROR;
    } 
    else 
    {
        /* fprintf(UT_logfile, "  OS_TaskRegister: called (SUCCESS)\n");*/
        status = OS_SUCCESS;
    }
    return status;
}

/* works like the regular TaskCreate, but doesn't actually create a task,
 * or use mutex locking */
int32 OS_TaskCreate (uint32 *task_id, const char *task_name,osal_task_entry function_pointer,
                      const uint32 *stack_pointer, uint32 stack_size, uint32 priority,
                      uint32 flags)
{
    int status;
    *task_id = 1;
    if (UT_OS_Fail & OS_TASKCREATE_FAIL)
    {
        /*fprintf(UT_logfile, "  OS_TaskCreate(%s,%d): called (FAILURE)\n",task_name, (int)*task_id);*/
        status = OS_ERROR;
    } 
    else 
    {
        /* fprintf(UT_logfile, "  OS_TaskCreate(%s,%d): called (SUCCESS)\n",task_name, (int)*task_id);*/
        status = OS_SUCCESS;
    }
    return status;

    
    
}
uint32 OS_TaskGetId (void)
{
    /* fprintf(UT_logfile, "  OS_TaskGetId: called\n"); */
    return 1;
}
int32 OS_TaskGetInfo (uint32 task_id, OS_task_prop_t *task_prop)  
{
    if( task_prop == NULL)
        return OS_INVALID_POINTER;
    
    task_prop -> creator =    0;
    task_prop -> OStask_id = 63;
    task_prop -> stack_size = UT_Task[task_id].stack_size;
    task_prop -> priority =   UT_Task[task_id].priority;
    
    strcpy(task_prop-> name, UT_Task[task_id].name);

    return OS_SUCCESS;
}
/*----------------------------------------------------------*/
int32 OS_mkfs (char *address, char *devname,char * volname, uint32 blocksize, 
               uint32 numblocks)
{
    int status;
    if (UT_OS_Fail & OS_MKFS_FAIL)
    {
        fprintf(UT_logfile, "  OS_mkfs (FAILURE): %s\n", devname);
        status = OS_FS_ERROR;
    } 
    else 
    {
        fprintf(UT_logfile, "  OS_mkfs (SUCCESS): %s\n", devname);
        status = OS_FS_SUCCESS;
    }
    return status;
}

int32 OS_rmfs (char *devname)
{
    int status;
    if (UT_OS_Fail & OS_RMFS_FAIL)
    {
        fprintf(UT_logfile, "  OS_rmfs (FAILURE): %s\n", devname);
        status = OS_FS_ERROR;
    } 
    else 
    {
        fprintf(UT_logfile, "  OS_rmfs (SUCCESS): %s\n", devname);
        status = OS_FS_SUCCESS;
    }
    return status;
}

int32 OS_mount (const char *devname, char* mountpoint)
{
    int status;
    if (UT_OS_Fail & OS_MOUNT_FAIL)
    {
        fprintf(UT_logfile, "  OS_mount called devname: %s mountpoint: %s (FAILURE)\n", devname, mountpoint);
        status = OS_FS_ERROR;
    } 
    else 
    {
        fprintf(UT_logfile, "  OS_mount called devname: %s mountpoint: %s (SUCCESS)\n", devname, mountpoint);
        status = OS_SUCCESS;
    }
    return status;
      
}
int32 OS_initfs (char *address,char *devname, char *volname, 
                uint32 blocksize, uint32 numblocks)
{
    int status;
    if (UT_OS_Fail & OS_INITFS_FAIL)
    {
        fprintf(UT_logfile, "  OS_initfs called: %s (FAILURE)\n", devname);
        status = OS_FS_ERROR;
    } 
    else 
    {
        fprintf(UT_logfile, "  OS_initfs called: %s (SUCCESS)\n", devname);
        status = OS_SUCCESS;
    }
    return status;

}

int32 OS_ShellOutputToFile(char* Cmd, int32 OS_fd)
{
      fprintf(UT_logfile, "  OS_ShellOutput to file called called: %s\n", Cmd);
    return OS_SUCCESS;
}
int32 OS_read  (int32  filedes, void *buffer, uint32 nbytes)
{
	if(OSReadRtn.count > 0)
	{
		OSReadRtn.count--;
		if (OSReadRtn.count == 0)
			return OSReadRtn.value;
	}

	if(OSReadRtn2.count > 0)
	{
		OSReadRtn2.count--;
		if(OSReadRtn2.count == 0)
			return OSReadRtn2.value;
	}

   if (UT_OS_Fail & OS_READ_FAIL)
	{
		return (nbytes - 1);
	}
	else
	{
		memcpy(buffer, &UT_ReadBuf[UT_ReadBufOffset], nbytes);
		UT_ReadBufOffset += nbytes;
		return nbytes;
	}
}
int32 OS_lseek  (int32  filedes, int32 offset, uint32 whence)
{
	if(OSlseekRtn.count > 0)
	{
		OSlseekRtn.count--;
		if(OSlseekRtn.count == 0)
			return OSlseekRtn.value;
	}
    if (UT_OS_Fail & OS_LSEEK_FAIL)
    {
        return OS_FS_ERROR;
    } 


    return offset;
}
int32 OS_BinSemCreate (uint32 *sem_id, const char *sem_name,
                       uint32 sem_initial_value, uint32 options)
{
    
  int status;
  static int sem_ctr = 0;

  *sem_id = sem_ctr;
  sem_ctr++;

    if (UT_OS_Fail & OS_SEMCREATE_FAIL)
    {
        /* fprintf(UT_logfile, "  OS_BinSemCreate called: %d (FAILURE)\n", sem_ctr);*/
        status = OS_ERROR;
    } 
    else 
    {
	    if(OS_BinSemCreateRtn.count > 0)
	    {
		    OS_BinSemCreateRtn.count--;
		    if(OS_BinSemCreateRtn.count == 0)
			    return OS_BinSemCreateRtn.value;
	    }

        /* fprintf(UT_logfile, "  OS_BinSemCreate called %d\n", OS_BinSemCreateRtn.value);*/
        status = OS_SUCCESS;
    }
    return status;
}
int32 OS_MutSemCreate (uint32 *sem_id, const char *sem_name, uint32 options)
{
    int status;

    if(MutSemCreateRtn.count > 0)
    {
      MutSemCreateRtn.count--;
      if(MutSemCreateRtn.count == 0)
        return MutSemCreateRtn.value;
    }

    if (UT_OS_Fail & OS_MUTCREATE_FAIL)
    {
        /* fprintf(UT_logfile, "  OS_MutSemCreate called: (FAILURE)\n");*/
        status = OS_ERROR;
    } 
    else 
    {
        status = OS_SUCCESS;
    }
    return status;

}
int32 OS_open (const char *path, int32 access,  uint32  mode)
{
   int status;
   static int NumOpen = 1;
    if (UT_OS_Fail & OS_OPEN_FAIL)
    {
        /* fprintf(UT_logfile, "  OS_open called: (FAILURE)\n");*/
        status = OS_ERROR;
    } 
    else 
    {
        /* fprintf(UT_logfile, "  OS_open called: (SUCCESS)\n");*/
        status = NumOpen;
        NumOpen++;
    }
    return status;
}
int32 OS_TaskDelay (uint32 millisecond)
{
    int status;
    if (UT_OS_Fail & OS_TASKDELAY_FAIL)
    {
        /* fprintf(UT_logfile, "  OS_TaskDelay called: (FAILURE)\n");*/
        status = OS_ERROR;
    } 
    else 
    {
        /* fprintf(UT_logfile, "  OS_TaskDelay called: (SUCCESS)\n");*/
        status = OS_SUCCESS;
        usleep(millisecond*1000);
    }
    return status;


 }
int32 OS_BinSemGive (uint32 sem_id)
{

  /* fprintf(UT_logfile, "  OS_BinSemGive(%d): called\n", (int)sem_id);*/
  
  return UT_BinSemFail;
}

void CFE_PSP_Panic(int32 ErrorCode)
{
    fprintf(UT_logfile,"  CFE_PSP_Panic Called\n");
}

int32 OS_BinSemGetInfo (uint32 sem_id, OS_bin_sem_prop_t *bin_prop)
{
    bin_prop ->creator =  0;
    strcpy(bin_prop-> name, "Name");

    return OS_SUCCESS;

}

int32 OS_MutSemGetInfo (uint32 sem_id, OS_mut_sem_prop_t *mut_prop)
{

    mut_prop ->creator =  1;
    strcpy(mut_prop-> name, "Name");

    return OS_SUCCESS;
}

int32 OS_CountSemGetInfo (uint32 sem_id, OS_count_sem_prop_t *count_prop)
{

    count_prop ->creator =  0;
    strcpy(count_prop-> name, "Name");

    return OS_SUCCESS;
} 
int32 OS_QueueGetInfo (uint32 sem_id, OS_queue_prop_t *queue_prop)
{

    queue_prop ->creator =  0;
    strcpy(queue_prop-> name, "Name");

    return OS_SUCCESS;
} 


int32 OS_MutSemDelete (uint32 sem_id)
{
    if(MutSemDelRtn.count > 0)
    {
        MutSemDelRtn.count--;
        if(MutSemDelRtn.count == 0)
          return MutSemDelRtn.value;
    }

        return OS_SUCCESS;

}

int32 OS_BinSemDelete (uint32 sem_id)
{
    if(BinSemDelRtn.count > 0)
    {
        BinSemDelRtn.count--;
        if(BinSemDelRtn.count == 0)
          return BinSemDelRtn.value;
    }

        return OS_SUCCESS;

}

int32 OS_CountSemDelete (uint32 sem_id)
{
    if(CountSemDelRtn.count > 0)
    {
        CountSemDelRtn.count--;
        if(CountSemDelRtn.count == 0)
          return CountSemDelRtn.value;
    }

        return OS_SUCCESS;

}
int32 OS_FDGetInfo (int32 filedes, OS_FDTableEntry *fd_prop)
{
    OS_FDTableEntry fd_prop2;

    fd_prop2.User = 0;
    strcpy(fd_prop2.Path,"PATH");
    fd_prop2.IsValid = TRUE;

    *fd_prop = fd_prop2;
    return OS_FS_SUCCESS;
}

int32 OS_TaskDelete (uint32 task_id)
{
    int status;
    if (UT_OS_Fail & OS_TASKDELETE_FAIL)
    {
        /* fprintf(UT_logfile, "  OS_TaskDelete called: (FAILURE)\n");*/
        status = OS_ERROR;
    } 
    else 
    {
        /* fprintf(UT_logfile, "  OS_TaskDelete called: (SUCCESS)\n");*/
        status = OS_SUCCESS;
    }
    return status;

}

int32 OS_fsBlocksFree (const char *name)
{

    if(BlocksFreeRtn.count > 0)
    {
        BlocksFreeRtn.count--;
        if(BlocksFreeRtn.count == 0)
          return BlocksFreeRtn.value;
    }
    return 100;


}


int32 OS_unmount (const char *mountpoint)
{
    if(UnmountRtn.count > 0)
    {
        UnmountRtn.count--;
        if(UnmountRtn.count == 0)
          return UnmountRtn.value;
    }
    return OS_FS_SUCCESS;


}


void OS_TaskExit()
{
  /* fprintf(UT_logfile,"OS_TaskExit Called\n");*/
}






/*njy*/
int32 OS_SymbolLookup (uint32 *symbol_address, char *symbol_name )
{
    if(SymbolLookupRtn.count > 0)
    {
        SymbolLookupRtn.count--;
        if(SymbolLookupRtn.count == 0)
            return SymbolLookupRtn.value;
    }
           *symbol_address = & dummy_function;
        
    return OS_SUCCESS;
}


int32 OS_ModuleLoad ( uint32 *module_id, char *module_name, char *filename )
{
    if(ModuleLoadRtn.count > 0)
    {
        ModuleLoadRtn.count--;
        if(ModuleLoadRtn.count == 0)
            return ModuleLoadRtn.value;
    }
    
    return OS_SUCCESS;
}

int32 OS_ModuleUnload ( uint32 module_id )
{
    if(ModuleUnloadRtn.count > 0)
    {
        ModuleUnloadRtn.count--;
        if(ModuleUnloadRtn.count == 0)
            return ModuleUnloadRtn.value;
    }
    
    return OS_SUCCESS;
}

int32 OS_ModuleInfo ( uint32 module_id, OS_module_record_t *module_info )
{
    if(ModuleInfoRtn.count > 0)
    {
        ModuleInfoRtn.count--;
        if(ModuleInfoRtn.count == 0)
            return ModuleInfoRtn.value;
    }
    return OS_SUCCESS;
}

int32 OS_BinSemTimedWait       (uint32 sem_id, uint32 msecs)
{
    return OS_SUCCESS;
}

int32 OS_HeapGetInfo       (OS_heap_prop_t *heap_prop)
{

    if(heap_prop == NULL)
      return OS_INVALID_POINTER;

    if(HeapGetInfoRtn.count > 0)
    {
        HeapGetInfoRtn.count--;
        if(HeapGetInfoRtn.count == 0)
            return HeapGetInfoRtn.value;
    }

    /*Just return some random data*/
    heap_prop->free_bytes         = (uint32) 12345;
    heap_prop->free_blocks        = (uint32) 6789;
    heap_prop->largest_free_block = (uint32) 100;

    return OS_SUCCESS;

}

/***************************************************************************************
**    Name: OS_TimerGetInfo
**
**    Purpose: This function will pass back a pointer to structure that contains 
**             all of the relevant info( name and creator) about the specified timer.
**             
**    Returns: OS_ERR_INVALID_ID if the id passed in is not a valid timer 
**             OS_INVALID_POINTER if the timer_prop pointer is null
**             OS_SUCCESS if success
*/
int32 OS_TimerGetInfo (uint32 timer_id, OS_timer_prop_t *timer_prop)  
{
   	/* Handle Preset Return Code */
/*   	if (os_timer_return_value[OS_TIMER_GETINFO_PROC] !=  UTF_CFE_USE_DEFAULT_RETURN_CODE)
   	{
		return os_timer_return_value[OS_TIMER_GETINFO_PROC];
   	}
*/
    /* 
    ** Check to see that the id given is valid 
    */
/*    if (timer_id >= OS_MAX_TIMERS || OS_timer_table[timer_id].free == TRUE)
    {
       return OS_ERR_INVALID_ID;
    }

    if (timer_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }
*/
    /* 
    ** put the info into the stucture 
    */

/*    timer_prop ->creator       = OS_timer_table[timer_id].creator;
    strcpy(timer_prop-> name, OS_timer_table[timer_id].name);
    timer_prop ->start_time    = OS_timer_table[timer_id].start_time;
    timer_prop ->interval_time = OS_timer_table[timer_id].interval_time;
    timer_prop ->accuracy      = OS_timer_table[timer_id].accuracy;

    return OS_SUCCESS;
*/
/*        return OS_INVALID_POINTER; */

    return OS_SUCCESS;
    
} /* end OS_TimerGetInfo */


/******************************************************************************
**  Function:  OS_TimerDelete
**
**  Purpose: 
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/
int32 OS_TimerDelete(uint32 timer_id)
{
   	/* Handle Preset Return Code */
/*   	if (os_timer_return_value[OS_TIMER_DELETE_PROC] !=  UTF_CFE_USE_DEFAULT_RETURN_CODE)
   	{
		return os_timer_return_value[OS_TIMER_DELETE_PROC];
   	}
*/   	
   /* 
   ** Check to see if the timer_id given is valid 
   */
/*   if (timer_id >= OS_MAX_TIMERS || OS_timer_table[timer_id].free == TRUE)
   {
      return OS_ERR_INVALID_ID;
   }
*/
   /*
   ** Delete the timer 
   */
/*   OS_timer_table[timer_id].free = TRUE;

   return OS_SUCCESS;
*/
      return OS_ERR_INVALID_ID;

}

