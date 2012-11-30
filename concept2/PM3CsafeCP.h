/*******************************************************************************
********************************************************************************

   COPYRIGHT © 2003 Concept 2

   Contains confidential and proprietary information which may not be copied,
   disclosed or used by others except as expressly authorized in writing
   by Concept 2.

   PM3DDI.h   External Interface Definitions for the PM3 CSAFE Protocol 
				  Interface DLL.

   About:     This module contains the DLL external interface definitions
				  that allow PC-based applications to communicate with PM3s
              using the CSAFE protocol.

   Target:        Win32 Compatible

   Written for:   Concept 2
                  105 Industrial Park Drive
                  Morrisville, VT 05661
                  802-888-5225 (Voice)
                  802-888-6331 (Fax)

   Contacts:      Scott Hamilton 
                  802-888-5226 x3062 (Voice)
                  scotth@concept2.com

   Written by:    Rob Macklin / Andrew Dombek
                  Microprocessor Designs, Inc.
                  65 Longmeadow Drive
                  PO Box 160
                  Shelburne, VT 05482
                  http://www.updesigns.com

   $Revision: 4 $

--------------------------------------------------------------------------------
   $Log: /Concept II/C2PM3/Pc_src/DLLs XPlatform/PM3CsafeCP/PM3CsafeCP.h $
 * 
 * 4     3/23/07 1:19p Adomb
 * PM3CsafeCP DLL V3.10 - Made minor change to allow the DLL to be
 * compiled on a Mac OS.
 * 
 * 3     9/13/06 12:42p Adomb
 * PM3CSafeCP Version 3.8 - Incorporated Mike H's Async I/O Update
 * 
 * 2     8/03/06 9:05a Adomb
 * PM3CsafeCP Version 3.6 - Added tkcmdsetCSAFE_get_status() command to be
 * compatible with old RowPro DLLs.
 * 
 * 1     7/27/05 8:02a Adomb
 * 
 * 4     5/27/04 11:08a Adomb
 * Added get logcard status command to do tickle function.
 * 
 * 3     4/27/04 7:58a Adomb
 * Initial support for single user read/repair/write LogCard.
 * 
 * 2     4/13/04 10:30a Adomb
 * Check in prior to starting work on new version.
 * 
 * 1     10/07/03 4:13p Adomb
 * Release 1.10 - Branched DLL from LogCard Utility.  Renamed INI files
 * with "RP" on the front.
 * 
 * 1     10/07/03 7:17a Adomb
 * Moved workspace
 * 
 * 7     7/09/03 5:51p Adomb
 * Release 1.08 - Added code to deal with differing LogCard read block
 * sizes with different releases of firmware.  Anything version 45 or
 * below is 32 bytes per block.  Anything above verions 45 is 64 bytes per
 * block.
 * 
 * 6     7/09/03 3:11p Adomb
 * Release 1.07 - Changed registry key from Concept 2 to Concept2.
 * Changed LogCard read block size from 32 to 64 bytes.
 * 
 * 5     6/16/03 4:11p Adomb
 * V1.03 - Supports reading the log card.  Additional error checking.
 * 
 * 3     6/10/03 11:19a Adomb
 * Release 1.02 - Added additional error checking.  Fixed problem with the
 * way the registry was being written for the .INI file.
 * 
 * 2     6/05/03 5:04p Adomb
 * DLL Release 1.01 - Fixed bug with buffer allocation, added error
 * checking
 * 
 * 1     6/05/03 9:48a Adomb
 * DLL Release 1.0 - For Peter L.
 * 
 * 1     xx/xx/03 xx:xxa Adomb
 * Initial check-in
 * 
********************************************************************************
*******************************************************************************/
#ifndef _PM3CSAFECP_H
#define _PM3CSAFECP_H

#ifdef _MSC_VER
   #ifdef PM3CSAFECP_EXPORTS
   #define PM3CSAFE_API extern "C" __declspec(dllexport)
   #else
   #define PM3CSAFE_API extern "C" __declspec(dllimport)
   #endif
#else
   #define PM3CSAFE_API
#endif

//  PM3 Event callback definition
#ifndef PM3PROC_TYPES
#define PM3PROC_TYPES
   #ifdef WIN32  
      // Use StdCall on Windows for VB6/.NET support 
      #define PM3CALLBACKAPI __stdcall 
   #else  
      // No StdCall on Mac/Linux  
      #define PM3CALLBACKAPI 
   #endif  

   typedef void (PM3CALLBACKAPI *PM3EVENT_PROC)(unsigned short, unsigned char);
   typedef void (PM3CALLBACKAPI *PM3ASYNC_PROC)(unsigned short, unsigned char *, unsigned short, short, short);
	typedef void (PM3CALLBACKAPI *PM3ASYNC_CSAFE_PROC)(unsigned short, unsigned long *, unsigned short, short, short);
#endif

/*
 * Public CSAFE status byte definitions
 */
#define SLAVE_STATE_MASK         0x0f
#define PREV_FRAME_STATUS_MASK   0x30
#define FRAME_COUNT_MASK         0X80

#define GET_SLAVE_STATE(val)     (val & SLAVE_STATE_MASK)
#define GET_FRAME_STATUS(val)    ((val & PREV_FRAME_STATUS_MASK) >> 4)
#define GET_FRAME_COUNT(val)     ((val & FRAME_COUNT_MASK) >> 7)

enum SLAVE_STATES {
   STATE_ERROR,
   STATE_READY,
   STATE_IDLE,
   STATE_HAVEID,
   STATE_INUSE=5,
   STATE_PAUSED,
   STATE_FINISHED,
   STATE_MANUAL,
   STATE_OFFLINE
};

enum SLAVE_STATUSES {
   STATUS_OK,
   STATUS_PREV_REJECT,
   STATUS_PREV_BAD,
   STATUS_PREV_NOT_READY
};

/*******************************************************************************
 *******************************************************************************
								  Definitions
********************************************************************************
*******************************************************************************/
#define TKFRAME_CSAFE_FRAME_STD_TYPE            0
#define TKFRAME_CSAFE_FRAME_EXT_TYPE            1

#define TKCMDSET_PROGRESS_MESSAGE_TIME   100			// Rate to message Windows app (in ms)

#define CSAFE_BUFFER_SIZE				256
#define CSAFE_DATA_MAXSIZE			   128-5   // Data max is half of frame maxsize to account for 
                                            // "stuffing", minus 5 bytes header/checksum info
// Max number of PM3 commands
#define MAX_PM3_CMDS                256
#define MAX_CSAFE_CMDS              256

// Max number of chars in an INI file key string
#define MAX_INI_FIELD_TEXT_LEN      128
// Max number of data types for a particular command or response
#define MAX_INI_TYPES_PER_CMD       24


/*
 * Data type map.
 */
#define UNDEFINED_TYPE     0
#define DTYPE_BYTE         1
#define DTYPE_INT2         2
#define DTYPE_INT3         3
#define DTYPE_INT4         4
#define DTYPE_FLOAT        5
#define DTYPE_STRING       6

 /*
 * Command type map.
 */
#define TKCMDSET_SHORT_PULL_CONFIG  1
#define TKCMDSET_SHORT_PUSH_CONFIG  2
#define TKCMDSET_SHORT_PUSH_DATA    3
#define TKCMDSET_SHORT_PULL_DATA    4
#define TKCMDSET_LONG_PULL_CONFIG   5
#define TKCMDSET_LONG_PUSH_CONFIG   6
#define TKCMDSET_LONG_PUSH_DATA     7
#define TKCMDSET_LONG_PULL_DATA     8

// Wrapper for proprietary PM3 CSAFE command
#define CSAFE_PM3_CMD               0x7E

/*******************************************************************************
 *******************************************************************************
								  TypeDefs
********************************************************************************
*******************************************************************************/
/* MPD Standard data type definitions */
#ifndef MPD_TYPES
#define MPD_TYPES
typedef unsigned char  UINT8_T;
typedef unsigned short UINT16_T;
typedef unsigned long  UINT32_T;
typedef char           INT8_T;
typedef short          INT16_T;
typedef long           INT32_T;
typedef unsigned char  BOOLEAN_T;
typedef float          FLOAT32_T;
typedef INT16_T        ERRCODE_T;
#endif

typedef struct {
    unsigned char frame_type;
    unsigned char dest_addr;
    unsigned char src_addr;
    unsigned char dest_intf;
    unsigned char src_intf;
} TKFRAME_FRAME_STATUS_T;

/*
 * Information defining the command and response data types.
 */
typedef struct {
   UINT8_T  cmd;
   UINT8_T cmd_type;
   INT8_T * cmd_name;
   INT8_T * cmd_text;
   UINT8_T num_cmd_data_types;
   UINT8_T cmd_data_type[MAX_INI_TYPES_PER_CMD];
   UINT8_T num_rsp_data_types;
   UINT8_T rsp_data_type[MAX_INI_TYPES_PER_CMD];
} TKCMDSET_PM3_COMMAND_T;

#ifdef _MSC_VER
   PM3CSAFE_API UINT32_T tkdatalog_device_memsize[];
#else
   #ifndef PM3CSAFECP_EXPORTS
      extern UINT32_T tkdatalog_device_memsize[];
   #endif
#endif


/********************************************************************************
   Prototypes for standard DLL functions
********************************************************************************/

BOOLEAN_T pm3csafecmds_get_index(UINT8_T cmd, UINT16_T *index);

/****************************************************************************
   tkcmdsetCSAFE_get_dll_version()

   Returns the current version number of this software.

   Inputs:     None

   Outputs:    None
   
   Returns:    UINT16_T    ver_info    High byte is major version number
                                       Low byte is minor version number

****************************************************************************/
PM3CSAFE_API UINT16_T tkcmdsetCSAFE_get_dll_version(void);

/****************************************************************************
   tkcmdsetCSAFE_get_error_name

   About:      Returns the name of the error associated with the code

   Inputs:     ERRCODE_T   ecode       Code to be looked up
               UINT16_T    namelen     Maximum length of name string

   Outputs:    char *      nameptr     Location to place name string

   Returns:    Nothing

****************************************************************************/
PM3CSAFE_API void tkcmdsetCSAFE_get_error_name(ERRCODE_T ecode,char * nameptr,
												UINT16_T namelen);

/****************************************************************************
   tkcmdsetCSAFE_get_error_text

   About:      Returns the text description of the error associated with the code

   Inputs:     ERRCODE_T   ecode       Code to be looked up
               UINT16_T    namelen     Maximum length of text description string

   Outputs:    char *      nameptr     Location to place text description string

   Returns:    Nothing

****************************************************************************/
PM3CSAFE_API void tkcmdsetCSAFE_get_error_text(ERRCODE_T ecode,char * textptr,
												UINT16_T textlen);


/********************************************************************************
   Prototypes for Command Set functions
********************************************************************************/

/****************************************************************************
   tkcmdsetCSAFE_get_cmd_text

   About:      Returns the text description of the PM3 command number

   Inputs:     UINT8_T     cmd         Command number
               UINT16_T    namelen     Maximum length of text description string

   Outputs:    char *      nameptr     Location to place text description string

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3CSAFE_API ERRCODE_T tkcmdsetCSAFE_get_cmd_text(UINT8_T cmd, char * textptr,
												UINT16_T textlen);

/****************************************************************************
   tkcmdsetCSAFE_get_cmd_name

   About:      Returns the text name of the PM3 command number

   Inputs:     UINT8_T     cmd         Command number
               UINT16_T    namelen     Maximum length of text name string

   Outputs:    char *      nameptr     Location to place text name string

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3CSAFE_API ERRCODE_T tkcmdsetCSAFE_get_cmd_name(UINT8_T cmd, char * textptr,
												UINT16_T textlen);

/****************************************************************************
   tkcmdsetCSAFE_get_cmd_data_types

   About:      Returns the command and response data types associated with
					the PM3 command number.

   Inputs:     UINT8_T     cmd						Command number

   Outputs:    UINT8_T *cmd_type						PM3 command type
					UINT8_T *num_cmd_data_types		Number of command data types
					UINT8_T cmd_data_type[]				Array of command data types
					UINT8_T * num_rsp_data_types		Number of response data types
					UINT8_T rsp_data_type[]				Array of response data types

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3CSAFE_API ERRCODE_T tkcmdsetCSAFE_get_cmd_data_types(UINT8_T cmd, UINT8_T *cmd_type, UINT8_T *num_cmd_data_types,
																  UINT8_T cmd_data_type[], UINT8_T * num_rsp_data_types,
																  UINT8_T rsp_data_type[]);

/****************************************************************************
   tkcmdsetCSAFE_init_protocol

   About:      Initializes the DLL error code interface and configures the 
               CSAFE protocol.  Uses extended frame addressing. 
   
   Inputs:     UINT16_T    timeout     Command/response time out in MS

   Outputs:    Nothing

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3CSAFE_API ERRCODE_T tkcmdsetCSAFE_init_protocol(UINT16_T timeout);

/****************************************************************************
   tkcmdsetCSAFE_write_logblk8

   About:      Writes a 8-bit wide block of data to a logcard memory location

   Inputs:     UINT16_T    unit_address	Address of PM device
               UINT32_T    address			Address of device memory to write
               UINT8_T *   val_ptr8			Location of data block to write
               UINT32_T    byte_len			Number of bytes to write

   Outputs:    None

   Returns:    ERRCODE_T   ecode				Zero if successful
														Error code otherwise

****************************************************************************/
PM3CSAFE_API ERRCODE_T tkcmdsetCSAFE_write_logblk8(UINT16_T unit_address, UINT32_T logcard_address, 
																 UINT8_T * val_ptr8, UINT32_T byte_len);
                                                 
/****************************************************************************
   tkcmdsetCSAFE_read_logblk8

   About:      Reads an 8-bit block of data from a logcard memory location.

   Inputs:     UINT16_T    unit_address   Address of PM device
               UINT32_T    logcard_address	Address of logcard memory to read
               UINT32_T    byte_len			Number of bytes to be read               

   Outputs:    UINT8_T *  val_ptr8			Location to store block of data read
					UINT32_T * num_read			Actual number of bytes read

   Returns:    ERRCODE_T   ecode				Zero if successful
														Error code otherwise

****************************************************************************/
PM3CSAFE_API ERRCODE_T tkcmdsetCSAFE_read_logblk8(UINT16_T unit_address, UINT32_T logcard_address, 
																	  UINT32_T byte_len, UINT8_T *val_ptr8, 
																	  UINT32_T *num_read);

/****************************************************************************
   tkcmdsetCSAFE_get_logcard_status

   About:      Get the logcard status
               (see Object.h)

   Inputs:     UINT16_T    unit_address	Address of PM device
					UINT8_T		*status        Pointer to status

   Outputs:    None - Write to status

   Returns:    ERRCODE_T   ecode				Zero if successful
														Error code otherwise

****************************************************************************/
PM3CSAFE_API ERRCODE_T tkcmdsetCSAFE_get_logcard_status(UINT16_T unit_address, 
													UINT8_T *status);

/****************************************************************************
   tkcmdsetCSAFE_get_logcard_state

   About:      Get the logcard state
               (see Object.h)

   Inputs:     UINT16_T    unit_address	Address of PM device
					UINT8_T		*state         Pointer to state

   Outputs:    None - Write to state

   Returns:    ERRCODE_T   ecode				Zero if successful
														Error code otherwise

****************************************************************************/
PM3CSAFE_API ERRCODE_T tkcmdsetCSAFE_get_logcard_state(UINT16_T unit_address, 
													UINT8_T *state);

/****************************************************************************
   tkcmdsetCSAFE_command

   About:      Sends a CSAFE command to a PM device and returns
               the response data.  Note: the unit address is determined
               using the DiscoverPM function in the PM3DDI DLL.
   
   Inputs:     UINT16_T    unit_address   Address of PM device
               UINT16_T     cmd_data_size  Size of cmd data     
               UINT32_T    cmd_data[]     Command data

   Outputs:    UINT16_T     *rsp_data_size Size of rsp data
               UINT32_T    rsp_data[]     Response data

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3CSAFE_API ERRCODE_T tkcmdsetCSAFE_command(UINT16_T unit_address,
                                             UINT16_T cmd_data_size, UINT32_T cmd_data[],
                                             UINT16_T *rsp_data_size, UINT32_T rsp_data[]);


/****************************************************************************
   tkcmdsetCSAFE_async_command

   About:      Sends a CSAFE command to a PM device asynchronously.  
               Note: the unit address is determined
               using the DiscoverPM function in the PM3DDI DLL.
   
   Inputs:     UINT16_T    unit_address   Address of PM device
               UINT16_T     cmd_data_size  Size of cmd data     
               UINT32_T    cmd_data[]     Command data
               UINT16_T    cmd_tag	      Identifier to tag this transaction

   Outputs:    Nothing

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3CSAFE_API ERRCODE_T tkcmdsetCSAFE_async_command(UINT16_T unit_address,
												   UINT16_T cmd_data_size,
												   UINT32_T cmd_data[],
												   UINT16_T cmd_tag);

/****************************************************************************
   tkcmdsetCSAFE_get_status

   About:      Gets the CSAFE status byte from the previous transaction.
   
   Inputs:     Nothing

   Outputs:    Nothing

   Returns:    UINT8_T     slave_status   CSAFE status byte

****************************************************************************/
PM3CSAFE_API UINT8_T tkcmdsetCSAFE_get_status(void);


/****************************************************************************
   tkcmdsetCSAFE_register_asyncio

   About:      Register a callback for asynchronous I/O functions.

   Inputs:     PM3ASYNC_PROC	callback	Callback function to receive events
               UINT16_T			timeout		Time to wait for response in milliseconds

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3CSAFE_API ERRCODE_T tkcmdsetCSAFE_register_asyncio(PM3ASYNC_CSAFE_PROC callback, UINT16_T timeout);

/****************************************************************************
   tkcmdsetCSAFE_unregister_asyncio

   About:      Unregister the asynchronous I/O callback

   Inputs:     None

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3CSAFE_API ERRCODE_T tkcmdsetCSAFE_unregister_asyncio();


#endif   /* _PM3CSAFECP_H */
