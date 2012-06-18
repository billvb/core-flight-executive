/******************************************************************************
** File:  ccsds.h
**
**      Copyright (c) 2004-2012, United States government as represented by the
**      administrator of the National Aeronautics Space Administration.
**      All rights reserved. This software(cFE) was created at NASA's Goddard
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used,
**      distributed and modified only pursuant to the terms of that agreement.
**
**
** Purpose:
**      Define typedefs and macros for CCSDS packet headers.
**
** $Log: ccsds.h  $
** Revision 1.4 2010/10/25 15:01:27EDT jmdagost 
** Corrected bad apostrophe in prologue.
** Revision 1.3 2010/10/04 15:25:32EDT jmdagost 
** Cleaned up copyright symbol.
** Revision 1.2 2010/09/21 16:15:16EDT jmdagost 
** Removed unused function prototypes.
** Revision 1.1 2008/04/17 08:05:18EDT ruperera 
** Initial revision
** Member added to project c:/MKSDATA/MKS-REPOSITORY/MKS-CFE-PROJECT/fsw/cfe-core/src/inc/project.pj
** Revision 1.4 2006/06/12 11:18:18EDT rjmcgraw 
** Added legal statement
** Revision 1.3 2006/04/28 15:09:44EDT rjmcgraw
** Corrected comments in CCSDS_CmdSecHdr_t definition
**
******************************************************************************/

#ifndef _ccsds_
#define _ccsds_

/*
** Include Files
*/

#include "common_types.h"
#include "cfe_time.h"


/*
** Type Definitions
*/

/**********************************************************************
** Structure definitions for CCSDS headers.  All items in the structure
** must be aligned on 16-bit words.  Bitfields must be avoided since
** some compilers (such as gcc) force them into 32-bit alignment.
**********************************************************************/

/*----- CCSDS packet primary header. -----*/

typedef struct {

   uint16  StreamId;     /* packet identifier word (stream ID) */
      /*  bits  shift   ------------ description ---------------- */
      /* 0x07FF    0  : application ID                            */
      /* 0x0800   11  : secondary header: 0 = absent, 1 = present */
      /* 0x1000   12  : packet type:      0 = TLM, 1 = CMD        */
      /* 0xE000   13  : CCSDS version, always set to 0            */

   uint16  Sequence;     /* packet sequence word */
      /*  bits  shift   ------------ description ---------------- */
      /* 0x3FFF    0  : sequence count                            */
      /* 0xC000   14  : segmentation flags:  3 = complete packet  */

   uint16  Length;       /* packet length word */
      /*  bits  shift   ------------ description ---------------- */
      /* 0xFFFF    0  : (total packet length) - 7                 */

} CCSDS_PriHdr_t;

/*----- CCSDS command secondary header. -----*/

typedef struct {

   uint16  Command;      /* command secondary header */
      /*  bits  shift   ------------ description ---------------- */
      /* 0x00FF    0  : checksum, calculated by ground system     */
      /* 0x7F00    8  : command function code                     */
      /* 0x8000   15  : reserved, set to 0                        */

} CCSDS_CmdSecHdr_t;

/*----- CCSDS telemetry secondary header. -----*/

typedef struct {

   uint8  Time[6];
      /* Elements [0-3] are intended to hold the seconds value            */
      /* Elements [4-5] hold the most significant 2 bytes of the 4 byte   */
      /* subsecond value.                                                 */

} CCSDS_TlmSecHdr_t;

/*----- Generic combined command header. -----*/

typedef struct {
   CCSDS_PriHdr_t       PriHdr;
   CCSDS_CmdSecHdr_t    SecHdr;
} CCSDS_CmdPkt_t;

/*----- Generic combined telemetry header. -----*/

typedef struct {
   CCSDS_PriHdr_t       PriHdr;
   CCSDS_TlmSecHdr_t    SecHdr;
} CCSDS_TlmPkt_t;


/*
** Macro Definitions
*/

/**********************************************************************
** Constant values.
**********************************************************************/

/* Value of packet type for a telemetry packet. */
#define CCSDS_TLM  0
/* Value of packet type for a command packet. */
#define CCSDS_CMD  1

/* Value of secondary header flag if secondary header not present. */
#define CCSDS_NO_SEC_HDR   0
/* Value of secondary header flag if secondary header exists. */
#define CCSDS_HAS_SEC_HDR  1

#define NUM_CCSDS_APIDS        2048
#define NUM_CCSDS_PKT_TYPES    2


/**********************************************************************
** Initial values for CCSDS header fields.
**********************************************************************/

/* Initial value of the sequence count. */
#define CCSDS_INIT_SEQ      0
/* Initial value of the sequence flags. */
#define CCSDS_INIT_SEQFLG   3
/* Initial value of the command function code. */
#define CCSDS_INIT_FC       0
/* Initial value of the command checksum. */
#define CCSDS_INIT_CHECKSUM 0

/* Note: the stream ID and length are always explicitly set for a packet,
** so default values are not required.  */


/**********************************************************************
** Macros for reading and writing bit fields in a 16-bit integer.
** These are used to implement the read and write macros below.
**********************************************************************/

/* Read bits specified by 'mask' from 'word' and shift down by 'shift'. */
#define CCSDS_RD_BITS(word,mask,shift) \
   (((word) & mask) >> shift)

/* Shift 'value' up by 'shift' and write to those bits in 'word' that
** are specified by 'mask'.  Other bits in 'word' are unchanged.   */
#define CCSDS_WR_BITS(word,mask,shift,value) \
   ((word) = (uint16)(((word) & ~mask) | (((value) & (mask >> shift)) << shift)))


/**********************************************************************
** Macros for reading and writing the fields in a CCSDS header.  All
** of the macros are used in a similar way:
**
**   CCSDS_RD_xxx(header)        -- Read field xxx from header.
**   CCSDS_WR_xxx(header,value)  -- Write value to field xxx of header.
**
** Note that 'header' is a reference to the actual header structure,
** not to a pointer to the structure.  If using a pointer, one must
** refer to the structure as *pointer.
**
** The CCSDS_WR_xxx macros may refer to 'header' more than once; thus
** the expression for 'header' must NOT contain any side effects.
**********************************************************************/

/* Read entire stream ID from primary header. */
#define CCSDS_RD_SID(phdr)         ((phdr).StreamId)
/* Write entire stream ID to primary header. */
#define CCSDS_WR_SID(phdr,value)   ((phdr).StreamId = (value))

/* Read application ID from primary header. */
#define CCSDS_RD_APID(phdr)         CCSDS_RD_BITS((phdr).StreamId, 0x07FF, 0)
/* Write application ID to primary header. */
#define CCSDS_WR_APID(phdr,value)   CCSDS_WR_BITS((phdr).StreamId, 0x07FF, 0, value)

/* Read secondary header flag from primary header. */
#define CCSDS_RD_SHDR(phdr)         CCSDS_RD_BITS((phdr).StreamId, 0x0800, 11)
/* Write secondary header flag to primary header. */
#define CCSDS_WR_SHDR(phdr,value)   CCSDS_WR_BITS((phdr).StreamId, 0x0800, 11, value)

/* Read packet type (0=TLM,1=CMD) from primary header. */
#define CCSDS_RD_TYPE(phdr)         CCSDS_RD_BITS((phdr).StreamId, 0x1000, 12)
/* Write packet type (0=TLM,1=CMD) to primary header. */
#define CCSDS_WR_TYPE(phdr,value)   CCSDS_WR_BITS((phdr).StreamId, 0x1000, 12, value)

/* Read CCSDS version from primary header. */
#define CCSDS_RD_VERS(phdr)         CCSDS_RD_BITS((phdr).StreamId, 0xE000, 13)
/* Write CCSDS version to primary header. */
#define CCSDS_WR_VERS(phdr,value)   CCSDS_WR_BITS((phdr).StreamId, 0xE000, 13, value)

/* Read sequence count from primary header. */
#define CCSDS_RD_SEQ(phdr)          CCSDS_RD_BITS((phdr).Sequence, 0x3FFF, 0)
/* Write sequence count to primary header. */
#define CCSDS_WR_SEQ(phdr,value)    CCSDS_WR_BITS((phdr).Sequence, 0x3FFF, 0, value)

/* Read sequence flags from primary header. */
#define CCSDS_RD_SEQFLG(phdr)       CCSDS_RD_BITS((phdr).Sequence, 0xC000, 14)
/* Write sequence flags to primary header. */
#define CCSDS_WR_SEQFLG(phdr,value) CCSDS_WR_BITS((phdr).Sequence, 0xC000, 14, value)

/* Read total packet length from primary header. */
#define CCSDS_RD_LEN(phdr)         ((phdr).Length + 7)
/* Write total packet length to primary header. */
#define CCSDS_WR_LEN(phdr,value)   ((phdr).Length = (value) - 7)

/* Read function code from command secondary header. */
#define CCSDS_RD_FC(shdr)           CCSDS_RD_BITS((shdr).Command, 0x7F00, 8)
/* Write function code to command secondary header. */
#define CCSDS_WR_FC(shdr,value)     CCSDS_WR_BITS((shdr).Command, 0x7F00, 8, value)

/* Read checksum from command secondary header. */
#define CCSDS_RD_CHECKSUM(shdr)     CCSDS_RD_BITS((shdr).Command, 0x00FF, 0)
/* Write checksum to command secondary header. */
#define CCSDS_WR_CHECKSUM(shdr,val) CCSDS_WR_BITS((shdr).Command, 0x00FF, 0, val)


/**********************************************************************
** Macros for clearing a CCSDS header to a standard initial state.  All
** of the macros are used in a similar way:
**   CCSDS_CLR_xxx_HDR(header)      -- Clear header of type xxx.
**********************************************************************/

/* Clear primary header. */
#define CCSDS_CLR_PRI_HDR(phdr) \
  ( (phdr).StreamId = 0,\
    (phdr).Sequence = (CCSDS_INIT_SEQ << 0) | (CCSDS_INIT_SEQFLG << 14),\
    (phdr).Length = 0 )

/* Clear command secondary header. */
#define CCSDS_CLR_CMDSEC_HDR(shdr) \
  ( (shdr).Command = (CCSDS_INIT_CHECKSUM << 0) | (CCSDS_INIT_FC << 8) )

/* Clear telemetry secondary header. */
#define CCSDS_CLR_TLMSEC_HDR(shdr) \
  ( (shdr).TimeStamp[0] = 0,\
    (shdr).TimeStamp[1] = 0,\
    (shdr).TimeStamp[2] = 0,\
    (shdr).TimeStamp[3] = 0 )


/**********************************************************************
** Macros for extracting fields from a stream ID.  All of the macros
** are used in a similar way:
**
**   CCSDS_SID_xxx(sid)          -- Extract field xxx from sid.
**********************************************************************/

/* Extract application ID from stream ID. */
#define CCSDS_SID_APID(sid)   CCSDS_RD_BITS(sid, 0x07FF, 0)

/* Extract secondary header flag from stream ID. */
#define CCSDS_SID_SHDR(sid)   CCSDS_RD_BITS(sid, 0x0800, 11)

/* Extract packet type (0=TLM,1=CMD) from stream ID. */
#define CCSDS_SID_TYPE(sid)   CCSDS_RD_BITS(sid, 0x1000, 12)

/* Extract CCSDS version from stream ID. */
#define CCSDS_SID_VERS(sid)   CCSDS_RD_BITS(sid, 0xE000, 13)


/**********************************************************************
** Macros for frequently used combinations of operations.
**
**   CCSDS_INC_SEQ(phdr)           -- Increment sequence count.
**********************************************************************/

/* Increment sequence count in primary header by 1. */
#define CCSDS_INC_SEQ(phdr) \
   CCSDS_WR_SEQ(phdr, CCSDS_RD_SEQ(phdr)+1)


/*********************************************************************/

/*
** Exported Functions
*/

/******************************************************************************
**  Function:  CCSDS_InitPkt()
**
**  Purpose:
**    Initialize a CCSDS packet.  The primary header is initialized with
**    specified values, and if the Clear flag is set, the rest of the packet
**    is filled with zeros.
**
**  Arguments:
**    PktPtr   : Pointer to primary header of packet.
**    StreamId : Stream ID to use for the packet.
**    Length   : Length of the packet in bytes.
**    Clear    : Indicates whether to clear the entire packet:
**                TRUE = fill sequence count and packet data with zeros
**                       (used after a cold restart)
**                FALSE = leave sequence count and packet data unchanged
**                       (used after a warm restart if data must be preserved)
**
**  Return:
**    (none)
*/

void CCSDS_InitPkt (CCSDS_PriHdr_t  *PktPtr,
                    uint16           StreamId,
                    uint16           Length,
                    boolean          Clear );

/******************************************************************************
**  Function:  CCSDS_LoadCheckSum()
**
**  Purpose:
**    Compute and load a checksum for a CCSDS command packet that has a
**    secondary header.
**
**  Arguments:
**    PktPtr   : Pointer to header of command packet.  The packet must
**               have a secondary header and the length in the primary
**               header must be correct.  The checksum field in the packet
**               will be modified.
**
**  Return:
**    (none)
*/

void CCSDS_LoadCheckSum (CCSDS_CmdPkt_t *PktPtr);

/******************************************************************************
**  Function:  CCSDS_ValidCheckSum()
**
**  Purpose:
**    Determine whether a checksum in a command packet is valid.
**
**  Arguments:
**    PktPtr   : Pointer to header of command packet.  The packet must
**               have a secondary header and the length in the primary
**               header must be correct.
**
**  Return:
**    TRUE if checksum of packet is valid; FALSE if not.
**    A valid checksum is 0.
*/

boolean CCSDS_ValidCheckSum (CCSDS_CmdPkt_t *PktPtr);

/******************************************************************************
**  Function:  CCSDS_ComputeCheckSum()
**
**  Purpose:
**    Compute the checksum for a command packet.  The checksum is the XOR of
**    all bytes in the packet; a valid checksum is zero.
**
**  Arguments:
**    PktPtr   : Pointer to header of command packet.  The packet must
**               have a secondary header and the length in the primary
**               header must be correct.
**
**  Return:
**    TRUE if checksum of packet is valid; FALSE if not.
*/

uint8 CCSDS_ComputeCheckSum (CCSDS_CmdPkt_t *PktPtr);


#endif  /* _ccsds_ */
/*****************************************************************************/
