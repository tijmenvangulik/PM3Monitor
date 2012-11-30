/*******************************************************************************
 *******************************************************************************

    csafe.h		    CSAFE Protocol C Header File

    About:          This module contains the CSAFE protocol definitions
					for the C2PM3 project

    Target CPU(s):  M68SZ328


    written for:    Concept II
					105 Industrial Park Drive
					Morrisville, VT 05661
					802-888-5226 (Voice)
					802-888-6331 (Fax)

    Contacts:       Scott Hamilton 
					802-888-5226 x3062 (Voice)

    written by:     Microprocessor Designs, Inc.
					65 Longmeadow Drive
					P.O. Box 160
					Shelburne, VT 05482
					802-985-2535 (Voice)
					802-985-9106 (FAX)


	$Revision: 3 $

        Copyright (c) 2002 - 2004, Concept II, Inc, Morrisville, VT
------------------------------------------------------------------------------
	$Log: /Concept II/C2PM3/Pc_src/DLLs XPlatform/Common/csafe.h $
 * 
 * 3     9/16/08 10:51a Adomb
 * Updated csafe.h which includes definitions for GET/SET HRM commands and
 * GET_UI_EVENTS command.
 * 
 * 33    7/17/08 3:59p Spilc
 * Added commands to get/set HRM, configure sensor channel,  and get User
 * Interface events from custom screen
 * 
 * 32    1/22/08 10:10a Jpiet
 * Changes to support PMx code merge
 * 
 * 31    4/24/07 10:00a Mlyon
 * Increased CSAFE Frame size maximum from 96 to 120 (to fit in the
 * largest USB report size supported)
 * 
 * 30    4/10/07 7:36a Mlyon
 * Clean-up comments
 * 
 * 29    7/25/06 11:36a Spilc
 * Added CSAFE_PM_GET_HEARTBEATDATA
 * 
 * 28    6/13/06 10:27a Mlyon
 * Added command used for DB Mfg to set RF channel status
 * 
 * 26    3/03/06 8:04a Mlyon
 * Added several new command defintions to support new features
 * 
 * 25    11/19/05 10:18a Mlyon
 * Intermediate checkin for local racing functionality
 * 
 * 24    8/05/05 3:23p Mlyon
 * Intermediate checkin prior to Rev 088 release
 * 
 * 23    6/03/05 5:16p Mlyon
 * Intermediate checkin for 88Beta build 11
 * 
 * 22    4/21/05 1:59p Mlyon
 * Numerous updates itemized in Rev 95x Beta Prerelease Changes.txt
 * 
 * 21    4/01/05 3:57p Mlyon
 * Refer to Prerelease Changes.txt for details of changes
 * 
 * 20    11/24/04 12:53p Mlyon
 * Minor updates as noted in prerelease changes text file
 * 
 * 19    11/10/04 2:18p Mlyon
 * Check-in w/ all changes up through Rev 968/Rev 060
 * 
 * 18    6/11/04 10:26a Mlyon
 * More Public CSAFE updates
 * 
 * 17    6/04/04 1:24p Mlyon
 * Intermediate w/ more public csafe additions
 * 
 * 16    6/02/04 3:57p Mlyon
 * Intermediate checkin w/ Lint changes, bug fixes and additions for
 * public CSAFE SDK
 * 
 * 15    4/09/04 6:28p Mlyon
 * Prerelease version 069
 * 
 * 14    3/19/04 5:14p Mlyon
 * Intermediate checkin of race functionality into base build (after some
 * testing of USB racing)
 * 
 * 13    1/05/04 7:23a Mlyon
 * Added venue race commands as place keepers; added forceplot buffer
 * command
 * 
 * 12    9/22/03 4:32p Mlyon
 * Improved comments about commands
 * 
 * 11    7/20/03 4:35p Mlyon
 * Fixed bug w/ too many pace parameter loading for interval workouts and
 * "short work period" bug for distance intervals
 * 
 * 10    7/10/03 7:35p Mlyon
 * Added CSAFE command for display update time and fixed several USB
 * toolkit bugs related to Interrupt OUT endpoint operation
 * 
 * 9     7/09/03 8:57a Mlyon
 * Updates to increase Csafe memory read blocksize maximum to 64 bytes;
 * also improved Csafe frame status setting under certain error
 * conditions.
 * 
 * 8     6/29/03 12:57p Mlyon
 * Numerous logging related bug fixes along w/ adding an error one-shot
 * timer and a factory default reset special function
 * 
 * 7     6/24/03 9:31a Mlyon
 * Numerous bug fixes and restructuring of tach isr to handle dual edge
 * detection on Model D
 * 
 * 6     6/16/03 2:01p Mlyon
 * Numerous bug fixes and added CSAFE functionality for configure fixed
 * piece workouts and preparing to row a workout
 * 
 * 5     6/05/03 8:26a Mlyon
 * Updates for CSAFE command processing, and rowing algorithm bug fixes,
 * and support for 8 MBit Flash
 * 
 * 4     6/03/03 6:24p Mlyon
 * Intermediate release w/ improvement to paceboat operatation, workout
 * termination logic, CSAFE command processing, added parameters for
 * remote screen control, upgrade to tarmap and io_mem primitive for 8Mbit
 * Flash support, etc.
 * 
 * 3     12/05/02 7:41a Mlyon
 * Intermediate check-in (during customworkout debug)
 * 
 * 2     9/23/02 10:27a Mlyon
 * Intermediate check-in after basic USB/SCI CSAFE communications
 * integration and basic datalogging functionality
 * 
 * 1     7/09/02 10:25a Mlyon
 * Initial check-in.
 
********************************************************************************
*******************************************************************************/
//#include "ap_globals.h"

#ifndef __CSAFE_H__
#define __CSAFE_H__

/*******************************************************************************
 *******************************************************************************
								  Definitions
********************************************************************************
*******************************************************************************/
/* Frame contents */
#define CSAFE_EXT_FRAME_START_BYTE          0xF0
#define CSAFE_FRAME_START_BYTE              0xF1
#define CSAFE_FRAME_END_BYTE                0xF2
#define CSAFE_FRAME_STUFF_BYTE              0xF3

#define CSAFE_FRAME_MAX_STUFF_OFFSET_BYTE   0x03

#define CSAFE_FRAME_FLG_LEN                 2
#define CSAFE_EXT_FRAME_ADDR_LEN            2
#define CSAFE_FRAME_CHKSUM_LEN              1

#define CSAFE_SHORT_CMD_TYPE_MSK            0x80
#define CSAFE_LONG_CMD_HDR_LENGTH           2
#define CSAFE_LONG_CMD_BYTE_CNT_OFFSET      1
#define CSAFE_RSP_HDR_LENGTH                2

#define CSAFE_FRAME_STD_TYPE                0
#define CSAFE_FRAME_EXT_TYPE                1

#define CSAFE_DESTINATION_ADDR_HOST         0x00
#define CSAFE_DESTINATION_ADDR_ERG_MASTER   0x01
#define CSAFE_DESTINATION_ADDR_BROADCAST    0xFF
#define CSAFE_DESTINATION_ADDR_ERG_DEFAULT  0xFD

#define CSAFE_FRAME_MAXSIZE                 96
#define CSAFE_INTERFRAMEGAP_MIN				50		// msec
#define CSAFE_CMDUPLIST_MAXSIZE             10
#define CSAFE_MEMORY_BLOCKSIZE              64
#define CSAFE_FORCEPLOT_BLOCKSIZE           32
#define CSAFE_HEARTBEAT_BLOCKSIZE			32

/* Manufacturer Info */
#define CSAFE_MANUFACTURE_ID                22      // assigned by Fitlinxx for Concept2
#define CSAFE_CLASS_ID                      2       // standard CSAFE equipment
#if (SW_PM4_BUILD_TYPE == SW_PM4_NONE)
#define CSAFE_MODEL_NUM						3		// PM3
#else
#define CSAFE_MODEL_NUM						4		// PM4
#endif

#define CSAFE_UNITS_TYPE					0		// Metric
#define CSAFE_SERIALNUM_DIGITS				9

#define CSAFE_HMS_FORMAT_CNT				3
#define CSAFE_YMD_FORMAT_CNT				3
#define CSAFE_ERRORCODE_FORMAT_CNT			3

/* Command space partitioning for standard commands */
#define CSAFE_CTRL_CMD_LONG_MIN          0x01
#define CSAFE_CFG_CMD_LONG_MIN           0x10
#define CSAFE_DATA_CMD_LONG_MIN          0x20
#define CSAFE_AUDIO_CMD_LONG_MIN         0x40
#define CSAFE_TEXTCFG_CMD_LONG_MIN       0x60
#define CSAFE_TEXTSTATUS_CMD_LONG_MIN    0x65
#define CSAFE_CAP_CMD_LONG_MIN           0x70
#define CSAFE_PMPROPRIETARY_CMD_LONG_MIN 0x76


#define CSAFE_CTRL_CMD_SHORT_MIN         0x80
#define CSAFE_STATUS_CMD_SHORT_MIN       0x91
#define CSAFE_DATA_CMD_SHORT_MIN         0xA0
#define CSAFE_AUDIO_CMD_SHORT_MIN        0xC0
#define CSAFE_TEXTCFG_CMD_SHORT_MIN      0xE0
#define CSAFE_TEXTSTATUS_CMD_SHORT_MIN   0xE5

/* Standard Short Control Commands */
enum CSAFE_SHORT_CTRL_CMDS {

    CSAFE_GETSTATUS_CMD = CSAFE_CTRL_CMD_SHORT_MIN,         // 0x80 
    CSAFE_RESET_CMD,                                        // 0x81
    CSAFE_GOIDLE_CMD,                                       // 0x82
    CSAFE_GOHAVEID_CMD,                                     // 0x83
    CSAFE_GOINUSE_CMD = 0x85,                               // 0x85
    CSAFE_GOFINISHED_CMD,                                   // 0x86
    CSAFE_GOREADY_CMD,                                      // 0x87
    CSAFE_BADID_CMD,                                        // 0x88
    CSAFE_CTRL_CMD_SHORT_MAX
};

/* Standard Short Status Commands */
enum CSAFE_SHORT_STATUS_CMDS {

    CSAFE_GETVERSION_CMD = CSAFE_STATUS_CMD_SHORT_MIN,      // 0x91
    CSAFE_GETID_CMD,                                        // 0x92
    CSAFE_GETUNITS_CMD,                                     // 0x93
    CSAFE_GETSERIAL_CMD,                                    // 0x94   
    CSAFE_GETLIST_CMD = 0x98,                               // 0x98
    CSAFE_GETUTILIZATION_CMD,                               // 0x99
    CSAFE_GETMOTORCURRENT_CMD,                              // 0x9A
    CSAFE_GETODOMETER_CMD,                                  // 0x9B
    CSAFE_GETERRORCODE_CMD,                                 // 0x9C
    CSAFE_GETSERVICECODE_CMD,                               // 0x9D
    CSAFE_GETUSERCFG1_CMD,                                  // 0x9E
    CSAFE_GETUSERCFG2_CMD,                                  // 0x9F
    CSAFE_STATUS_CMD_SHORT_MAX
};

/* Standard Short Data Commands */
enum CSAFE_SHORT_DATA_CMDS {

    CSAFE_GETTWORK_CMD = CSAFE_DATA_CMD_SHORT_MIN,          // 0xA0
    CSAFE_GETHORIZONTAL_CMD,                                // 0xA1
    CSAFE_GETVERTICAL_CMD,                                  // 0xA2
    CSAFE_GETCALORIES_CMD,                                  // 0xA3
    CSAFE_GETPROGRAM_CMD,                                   // 0xA4
    CSAFE_GETSPEED_CMD,                                     // 0xA5
    CSAFE_GETPACE_CMD,                                      // 0xA6
    CSAFE_GETCADENCE_CMD,                                   // 0xA7
    CSAFE_GETGRADE_CMD,                                     // 0xA8
    CSAFE_GETGEAR_CMD,                                      // 0xA9
    CSAFE_GETUPLIST_CMD,                                   	// 0xAA
    CSAFE_GETUSERINFO_CMD,                                  // 0xAB
    CSAFE_GETTORQUE_CMD,                                    // 0xAC
    CSAFE_GETHRCUR_CMD = 0xB0,                              // 0xB0
    CSAFE_GETHRTZONE_CMD = 0xB2,                            // 0xB2
    CSAFE_GETMETS_CMD,                                      // 0xB3
    CSAFE_GETPOWER_CMD,                                     // 0xB4
    CSAFE_GETHRAVG_CMD,                                     // 0xB5
    CSAFE_GETHRMAX_CMD,                                     // 0xB6
    CSAFE_GETUSERDATA1_CMD= 0xBE,                           // 0xBE
    CSAFE_GETUSERDATA2_CMD,                                 // 0xBF
    CSAFE_DATA_CMD_SHORT_MAX
};

/* Standard Short Audio Commands */
enum CSAFE_SHORT_AUDIO_CMDS {

    CSAFE_GETAUDIOCHANNEL_CMD = CSAFE_AUDIO_CMD_SHORT_MIN,  // 0xC0
    CSAFE_GETAUDIOVOLUME_CMD,                               // 0xC1
    CSAFE_GETAUDIOMUTE_CMD,                                 // 0xC2 
    CSAFE_AUDIO_CMD_SHORT_MAX
};

/* Standard Short Text Configuration Commands */
enum CSAFE_SHORT_TEXTCFG_CMDS {

    CSAFE_ENDTEXT_CMD = CSAFE_TEXTCFG_CMD_SHORT_MIN,        // 0xE0
    CSAFE_DISPLAYPOPUP_CMD,                                 // 0xE1
    CSAFE_TEXTCFG_CMD_SHORT_MAX
};

/* Standard Short Text Status Commands */
enum CSAFE_SHORT_TEXTSTATUS_CMDS {

    CSAFE_GETPOPUPSTATUS_CMD = CSAFE_TEXTSTATUS_CMD_SHORT_MIN, // 0xE5
    CSAFE_TEXTSTATUS_CMD_SHORT_MAX
};

/* Standard Long commands */

/* Standard Long Control Commands */
enum CSAFE_LONG_CTRL_CMDS {

    CSAFE_AUTOUPLOAD_CMD = CSAFE_CTRL_CMD_LONG_MIN,         // 0x01
    CSAFE_UPLIST_CMD,                                       // 0x02
    CSAFE_UPSTATUSSEC_CMD = 0x04,                           // 0x04
    CSAFE_UPLISTSEC_CMD,                                    // 0x05
    CSAFE_CTRL_CMD_LONG_MAX
};

/* Standard Long Configuration Commands */
enum CSAFE_LONG_CFG_CMDS {

    CSAFE_IDDIGITS_CMD = CSAFE_CFG_CMD_LONG_MIN,            // 0x10
    CSAFE_SETTIME_CMD,                                      // 0x11
    CSAFE_SETDATE_CMD,                                      // 0x12
    CSAFE_SETTIMEOUT_CMD,                                   // 0x13
    CSAFE_SETUSERCFG1_CMD = 0x1A,                           // 0x1A
    CSAFE_SETUSERCFG2_CMD,                                  // 0x1B
    CSAFE_CFG_CMD_LONG_MAX
};

/* Standard Long Data Commands */
enum CSAFE_LONG_DATA_CMDS {

    CSAFE_SETTWORK_CMD = CSAFE_DATA_CMD_LONG_MIN,           // 0x20
    CSAFE_SETHORIZONTAL_CMD,                                // 0x21
    CSAFE_SETVERTICAL_CMD,                                  // 0x22
    CSAFE_SETCALORIES_CMD,                                  // 0x23
    CSAFE_SETPROGRAM_CMD,                                   // 0x24
    CSAFE_SETSPEED_CMD,                                     // 0x25
    CSAFE_SETGRADE_CMD = 0x28,                              // 0x28
    CSAFE_SETGEAR_CMD,                                      // 0x29
    CSAFE_SETUSERINFO_CMD = 0x2B,                           // 0x2B
    CSAFE_SETTORQUE_CMD,                                    // 0x2C
    CSAFE_SETLEVEL_CMD,                                     // 0x2D
    CSAFE_SETTARGETHR_CMD = 0x30,                           // 0x30
    CSAFE_SETGOAL_CMD = 0x32,                               // 0x32
    CSAFE_SETMETS_CMD,                               		// 0x33
    CSAFE_SETPOWER_CMD,                                     // 0x34
    CSAFE_SETHRZONE_CMD,                                    // 0x35
    CSAFE_SETHRMAX_CMD,                                     // 0x36
    CSAFE_DATA_CMD_LONG_MAX
};

/* Standard Long Audio Commands */
enum CSAFE_LONG_AUDIO_CMDS {

    CSAFE_SETCHANNELRANGE_CMD = CSAFE_AUDIO_CMD_LONG_MIN,   // 0x40
    CSAFE_SETVOLUMERANGE_CMD,                               // 0x41
    CSAFE_SETAUDIOMUTE_CMD,                                 // 0x42
    CSAFE_SETAUDIOCHANNEL_CMD,                              // 0x43
    CSAFE_SETAUDIOVOLUME_CMD,                               // 0x44
    CSAFE_AUDIO_CMD_LONG_MAX
};

/* Standard Long Text Configuration Commands */
enum CSAFE_LONG_TEXTCFG_CMDS {

    CSAFE_STARTTEXT_CMD = CSAFE_TEXTCFG_CMD_LONG_MIN,       // 0x60
    CSAFE_APPENDTEXT_CMD,                                   // 0x61
    CSAFE_TEXTCFG_CMD_LONG_MAX
};

/* Standard Long Text Status Commands */
enum CSAFE_LONG_TEXTSTATUS_CMDS {

    CSAFE_GETTEXTSTATUS_CMD = CSAFE_TEXTSTATUS_CMD_LONG_MIN, // 0x65
    CSAFE_TEXTSTATUS_CMD_LONG_MAX
};

/* Standard Long Capabilities Commands */
enum CSAFE_LONG_CAP_CMDS {

    CSAFE_GETCAPS_CMD = CSAFE_CAP_CMD_LONG_MIN,            // 0x70 
    CSAFE_GETUSERCAPS1_CMD = 0x7E,                         // 0x7E
    CSAFE_GETUSERCAPS2_CMD = 0x7F,                         // 0x7F
    CSAFE_CAP_CMD_LONG_MAX
};

/*
    The currently defined CSAFE command space is augmented by adding 4 command
    wrappers to allow pushing and pulling of configuration/data from the
    host to the PM
    
    CSAFE_SETPMCFG_CMD    Push configuration from host to PM
    CSAFE_SETPMDATA_CMD   Push data from host to PM
    CSAFE_GETPMCFG_CMD    Pull configuration to host from PM
    CSAFE_GETPMDATA_CMD   PUll data to host from PM
    
    Note: These commands have been added for Concept 2 and do not comply
          with the existing CSAFE command set
*/
enum CSAFE_LONG_PMPROPRIETARY_CMDS {

    CSAFE_SETPMCFG_CMD = CSAFE_PMPROPRIETARY_CMD_LONG_MIN, // 0x76
    CSAFE_SETPMDATA_CMD,                                // 0x77
    CSAFE_GETPMCFG_CMD = 0x7E,                          // 0x7E
    CSAFE_GETPMDATA_CMD,                                // 0x7F
    CSAFE_PMPROPRIETARY_CMD_LONG_MAX
};

/* Command space partitioning for PM proprietary commands */
#define CSAFE_GETPMCFG_CMD_SHORT_MIN        0x80
#define CSAFE_GETPMCFG_CMD_LONG_MIN         0x50
#define CSAFE_SETPMCFG_CMD_SHORT_MIN        0xE0
#define CSAFE_SETPMCFG_CMD_LONG_MIN         0x00
#define CSAFE_GETPMDATA_CMD_SHORT_MIN       0xA0
#define CSAFE_GETPMDATA_CMD_LONG_MIN        0x68
#define CSAFE_SETPMDATA_CMD_SHORT_MIN       0xD0
#define CSAFE_SETPMDATA_CMD_LONG_MIN        0x30

/* 
    Custom Short PULL Configuration Commands for PM
*/
enum CSAFE_PM_SHORT_PULL_CFG_CMDS {

    CSAFE_PM_GET_FW_VERSION = CSAFE_GETPMCFG_CMD_SHORT_MIN,         // 0x80 
    CSAFE_PM_GET_HW_VERSION,                                        // 0x81  
    CSAFE_PM_GET_HW_ADDRESS,                                        // 0x82
    CSAFE_PM_GET_TICK_TIMEBASE,                                     // 0x83
    CSAFE_PM_GET_HRM,                                               // 0x84
    // Unused,                                                      // 0x85
    CSAFE_PM_GET_SCREENSTATESTATUS = 0x86,                          // 0x86
    CSAFE_PM_GET_RACE_LANE_REQUEST,                                 // 0x87
    CSAFE_PM_GET_ERG_LOGICALADDR_REQUEST,                           // 0x88
    CSAFE_PM_GET_WORKOUTTYPE,                                       // 0x89
    CSAFE_PM_GET_DISPLAYTYPE,                                       // 0x8A
    CSAFE_PM_GET_DISPLAYUNITS,                                      // 0x8B
    CSAFE_PM_GET_LANGUAGETYPE,                                      // 0x8C
    CSAFE_PM_GET_WORKOUTSTATE,                                      // 0x8D
    CSAFE_PM_GET_INTERVALTYPE,                                      // 0x8E
    CSAFE_PM_GET_OPERATIONALSTATE,                                  // 0x8F
    CSAFE_PM_GET_LOGCARDSTATE,                                      // 0x90
    CSAFE_PM_GET_LOGCARDSTATUS,                                     // 0x91
    CSAFE_PM_GET_POWERUPSTATE,                                      // 0x92
    CSAFE_PM_GET_ROWINGSTATE,                                       // 0x93
    CSAFE_PM_GET_SCREENCONTENT_VERSION,                             // 0x94
    CSAFE_PM_GET_COMMUNICATIONSTATE,                                // 0x95
    CSAFE_PM_GET_RACEPARTICIPANTCOUNT,                              // 0x96
    CSAFE_PM_GET_BATTERYLEVELPERCENT,                               // 0x97
    CSAFE_PM_GET_RACEMODESTATUS,                                    // 0x98
    CSAFE_PM_GET_INTERNALLOGPARAMS,                                 // 0x99
    CSAFE_PM_GET_PRODUCTCONFIGURATION,                              // 0x9A  
    CSAFE_PM_GET_ERGSLAVEDISCOVERREQUESTSTATUS,                     // 0x9B
    CSAFE_PM_GET_WIFICONFIG,                                        // 0x9C
    CSAFE_PM_GET_CPUTICKRATE,                                       // 0x9D
	CSAFE_PM_GET_LOGCARDCENSUS,										// 0x9E
    CSAFE_PM_GET_WORKOUTINTERVALCOUNT,                              // 0x9F
    CSAFE_GETPMCFG_CMD_SHORT_MAX
    
};

/* 
    Custom Short PULL Data Commands for PM
*/
enum CSAFE_PM_SHORT_PULL_DATA_CMDS {

    CSAFE_PM_GET_WORKTIME = CSAFE_GETPMDATA_CMD_SHORT_MIN,          // 0xA0         
    CSAFE_PM_GET_PROJECTED_WORKTIME,                                // 0xA1
    CSAFE_PM_GET_TOTAL_RESTTIME,                                    // 0xA2
    CSAFE_PM_GET_WORKDISTANCE,                                      // 0xA3
    CSAFE_PM_GET_TOTAL_WORKDISTANCE,                                // 0xA4
    CSAFE_PM_GET_PROJECTED_WORKDISTANCE,                            // 0xA5
    CSAFE_PM_GET_RESTDISTANCE,                                      // 0xA6
    CSAFE_PM_GET_TOTAL_RESTDISTANCE,                                // 0xA7
    CSAFE_PM_GET_STROKE_500MPACE,                                   // 0xA8
    CSAFE_PM_GET_STROKE_POWER,                                      // 0xA9
    CSAFE_PM_GET_STROKE_CALORICBURNRATE,                            // 0xAA
    CSAFE_PM_GET_SPLIT_AVG_500MPACE,                                // 0xAB
    CSAFE_PM_GET_SPLIT_AVG_POWER,                                   // 0xAC
    CSAFE_PM_GET_SPLIT_AVG_CALORICBURNRATE,                         // 0xAD
    CSAFE_PM_GET_SPLIT_AVG_CALORIES,                                // 0xAE
    CSAFE_PM_GET_TOTAL_AVG_500MPACE,                                // 0xAF
    CSAFE_PM_GET_TOTAL_AVG_POWER,                                   // 0xB0
    CSAFE_PM_GET_TOTAL_AVG_CALORICBURNRATE,                         // 0xB1
    CSAFE_PM_GET_TOTAL_AVG_CALORIES,                                // 0xB2
    CSAFE_PM_GET_STROKERATE,                                        // 0xB3
    CSAFE_PM_GET_SPLIT_AVG_STROKERATE,                              // 0xB4
    CSAFE_PM_GET_TOTAL_AVG_STROKERATE,                              // 0xB5
    CSAFE_PM_GET_AVG_HEARTRATE,                                     // 0xB6
    CSAFE_PM_GET_ENDING_AVG_HEARTRATE,                              // 0xB7
    CSAFE_PM_GET_REST_AVG_HEARTRATE,                                // 0xB8
    CSAFE_PM_GET_SPLITTIME,                                         // 0xB9
    CSAFE_PM_GET_LASTSPLITTIME,                                     // 0xBA
    CSAFE_PM_GET_SPLITDISTANCE,                                     // 0xBB
    CSAFE_PM_GET_LASTSPLITDISTANCE,                                 // 0xBC
    CSAFE_PM_GET_LASTRESTDISTANCE,                                  // 0xBD
    CSAFE_PM_GET_TARGETPACETIME,                                    // 0xBE
    CSAFE_PM_GET_STROKESTATE,                                       // 0xBF
    CSAFE_PM_GET_STROKERATESTATE,                                   // 0xC0
    CSAFE_PM_GET_DRAGFACTOR,                                        // 0xC1
    CSAFE_PM_GET_ENCODERPERIOD,                                     // 0xC2
    CSAFE_PM_GET_HEARTRATESTATE,                                    // 0xC3
    CSAFE_PM_GET_SYNCDATA,                                          // 0xC4
    CSAFE_PM_GET_SYNCDATAALL,                                       // 0xC5
    CSAFE_PM_GET_RACEDATA,                                          // 0xC6
    CSAFE_PM_GET_TICKTIME,                                          // 0xC7
    CSAFE_PM_GET_ERRORTYPE,                                         // 0xC8
    CSAFE_PM_GET_ERRORVALUE,                                        // 0xC9
    CSAFE_PM_GET_STATUSTYPE,                                        // 0xCA
    CSAFE_PM_GET_STATUSVALUE,                                       // 0xCB
    CSAFE_PM_GET_EPMSTATUS,                                         // 0xCC
    CSAFE_PM_GET_DISPLAYUPDATETIME,                                 // 0xCD
    CSAFE_PM_GET_SYNCFRACTIONALTIME,                                // 0xCE
    CSAFE_PM_GET_RESTTIME,                                          // 0xCF
    CSAFE_GETPMDATA_CMD_SHORT_MAX
};

/* 
    Custom Short PUSH Data Commands for PM
*/
enum CSAFE_PM_SHORT_PUSH_DATA_CMDS {

    CSAFE_PM_SET_SYNC_DISTANCE = CSAFE_SETPMDATA_CMD_SHORT_MIN,     // 0xD0
    CSAFE_PM_SET_SYNC_STROKEPACE,                                   // 0xD1
    CSAFE_PM_SET_SYNC_AVG_HEARTRATE,                                // 0xD2
    CSAFE_PM_SET_SYNC_TIME,                                         // 0xD3
    CSAFE_PM_SET_SYNC_SPLIT_DATA,                                   // 0xD4
    CSAFE_PM_SET_SYNC_ENCODER_PERIOD,                               // 0xD5
    CSAFE_PM_SET_SYNC_VERSION_INFO,                                 // 0xD6
    CSAFE_PM_SET_SYNC_RACETICKTIME,                                 // 0xD7
    CSAFE_PM_SET_SYNC_DATAALL,                                      // 0xD8
    // Unused,                                                      // 0xD9
    // Unused,                                                      // 0xDA
    // Unused,                                                      // 0xDB
    // Unused,                                                      // 0xDC
    // Unused,                                                      // 0xDD
    // Unused,                                                      // 0xDE
    // Unused,                                                      // 0xDF
    CSAFE_SETPMDATA_CMD_SHORT_MAX
};

/* 
    Custom Short PUSH Configuration Commands for PM
*/
enum CSAFE_PM_SHORT_PUSH_CFG_CMDS {

    CSAFE_PM_SET_RESET_ALL = CSAFE_SETPMCFG_CMD_SHORT_MIN,          // 0xE0  
    CSAFE_PM_SET_RESET_ERGNUMBER,                                   // 0xE1
    // Unused,                                                      // 0xE2
    // Unused,                                                      // 0xE3
    // Unused,                                                      // 0xE4
    // Unused,                                                      // 0xE5
    // Unused,                                                      // 0xE6
    // Unused,                                                      // 0xE7
    // Unused,                                                      // 0xE8
    // Unused,                                                      // 0xE9
    // Unused,                                                      // 0xEA
    // Unused,                                                      // 0xEB
    // Unused,                                                      // 0xEC
    // Unused,                                                      // 0xED
    // Unused,                                                      // 0xEE
    // Unused,                                                      // 0xEF
    CSAFE_SETPMCFG_CMD_SHORT_MAX
};

/* 
    Custom Long PUSH Configuration Commands for PM
*/
enum CSAFE_PM_LONG_PUSH_CFG_CMDS {

    CSAFE_PM_SET_BAUDRATE = CSAFE_SETPMCFG_CMD_LONG_MIN,            // 0x00
    CSAFE_PM_SET_WORKOUTTYPE,                                       // 0x01
    CSAFE_PM_SET_STARTTYPE,                                         // 0x02
    CSAFE_PM_SET_WORKOUTDURATION,                                   // 0x03
    CSAFE_PM_SET_RESTDURATION,                                      // 0x04
    CSAFE_PM_SET_SPLITDURATION,                                     // 0x05
    CSAFE_PM_SET_TARGETPACETIME,                                    // 0x06
    CSAFE_PM_SET_INTERVALIDENTIFIER,                                // 0x07
    CSAFE_PM_SET_OPERATIONALSTATE,                                  // 0x08
    CSAFE_PM_SET_RACETYPE,                                          // 0x09
    CSAFE_PM_SET_WARMUPDURATION,                                    // 0x0A
    CSAFE_PM_SET_RACELANESETUP,                                     // 0x0B
    CSAFE_PM_SET_RACELANEVERIFY,                                    // 0x0C
    CSAFE_PM_SET_RACESTARTPARAMS,                                   // 0x0D
    CSAFE_PM_SET_ERGSLAVEDISCOVERYREQUEST,                          // 0x0E
    CSAFE_PM_SET_BOATNUMBER,                                        // 0x0F
    CSAFE_PM_SET_ERGNUMBER,                                         // 0x10
    CSAFE_PM_SET_COMMUNICATIONSTATE,                                // 0x11
    CSAFE_PM_SET_CMDUPLIST,                                         // 0x12
    CSAFE_PM_SET_SCREENSTATE,                                       // 0x13
    CSAFE_PM_CONFIGURE_WORKOUT,                                     // 0x14
    CSAFE_PM_SET_TARGETAVGWATTS,                                    // 0x15
    CSAFE_PM_SET_TARGETCALSPERHR,                                   // 0x16
    CSAFE_PM_SET_INTERVALTYPE,                                      // 0x17
    CSAFE_PM_SET_WORKOUTINTERVALCOUNT,                              // 0x18
    CSAFE_PM_SET_DISPLAYUPDATERATE,                                 // 0x19
    CSAFE_PM_SET_AUTHENPASSWORD,                                    // 0x1A
    CSAFE_PM_SET_TICKTIME,                                          // 0x1B
    CSAFE_PM_SET_TICKTIMEOFFSET,                                    // 0x1C
    CSAFE_PM_SET_RACEDATASAMPLETICKS,                               // 0x1D
    CSAFE_PM_SET_RACEOPERATIONTYPE,                                 // 0x1E
    CSAFE_PM_SET_RACESTATUSDISPLAYTICKS,                            // 0x1F
    CSAFE_PM_SET_RACESTATUSWARNINGTICKS,                            // 0x20
    CSAFE_PM_SET_RACEIDLEMODEPARAMS,                                // 0x21
    CSAFE_PM_SET_DATETIME,                                          // 0x22
    CSAFE_PM_SET_LANGUAGETYPE,                                      // 0x23
    CSAFE_PM_SET_WIFICONFIG,                                        // 0x24
    CSAFE_PM_SET_CPUTICKRATE,                                       // 0x25
    CSAFE_PM_SET_LOGCARDUSER,                                       // 0x26
    CSAFE_PM_SET_SCREENERRORMODE,                                   // 0x27
    CSAFE_PM_SET_CABLETEST,                                         // 0x28
    CSAFE_PM_SET_USER_ID,                                           // 0x29
    CSAFE_PM_SET_USER_PROFILE,                                      // 0x2A
    CSAFE_PM_SET_HRM,                                               // 0x2B
    // Unused,                                                      // 0x2C
    // Unused,                                                      // 0x2D
    // Unused,                                                      // 0x2E
#if (TKRF_SENSOR_CHANNEL == TKRF_SENSOR_CHANNEL_ENABLE)
	CSAFE_PM_SET_SENSOR_CHANNEL = 0x2F,								// 0x2F sensor channel
#else
    // Unused,                                                      // 0x2F
#endif    
    CSAFE_SETPMCFG_CMD_LONG_MAX
};


/* 
    Custom Long PUSH Data Commands for PM
*/
enum CSAFE_PM_LONG_PUSH_DATA_CMDS {

    CSAFE_PM_SET_TEAM_DISTANCE = CSAFE_SETPMDATA_CMD_LONG_MIN,      // 0x30
    CSAFE_PM_SET_TEAM_FINISH_TIME,                                  // 0x31
    CSAFE_PM_SET_RACEPARTICIPANT,                                   // 0x32
    CSAFE_PM_SET_RACESTATUS,                                        // 0x33
    CSAFE_PM_SET_LOGCARDMEMORY,                                     // 0x34
    CSAFE_PM_SET_DISPLAYSTRING,                                     // 0x35
    CSAFE_PM_SET_DISPLAYBITMAP,                                     // 0x36
#if (LOCALRACING_MODE == LOCALRACING_MODE_ENABLE)
    CSAFE_PM_SET_LOCALRACEPARTICIPANT,                              // 0x37
#endif
    // Unused,                                                      // 0x38
    // Unused,                                                      // 0x39
    // Unused,                                                      // 0x3A
    // Unused,                                                      // 0x3B
    // Unused,                                                      // 0x3C
    // Unused,                                                      // 0x3D
    // Unused,                                                      // 0x3E
    // Unused,                                                      // 0x3F
    // Unused,                                                      // 0x40
    // Unused,                                                      // 0x41
    // Unused,                                                      // 0x42
    // Unused,                                                      // 0x43
    // Unused,                                                      // 0x44
    // Unused,                                                      // 0x45
    // Unused,                                                      // 0x46
    // Unused,                                                      // 0x47
    // Unused,                                                      // 0x48
    // Unused,                                                      // 0x49
    // Unused,                                                      // 0x4A
    // Unused,                                                      // 0x4B
    // Unused,                                                      // 0x4C
    // Unused,                                                      // 0x4D
#if (SW_PM4_BUILD_TYPE == SW_PM4_BASE)
#if (SW_PM4_BUILD_SUB_TYPE == SW_PM4_SUB_DBMFG)
    CSAFE_PM_SET_ANTRFMODE = 0x4E,                                  // 0x4E mfg support only
#endif // SW_PM4_BUILD_SUB_TYPE
#endif // SW_PM4_BUILD_TYPE
    CSAFE_PM_SET_MEMORY = 0x4F,                                     // 0x4F debug only
    CSAFE_SETPMDATA_CMD_LONG_MAX  

};

/* 
    Custom Long PULL Configuration Commands for PM
*/
enum CSAFE_PM_LONG_PULL_CFG_CMDS {

    CSAFE_PM_GET_ERGNUMBER = CSAFE_GETPMCFG_CMD_LONG_MIN,           // 0x50
    CSAFE_PM_GET_ERGNUMBERREQUEST,                                  // 0x51
    CSAFE_PM_GET_USERIDSTRING,  								    // 0x52
#if (LOCALRACING_MODE == LOCALRACING_MODE_ENABLE)
    CSAFE_PM_GET_LOCALRACEPARTICIPANT,                              // 0x53
#endif
    CSAFE_PM_GET_USER_ID = 0x54,                                    // 0x54
    CSAFE_PM_GET_USER_PROFILE,                                      // 0x55
//    CSAFE_PM_GET_WORKOUTPARAMETERS,                               // 0x56
    // Unused,                                                      // 0x57
    // Unused,                                                      // 0x58
    // Unused,                                                      // 0x59
    // Unused,                                                      // 0x5A
    // Unused,                                                      // 0x5B
    // Unused,                                                      // 0x5C
    // Unused,                                                      // 0x5D
    // Unused,                                                      // 0x5E
    // Unused,                                                      // 0x5F
    // Unused,                                                      // 0x60
    // Unused,                                                      // 0x61
    // Unused,                                                      // 0x62
    // Unused,                                                      // 0x63
    // Unused,                                                      // 0x64
    // Unused,                                                      // 0x65
    // Unused,                                                      // 0x66
    // Unused,                                                      // 0x67
    CSAFE_GETPMCFG_CMD_LONG_MAX 
};

/* 
    Custom Long PULL Data Commands for PM
*/
enum CSAFE_PM_LONG_PULL_DATA_CMDS {

    CSAFE_PM_GET_MEMORY = CSAFE_GETPMDATA_CMD_LONG_MIN,             // 0x68  
    CSAFE_PM_GET_LOGCARDMEMORY,                                     // 0x69
    CSAFE_PM_GET_INTERNALLOGMEMORY,                                 // 0x6A
    CSAFE_PM_GET_FORCEPLOTDATA,                                     // 0x6B
    CSAFE_PM_GET_HEARTBEATDATA,                                     // 0x6C
    CSAFE_PM_GET_UI_EVENTS,                                         // 0x6D
    // Unused,                                                      // 0x6E
    // Unused,                                                      // 0x6F
    // Unused,                                                      // 0x70
    // Unused,                                                      // 0x71
    // Unused,                                                      // 0x72
    // Unused,                                                      // 0x73
    // Unused,                                                      // 0x74
    // Unused,                                                      // 0x75
    // Command Wrapper,                                             // 0x76
    // Command Wrapper,                                             // 0x77
    // Unused,                                                      // 0x78
    // Unused,                                                      // 0x79
    // Unused,                                                      // 0x7A
    // Unused,                                                      // 0x7B
    // Unused,                                                      // 0x7C
    // Unused,                                                      // 0x7D
    // Command Wrapper,                                             // 0x7E
    // Command Wrapper,                                             // 0x7F
    CSAFE_GETPMDATA_CMD_LONG_MAX     
};


/* Status byte flag and mask definitions */
#define CSAFE_PREVOK_FLG                    0x00
#define CSAFE_PREVREJECT_FLG                0x10
#define CSAFE_PREVBAD_FLG                   0x20
#define CSAFE_PREVNOTRDY_FLG                0x30
#define CSAFE_PREVFRAMESTATUS_MSK           0x30

#define CSAFE_SLAVESTATE_ERR_FLG            0x00
#define CSAFE_SLAVESTATE_RDY_FLG            0x01
#define CSAFE_SLAVESTATE_IDLE_FLG           0x02
#define CSAFE_SLAVESTATE_HAVEID_FLG         0x03
#define CSAFE_SLAVESTATE_INUSE_FLG          0x05
#define CSAFE_SLAVESTATE_PAUSE_FLG          0x06
#define CSAFE_SLAVESTATE_FINISH_FLG         0x07
#define CSAFE_SLAVESTATE_MANUAL_FLG         0x08
#define CSAFE_SLAVESTATE_OFFLINE_FLG        0x09

#define CSAFE_FRAMECNT_FLG                  0x80

#define CSAFE_SLAVESTATE_MSK                0x0F

/* CSAFE_AUTOUPLOAD_CMD flag definitions */
#define CSAFE_AUTOSTATUS_FLG                0x01
#define CSAFE_UPSTATUS_FLG                  0x02
#define CSAFE_UPLIST_FLG                    0x04
#define CSAFE_ACK_FLG                       0x10
#define CSAFE_EXTERNCONTROL_FLG             0x40

/* CSAFE Slave Capabilities Codes */
#define CSAFE_CAPCODE_PROTOCOL				0x00
#define CSAFE_CAPCODE_POWER					0x01
#define CSAFE_CAPCODE_TEXT					0x02

/* CSAFE units format definitions: CSAFE_<type>_<unit>_<tens>_<decimals> */
#define CSAFE_DISTANCE_MILE_0_0             0x01
#define CSAFE_DISTANCE_MILE_0_1             0x02
#define CSAFE_DISTANCE_MILE_0_2             0x03
#define CSAFE_DISTANCE_MILE_0_3             0x04
#define CSAFE_DISTANCE_FEET_0_0             0x05
#define CSAFE_DISTANCE_INCH_0_0             0x06
#define CSAFE_WEIGHT_LBS_0_0                0x07
#define CSAFE_WEIGHT_LBS_0_1                0x08
#define CSAFE_DISTANCE_FEET_1_0           	0x0A
#define CSAFE_SPEED_MILEPERHOUR_0_0          0x10
#define CSAFE_SPEED_MILEPERHOUR_0_1          0x11
#define CSAFE_SPEED_MILEPERHOUR_0_2          0x12
#define CSAFE_SPEED_FEETPERMINUTE_0_0        0x13
#define CSAFE_DISTANCE_KM_0_0               0x21
#define CSAFE_DISTANCE_KM_0_1               0x22
#define CSAFE_DISTANCE_KM_0_2               0x23
#define CSAFE_DISTANCE_METER_0_0            0x24
#define CSAFE_DISTANCE_METER_0_1            0x25
#define CSAFE_DISTANCE_CM_0_0               0x26
#define CSAFE_WEIGHT_KG_0_0                 0x27
#define CSAFE_WEIGHT_KG_0_1                 0x28
#define CSAFE_SPEED_KMPERHOUR_0_0            0x30
#define CSAFE_SPEED_KMPERHOUR_0_1            0x31
#define CSAFE_SPEED_KMPERHOUR_0_2            0x32
#define CSAFE_SPEED_METERPERMINUTE_0_0       0x33
#define CSAFE_PACE_MINUTEPERMILE_0_0        0x37
#define CSAFE_PACE_MINUTEPERKM_0_0          0x38
#define CSAFE_PACE_SECONDSPERKM_0_0         0x39
#define CSAFE_PACE_SECONDSPERMILE_0_0       0x3A
#define CSAFE_DISTANCE_FLOORS_0_0           0x41
#define CSAFE_DISTANCE_FLOORS_0_1           0x42
#define CSAFE_DISTANCE_STEPS_0_0            0x43
#define CSAFE_DISTANCE_REVS_0_0             0x44
#define CSAFE_DISTANCE_STRIDES_0_0          0x45
#define CSAFE_DISTANCE_STROKES_0_0          0x46
#define CSAFE_MISC_BEATS_0_0                0x47
#define CSAFE_ENERGY_CALORIES_0_0           0x48
#define CSAFE_GRADE_PERCENT_0_0             0x4A
#define CSAFE_GRADE_PERCENT_0_2             0x4B
#define CSAFE_GRADE_PERCENT_0_1             0x4C
#define CSAFE_CADENCE_FLOORSPERMINUTE_0_1   0x4F
#define CSAFE_CADENCE_FLOORSPERMINUTE_0_0   0x50
#define CSAFE_CADENCE_STEPSPERMINUTE_0_0    0x51
#define CSAFE_CADENCE_REVSPERMINUTE_0_0     0x52
#define CSAFE_CADENCE_STRIDESPERMINUTE_0_0  0x53
#define CSAFE_CADENCE_STROKESPERMINUTE_0_0  0x54
#define CSAFE_MISC_BEATSPERMINUTE_0_0       0x55
#define CSAFE_BURN_CALORIESPERMINUTE_0_0    0x56
#define CSAFE_BURN_CALORIESPERHOUR_0_0      0x57
#define CSAFE_POWER_WATTS_0_0               0x58
#define CSAFE_ENERGY_INCHLB_0_0             0x5A
#define CSAFE_ENERGY_FOOTLB_0_0             0x5B
#define CSAFE_ENERGY_NM_0_0                 0x5C

/* Conversion constants */
#define CSAFE_KG_TO_LBS						(2.2046)
#define CSAFE_LBS_TO_KG						(1./CSAFE_KG_TO_LBS)

/* ID Digits */
#define CSAFE_IDDIGITS_MIN					2
#define CSAFE_IDDIGITS_MAX					5
#define CSAFE_DEFAULT_IDDIGITS				5
#define CSAFE_DEFAULT_ID					0
#define CSAFE_MANUAL_ID					    999999999

/* Slave State Tiimeout Parameters */
#define CSAFE_DEFAULT_SLAVESTATE_TIMEOUT	20		// seconds
#define CSAFE_PAUSED_SLAVESTATE_TIMEOUT		220		// seconds
#define CSAFE_INUSE_SLAVESTATE_TIMEOUT		6		// seconds
#define CSAFE_IDLE_SLAVESTATE_TIMEOUT	    30		// seconds

/* Base Year */
#define CSAFE_BASE_YEAR						1900

/* Default time intervals */
#define CSAFE_DEFAULT_STATUSUPDATE_INTERVAL	256		// seconds
#define CSAFE_DEFAULT_CMDUPLIST_INTERVAL	256		// seconds

#if (SW_PMX_BUILD_TYPE == SW_PMX_BASE)
#pragma pack(push, 2)
#endif //SW_PMX_BUILD_TYPE

typedef struct {
    UINT8_T first_racer_id;
    UINT8_T first_racer_position;
    INT32_T first_racer_delta;
    UINT8_T second_racer_id;
    UINT8_T second_racer_position;
    INT32_T second_racer_delta;
    UINT8_T third_racer_id;
    UINT8_T third_racer_position;
    INT32_T third_racer_delta;
    UINT8_T fourth_racer_id;
    UINT8_T fourth_racer_position;
    INT32_T fourth_racer_delta;
    UINT32_T team_distance;
    UINT8_T mode;
} CSAFE_LOCALRACE_RACESTATUS_T;

/* Race Data Structure */
typedef struct {
    UINT32_T timeticks;
    UINT32_T distance;
    UINT16_T pace;
    UINT32_T elapsedtime;
    UINT8_T stroke_rate;
    UINT8_T racestate;
    UINT8_T batterylevel;
    UINT8_T strokestate;    
    UINT8_T rowing;    
    UINT8_T epmstatus;
    UINT8_T raceoperationtype;
    UINT8_T racestartstate;
} CSAFE_RACER_DATA_T;
//
/*typedef struct {

    CSAFE_RACER_DATA_T data[APGLOBALS_RACERDATA_NUM];
} CSAFE_RACE_DATA_T; */
/* Race Status Structures */
typedef struct {

    UINT8_T racerid;
    UINT8_T racerposition;
    INT32_T racerdelta;
} CSAFE_RACER_STATUS_T;

/*typedef struct {
    CSAFE_RACER_STATUS_T status[APGLOBALS_RACERSTATUS_NUM];
    UINT32_T teamdistance;
    UINT8_T mode;
} CSAFE_RACE_STATUS_T;*/

#if (SW_PMX_BUILD_TYPE == SW_PMX_BASE)
#pragma pack(pop)
#endif //SW_PMX_BUILD_TYPE
/*******************************************************************************
 *******************************************************************************
									  Macros
********************************************************************************
*******************************************************************************/   


#ifdef __cplusplus
extern "C" {
#endif

void myfunc(void);


#ifdef __cplusplus
}
#endif

#endif /* __CSAFE_H__ */


