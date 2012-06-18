/*
**
**  Filename: cfe_error.h
**  $Id: cfe_error.h 1.11 2010/10/25 17:49:30EDT jmdagost Exp  $
**
**      Copyright (c) 2004-2012, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software(cFE) was created at NASA's Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used, 
**      distributed and modified only pursuant to the terms of that agreement. 
**
**  Title:    cFE Status Code Definition Header File
**
**  Purpose:
**            Common source of cFE API return status codes.
**
**  Design Notes:
**
**  References:
**     Flight Software Branch C Coding Standard Version 1.0a
**
**  $Date: 2010/10/25 17:49:30EDT $
**  $Revision: 1.11 $
**  $Log: cfe_error.h  $
**  Revision 1.11 2010/10/25 17:49:30EDT jmdagost 
**  Added FS Filename Length Error code, renumbered subsequent FS codes.
**  Revision 1.10 2010/10/04 17:06:28EDT jmdagost 
**  Cleaned up copyright symbol.
**  Revision 1.9 2009/07/29 15:55:18EDT aschoeni 
**  Added an error code for GetPoolBufInfo when the buffer is not in the pool
**  Revision 1.8 2009/07/24 13:51:32EDT aschoeni 
**  Added SB_BUFFER_INVALID error type to support Zero Copy Mode
**  Revision 1.7 2009/05/01 14:34:15EDT dkobe 
**  Added special note to CFE_TBL_ERR_NEVER_LOADED error code
**  Revision 1.6 2009/05/01 14:26:32EDT dkobe 
**  Modified return code contents to emphasize success vs failure of API calls
**  Revision 1.5 2008/12/08 12:06:59EST dkobe 
**  Updates to correct doxygen errors
**  Revision 1.4 2008/08/27 16:58:29EDT apcudmore 
**  added error code for timer delete error in ES task cleanup
**  Revision 1.3 2008/08/06 22:42:36EDT dkobe 
**  Added CFE_TIME_RegisterSynchCallback, CFE_TIME_UnregisterSynchCallback and CFE_TIME_CleanUpApp
**  Revision 1.2 2008/06/20 15:40:32EDT apcudmore 
**  Added support for OSAL based module loader
**   - Removed calls and references to the BSP based module loader
**  Revision 1.1 2008/04/17 08:05:18EDT ruperera 
**  Initial revision
**  Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/src/inc/project.pj
**  Revision 1.40 2007/05/25 16:24:49EDT David Kobe (dlkobe) 
**  Continued updating doxygen comments
**  Revision 1.39 2007/05/25 11:57:50EDT apcudmore 
**  Added CFE_ES_APP_CLEANUP_ERR
**  Revision 1.38 2007/05/23 11:22:01EDT dlkobe 
**  Added doxygen formatting
**  Revision 1.37 2007/05/14 10:26:03EDT apcudmore 
**  Preserve the EVS Local event log on a processor restart:
**  - moved the EVS Log to the ES reset data structure
**  - Added a couple of EVS Log related variables into the EVS_Log_t structure
**  - Added a routine to the EVS early init to get a pointer to the EVS Log 
**  - Added code to the EVS housekeeping service routine to copy Log variables into TLM
**  Revision 1.36 2007/05/04 15:53:17EDT dlkobe 
**  Added error codes returned by CFE_ES_DeleteCDS function
**  Revision 1.35 2007/04/28 14:48:59EDT dlkobe 
**  Baseline Implementation of Critical Tables
**  Revision 1.32 2007/04/04 16:06:39EDT dlkobe 
**  Made numerous changes to correct issues identified during ES code walkthrough
**  Revision 1.30 2007/03/23 10:47:22EST apcudmore 
**  Added CFE_ES_GetTaskInfo call and corresponding CFE_ES_TaskInfo_t data structure.
**
**/

/*
** Ensure that header is included only once...
*/
#ifndef _cfe_error_
#define _cfe_error_

/* Include Files */
#include "osapi.h"

/*
**  Status Codes are 32 bit values formatted as follows:
**
**   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
**   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
**  +---+---+-----+-----------------+-------------------------------+
**  |Sev| R | Srv | Mission Defined |               Code            |
**  +---+---+-----+-----------------+-------------------------------+
**
**  where
**
**      Sev - is the severity code
**
**          00 - Success
**          01 - Informational
**          11 - Error
**
**      R - are reserved bits
**
**      Srv - is the cFE Service Identifier
**
**          000 - Not a cFE Service
**          001 - Events Services
**          010 - Executive Services
**          011 - File Services
**          100 - OS API Services
**          101 - Software Bus Services
**          110 - Tables Services
**          111 - Time Services
**
**
**      Mission Defined - These bits are available for Mission
**                        specific coding standards.  They can
**                        be used to classify error codes related
**                        to mission specific library function calls, etc.
**
**      Code - is the status code
*/


/*
** Error Severity
*/
#define CFE_SEVERITY_BITMASK     (0xc0000000L)

#define CFE_SEVERITY_SUCCESS     (0x00000000L)
#define CFE_SEVERITY_INFO        (0x40000000L)
#define CFE_SEVERITY_ERROR       (0xc0000000L)

/*
** cFE Service Identifiers
*/
#define CFE_SERVICE_BITMASK      (0x0e000000L)

#define CFE_EVENTS_SERVICE       (0x02000000L)
#define CFE_EXECUTIVE_SERVICE    (0x04000000L)
#define CFE_FILE_SERVICE         (0x06000000L)
#define CFE_OSAPI_SERVICE        (0x08000000L)
#define CFE_SOFTWARE_BUS_SERVICE (0x0a000000L)
#define CFE_TABLE_SERVICE        (0x0c000000L)
#define CFE_TIME_SERVICE         (0x0e000000L)

/*
************* COMMON STATUS CODE *************
*/

/**
**  Operation was performed successfully
*/
#define CFE_SUCCESS              (0L)

/*
************* EVENTS SERVICES STATUS CODES *************
*/

/**
**  #CFE_EVS_Register() FilterScheme parameter was illegal
**
*/
#define CFE_EVS_UNKNOWN_FILTER            (0xc2000001L)

/**
**  Calling application never previously called #CFE_EVS_Register()
**
*/
#define CFE_EVS_APP_NOT_REGISTERED        (0xc2000002L)

/**
**  Application ID returned by #CFE_ES_GetAppIDByName is greater
**  than #CFE_ES_MAX_APPLICATIONS
**
*/
#define CFE_EVS_APP_ILLEGAL_APP_ID        (0xc2000003L)

/**
**  Number of Application event filters input upon
**  registration is greater than #CFE_EVS_MAX_EVENT_FILTERS
**
*/
#define CFE_EVS_APP_FILTER_OVERLOAD        (0xc2000004L)

/**
**  Could not get pointer to the ES Reset area, so we could
**  not get the pointer to the EVS Log.
**
*/
#define CFE_EVS_RESET_AREA_POINTER        (0xc2000005L)


/**
**  Current version of cFE does not have the function or the feature
**  of the function implemented.  This could be due to either an early
**  build of the cFE for this platform or the platform does not support
**  the specified feature.
**
*/
#define CFE_EVS_NOT_IMPLEMENTED (0xc200ffffL)

/*
************* EXECUTIVE SERVICES STATUS CODES *************
*/

/**
**  The given application ID does not reflect a currently active application.
**
*/
#define CFE_ES_ERR_APPID     (0xc4000001L)

/**
**  There is no match for the given application name in the current application list.
**
*/
#define CFE_ES_ERR_APPNAME   (0xc4000002L)

/**
**  Invalid pointer argument (NULL)
**
*/
#define CFE_ES_ERR_BUFFER    (0xc4000003L)

/**
**  There was an error loading or creating the App.
**
*/
#define CFE_ES_ERR_APP_CREATE  (0xc4000004L)

/**
**  There was an error creating a child task.
**
*/
#define CFE_ES_ERR_CHILD_TASK_CREATE  (0xc4000005L)

/**
**  The cFE system Log is full.
**
*/
#define CFE_ES_ERR_SYS_LOG_FULL  (0xc4000006L)

/**
**  The Memory Pool handle is invalid.
**
*/
#define CFE_ES_ERR_MEM_HANDLE  (0xc4000007L)

/**
**  The block size requested is invalid.
**
*/
#define CFE_ES_ERR_MEM_BLOCK_SIZE  (0xc4000008L)

/**
**  Could not load the shared library.
**
*/
#define CFE_ES_ERR_LOAD_LIB  (0xc4000009L)

/**
**  Bad parameter passed into an ES API.
**
*/
#define CFE_ES_BAD_ARGUMENT  (0xc400000aL)

/** 
**  Errors occured when trying to register a child task.
**
*/
#define CFE_ES_ERR_CHILD_TASK_REGISTER  (0xc400000bL)

/** 
**  Error occured ehen trying to pass a system call to the OS shell
**
*/
#define CFE_ES_ERR_SHELL_CMD  (0xc400000cL)

/**
**  The Application is receiving the pointer to a CDS that was already present.
**
*/
#define CFE_ES_CDS_ALREADY_EXISTS  (0x4400000dL)


/**
**  The Application is requesting a CDS Block that is larger than the remaining
**  CDS memory.
**
*/
#define CFE_ES_CDS_INSUFFICIENT_MEMORY  (0xc400000eL)


/**
**  The Application is requesting a CDS Block with an invalid ASCII string name.
**  Either the name is too long (> #CFE_ES_CDS_MAX_NAME_LENGTH) or was an empty string.
**
*/
#define CFE_ES_CDS_INVALID_NAME  (0xc400000fL)


/**
**  The Application is requesting a CDS Block with a size of zero.
**
*/
#define CFE_ES_CDS_INVALID_SIZE  (0xc4000010L)


/**
**  The CDS Registry has as many entries in it as it can hold.  The
**  CDS Registry size can be adjusted with the #CFE_ES_CDS_MAX_NUM_ENTRIES
**  macro defined in the cfe_platform_cfg.h file.
**
*/
#define CFE_ES_CDS_REGISTRY_FULL  (0xc4000011L)


/**
**  The CDS contents are invalid.
**
*/
#define CFE_ES_CDS_INVALID  (0xc4000012L)


/**
**  The CDS was inaccessible
**
*/
#define CFE_ES_CDS_ACCESS_ERROR  (0xc4000013L)


/**
**  Occurs when a file operation fails
**
*/
#define CFE_ES_FILE_IO_ERR  (0xc4000014L)


/**
**  Occurs when the BSP is not successful in returning the reset area address.
**
*/
#define CFE_ES_RST_ACCESS_ERR  (0xc4000015L)

/**
**  Occurs when the Task ID passed into #CFE_ES_GetTaskInfo is invalid.
**
*/
#define CFE_ES_ERR_TASKID  (0xc4000016L)

/**
**  Occurs when the #CFE_ES_RegisterApp fails.
**
*/
#define CFE_ES_ERR_APP_REGISTER  (0xc4000017L)

/**
**  There was an error deleting a child task.
**
*/
#define CFE_ES_ERR_CHILD_TASK_DELETE  (0xc4000018L)

/**
**  There was an attempt to delete a cFE App Main Task with
**  the #CFE_ES_DeleteChildTask API.
**
*/
#define CFE_ES_ERR_CHILD_TASK_DELETE_MAIN_TASK  (0xc4000019L)

/**
**  Occurs when trying to read a CDS Data block and the CRC of the current
**  data does not match the stored CRC for the data.  Either the contents of
**  the CDS Data Block are corrupted or the CDS Control Block is corrupted.
**
*/
#define CFE_ES_CDS_BLOCK_CRC_ERR  (0xc400001AL)

/**
**  Occurs when trying to delete a Mutex that belongs to a task that ES
**  is cleaning up.
**
*/
#define CFE_ES_MUT_SEM_DELETE_ERR  (0xc400001BL)


/**
**  Occurs when trying to delete a Binary Semaphore that belongs to a task that ES
**  is cleaning up.
**
*/
#define CFE_ES_BIN_SEM_DELETE_ERR  (0xc400001CL)

/**
**  Occurs when trying to delete a Counting Semaphore that belongs to a task that ES
**  is cleaning up.
**
*/
#define CFE_ES_COUNT_SEM_DELETE_ERR  (0xc400001DL)

/**
**  Occurs when trying to delete a Queue that belongs to a task that ES
**  is cleaning up.
**
*/
#define CFE_ES_QUEUE_DELETE_ERR  (0xc400001EL)

/**
**  Occurs when trying to close a file that belongs to a task that ES
**  is cleaning up.
**
*/
#define CFE_ES_FILE_CLOSE_ERR  (0xc400001FL)

/**
**  Occurs when Table Services is trying to delete a Critical Data Store that
**  is not a Critical Table Image or when Executive Services is trying to delete
**  a Critical Table Image.
**
*/
#define CFE_ES_CDS_WRONG_TYPE_ERR  (0xc4000020L)

/**
**  Occurs when a search of the Critical Data Store Registry does not find a
**  critical data store with the specified name.
**
*/
#define CFE_ES_CDS_NOT_FOUND_ERR   (0xc4000021L)

/**
**  Occurs when an attempt was made to delete a CDS when an application
**  with the same name associated with the CDS is still present.  CDSs
**  can ONLY be deleted when Applications that created them are not present
**  in the system.
**
*/
#define CFE_ES_CDS_OWNER_ACTIVE_ERR (0xc4000022L)


/**
**  Occurs when an attempt was made to Clean Up an application 
**  which involves calling Table, EVS, and SB cleanup functions, then
**  deleting all ES resources, child tasks, and unloading the 
**  object module. The approach here is to keep going even though one
**  of these steps had an error. There will be syslog messages detailing
**  each problem.
**
*/
#define CFE_ES_APP_CLEANUP_ERR (0xc4000023L)

/**
**  Occurs when trying to delete a Timer that belongs to a task that ES
**  is cleaning up.
**
*/
#define CFE_ES_TIMER_DELETE_ERR (0xc4000024L)

/**
**  The specified address is not in the memory pool.
**
*/
#define CFE_ES_BUFFER_NOT_IN_POOL (0xc4000025L)


/**
**  Current version of cFE does not have the function or the feature
**  of the function implemented.  This could be due to either an early
**  build of the cFE for this platform or the platform does not support
**  the specified feature.
**
*/
#define CFE_ES_NOT_IMPLEMENTED  (0xc400ffffL)


/*
************* FILE SERVICES STATUS CODES *************
*/

/*
**  A parameter given by a caller to a File Services API did not pass 
**  validation checks.
**
*/
#define CFE_FS_BAD_ARGUMENT             (0xc6000001L)

/*
**  FS was unable to extract a filename from a path string
**
*/
#define CFE_FS_INVALID_PATH             (0xc6000002L)

/*
**  FS filename string is too long
**
*/
#define CFE_FS_FNAME_TOO_LONG           (0xc6000003L)

/*
** The GZIP file contains invalid data and cannot be read
*/
#define CFE_FS_GZIP_BAD_DATA            (0xc6000004L)

/*
** The GZIP file codeblock is bad, which means the file is 
** most likely corrupted
*/
#define CFE_FS_GZIP_BAD_CODE_BLOCK      (0xc6000005L)

/*
** The memory buffer used by the decompression routine is 
** exhausted.
*/
#define CFE_FS_GZIP_NO_MEMORY           (0xc6000006L)

/*
** There is a CRC error in the GZIP file, which means the 
** file is most likely corrupted.
*/
#define CFE_FS_GZIP_CRC_ERROR           (0xc6000007L)

/*
** There is a length error in the GZIP internal data 
** structures, which means the file is most likely corrupted.
*/
#define CFE_FS_GZIP_LENGTH_ERROR        (0xc6000008L)

/*
** An error occurred trying to write the uncompressed 
** file.
*/
#define CFE_FS_GZIP_WRITE_ERROR         (0xc6000009L)

/*
** An error occurred trying to read the GZIP file
*/ 
#define CFE_FS_GZIP_READ_ERROR          (0xc600000AL)

/* 
** An error occurred trying to open the DestinationFile
** where the GZIP file will be uncompressed. The 
** function must be able to open a new write-only file to
** store the uncompressed file in.
*/
#define CFE_FS_GZIP_OPEN_OUTPUT         (0xc600000BL)

/*
** An error occurred trying to open the GZIP file 
** to be decompressed. The function must be able to open
** the GZIP file as read-only in order to decompress it 
** to a new file ( most likely in a RAM disk )
*/
#define CFE_FS_GZIP_OPEN_INPUT          (0xc600000CL)

/*
** An error occured trying to read the GZIP file header,
** which means the file is most likely corrupted or 
** not a valid GZIP file.
*/
#define CFE_FS_GZIP_READ_ERROR_HEADER   (0xc600000DL)

/*
** An error occurred trying to read the GZIP index, 
** which means the file is most likely corrupted.
*/
#define CFE_FS_GZIP_INDEX_ERROR         (0xc600000EL)

/*
** The file to be decompressed is not a valid GZIP file
*/
#define CFE_FS_GZIP_NON_ZIP_FILE        (0xc600000FL) 

/**
**  Current version of cFE does not have the function or the feature
**  of the function implemented.  This could be due to either an early
**  build of the cFE for this platform or the platform does not support
**  the specified feature.
**
*/
#define CFE_FS_NOT_IMPLEMENTED  (0xc600ffffL)

/*
************* OSAPI STATUS CODES *************
*/

/**
**  
**
*/
#define CFE_OS_ERROR                       (OS_ERROR)

/**
**  
**
*/
#define CFE_OS_INVALID_POINTER             (OS_INVALID_POINTER)

/**
**  
**
*/
#define CFE_OS_ERROR_ADDRESS_MISALIGNED    (OS_ERROR_ADDRESS_MISALIGNED)

/**
**  
**
*/
#define CFE_OS_ERROR_TIMEOUT               (OS_ERROR_TIMEOUT)

/**
**  
**
*/
#define CFE_OS_INVALID_INT_NUM             (OS_INVALID_INT_NUM)

/**
**  
**
*/
#define CFE_OS_SEM_FAILURE                 (OS_SEM_FAILURE)

/**
**  
**
*/
#define CFE_OS_SEM_TIMEOUT                 (OS_SEM_TIMEOUT)

/**
**  
**
*/
#define CFE_OS_QUEUE_EMPTY                 (OS_QUEUE_EMPTY)

/**
**  
**
*/
#define CFE_OS_QUEUE_FULL                  (OS_QUEUE_FULL)

/**
**  
**
*/
#define CFE_OS_QUEUE_TIMEOUT               (OS_QUEUE_TIMEOUT)

/**
**  
**
*/
#define CFE_OS_QUEUE_INVALID_SIZE          (OS_QUEUE_INVALID_SIZE)

/**
**  
**
*/
#define CFE_OS_QUEUE_ID_ERROR              (OS_QUEUE_ID_ERROR)

/**
**  
**
*/
#define CFE_OS_ERR_NAME_TOO_LONG           (OS_ERR_NAME_TOO_LONG)

/**
**  
**
*/
#define CFE_OS_ERR_NO_FREE_IDS             (OS_ERR_NO_FREE_IDS)

/**
**  
**
*/
#define CFE_OS_ERR_NAME_TAKEN              (OS_ERR_NAME_TAKEN)

/**
**  
**
*/
#define CFE_OS_ERR_INVALID_ID              (OS_ERR_INVALID_ID)

/**
**  
**
*/
#define CFE_OS_ERR_NAME_NOT_FOUND          (OS_ERR_NAME_NOT_FOUND)

/**
**  
**
*/
#define CFE_OS_ERR_SEM_NOT_FULL            (OS_ERR_SEM_NOT_FULL)

/**
**  
**
*/
#define CFE_OS_ERR_INVALID_PRIORITY        (OS_ERR_INVALID_PRIORITY)

/**
**  
**
*/
#define CFE_OS_ERROR_TASK_ID               (OS_ERROR_TASK_ID)

/**
**  
**
*/
#define CFE_OS_SEM_UNAVAILABLE             (OS_SEM_UNAVAILABLE)

/**
**  
**
*/
#define CFE_OS_FS_ERROR                    (OS_FS_ERROR)

/**
**  
**
*/
#define CFE_OS_FS_ERR_INVALID_POINTER      (OS_FS_ERR_INVALID_POINTER)

/**
**  
**
*/
#define CFE_OS_FS_ERR_PATH_TOO_LONG        (OS_FS_ERR_PATH_TOO_LONG)

/**
**  
**
*/
#define CFE_OS_FS_ERR_NAME_TOO_LONG        (OS_FS_ERR_NAME_TOO_LONG)

/**
**  
**
*/
#define CFE_OS_FS_ERR_DRIVE_NOT_CREATED    (OS_FS_ERR_DRIVE_NOT_CREATED)

/**
**  Current version of cFE does not have the function or the feature
**  of the function implemented.  This could be due to either an early
**  build of the cFE for this platform or the platform does not support
**  the specified feature.
**
*/
#define CFE_OSAPI_NOT_IMPLEMENTED          (OS_FS_UNIMPLEMENTED)

/*
************* SOFTWARE BUS SERVICES STATUS CODES *************
*/

/**
**  In #CFE_SB_RcvMsg, this return value indicates that a packet has not 
**  been received in the time given in the "timeout" parameter.
**
*/
#define CFE_SB_TIME_OUT         (0xca000001L)


/**
**  When "Polling" a pipe for a message in #CFE_SB_RcvMsg, this return 
**  value indicates that there was not a message on the pipe.
**
*/
#define CFE_SB_NO_MESSAGE       (0xca000002L)


/**
**  A parameter given by a caller to a Software Bus API did not pass 
**  validation checks.
**
*/
#define CFE_SB_BAD_ARGUMENT     (0xca000003L)


/**
**  This error code will be returned from #CFE_SB_CreatePipe when the  
**  SB cannot accomodate the request to create a pipe because the maximum 
**  number of pipes (#CFE_SB_MAX_PIPES) are in use. This configuration 
**  parameter is defined in the cfe_platform_cfg.h file.
**
*/
#define CFE_SB_MAX_PIPES_MET    (0xca000004L)


/**
**  The maximum number of queues(#OS_MAX_QUEUES) are in use. Or possibly a 
**  lower level problem with creating the underlying queue has occurred
**  such as a lack of memory. If the latter is the problem, the status 
**  code displayed in the event must be tracked.       
**
*/
#define CFE_SB_PIPE_CR_ERR      (0xca000005L)


/**
**  This return value indicates an error at the Queue read level. This
**  error typically cannot be corrected by the caller. Some possible 
**  causes are: queue was not properly initialized or created, the number
**  of bytes read from the queue was not the number of bytes requested in
**  the read. The queue id is invalid. Similar errors regarding the pipe
**  will be caught by higher level code in the Software Bus.
**
*/
#define CFE_SB_PIPE_RD_ERR      (0xca000006L)


/**
**  The size field in the message header indicates the message exceeds the  
**  max Software Bus message size. The max size is defined by   
**  configuration parameter #CFE_SB_MAX_SB_MSG_SIZE in cfe_mission_cfg.h
**
*/
#define CFE_SB_MSG_TOO_BIG      (0xca000007L)


/**
**  This error code will be returned from #CFE_SB_SendMsg when the memory 
**  in the SB message buffer pool has been depleted. The amount of memory  
**  in the pool is dictated by the configuration parameter 
**  #CFE_SB_BUF_MEMORY_BYTES specified in the cfe_platform_cfg.h file. Also 
**  the memory statistics, including current utilization figures and high 
**  water marks for the SB Buffer memory pool can be monitored by sending 
**  a Software Bus command to send the SB statistics packet.
**
*/
#define CFE_SB_BUF_ALOC_ERR     (0xca000008L)


/**
**  Will be returned when calling one of the SB subscription API's if the
**  SB routing table cannot accomodate another unique message ID because
**  the platform configuration parameter #CFE_SB_MAX_MSG_IDS has been met.
**
*/
#define CFE_SB_MAX_MSGS_MET     (0xca000009L)


/**
**  Will be returned when calling one of the SB subscription API's if the
**  SB routing table cannot accomodate another destination for a 
**  particular the given message ID. This occurs when the number of   
**  destinations in use meets the platform configuration parameter 
**  #CFE_SB_MAX_DEST_PER_PKT.
**
*/
#define CFE_SB_MAX_DESTS_MET    (0xca00000aL)


/**
**  This error code is returned by the #CFE_SB_Unsubscribe API if there has
**  not been an entry in the routing tables for the MsgId/PipeId given as
**  parameters.
*/
#define CFE_SB_NO_SUBSCRIBERS   (0xca00000bL)


/**
**  This error code will be returned by the #CFE_SB_Subscribe API if the 
**  code detects an internal index is out of range. The most likely 
**  cause would be a Single Event Upset.
**
*/
#define CFE_SB_INTERNAL_ERR     (0xca00000cL)


/**
**  This error code will be returned when a request such as ...SetMsgTime
**  is made on a packet that does not include a field for msg time.
**
*/
#define CFE_SB_WRONG_MSG_TYPE     (0xca00000dL)


/**
**  This error code will be returned when a request to release or send a
**  zero copy buffer is invalid, such as if the handle or buffer is not
**  correct or the buffer was previously released.
**
*/
#define CFE_SB_BUFFER_INVALID     (0xca00000eL)


/**
**  Current version of cFE does not have the function or the feature
**  of the function implemented.  This could be due to either an early
**  build of the cFE for this platform or the platform does not support
**  the specified feature.
**
*/
#define CFE_SB_NOT_IMPLEMENTED  (0xca00ffffL)

/*
************* TABLE SERVICES STATUS CODES *************
*/

/**
**  The calling Application attempted to pass a
**  Table handle that represented too large an index or
**  identified a Table Access Descriptor that was not used.
**
*/
#define CFE_TBL_ERR_INVALID_HANDLE (int32)(0xcc000001L)

/** 
**  The calling Application attempted to register a table whose
**  name length exceeded the platform configuration value of
**  #CFE_TBL_MAX_NAME_LENGTH or was zero characters long.
**
*/
#define CFE_TBL_ERR_INVALID_NAME (int32)(0xcc000002L)

/**
**  The calling Application attempted to register a table:
**    a) that was a double buffered table with size greater than #CFE_TBL_MAX_DBL_TABLE_SIZE
**    b) that was a single buffered table with size greater than #CFE_TBL_MAX_SNGL_TABLE_SIZE
**    c) that had a size of zero
**
*/
#define CFE_TBL_ERR_INVALID_SIZE (int32)(0xcc000003L)

/**
**   The calling Application has identified a table that has a load pending.
**
*/
#define CFE_TBL_INFO_UPDATE_PENDING (int32)(0x4c000004L)

/**
**  This is an error indicating that the table has never been loaded from
**  either a file or a copy from a block of memory so the contents that the
**  returned pointer is pointing to are zeros.
**  <B> NOTE: Unlike other most other errors, this error condition still
**  returns a valid table pointer.  This pointer must be released with the
**  #CFE_TBL_ReleaseAddress API before the table can be loaded with data. </B>
**
*/
#define CFE_TBL_ERR_NEVER_LOADED (int32)(0xcc000005L)

/**
**  An application attempted to create a table and the Table
**  registry already contained #CFE_TBL_MAX_NUM_TABLES in it.
**
*/
#define CFE_TBL_ERR_REGISTRY_FULL (int32)(0xcc000006L)

/**
**  This is an error that the registration is trying to replace
**  an existing table with the same name.  The previous table 
**  stays in place and the new table is rejected.
**
*/
#define CFE_TBL_WARN_DUPLICATE (int32)(0x4c000007L)

/**
**   The calling application either failed when calling #CFE_TBL_Register,
**   failed when calling #CFE_TBL_Share or forgot to call either one.
**
*/
#define CFE_TBL_ERR_NO_ACCESS   (int32)(0xcc000008L)

/**
**  The calling application is trying to access a table that has
**  been unregistered.
**
*/
#define CFE_TBL_ERR_UNREGISTERED (int32)(0xcc000009L)

/**
**  The calling application does not have a legitimate Application ID.
**  Most likely cause is a failure to register with the cFE via the
**  #CFE_ES_RegisterApp function.
**
*/
#define CFE_TBL_ERR_BAD_APP_ID (int32)(0xcc00000AL)

/**
**   An application attempted to create a table and the Table
**   Handle Array already used all CFE_TBL_MAX_NUM_HANDLES in it.
**
*/
#define CFE_TBL_ERR_HANDLES_FULL (int32)(0xcc00000BL)

/**
**  An application attempted to register a table with the same name
**  as a table that is already in the registry.  The size of the new
**  table is different from the size already in the registry.
**
*/
#define CFE_TBL_ERR_DUPLICATE_DIFF_SIZE (int32)(0xcc00000CL)

/**
**  An application attempted to register a table with the same name
**  as a table that is already in the registry.  The previously registered
**  table is owned by a different application.
**
*/
#define CFE_TBL_ERR_DUPLICATE_NOT_OWNED (int32)(0xcc00000DL)

/**
**  The calling Application has identified a table that has been updated.<BR>
**  \b NOTE: This is a nominal return code informing the calling application
**  that the table identified in the call has had its contents updated since
**  the last time the application obtained its address or status.
**
*/
#define CFE_TBL_INFO_UPDATED (int32)(0x4c00000EL)

/**
**  The calling Application has tried to allocate a working buffer but
**  none were available.
**
*/
#define CFE_TBL_ERR_NO_BUFFER_AVAIL (int32)(0xcc00000FL)

/**
**  The calling Application has attempted to perform a load on a
**  table that was created with "Dump Only" attributes.
**
*/
#define CFE_TBL_ERR_DUMP_ONLY (int32)(0xcc000010L)

/**
**  The calling Application called #CFE_TBL_Load with an illegal
**  value for the second parameter.
**
*/
#define CFE_TBL_ERR_ILLEGAL_SRC_TYPE (int32)(0xcc000011L)

/**
**  The calling Application called #CFE_TBL_Load when another Application
**  was trying to load the table.
**
*/
#define CFE_TBL_ERR_LOAD_IN_PROGRESS (int32)(0xcc000012L)

/**
**  The calling Application called #CFE_TBL_Load with a bad filename.
**
*/
#define CFE_TBL_ERR_FILE_NOT_FOUND (int32)(0xcc000013L)

/**
**  The calling Application called #CFE_TBL_Load with a filename that specified a file
**  that contained more data than the size of the table OR which contained more data
**  than specified in the table header.
**
*/
#define CFE_TBL_ERR_FILE_TOO_LARGE (int32)(0xcc000014L)

/**
**  The calling Application called #CFE_TBL_Load with a filename that specified a file
**  that started with the first byte of the table but contained less data than the size of the table.
**  It should be noted that #CFE_TBL_WARN_PARTIAL_LOAD also indicates a partial load (one that starts
**  at a non-zero offset).
**
*/
#define CFE_TBL_WARN_SHORT_FILE (int32)(0x4c000015L)

/**
**  The calling Application called #CFE_TBL_Load with a filename that specified a file
**  whose content ID was not that of a table image.
**
*/
#define CFE_TBL_ERR_BAD_CONTENT_ID (int32)(0xcc000016L)

/**
**  The calling Application has attempted to update a table without a pending load.
**
*/
#define CFE_TBL_INFO_NO_UPDATE_PENDING (int32)(0x4c000017L)

/**
**  The calling Application tried to update a table that is locked by another user.
**
*/
#define CFE_TBL_INFO_TABLE_LOCKED (int32)(0x4c000018L)

/**
**   The calling Application should call #CFE_TBL_Validate for the specified table.
**
*/
#define CFE_TBL_INFO_VALIDATION_PENDING (int32)(0x4c000019L)

/**
**  The calling Application tried to validate a table that did not have a validation request pending.
**
*/
#define CFE_TBL_INFO_NO_VALIDATION_PENDING (int32)(0x4c00001AL)

/**
**  The calling Application tried to access a table file whose Subtype identifier indicated it was not
**  a table image file.
**
*/
#define CFE_TBL_ERR_BAD_SUBTYPE_ID (int32)(0xcc00001BL)

/**
**  The calling Application tried to access a table file whose Subtype identifier indicated it was not
**  a table image file.
**
*/
#define CFE_TBL_ERR_FILE_SIZE_INCONSISTENT (int32)(0xcc00001CL)

/**
**  The calling Application tried to access a table file whose standard cFE File Header was the wrong size, etc.
**
*/
#define CFE_TBL_ERR_NO_STD_HEADER (int32)(0xcc00001DL)

/**
**  The calling Application tried to access a table file whose standard cFE
**  Table File Header was the wrong size, etc.
**
*/
#define CFE_TBL_ERR_NO_TBL_HEADER (int32)(0xcc00001EL)


/**
**  The calling Application tried to load a table using a filename
**  that was too long.
**
*/
#define CFE_TBL_ERR_FILENAME_TOO_LONG (int32)(0xcc00001FL)


/**
**  The calling Application tried to load a table using a file whose
**  header indicated that it was for a different table.
**
*/
#define CFE_TBL_ERR_FILE_FOR_WRONG_TABLE (int32)(0xcc000020L)


/**
**  The calling Application tried to load a table file whose header
**  claimed the load was larger than what was actually read from the file.
**
*/
#define CFE_TBL_ERR_LOAD_INCOMPLETE (int32)(0xcc000021L)


/**
**  The calling Application tried to load a table file whose header
**  claimed the load did not start with the first byteIt should be noted
**  that #CFE_TBL_WARN_SHORT_FILE also indicates a partial load.
**
*/
#define CFE_TBL_WARN_PARTIAL_LOAD (int32)(0x4c000022L)


/**
**  The calling Application tried to load a table file whose header
**  claimed the load did not start with the first byte and the table
**  image had NEVER been loaded before.  Partial loads are not allowed
**  on uninitialized tables.  It should be noted that
**  #CFE_TBL_WARN_SHORT_FILE also indicates a partial load.
**
*/
#define CFE_TBL_ERR_PARTIAL_LOAD (int32)(0xcc000023L)


/**
**  The calling Application should call #CFE_TBL_Manage for the specified table.
**  The ground has requested a dump of the Dump-Only table and needs to synchronize
**  with the owning application.
**
*/
#define CFE_TBL_INFO_DUMP_PENDING (int32)(0x4c000024L)


/**
**   The calling Application has used an illegal combination of table options.
**   A summary of the illegal combinations are as follows:
**   \par   #CFE_TBL_OPT_USR_DEF_ADDR cannot be combined with any of the following:
**             -# #CFE_TBL_OPT_DBL_BUFFER
**             -# #CFE_TBL_OPT_LOAD_DUMP
**             -# #CFE_TBL_OPT_CRITICAL
**   \par   #CFE_TBL_OPT_DBL_BUFFER cannot be combined with the following:
**             -# #CFE_TBL_OPT_USR_DEF_ADDR
**             -# #CFE_TBL_OPT_DUMP_ONLY
**
*/
#define CFE_TBL_ERR_INVALID_OPTIONS (int32)(0xcc000025L)


/**
**  The calling Application attempted to register a table as "Critical".
**  Table Services failed to create an appropriate Critical Data Store
**  (See System Log for reason) to save the table contents.  The table
**  will be treated as a normal table from now on.
**
*/
#define CFE_TBL_WARN_NOT_CRITICAL (int32)(0x4c000026L)


/**
**  The calling Application registered a critical table whose previous
**  contents were discovered in the Critical Data Store.  The discovered
**  contents were copied back into the newly registered table as the
**  table's initial contents.<BR>
**  \b NOTE: In this situation, the contents of the table are \b NOT 
**  validated using the table's validation function.
**
*/
#define CFE_TBL_INFO_RECOVERED_TBL (int32)(0x4c000027L)


/**
**  Current version of cFE does not have the function or the feature
**  of the function implemented.  This could be due to either an early
**  build of the cFE for this platform or the platform does not support
**  the specified feature.
**
*/
#define CFE_TBL_NOT_IMPLEMENTED (int32)(0xcc00ffffL)


/*
************* TIME SERVICES STATUS CODES *************
*/

/**
**  Current version of cFE does not have the function or the feature
**  of the function implemented.  This could be due to either an early
**  build of the cFE for this platform or the platform does not support
**  the specified feature.
**
*/
#define CFE_TIME_NOT_IMPLEMENTED (0xce00ffffL)

/**
**  One of the TIME Services API functions to set the time with data
**  from an external time source has been called, but TIME Services
**  has been commanded to not accept external time data.  However,
**  the command is still a signal for the Time Server to generate
**  a "time at the tone" command packet using internal data.
**
*/
#define CFE_TIME_INTERNAL_ONLY (0xce000001L)

/**
**  One of the TIME Services API functions to set the time with data
**  from an external time source has been called, but TIME Services
**  has determined that the new time data is invalid.  However,
**  the command is still a signal for the Time Server to generate
**  a "time at the tone" command packet using internal data.
**
** Note that the test for invalid time update data only occurs if TIME
**        Services has previously been commanded to set the clock state
**        to "valid".
*/
#define CFE_TIME_OUT_OF_RANGE (0xce000002L)

/**
**  An attempt to register too many cFE Time Services Synchronization
**  callbacks has been made.  Adjust the #CFE_TIME_MAX_NUM_SYNCH_FUNCS
**  configuration parameter, recompile and try again.
**
*/
#define CFE_TIME_TOO_MANY_SYNCH_CALLBACKS (0xce000003L)

/**
**  An attempt to unregister a cFE Time Services Synchronization
**  callback has failed because the specified callback function was not
**  located in the Synchronization Callback Registry.
**
*/
#define CFE_TIME_CALLBACK_NOT_REGISTERED (0xce000004L)

#endif  /* _cfe_error_ */
