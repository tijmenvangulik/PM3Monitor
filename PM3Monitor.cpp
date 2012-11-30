/*
 *  Pm3.cpp
 *  PMSDKDemo
 *
 *  Created by tijmen on 17-11-12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "PM3Monitor.h"
#include "PM3DDICP.h"
#include "PM3CSafeCP.h"
#include "PM3USBCP.h"
#include "csafe.h"
#include <iostream>
#include <sstream>

const char* strokePhaseToString(StrokePhase strokePhase)
{
	switch (strokePhase)
	{   
		case StrokePhase_Idle : return "Idle";
		case StrokePhase_Catch : return "Catch";
		case StrokePhase_Drive : return "Drive";
		case StrokePhase_Dwell: return  "Dwell";
		case StrokePhase_Recovery : return"Recovery";
	}
	return "";
}

PM3Exception::PM3Exception(int a_errorCode,const string a_errorText)
{ 

  ostringstream convert; 
  
  convert << "PM3 error "<< a_errorCode;

  if (a_errorCode<0)
  {	  char errorTextUSB[255];
	  errorTextUSB[0] = 0;
	  tkcmdsetUSB_get_error_text(a_errorCode,errorTextUSB,255);
	  convert << ", "<< errorTextUSB;
  } 		
  
  convert << ", " << a_errorText;
	
  errorText = convert.str();
  errorCode = a_errorCode;
}

PM3Exception::PM3Exception(const PM3Exception &e)
{ errorCode = e.errorCode;
  errorText = e.errorText;
}

const char* PM3Exception::what()
{ return errorText.c_str() ;
}
 
void PM3Exception::remove()
{ this->~PM3Exception();
}


PM3Exception::~PM3Exception() throw()
{ 
}

void handleError(int errorCode, const string errorText)
{ if (errorCode!=0)
  { throw PM3Exception(errorCode, errorText );
  }
}

PM3Monitor::PM3Monitor()
{ 
}

unsigned short int PM3Monitor::deviceNumber()
{ return _deviceNumber;
}

void PM3Monitor::initCommandBuffer()
{   _cmd_data_size = 0;
	_rsp_data_size = CM_DATA_BUFFER_SIZE;
	_rsp_data[0] = 0;
}

void PM3Monitor::addCSafeData(UInt32 data)
{ _cmd_data[_cmd_data_size++] = data;	
}

void PM3Monitor::executeCSafeCommand(const char* description )
{  handleError( 
			   tkcmdsetCSAFE_command(_deviceNumber, _cmd_data_size, _cmd_data, &_rsp_data_size, _rsp_data),
			   description);	
}

unsigned short int PM3Monitor::initialize()
{  
	if (!_initialized)
    { 
       
		handleError(
		  tkcmdsetDDI_init(),
		  "tkcmdsetDDI_init");
			// Init CSAFE protocol

		handleError(
		  tkcmdsetCSAFE_init_protocol(1000),
		  "tkcmdsetCSAFE_init_protocol");
		handleError( 
		  tkcmdsetDDI_discover_pm3s((char *)TKCMDSET_PM3_PRODUCT_NAME2, 0, &_deviceCount),
		  "tkcmdsetDDI_discover_pm3s");

		_initialized = true;

   }
  return _deviceCount;
}
void PM3Monitor::start(unsigned short int a_deviceNumber,void *eventData)
{  if (!_initialized)
	  throw PM3Exception(ERROR_INIT_NOT_CALLED,"initialize not called");
	
	_eventData = eventData;
	_deviceNumber = a_deviceNumber; 
	
	if (_deviceCount > 0)
	{
		_initialized = true;
	}
	else throw PM3Exception(ERROR_INIT_DEVICE_NOT_FOUND,"Init error, PM3 device not found.");
	reset();	
}

void PM3Monitor::reset()
{
	initCommandBuffer();

	// Reset
	
	addCSafeData(CSAFE_GOFINISHED_CMD);
	addCSafeData(CSAFE_GOIDLE_CMD);

	// Start.
 	addCSafeData(CSAFE_GOHAVEID_CMD);
	addCSafeData(CSAFE_GOINUSE_CMD);
	
	executeCSafeCommand("reset tkcmdsetCSAFE_command" );
}
		
unsigned short int PM3Monitor::deviceCount()
{ return _deviceCount;
}

void PM3Monitor::highResolutionUpdate()
{
    _previousStrokePhase = _currentStrokePhase;

    initCommandBuffer();


	// Get the stroke state.

	addCSafeData(CSAFE_SETUSERCFG1_CMD);
	addCSafeData(0x01);
	addCSafeData(CSAFE_PM_GET_STROKESTATE);

	executeCSafeCommand("highResolutionUpdate tkcmdsetCSAFE_command");

	uint currentbyte = 0;
	uint datalength = 0;

	if (_rsp_data[currentbyte] == CSAFE_SETUSERCFG1_CMD)
	{
		currentbyte += 2;
	}

	if (_rsp_data[currentbyte] == CSAFE_PM_GET_STROKESTATE)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++;

		switch (_rsp_data[currentbyte])
		{
			case 0:
			case 1:
				_currentStrokePhase = StrokePhase_Catch;
				break;
			case 2:
				_currentStrokePhase = StrokePhase_Drive;
				break;
			case 3:
				_currentStrokePhase = StrokePhase_Dwell;
				break;
			case 4:
				_currentStrokePhase = StrokePhase_Recovery;
				break;
		}
		currentbyte += datalength;
	}

	// Get any force curve points available.
	

	accumulateForceCurve();

	if (_currentStrokePhase != _previousStrokePhase)
	{
		// If this is the dwell, complete the power curve.
		if (_currentStrokePhase < _previousStrokePhase)
		{   
		    lowResolutionUpdate();						
		}

		// Update the stroke phase.
		newStrokePhase();
	}
}

void PM3Monitor::update()
{ highResolutionUpdate();
}


void PM3Monitor::accumulateForceCurve()
{
    initCommandBuffer();
	
	addCSafeData(CSAFE_SETUSERCFG1_CMD);
	addCSafeData(0x03);
	addCSafeData(CSAFE_PM_GET_FORCEPLOTDATA);
	addCSafeData(0x01);
	addCSafeData(0x20);

	// Handle power curve.

	uint nPointsReturned = 0xFF;

	while (0 < nPointsReturned)
	{  
		// Get any points available and consume them into the array.
		_rsp_data_size = CM_DATA_BUFFER_SIZE;
		
		//clear just in case this may be the problem
		for (int i=0;i<CM_DATA_BUFFER_SIZE;i++)
			_rsp_data[i] =0;	
		
		executeCSafeCommand("accumulateForceCurve tkcmdsetCSAFE_command");
		
		nPointsReturned = _rsp_data[4];
		
		for (unsigned short int i = 0; i < nPointsReturned; i += 2)
		{  _strokeData.forcePlotPoints[_strokeData.forcePlotCount] = _rsp_data[5 + i] + (_rsp_data[6 + i] << 8);
			incrementalPowerCurveUpdate();
			(_strokeData.forcePlotCount)++;
			if (_strokeData.forcePlotCount>MAX_PLOT_POINTS)
			 throw PM3Exception(ERROR_POINT_BUFFER_OVERFLOW,"Plot point buffer overflow");
			
		}
	

	}
}	
void PM3Monitor::incrementalPowerCurveUpdate()
{ if (onIncrementalPowerCurveUpdate!=NULL) 
	onIncrementalPowerCurveUpdate(_eventData, _strokeData.forcePlotPoints[_strokeData.forcePlotCount], _strokeData.forcePlotCount);
}

void PM3Monitor::newStrokePhase()
{ if (onNewStrokePhase!=NULL) 
    onNewStrokePhase(_eventData,_currentStrokePhase);
}

void PM3Monitor::strokeDataUpdate()
{ if (onStrokeDataUpdate!=NULL) 
    onStrokeDataUpdate(_eventData,_strokeData);
}


void PM3Monitor::lowResolutionUpdate()
{   
	int oldforcePlotCount = _strokeData.forcePlotCount-1;
	
	//search for the part where the curve ended (this is where power is low and power is going down  
	unsigned int prefStrokeData=10000;
	for( int i= _strokeData.forcePlotCount-1;i>=(oldforcePlotCount /2);i--) //search from end till the middle
	{	if ( (prefStrokeData<15) && //it is low , must be at end of the stroke
		     (_strokeData.forcePlotPoints[i] > prefStrokeData )//it is rising again
		   )
    	{	_strokeData.forcePlotCount=i+1; //use the previous one which was less
			break; //found the real end of the stroke
	    }
		prefStrokeData=_strokeData.forcePlotPoints[i];
	}
	
	initCommandBuffer();

	// Header and number of extension commands.

	addCSafeData(CSAFE_SETUSERCFG1_CMD);
	addCSafeData( 0x03);

	// Three PM3 extension commands.

	addCSafeData(CSAFE_PM_GET_DRAGFACTOR);
	addCSafeData(CSAFE_PM_GET_WORKDISTANCE);
	addCSafeData(CSAFE_PM_GET_WORKTIME);

	// Standard commands.

	addCSafeData(CSAFE_GETPACE_CMD);
	addCSafeData(CSAFE_GETPOWER_CMD);
	addCSafeData(CSAFE_GETCADENCE_CMD);

	executeCSafeCommand("lowResolutionUpdate tkcmdsetCSAFE_command" );
	
    uint currentbyte = 0;
	uint datalength = 0;

	if (_rsp_data[currentbyte] == CSAFE_SETUSERCFG1_CMD)
	{
		currentbyte += 2;
	}

	if (_rsp_data[currentbyte] == CSAFE_PM_GET_DRAGFACTOR)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++;
		
		_strokeData.dragFactor = _rsp_data[currentbyte];
		
		currentbyte += datalength;
	}

	if (_rsp_data[currentbyte] == CSAFE_PM_GET_WORKDISTANCE)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++;
		
		uint distanceTemp = (_rsp_data[currentbyte] + (_rsp_data[currentbyte + 1] << 8) + (_rsp_data[currentbyte + 2] << 16) + (_rsp_data[currentbyte + 3] << 24)) / 10;
		//uint fractionTemp = _rsp_data[currentbyte + 4];
		
		_strokeData.workDistance = distanceTemp;
		
		currentbyte += datalength;
	}

	if (_rsp_data[currentbyte] == CSAFE_PM_GET_WORKTIME)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++;
		
		if (datalength == 5)
		{
			uint timeInSeconds = (_rsp_data[currentbyte] + (_rsp_data[currentbyte + 1] << 8) + (_rsp_data[currentbyte + 2] << 16) + (_rsp_data[currentbyte + 3] << 24)) / 100;
			uint fraction = _rsp_data[currentbyte + 4];
			
			_strokeData.workTime = timeInSeconds + (fraction / 100.0);
			_strokeData.workTimehours = timeInSeconds / 3600;
			_strokeData.workTimeminutes = (timeInSeconds / 60) % 60;
			_strokeData.workTimeseconds = timeInSeconds % 60;
		}
		currentbyte += datalength;
	}

	if (_rsp_data[currentbyte] == CSAFE_GETPACE_CMD)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++;
		
		// Pace is in seconds/Km
		
		uint pace = _rsp_data[currentbyte] + (_rsp_data[currentbyte + 1] << 8);
		// get pace in seconds / 500m
		
		double fPace = pace / 2.0;
		// convert it to mins/500m
		
		_strokeData.splitMinutes = floor(fPace / 60);
		_strokeData.splitSeconds = fPace - (_strokeData.splitMinutes * 60);
				
		currentbyte += datalength;
	}

	if (_rsp_data[currentbyte] == CSAFE_GETPOWER_CMD)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++;
		
		_strokeData.power = _rsp_data[currentbyte] + (_rsp_data[currentbyte + 1] << 8);
		
		currentbyte += datalength;
	}

	if (_rsp_data[currentbyte] == CSAFE_GETCADENCE_CMD)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++;
		
		uint currentSPM = _rsp_data[currentbyte];
		
		if (0 < currentSPM)
		{
			_nSPM += currentSPM;
			_nSPMReads++;
			
			_strokeData.strokesPerMinute = currentSPM;
			_strokeData.strokesPerMinuteAverage = (_nSPM * 1.0) / (_nSPMReads * 1.0);
		}
		
		currentbyte += datalength;
	}
	strokeDataUpdate();
	
	//Copy the part which was left out as begining 
	int i2 = 0;
	for( int i= _strokeData.forcePlotCount;i<oldforcePlotCount;i++) 
	{	_strokeData.forcePlotPoints[i2] =_strokeData.forcePlotPoints[i];
		i2++;
	}
	_strokeData.forcePlotCount = i2;
	
	
}
