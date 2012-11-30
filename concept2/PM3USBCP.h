/*******************************************************************************
********************************************************************************

   COPYRIGHT © 2003 Concept 2

   Contains confidential and proprietary information which may not be copied,
   disclosed or used by others except as expressly authorized in writing
   by Concept 2.

   PM3USBCP.h       External Interface Definitions for the PM3/USB interface DLL.

   About:         This module contains external interface definitions for the
				  low level DLL functions that allow applications to interface 
				  with PM3s over USB using HID.
				  The functions in this module are intended to be used with
				  the PM3 Device Discovery and Interface DLL, which is the
				  main API DLL for PC-based applications.

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

   $Revision: 6 $

--------------------------------------------------------------------------------
   $Log: /Concept II/C2PM3/Pc_src/DLLs XPlatform/PM3USBCP/PM3USBCP.h $
 * 
 * 6     9/16/08 10:54a Adomb
 * PM3USBCP V2.3 - Add command to read/write EEPROM.
 * 
 * 5     1/30/08 3:05p Adomb
 * PM3USBCP V2.2 - Additional fixes to handle dynamic report sizes.
 * 
 * 4     11/12/07 8:16a Adomb
 * PM3USBCP.DLL V2.0 - Added functionality to support the use of larger
 * report sizes, fixed minor bugs.
 * 
 * 3     3/23/07 1:22p Adomb
 * PM3USBCP DLL V1.19 - Fixed bug in echo() command.  Updated to allow DLL
 * to be compiled on the Mac.
 * 
 * 2     1/31/07 8:36a Adomb
 * PM3USBCP V1.17 - Incorporated Mike H's additions to support new
 * features for opening USB devices, including a new command
 * tkcmdsetUSB_reset_port(), and an auto-opening function.
 * 
 * 1     9/13/06 11:18a Adomb
 * Mike H's additions to support async I/O.  Files were renamed to be
 * consistent the the other DLLs (removed LC, appended CP).
 * 
 * 5     8/03/06 1:18p Adomb
 * Mike H's additions to support async I/O and fix for USB ID switching
 * 
 * 4     6/21/06 7:30a Adomb
 * Implemented new command to get the loader version number.
 * 
 * 3     4/19/06 4:11p Adomb
 * Checked in before sending code to Mike H.
 * 
 * 2     10/19/05 2:17p Adomb
 * 
 * 2     4/13/04 10:31a Adomb
 * Check in prior to starting work on new version.
 * 
 * 1     10/07/03 4:16p Adomb
 * Release 1.07 - Branched DLLs from LogCard Utility.  Changed INI file
 * names with "RP" on the front.
 * 
 * 1     10/07/03 7:23a Adomb
 * 
 * 1     6/05/03 9:57a Adomb
 * 
 * 1     xx/xx/03 xx:xxa Adomb
 * Initial check-in
 * 
********************************************************************************
*******************************************************************************/
#ifndef _PM3USBCP_H
#define _PM3USBCP_H

#ifdef _MSC_VER
   #ifdef PM3USBCP_EXPORTS
   #define PM3USB_API  extern "C" __declspec(dllexport)
   #else
   #define PM3USB_API  extern "C" __declspec(dllimport)
   #endif

#else
   #define PM3USB_API
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

#ifndef NO_ERR
#define NO_ERR	0
#endif

/* Definitions for PM3 identifier strings */
//#define TKCMDSET_PM3_MANUFACTURER_NAME       L"Concept II"
#define TKCMDSET_PM3_PRODUCT_NAME            "Concept II PM3"
#define TKCMDSET_PM3_PRODUCT_NAME2           "Concept2 Performance Monitor 3 (PM3)"
#define TKCMDSET_PM3TESTER_PRODUCT_NAME      "Concept 2 PM3 Tester"
//#define TKCMDSET_PM3_PRODUCT_NAME            L"Back-UPS 350 FW: 5.2.D USB FW: c1 "

#define TKCMDSET_PROGRESS_MESSAGE            (WM_USER + 100)
#define TKCMDSET_WRITE_RAM_DATA              1
#define TKCMDSET_READ_RAM_DATA               2
#define TKCMDSET_WRITE_FLASH_DATA            3
#define TKCMDSET_READ_FLASH_DATA             4
#define TKCMDSET_WRITE_EE_DATA               5
#define TKCMDSET_READ_EE_DATA                6

#define DDI_CMD_DATA_OFFSET   1
#define DDI_RSP_DATA_OFFSET   1


#define TKCMDSET_PM3_CONNECT     1
#define TKCMDSET_PM3_DISCONNECT  2

/********************************************************************************
   Prototypes for standard DLL functions
********************************************************************************/


/****************************************************************************
   tkcmdsetUSB_get_dll_version()

   Returns the current version number of this software.

   Inputs:     None

   Outputs:    None
   
   Returns:    UINT16_T    ver_info    High byte is major version number
                                       Low byte is minor version number

****************************************************************************/
PM3USB_API UINT16_T tkcmdsetUSB_get_dll_version(void);

/****************************************************************************
   tkcmdsetUSB_get_error_name

   About:      Returns the name of the error associated with the code

   Inputs:     ERRCODE_T   ecode       Code to be looked up
               UINT16_T    namelen     Maximum length of name string

   Outputs:    char *      nameptr     Location to place name string

   Returns:    Nothing

****************************************************************************/
PM3USB_API void tkcmdsetUSB_get_error_name(ERRCODE_T ecode,char * nameptr,
												UINT16_T namelen);

/****************************************************************************
   tkcmdsetUSB_get_error_text

   About:      Returns the text description of the error associated with the code

   Inputs:     ERRCODE_T   ecode       Code to be looked up
               UINT16_T    namelen     Maximum length of text description string

   Outputs:    char *      nameptr     Location to place text description string

   Returns:    Nothing

****************************************************************************/
PM3USB_API void tkcmdsetUSB_get_error_text(ERRCODE_T ecode,char * textptr,
												UINT16_T textlen);



/********************************************************************************
   Prototypes for Command Set functions
********************************************************************************/

/****************************************************************************
   tkcmdsetUSB_init

   About:      Initializes the Command Set Toolkit functions.

   Inputs:     None

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_init();

/****************************************************************************
   tkcmdsetUSB_register_events

   About:      Register a callback for device connect/disconnect events.

   Inputs:     PM3EVENT_PROC	callback	Callback function to receive events

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_register_events(PM3EVENT_PROC callback);

/****************************************************************************
   tkcmdsetUSB_unregister_events

   About:      Unregister the device event callback

   Inputs:     None

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_unregister_events();

/****************************************************************************
   tkcmdsetUSB_register_asyncio

   About:      Register a callback for asynchronous I/O functions.

   Inputs:     PM3ASYNC_PROC		callback	Callback function to receive events
               UINT16_T			timeout		Time to wait for response in milliseconds

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_register_asyncio(PM3ASYNC_PROC callback, UINT16_T timeout);

/****************************************************************************
   tkcmdsetUSB_unregister_asyncio

   About:      Unregister the asynchronous I/O callback

   Inputs:     None

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_unregister_asyncio();

/****************************************************************************
   tkcmdsetUSB_find_devices

   About:      Gets port numbers of all USB HID 
               devices that match the product name

   Inputs:     INT8_T *    product_name Name of USB device to open

   Outputs:    UINT8_T *   num_found   Number of devices that match name
               UINT16_T    port_list[] Port numbers of devices that match

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_find_devices(INT8_T *product_name, UINT8_T *num_found, UINT16_T port_list[]);

/****************************************************************************
   tkcmdsetUSB_set_feature

   About:      Sends a USB SetFeature Standard Request to the device
               This is independent of the command / response protocol

   Inputs:     UINT16_T     port        Communication port to use
               UINT16_T    feature     Identifier for desired feature to activate

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_set_feature(UINT16_T port, UINT16_T feature,
											 UINT16_T timeout);

/****************************************************************************
   tkcmdsetUSB_do_command

   About:      Handle the command / response transaction with the USB device

   Inputs:     UINT16_T    port        Identifier for device
               UINT8_T *   tx_ptr      Pointer to data block to be transmitted
               UINT16_T    timeout     Time to wait for response in milliseconds

   Outputs:    UINT8_T *   rx_ptr      Pointer to data block to save received data

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_do_command(UINT16_T port, 
											UINT8_T *tx_ptr, UINT16_T tx_len,
											UINT8_T *rx_ptr, UINT16_T *rx_len,
											UINT16_T timeout);

/****************************************************************************
   tkcmdsetUSB_send_command

   About:   Send an asynchronous command to the USB device.

   Inputs:  UINT16_T    port				Identifier for device
            UINT8_T *   tx_ptr			Pointer to data block to be transmitted
			   UINT16_T	   tx_len			Transmit buffer length
			   UINT16_T	   cmd_tag			Identifier Tag to pass back to the caller on recv

   Outputs: None

   Returns: ERRCODE_T   ecode			   Zero if successful
													Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_send_command(UINT16_T port, UINT8_T *tx_ptr, UINT16_T tx_len, UINT16_T cmd_tag);

/****************************************************************************
   tkcmdsetUSB_do_DDIcommand

   About:      Send and receive a data block from the DDI interface.

   Inputs:     UINT16_T    port        Identifier for device
               UINT8_T *   cmd_ptr     Pointer to data block to be transmitted
               UINT16_T    cmd_len     Number of bytes in the command
               UINT16_T    timeout     Time to wait for response in milliseconds

   Outputs:    UINT8_T *   rsp_ptr     Pointer to data block to save received data
               UINT16_T*   rsp_len     Number of bytes in the response

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_do_DDIcommand(UINT16_T port, 
											   UINT8_T *cmd_ptr, UINT16_T cmd_len,
											   UINT8_T *rsp_ptr, UINT16_T *rsp_len,
											   UINT16_T timeout);

/****************************************************************************
   tkcmdsetUSB_echo

   About:      Display a message on the PM

   Inputs:     UINT16_T    port        Identifier for device
               UINT8_T *   cmd_ptr     Location of data to send to device
               UINT16_T    cmd_len     Length of data to send

   Outputs:    Nothing

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_echo(UINT16_T port, UINT8_T * cmd_ptr, UINT16_T cmd_len);

/****************************************************************************
   tkcmdsetUSB_status

   About:      Reads status information from the device

   Inputs:     UINT16_T    port        Identifier for device

   Outputs:    UINT32_T *  stat_ptr    Location to store status information

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_status(UINT16_T port, UINT32_T *stat_ptr);

/****************************************************************************
   tkcmdsetUSB_special

   About:      Performs special operations based on the command

   Inputs:     UINT16_T    port        Identifier for device
               UINT16_T    cmd         Special command to execute
               UINT32_T    in_data     Value send with command

   Outputs:    UINT32_T *  out_data    Location to store value returned
                                       with response

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_special(UINT16_T port, UINT16_T cmd, 
                                      UINT32_T in_data, UINT32_T *out_data);

/****************************************************************************
   tkcmdsetUSB_fw_version

   About:      Reads the firmware version information from the PM3

   Inputs:     UINT16_T    port        Communication port to use

   Outputs:    UINT8_T *   ver_ptr     FW version string stored at this location

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_fw_version(UINT16_T port, INT8_T * ver_ptr, UINT8_T ver_len);


/****************************************************************************
   tkcmdsetUSB_hw_version

   About:      Reads the hardware version information from the PM3

   Inputs:     UINT16_T     port        Communication port to use

   Outputs:    UINT8_T *   ver_ptr     HW version string stored at this location

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_hw_version(UINT16_T port, INT8_T * ver_ptr, UINT8_T ver_len);


/****************************************************************************
   tkcmdsetUSB_loader_fw_version

   About:      Reads the loader firmware version information from the device.
               SUPPORTED ONLY BY PM3 VERSION 95 OR GREATER APPLICATIONS.

   Inputs:     UINT16_T    port        Identifier for device

   Outputs:    UINT8_T *   ver_ptr     FW version string stored at this location

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_loader_fw_version(UINT16_T port, INT8_T *ver_ptr, UINT8_T ver_len);

/****************************************************************************
   tkcmdsetDDI_serial_number

   About:      Reads the serial number information from the PM3

   Inputs:     UINT16_T    port        Communication port to use

   Outputs:    UINT8_T *   ser_ptr     Serial number string stored at this location

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_serial_number(UINT16_T port, INT8_T * ser_ptr, UINT8_T ser_len);


/****************************************************************************
   tkcmdsetUSB_reset_port

   About:      Resets a single USB device by closing and
			   opening the specified port

   Inputs:     UINT16_T    port        Communication port to use

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_reset_port(UINT16_T port);

/****************************************************************************
   tkcmdsetUSB_shutdown

   About:      Shuts down a single USB device and releases 
               associated resources

   Inputs:     UINT16_T    port        Communication port to use

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_shutdown(UINT16_T port);

/****************************************************************************
   tkcmdsetUSB_shutdown_all

   About:      Shuts down a single USB device and releases 
               associated resources

   Inputs:     None	

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_shutdown_all();


/****************************************************************************
   tkcmdsetUSB_write_flashblk16

   About:      Writes a 16-bit block of data to a FLASH memory location
               This implementation supports only 1 block write at at time 
               (depends on TKUSB_MAX_PACKET_SIZE)

   Inputs:     UINT8_T     port        Identifier for device
               UINT32_T    address     Address of device memory to write
               UINT16_T *  val_ptr     Location of data block to write
               UINT32_T    len         Number of words to write
               UINT16_T *packet_rsp_len Expected response packet size
                        (used to allow lower levels to select the correct response report ID)

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_write_flashblk16(UINT16_T port, UINT32_T address, UINT16_T * val_ptr16, 
                                                  UINT32_T len, UINT16_T *packet_rsp_len);

/****************************************************************************
   tkcmdsetUSB_read_flashblk16

   About:      Reads a 16-bit block of data from a FLASH memory location
               This implementation supports only 1 block read at at time 
               (depends on TKUSB_MAX_PACKET_SIZE)

   Inputs:     UINT8_T     port        Identifier for device
               UINT32_T    address     Address of device memory to read
               UINT32_T    len         Number of words to read
               UINT16_T *packet_rsp_len Expected response packet size
                        (used to allow lower levels to select the correct response report ID)

   Outputs:    UINT16_T *  val_ptr     Location to store block of data read
               

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_read_flashblk16(UINT16_T port, UINT32_T address, UINT16_T *val_ptr16, 
                                                 UINT32_T len, UINT16_T *packet_rsp_len);

/****************************************************************************
   tkcmdsetUSB_write_memblk16

   About:      Writes a 16-bit block of data to a memory location
               This implementation supports only 1 block write at at time 
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
PM3USB_API ERRCODE_T tkcmdsetUSB_write_memblk16(UINT16_T port, UINT32_T address, UINT16_T * val_ptr16, 
                                                UINT32_T len, UINT16_T *packet_rsp_len);

/****************************************************************************
   tkcmdsetUSB_write_eeblk8

   About:      Writes an 8-bit block of data to a EEPROM memory location
               This implementation supports only 1 block write at at time 
               (depends on TKUSB_MAX_PACKET_SIZE)

   Inputs:     UINT8_T     port        Identifier for device
               UINT32_T    address     Address of device memory to write
               UINT8_T *  val_ptr     Location of data block to write
               UINT16_T    len         Number of bytes to write
               UINT16_T *packet_rsp_len Expected response packet size
                        (used to allow lower levels to select the correct response report ID)

   Outputs:    None

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_write_eeblk8(UINT16_T port, UINT32_T address, UINT8_T * val_ptr8, 
                                                  UINT16_T len, UINT16_T *packet_rsp_len);

/****************************************************************************
   tkcmdsetUSB_read_eeblk8

   About:      Reads a 16-bit block of data from a EEPROM memory location
               This implementation supports only 1 block read at at time 
               (depends on TKUSB_MAX_PACKET_SIZE)

   Inputs:     UINT8_T     port        Identifier for device
               UINT32_T    address     Address of device memory to read
               UINT16_T    len         Number of bytes to read
               UINT16_T *packet_rsp_len Expected response packet size
                        (used to allow lower levels to select the correct response report ID)

   Outputs:    UINT8_T *  val_ptr      Location to store block of data read
               

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_read_eeblk8(UINT16_T port, UINT32_T address, UINT8_T *val_ptr8, 
                                                 UINT16_T len, UINT16_T *packet_rsp_len);

/****************************************************************************
   tkcmdsetUSB_max_report_size

   About:      Gets the maximum allowable packet (report) size from the device

   Inputs:     UINT16_T    port        Identifier for device

   Outputs:    UINT16_T *  size_ptr    Location to store packet size

   Returns:    ERRCODE_T   ecode       Zero if successful
                                       Error code otherwise

****************************************************************************/
PM3USB_API ERRCODE_T tkcmdsetUSB_max_report_size(UINT16_T port, UINT16_T *size_ptr);

#endif /* _PM3USBCP_H */