/*******************************************************************************
********************************************************************************

   COPYRIGHT � 2003 Concept 2

   Contains confidential and proprietary information which may not be copied,
   disclosed or used by others except as expressly authorized in writing
   by Concept 2.

   PM3DDI.h   External Interface Definitions for the PM3 Device Discovery & 
				  Interface DLL.

   About:     This module contains the DLL external interface definitions
				  that allow PC-based applications to communicate with PM3s. 
				  Functions in this module deal with discovering PM3s over
				  various media interfaces, and transferring commands and
				  data between the application and PM3s.  This DLL integrates
				  with the lower level media interface DLLs, as well as 
				  (optional) protocol interface DLLs.

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

   $Revision: 12 $

--------------------------------------------------------------------------------
   $Log: /Concept II/C2PM3/Pc_src/DLLs XPlatform/PM3DDICP/PM3DDICP.h $
 * 
 * 12    10/13/08 8:50a Adomb
 * Glenn R added asyncIO routines.
 * 
 * 11    9/16/08 10:54a Adomb
 * PM3DDICP V3.3 - Add command to read/write EEPROM.
 * 
 * 10    1/30/08 3:05p Adomb
 * PM3DDICP V3.2 - Additional fixes to handle dynamic report sizes.
 * 
 * 9     11/12/07 8:15a Adomb
 * PM3DDICP.DLL V3.0 - Added functionality to support interrogation of
 * report sizes.
 * 
 * 8     7/27/07 10:21a Adomb
 * PM3DDICP DLL V2.7 - Added tkcmdsetDDI_find_devices() function.
 * 
 * 7     6/26/07 11:44a Adomb
 * PM3DDI DLL V2.6 - Added product strings in the header file for the PM3A
 * and PM4A.
 * 
 * 6     10/23/06 11:32a Adomb
 * Added a #define to make it compile for the Mac.
 * 
 * 5     9/13/06 12:42p Adomb
 * PM3DDICP Version 2.4 - Incorporated Mike H's Async I/O Update
 * 
 * 4     6/21/06 7:29a Adomb
 * Fixed bug in set_feature command.  Implemented new command to get the
 * loader version number.
 * 
 * 3     5/11/06 8:07a Adomb
 * 
 * 2     4/19/06 4:11p Adomb
 * Checked in before sending code to Mike H.
 * 
 * 1     7/27/05 8:02a Adomb
 * 
 * 2     4/13/04 10:30a Adomb
 * Check in prior to starting work on new version.
 * 
 * 1     10/07/03 4:14p Adomb
 * Release 1.07 - Branched DLLs from LogCard Utility.  INI file names
 * changed with "RP" on the front.
 * 
 * 1     10/07/03 7:18a Adomb
 * Moved workspace
 * 
 * 3     7/09/03 3:12p Adomb
 * Release 1.03 - Changed registry key from Concept 2 to Concept2
 * 
 * 1     6/05/03 9:54a Adomb
 * Release 1.0 - For Peter L.
 * 
 * 1     xx/xx/03 xx:xxa Adomb
 * Initial check-in
 * 
********************************************************************************
*******************************************************************************/
#ifndef _PM3DDI_H
#define _PM3DDI_H

#ifdef _MSC_VER
   #ifdef PM3DDICP_EXPORTS
   #define PM3DDI_API  extern "C" __declspec(dllexport)
   #else
   #define PM3DDI_API  extern "C" __declspec(dllimport)
   #endif
#else
   #define PM3DDI_API
#endif

//  PM3 Event callback definition
#ifndef PM3PROC_TYPES
#define PM3PROC_TYPES
   #ifdef WIN32
	  #define ASYNCAPI __stdcall
      // Use StdCall on Windows for VB6/.NET support
      typedef void (__stdcall *PM3EVENT_PROC)(unsigned short, unsigned char);
	  typedef void (__stdcall *PM3ASYNC_PROC)(unsigned short, unsigned char *, unsigned short, short, short);
	  typedef void (__stdcall *PM3ASYNC_CSAFE_PROC)(unsigned short, unsigned long *, unsigned short, short, short);
   #else
      #define ASYNCAPI
      // No StdCall on Mac/Linux
      typedef void (*PM3EVENT_PROC)(unsigned short, unsigned char);
      typedef void (*PM3ASYNC_PROC)(unsigned short, unsigned char *, unsigned short, short, short);
      typedef void (*PM3ASYNC_CSAFE_PROC)(unsigned short, unsigned long *, unsigned short, short, short);
   #endif
#endif


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

// Maximum size of command/response buffer
#define MAX_CMDRSP_BUFFER_SIZE   512

// Maximum number of USB ports
#define TKUSB_MAX_USB_PORTS      256

/* Maximum number of active PM3 devices */
#define TKPM3DDI_MAX_DEVICES 64

/* Media types */
#define TKPM3DDI_TYPE_UNDEFINED  0
#define TKPM3DDI_TYPE_USB        1
#define TKPM3DDI_TYPE_IP         2
#define TKPM3DDI_TYPE_ASYNC      3

/* Communication status states	*/
#define TKPM3DDI_UNIT_UNKNOWN	0
#define TKPM3DDI_UNIT_OK		1
#define TKPM3DDI_UNIT_ERROR		2

/* Definitions for PM3 identifier strings */
#define TKCMDSET_PM3_PRODUCT_NAME            "Concept II PM3"
#define TKCMDSET_PM3_PRODUCT_NAME2           "Concept2 Performance Monitor 3 (PM3)"
#define TKCMDSET_PM3TESTER_PRODUCT_NAME      "Concept 2 PM3 Tester"
#define TKCMDSET_PM4_PRODUCT_NAME            "Concept2 Performance Monitor 4 (PM4)"

#define TKCMDSET_PROGRESS_MESSAGE            (WM_USER + 100)
#define TKCMDSET_WRITE_RAM_DATA              1
#define TKCMDSET_READ_RAM_DATA               2
#define TKCMDSET_WRITE_FLASH_DATA            3
#define TKCMDSET_READ_FLASH_DATA             4
#define TKCMDSET_WRITE_EE_DATA               5
#define TKCMDSET_READ_EE_DATA                6
#define TKCMDSET_WRITE_LOG_DATA              7
#define TKCMDSET_READ_LOG_DATA               8

/********************************************************************************
   Prototypes for standard DLL functions
********************************************************************************/


/****************************************************************************
   tkcmdsetDDI_get_dll_version()

   Returns the current version number of this software.

   Inputs:     None

   Outputs:    None
   
   Returns:    UINT16_T    ver_info    High byte is major version number
                                       Low byte is minor version number

****************************************************************************/
PM3DDI_API UINT16_T tkcmdsetDDI_get_dll_version(void);

/****************************************************************************
   tkcmdsetDDI_get_error_name

   About:      Returns the name of the error associated with the code

   Inputs:     ERRCODE_T   ecode       Code to be looked up
               UINT16_T    namelen     Maximum length of name string

   Outputs:    char *      nameptr     Location to place name string

   Returns:    Nothing

****************************************************************************/
PM3DDI_API void tkcmdsetDDI_get_error_name(ERRCODE_T ecode,char * nameptr,
												UINT16_T namelen);

/****************************************************************************
   tkcmdsetDDI_get_error_text

   About:      Returns the text description of the error associated with the code

   Inputs:     ERRCODE_T   ecode       Code to be looked up
               UINT16_T    namelen     Maximum length of text description string

   Outputs:    char *      nameptr     Location to place text description string

   Returns:    Nothing

****************************************************************************/
PM3DDI_API void tkcmdsetDDI_get_error_text(ERRCODE_T ecode,char * textptr,
												UINT16_T textlen);



/********************************************************************************
   Prototypes for Command Set functions
********************************************************************************/


/****************************************************************************
   tkcmdsetDDI_init

   About:      Initializes the Command Set Toolkit functions.

   Inputs:     None

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_init(void);

/****************************************************************************
   tkcmdsetDDI_find_devices

   About:      Gets port numbers of all USB HID 
               devices that match the product name

   Inputs:     INT8_T *    product_name Name of USB device to open

   Outputs:    UINT8_T *   num_found   Number of devices that match name
               UINT16_T    port_list[] Port numbers of devices that match

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_find_devices(INT8_T *product_name, UINT8_T *num_found, UINT16_T port_list[]);


/****************************************************************************
   tkcmdsetDDI_discover_pm3s

   About:      Discover all PM3 devices connected to the PC via various
               media interfaces.  Create a PM3 device map that correlates
               consecutive unit identifiers to each device port number and 
               media interface location.  Note that the calling function
               provides the starting address of the unit ID.

   Inputs:     INT8_T      *product_name  Name of product to discover
               UINT16_T    starting_address  Address of first unit

   Outputs:    UINT16_T    *num_units  Number of devices found
                

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_discover_pm3s(INT8_T *product_name, 
                                               UINT16_T starting_address,
                                               UINT16_T *num_units);

/****************************************************************************
   tkcmdsetDDI_init_protocol

   About:      Initialize a protocol engine that will be used for device
               communications.  This is typically called by an external
               DLL to setup a specific communications protocol.

   Inputs:     ERRCODE_T   *frame_builder()  Ptr to frame builder function
               ERRCODE_T   *frame_checker()  Ptr to frame checker function
               UINT16_T    timeout           Command/response time out in MS
               UINT16_T    buffer_size       Max frame size
               UINT8_T     retries           Number of command retries

   Outputs:    None
                

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_init_protocol(ERRCODE_T (*) (UINT8_T *, UINT8_T *, UINT16_T *), 
                                               ERRCODE_T (*) (UINT8_T *, UINT8_T *, UINT16_T *),
                                               UINT16_T timeout, 
                                               UINT16_T buffer_size, UINT8_T retries);

/****************************************************************************
   tkcmdsetDDI_do_protocol

   About:      Utilizing the protocol engine that was previously setup
               in tkcmdsetDDI_init_protocol(), build and send a command
               frame, then receive and check a response frame.
               If the frame is valid, return the data.  

   Inputs:     UINT16_T    port              Identifier for device
               UINT16_T    *num_cmd_bytes    Number of data bytes to send
               UINT8_T     *cmd_data         Data bytes to send


   Outputs:    UINT16_T    *num_rsp_bytes    Number of received bytes
               UINT8_T     *response         Response bytes
               
 

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_do_protocol(UINT16_T port, 
                                             UINT16_T *num_cmd_bytes, UINT8_T *cmd_data,
                                             UINT16_T *num_rsp_bytes, UINT8_T *response);

/****************************************************************************
   tkcmdsetDDI_do_protocol_async

   About:      Utilizing the protocol engine that was previously setup
               in tkcmdsetDDI_init_protocol(), build and send a command
               frame asynchronously.  

   Inputs:     UINT16_T    port              Identifier for device
               UINT16_T    *num_cmd_bytes    Number of data bytes to send
               UINT8_T     *cmd_data         Data bytes to send
               UINT16_T    cmd_tag     Identifier to tag this transaction with

   Outputs:    Nothing
               
 

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_do_protocol_async(UINT16_T port, 
												   UINT16_T *num_cmd_bytes,
												   UINT8_T *cmd_data,
												   UINT16_T cmd_tag);

/****************************************************************************
   tkcmdsetDDI_set_feature

   About:      Sends a USB SetFeature Standard Request to the device
               This is independent of the command / response protocol

   Inputs:     UINT16_T     port        Communication port to use
               UINT16_T    feature     Identifier for desired feature to activate

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_set_feature(UINT16_T port, UINT16_T feature);

/****************************************************************************
   tkcmdsetDDI_do_command

   About:      Handle the command / response transaction with the device

   Inputs:     UINT16_T    port        Identifier for device
               UINT8_T *   tx_ptr      Pointer to data block to be transmitted
               UINT16_T    tx_len      Number of bytes in the command
               UINT16_T    timeout     Time to wait for response in milliseconds

   Outputs:    UINT8_T *   rx_ptr      Pointer to data block to save received data
               UINT16_T*   rx_len      Number of bytes in the response

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_do_command(UINT16_T port, UINT8_T *tx_ptr,
                                            UINT16_T tx_len, UINT8_T *rx_ptr, 
                                            UINT16_T * rx_len, UINT16_T timeout);

/****************************************************************************
   tkcmdsetDDI_do_command_async

   About:      Handle the command transaction with the device

   Inputs:     UINT16_T    port        Identifier for device
               UINT8_T *   tx_ptr      Pointer to data block to be transmitted
               UINT16_T    tx_len      Number of bytes in the command
               UINT16_T    cmd_tag     Identifier to tag this transaction with

   Outputs:    Nothing

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_do_command_async(UINT16_T port,
												  UINT8_T *tx_ptr,
												  UINT16_T tx_len,
												  UINT16_T cmd_tag);

/****************************************************************************
   tkcmdsetDDI_echo

   About:      Display a message on the PM

   Inputs:     UINT16_T    port        Identifier for device
               UINT8_T *   cmd_ptr     Location of data to send to device
               UINT16_T    cmd_len     Length of data to send

   Outputs:    Nothing

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_echo(UINT16_T port, UINT8_T * cmd_ptr, UINT16_T cmd_len);

/****************************************************************************
   tkcmdsetDDI_fw_version

   About:      Reads the firmware version information from the PM3

   Inputs:     UINT16_T    port        Communication port to use
               UINT8_T    ver_len      Length of command in bytes

   Outputs:    INT8_T *   ver_ptr      FW version string stored at this location

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_fw_version(UINT16_T port, INT8_T * ver_ptr,
                                            UINT8_T ver_len);


/****************************************************************************
   tkcmdsetDDI_hw_version

   About:      Reads the hardware version information from the PM3

   Inputs:     UINT16_T     port       Communication port to use

   Outputs:    INT8_T *   ver_ptr      HW version string stored at this location
               UINT8_T    ver_len      Length of command in bytes

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_hw_version(UINT16_T port, INT8_T * ver_ptr,
                                            UINT8_T vers_len);

/****************************************************************************
   tkcmdsetDDI_loader_fw_version

   About:      Reads loader firmware version information from the device.
               SUPPORTED ONLY BY PM3 VERSIONS 95 OR GREATER.

   Inputs:     UINT16_T     port        Identifier for device
               INT8_T       ver_len     Length of command in bytes

   Outputs:    UINT8_T *   ver_ptr     FW version string stored at this location

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_loader_fw_version(UINT16_T port, INT8_T * ver_ptr,
                                            UINT8_T ver_len);

/****************************************************************************
   tkcmdsetDDI_serial_number

   About:      Reads the serial number information from the device

   Inputs:     UINT16_T     port       Identifier for device
               UINT8_T      ser_len    Length of command in bytes

   Outputs:    INT8_T *    ser_ptr     Serial number string stored at this location

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_serial_number(UINT16_T port, INT8_T *ser_ptr,
                                               UINT8_T ser_len);

/****************************************************************************
   tkcmdsetDDI_status

   About:      Reads status information from the device

   Inputs:     UINT16_T     port        Identifier for device

   Outputs:    UINT32_T *  stat_ptr    Location to store status information

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_status(UINT16_T port, UINT32_T *stat_ptr);

/****************************************************************************
   tkcmdsetDDI_special

   About:      Performs special operations based on the command

   Inputs:     UINT16_T     port        Identifier for device
               UINT16_T    cmd         Special command to execute
               UINT32_T    in_data     Value send with command

   Outputs:    UINT32_T *  out_data    Location to store value returned
                                       with response

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_special(UINT16_T port, UINT16_T cmd, 
										 UINT32_T in_data, UINT32_T *out_data);

/****************************************************************************
   tkcmdsetDDI_register_asyncio

   About:      Register a callback for asynchronous I/O functions.

   Inputs:     PM3ASYNC_PROC	callback	Callback function to receive events
               UINT16_T			timeout		Time to wait for response in milliseconds

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_register_asyncio(PM3ASYNC_PROC callback, UINT16_T timeout);

/****************************************************************************
   tkcmdsetDDI_unregister_asyncio

   About:      Unregister the asynchronous I/O callback

   Inputs:     None

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_unregister_asyncio();

/****************************************************************************
   tkcmdsetDDI_write_flashblk16

   About:      Writes a 16-bit block of data to a FLASH memory location
               Note: The USB implementation supports only 1 block write at at time 
               (depends on TKUSB_MAX_PACKET_SIZE)

   Inputs:     UINT8_T     port        Identifier for device
               UINT32_T    address     Address of device memory to write
               UINT16_T *  val_ptr     Location of data block to write
               UINT32_T    len         Number of words to write
               UINT16_T *  packet_rsp_len     Expected response packet size
                        (used to allow lower levels to select the correct response report ID)

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_write_flashblk16(UINT16_T port, UINT32_T address, UINT16_T * val_ptr16, 
                                                  UINT32_T len, UINT16_T * packet_rsp_len);

/****************************************************************************
   tkcmdsetDDI_read_flashblk16

   About:      Reads a 16-bit block of data from a FLASH memory location
               Note: The USB implementation supports only 1 block read at at time 
               (depends on TKUSB_MAX_PACKET_SIZE)

   Inputs:     UINT8_T     port        Identifier for device
               UINT32_T    address     Address of device memory to read
               UINT32_T    len         Number of words to read
               UINT16_T *  packet_rsp_len     Expected response packet size
                        (used to allow lower levels to select the correct response report ID)

   Outputs:    UINT16_T *  val_ptr     Location to store block of data read

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_read_flashblk16(UINT16_T port, UINT32_T address, UINT16_T * val_ptr16, 
                                                 UINT32_T len, UINT16_T * packet_rsp_len);

/****************************************************************************
   tkcmdsetDDI_write_memblk16

   About:      Writes a 16-bit block of data to a memory location
               Note: The USB implementation supports only 1 block write at at time 
               (depends on TKUSB_MAX_PACKET_SIZE)

   Inputs:     UINT8_T     port        Identifier for device
               UINT32_T    address     Address of device memory to write
               UINT16_T *  val_ptr     Location of data block to write
               UINT32_T    len         Number of words to write
               UINT16_T *  packet_rsp_len     Expected response packet size
                        (used to allow lower levels to select the correct response report ID)

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_write_memblk16(UINT16_T port, UINT32_T address, UINT16_T * val_ptr16, 
                                                UINT32_T len, UINT16_T * packet_rsp_len);

/****************************************************************************
   tkcmdsetDDI_write_eeblk8

   About:      Writes an 8-bit block of data to a EEPROM memory location
               Note: The USB implementation supports only 1 block write at at time 
               (depends on TKUSB_MAX_PACKET_SIZE)

   Inputs:     UINT8_T     port        Identifier for device
               UINT32_T    address     Address of device memory to write
               UINT8_T *  val_ptr     Location of data block to write
               UINT16_T    len         Number of bytes to write
               UINT16_T *  packet_rsp_len     Expected response packet size
                        (used to allow lower levels to select the correct response report ID)

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_write_eeblk8(UINT16_T port, UINT32_T address, UINT8_T * val_ptr8, 
                                                  UINT16_T len, UINT16_T * packet_rsp_len);

/****************************************************************************
   tkcmdsetDDI_read_eeblk8

   About:      Reads an 8-bit block of data from a EEPROM memory location
               Note: The USB implementation supports only 1 block read at at time 
               (depends on TKUSB_MAX_PACKET_SIZE)

   Inputs:     UINT8_T     port        Identifier for device
               UINT32_T    address     Address of device memory to read
               UINT16_T    len         Number of words to read
               UINT16_T *  packet_rsp_len     Expected response packet size
                        (used to allow lower levels to select the correct response report ID)

   Outputs:    UINT8_T *  val_ptr     Location to store block of data read

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_read_eeblk8(UINT16_T port, UINT32_T address, UINT8_T * val_ptr, 
                                                 UINT16_T len, UINT16_T * packet_rsp_len);

/****************************************************************************
   tkcmdsetDDI_shutdown

   About:      Shuts down the Command Set Toolkit functions

   Inputs:     UINT16_T     port        Communication port to use

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_shutdown(UINT16_T port);

/****************************************************************************
   tkcmdsetDDI_shutdown_all

   About:      Shuts down the Command Set Toolkit functions (all ports)

   Inputs:     None

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_shutdown_all();

/****************************************************************************
   tkcmdsetDDI_max_report_size

   About:      Returns the largest USB report size available (buffer size)

   Inputs:     UINT16_T     port       Communication port to use

   Outputs:    size_ptr                Maximum number of bytes available in USB report

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_max_report_size(UINT16_T port, UINT16_T *size_ptr);

//
//
// GR - added from previous version
//
//

/****************************************************************************
   tkcmdsetDDI_register_events

   About:      Register a callback for device event functions.

   Inputs:     PM3EVENT_PROC	callback	Callback function to receive events

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_register_events(PM3EVENT_PROC callback);

/****************************************************************************
   tkcmdsetDDI_unregister_events

   About:      Unregister the device event callback

   Inputs:     None

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3DDI_API ERRCODE_T tkcmdsetDDI_unregister_events();

#endif /* _PM3DDI_H */