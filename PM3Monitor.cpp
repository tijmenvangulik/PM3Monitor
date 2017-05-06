/*
 *  Pm3.cpp
 *  PMSDKDemo
 *
 *  Created by tijmen on 17-11-12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "PM3Monitor.h"
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
	
	convert << "PM error "<< a_errorCode;
	
	if (a_errorCode<0)
	{	  
		char errorTextUSB[255];
		errorTextUSB[0] = 0;
		tkcmdsetUSB_get_error_text(a_errorCode,errorTextUSB,255);
		convert << ", "<< errorTextUSB;
	} 		
	
	convert << ", " << a_errorText;
	
	errorText = convert.str();
	errorCode = a_errorCode;
}

PM3Exception::PM3Exception(const PM3Exception &e)
{ 
	errorCode = e.errorCode;
	errorText = e.errorText;
}

const char* PM3Exception::what()
{ 
	return errorText.c_str() ;
}

void PM3Exception::remove()
{ 
	this->~PM3Exception();
}


PM3Exception::~PM3Exception() throw()
{ 
}

void handleError(int errorCode, const string errorText)
{ 
	if (errorCode!=0 &&
        errorCode!=-10201 ) //ignore ergometer not found
	{ 
		throw PM3Exception(errorCode, errorText );
	}
}

unsigned short int PM3Monitor::deviceNumber()
{
	return _deviceNumber;
}

void PM3Monitor::initCommandBuffer()
{   
	_cmd_data_size = 0;
	_rsp_data_size = CM_DATA_BUFFER_SIZE;
	_rsp_data[0] = 0;
}

void PM3Monitor::addCSafeData(UInt32 data)
{ 
	_cmd_data[_cmd_data_size++] = data;	
}

void PM3Monitor::executeCSafeCommand(const char* description )
{  
	handleError( 
				tkcmdsetCSAFE_command(_deviceNumber, _cmd_data_size, _cmd_data, &_rsp_data_size, _rsp_data),
				description);	
}

unsigned short int PM3Monitor::initialize()
{  
		
		handleError(
					tkcmdsetDDI_init(),
					"tkcmdsetDDI_init");
		// Init CSAFE protocol
		
		handleError(
					tkcmdsetCSAFE_init_protocol(1000),
					"tkcmdsetCSAFE_init_protocol");

        updateDeviceCount();
		
        _initialized = true;
		
	return _deviceCount;
}
void PM3Monitor::updateDeviceCount()
{  //https://github.com/kdahlhaus/easy-erg/blob/c30ec1d308464c17f7dca7623605e776f3d170db/src/ErgNet.cpp
    UINT16_T deviceCount = 0;
    
    //PM3 to PM5 exist , just in case already support PM 6-8
    for(uint i =3;i<=8;i++) {
        ostringstream monitorStream;
        
        monitorStream << "Concept2 Performance Monitor "<<i<<" (PM"<<i<<")";
        deviceCount=0;
        string monitorString = monitorStream.str();
        handleError(
                    tkcmdsetDDI_discover_pm3s((char *)monitorString.c_str(), 0, &deviceCount),
                    "tkcmdsetDDI_discover_pm");
        _deviceCount+=deviceCount;
        
    }
}

void PM3Monitor::start(unsigned short int a_deviceNumber,PM3MonitorHandler& handler)
{  
	//reset the numbers
	_strokeData.forcePlotCount = 0;
	_strokeData.strokesPerMinuteAverage =0 ;

	_strokeData.workDistance = 0;
	_strokeData.workTime = 0;
	_strokeData.splitMinutes = 0; 
	_strokeData.splitSeconds = 0;
	_strokeData.power = 0;
	_strokeData.strokesPerMinute = 0;	
	_strokeData.dragFactor = 0;
	
	_nSPMReads = 0;
	_nSPM = 0 ;
	
	_trainingData.workoutType = 0; 
	_trainingData.duration = 0; 	
	_trainingData.distance = 0;
	_trainingData.workoutState = 0; 
	_trainingData.workoutIntervalCount = 0;
	_trainingData.intervalType = 0;
	_trainingData.endDuration = 0;
	_trainingData.endDistance = 0;

	updateDeviceCount();
	
	_handler = &handler;
	_deviceNumber = a_deviceNumber; 
	
	if (_deviceCount > 0)
	{
		_initialized = true;
	}
	else throw PM3Exception(ERROR_INIT_DEVICE_NOT_FOUND,"Init error, ergometer device not found.");
	
    trainingDataUpdate();
	
}

void PM3Monitor::setHandler(PM3MonitorHandler& handler)
{
	_handler = &handler;
}

void PM3Monitor::setDeviceNumber(unsigned short int value)
{
	_deviceNumber = value;
}

void PM3Monitor::reset()
{
    
	initCommandBuffer();
	
	// Reset
	//
    
	addCSafeData(CSAFE_GOFINISHED_CMD);
	addCSafeData(CSAFE_GOIDLE_CMD);
	
	
	executeCSafeCommand("go idle tkcmdsetCSAFE_command" );

	initCommandBuffer();
	// Start.
 	addCSafeData(CSAFE_GOHAVEID_CMD);
	addCSafeData(CSAFE_GOINUSE_CMD);
	
	executeCSafeCommand("go in use tkcmdsetCSAFE_command" );

	try 
	{
	}
		
	catch (...) 
	{
		throw;
		//some how this somtimes goes wrong, ignore because it still works fine
		//todo find out why and fix it
	}
	
	initCommandBuffer();
	addCSafeData(CSAFE_RESET_CMD);
	executeCSafeCommand("reset tkcmdsetCSAFE_command" );
	
	trainingDataUpdate();
	
	
}

unsigned short int PM3Monitor::deviceCount()
{ 
	return _deviceCount;
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
		int duration = calcStrokePhaseDuration();
		// If this is the dwell, complete the power curve.
		//if (_previousStrokePhase == StrokePhase_Drive)
		if (_currentStrokePhase == StrokePhase_Recovery)
		{   
		    lowResolutionUpdate();						
		}
		
		// Update the stroke phase.
		newStrokePhase(_currentStrokePhase,duration);
	}
}


int PM3Monitor::calcStrokePhaseDuration()
{
	//calc the duration for the drive and the recovery
	
#ifdef BOOST
    time_duration duration;// = time_duration(0,0,0,0)
#else
    float duration;
#endif

	if (_currentStrokePhase == StrokePhase_Recovery ||
		_currentStrokePhase == StrokePhase_Drive) 
	{
#ifdef BOOST
		ptime endPhase =  microsec_clock::universal_time();
		duration = endPhase - _startPhase;
#else
		time_t endPhase =  clock();
        duration = (((float)endPhase - (float)_startPhase) / 1000000.0F ) * 1000;
#endif

		_startPhase = endPhase;
	}
#ifdef BOOST
	return duration.total_milliseconds();
#else
	return duration;
#endif
}


void PM3Monitor::update()
{	
	highResolutionUpdate();
	
#ifdef BOOST
	ptime currenttime = microsec_clock::universal_time();
	time_duration diff;
	diff = currenttime- _lastTraingTime; //note _lastTraingTime is initialized in trainingDataUpdate, which is called in the begining
	int diffMs = diff.total_milliseconds();
#else
	time_t currenttime =  clock();
	int diffMs = (((float)currenttime - (float)_lastTraingTime) / 1000000.0F ) * 1000;
#endif
	
	//when work out is buzy update every second, before update every 200 ms
	if ( ( _trainingData.workoutState!=wsWorkoutRow && diffMs>200) ||
	     ( _trainingData.workoutState==wsWorkoutRow && diffMs>1000) )
		trainingDataUpdate();
	
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
		short unsigned int orgCount = _strokeData.forcePlotCount;
		for (unsigned short int i = 0; i < nPointsReturned; i += 2)
		{  
			_strokeData.forcePlotPoints[_strokeData.forcePlotCount] = _rsp_data[5 + i] + (_rsp_data[6 + i] << 8);
			(_strokeData.forcePlotCount)++;
			if (_strokeData.forcePlotCount>=MAX_PLOT_POINTS)
				throw PM3Exception(ERROR_POINT_BUFFER_OVERFLOW,"Plot point buffer overflow");
			
		}
		if (nPointsReturned>0)
			incrementalPowerCurveUpdate(_strokeData.forcePlotPoints,
										orgCount,
										_strokeData.forcePlotCount);
		
		
	}
}

void PM3Monitor::trainingDataChanged() {
	if (_handler!=NULL)  
		_handler->onTrainingDataChanged(*this, _trainingData);
	
}

void PM3Monitor::incrementalPowerCurveUpdate(unsigned short int forcePlotPoints[],unsigned short int a_beginIndex,unsigned short int a_endIndex)
{   
	if (_handler!=NULL)  
		_handler->onIncrementalPowerCurveUpdate(*this, forcePlotPoints,a_beginIndex,a_endIndex);
}

void PM3Monitor::newStrokePhase(StrokePhase strokePhase, int phaseDuration)
{	
	if (_handler!=NULL)
		_handler->onNewStrokePhase(*this,strokePhase,phaseDuration);
}

void PM3Monitor::strokeDataUpdate(StrokeData &strokeData)
{	
	if (_handler!=NULL)
		_handler->onStrokeDataUpdate(*this,strokeData);
}

void PM3Monitor::trainingDataUpdate()
{
#ifdef BOOST
	_lastTraingTime = microsec_clock::universal_time();
#else
    _lastTraingTime = clock();
#endif
    
	initCommandBuffer();
	
	addCSafeData(CSAFE_SETUSERCFG1_CMD);
	addCSafeData(0x07);
	
	addCSafeData(CSAFE_PM_GET_WORKOUTTYPE);
	addCSafeData(CSAFE_PM_GET_WORKOUTSTATE);
	addCSafeData(CSAFE_PM_GET_WORKOUTINTERVALCOUNT);
	addCSafeData(CSAFE_PM_GET_INTERVALTYPE);
	addCSafeData(CSAFE_PM_GET_RESTTIME);
	addCSafeData(CSAFE_PM_GET_WORKTIME);
	addCSafeData(CSAFE_PM_GET_WORKDISTANCE);
	
	addCSafeData(CSAFE_GETTWORK_CMD);
	addCSafeData(CSAFE_GETHORIZONTAL_CMD);
	
	executeCSafeCommand("TrainingDataUpdate tkcmdsetCSAFE_command" );
	
    uint currentbyte = 0;
	uint datalength = 0;
	uint actualDistance = 0;
	uint actualTime = 0;
	double duration = 0;
	double distance = 0;
	
	bool changed = false;
	
	if (_rsp_data[currentbyte] == CSAFE_SETUSERCFG1_CMD)
	{
		currentbyte += 2;
	}
			
	if (_rsp_data[currentbyte] == CSAFE_PM_GET_WORKOUTTYPE)  
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++;
		
		uint workoutType = _rsp_data[currentbyte];
		if (_trainingData.workoutType!=workoutType) 
		{
			_trainingData.workoutType=workoutType;
			changed = true;
		}
		currentbyte += datalength;
		
		
	}

	if (_rsp_data[currentbyte] == CSAFE_PM_GET_WORKOUTSTATE)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++; 
		
		uint workoutState = _rsp_data[currentbyte];
		if (_trainingData.workoutState!=workoutState) 
		{
			_trainingData.workoutState=workoutState;
			changed = true;
		}
		currentbyte += datalength;
		
	}
	if (_rsp_data[currentbyte] == CSAFE_PM_GET_WORKOUTINTERVALCOUNT)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++; 
		
		uint workoutIntervalCount = _rsp_data[currentbyte];
		if (_trainingData.workoutIntervalCount!=workoutIntervalCount) 
		{
			_trainingData.workoutIntervalCount=workoutIntervalCount;
			changed = true;
		}
		currentbyte += datalength;
		
	}
	if (_rsp_data[currentbyte] == CSAFE_PM_GET_INTERVALTYPE)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++; 
		
		uint intervalType = _rsp_data[currentbyte];
		if (_trainingData.intervalType!=intervalType) 
		{
			_trainingData.intervalType=intervalType;
			changed = true;
		}
		currentbyte += datalength;
		
	}
	if (_rsp_data[currentbyte] == CSAFE_PM_GET_RESTTIME)
	 {
	 currentbyte++;
	 datalength = _rsp_data[currentbyte];
	 currentbyte++; 
	 
	 uint restTime = _rsp_data[currentbyte] + (_rsp_data[currentbyte + 1] << 8);
		 if (_trainingData.restTime!=restTime) 
		 {
			 _trainingData.restTime=restTime;
			 changed = true;
		 }
	 currentbyte += datalength;
	 
	 }
	if (_rsp_data[currentbyte] == CSAFE_PM_GET_WORKTIME)  
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++; 
		
		uint timeInSeconds = (_rsp_data[currentbyte] + (_rsp_data[currentbyte + 1] << 8) + 
							  (_rsp_data[currentbyte + 2] << 16) + (_rsp_data[currentbyte + 3] << 24)) / 100;
		uint fraction = _rsp_data[currentbyte + 4];
		
		duration = timeInSeconds + (fraction / 100.0);
		
		currentbyte += datalength;
		
		
	}
	if (_rsp_data[currentbyte] == CSAFE_PM_GET_WORKDISTANCE)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++; 
		
		uint meters = (_rsp_data[currentbyte] + (_rsp_data[currentbyte + 1] << 8) +
					(_rsp_data[currentbyte + 2] << 16) + 
					(_rsp_data[currentbyte + 3] << 24)) / 10;
		uint fraction = _rsp_data[currentbyte + 4];
		distance =  meters + (fraction /10.0);
		
		currentbyte += datalength;
		
	}
	if (_rsp_data[currentbyte] == CSAFE_GETTWORK_CMD)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++; 
		
		actualTime = (_rsp_data[currentbyte]*60*60) + 
		(_rsp_data[currentbyte + 1] *60)+ 
		(_rsp_data[currentbyte + 2 ]);
		
		currentbyte += datalength;
		
	}
	if (_rsp_data[currentbyte] == CSAFE_GETHORIZONTAL_CMD)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++; 
		
		actualDistance = _rsp_data[currentbyte] + 
						(_rsp_data[currentbyte + 1] << 8);
		
		currentbyte += datalength;
		
	}
	//total time and distance can be changed because the rower is rowing.
	//the work time and work distance should be 0 for initial change
	if ( _currentStrokePhase==StrokePhase_Catch && actualTime==0 && actualDistance==0 ) { 
		
		uint durationRound = round(duration);
		
		if (_trainingData.duration!=durationRound) 
		{
			_trainingData.duration=durationRound;
			changed = true;
		}
		uint distanceRound = round(distance);
		if (_trainingData.distance!=distanceRound) 
		{
			_trainingData.distance=distanceRound;
			changed = true;
		}
	}
	//if (_trainingData.workoutState==wsWorkoutLogged) {
    //    _trainingData.endDuration=_trainingData.endDuration;
    //}
	if (_trainingData.workoutState==wsWorkoutLogged && 
		 ( _trainingData.endDuration==0 &&
	 	   _trainingData.endDistance==0) ) {
		//otherwise the work time does not reflect the last time and distance
		if ( _trainingData.workoutType>=FixedDistanceNoSplits &&
			 _trainingData.workoutType<=FixedTimeSplits ) {
			
			if (duration!=0) { //doing an fixed distance
				_strokeData.workTime = duration;
				_strokeData.workDistance = 0;
				_strokeData.time=duration;
				_strokeData.distance = _trainingData.distance;
                _trainingData.endDistance=_trainingData.distance;
                _trainingData.endDuration=duration;
			}
			if (distance!=0) { //doing a fixed time
				_strokeData.workDistance = distance;
				_strokeData.workTime = 0;
				_strokeData.time= _trainingData.duration;
				_strokeData.distance = distance;
                _trainingData.endDuration=_trainingData.duration;
			}
			strokeDataUpdate(_strokeData);//send the updated last end time/ duration to the server
		}
		changed= true;
	}
    if (_trainingData.workoutState!=wsWorkoutLogged &&
        ( _trainingData.endDistance!=0 ||  _trainingData.endDuration!=0)) {
        _trainingData.endDistance=0;
        _trainingData.endDuration=0;
        changed=true;
    }
	if (changed) trainingDataChanged();
	
}
void PM3Monitor::lowResolutionUpdate()
{   
	int oldforcePlotCount = _strokeData.forcePlotCount-1;
	
	//search for the part where the curve ended (this is where power is low and power is going down  
	unsigned int prefStrokeData=10000;
	for( int i= _strokeData.forcePlotCount-1;i>=(oldforcePlotCount /2);i--) //search from end till the middle
	{	
		if ( (prefStrokeData<15) && //it is low , must be at end of the stroke
			(_strokeData.forcePlotPoints[i] > prefStrokeData )//it is rising again
			)
		{	
			_strokeData.forcePlotCount=i+2; //use the previous one which was less
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
	addCSafeData(CSAFE_GETTWORK_CMD);
	addCSafeData(CSAFE_GETHORIZONTAL_CMD);
	addCSafeData(CSAFE_GETCALORIES_CMD);
	
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
		
		uint distanceTemp = (_rsp_data[currentbyte] + (_rsp_data[currentbyte + 1] << 8) +
							 (_rsp_data[currentbyte + 2] << 16) + 
							 (_rsp_data[currentbyte + 3] << 24)) / 10;
		uint fractionTemp = _rsp_data[currentbyte + 4];
		
		_strokeData.workDistance = distanceTemp + (fractionTemp /10.0);
		
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
        if (pace>0) {
            //get cal/hr: Calories/Hr = (((2.8 / ( pace * pace * pace )) * ( 4.0 * 0.8604)) + 300.0)
            double paced = pace/1000.0; // formular needs pace in sec/m (not sec/km)
            _strokeData.caloriesPerHour = round(( (2.8 / (paced*paced*paced) ) * ( 4.0 * 0.8604) ) + 300.0) ;
            
        }
        else _strokeData.caloriesPerHour =0;
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
		
		if ( currentSPM > 0)
		{
			_nSPM += currentSPM;
			_nSPMReads++;
			
			_strokeData.strokesPerMinute = currentSPM;
			_strokeData.strokesPerMinuteAverage = (double)_nSPM / (double)_nSPMReads;
		}
		
		currentbyte += datalength;
	}
	
	if (_rsp_data[currentbyte] == CSAFE_GETTWORK_CMD)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++; 
		
		_strokeData.time = (_rsp_data[currentbyte]*60*60) + 
		(_rsp_data[currentbyte + 1] *60)+ 
		(_rsp_data[currentbyte + 2 ]);
		
		currentbyte += datalength;
		
	}
	if (_rsp_data[currentbyte] == CSAFE_GETHORIZONTAL_CMD)
	{
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++; 
		
		_strokeData.distance = _rsp_data[currentbyte] + 
								(_rsp_data[currentbyte + 1] << 8);
		
		currentbyte += datalength;
		
	}
    if (_rsp_data[currentbyte] == CSAFE_GETCALORIES_CMD)
	{  // Calories in 1 cal resolution, total/accumulated calories burned.
		currentbyte++;
		datalength = _rsp_data[currentbyte];
		currentbyte++;
		// Byte 0: Total Calories (LSB)  |  Byte 1: Total Calories (MSB)
		uint cal = _rsp_data[currentbyte] + (_rsp_data[currentbyte + 1] << 8);
		_strokeData.totCalories = cal;
		currentbyte += datalength;
	}

	strokeDataUpdate(_strokeData);
	
	//Copy the part which was left out as begining 
	int i2 = 0;
	if (_strokeData.forcePlotCount>=1)
		_strokeData.forcePlotCount-=1;
	for( int i= _strokeData.forcePlotCount;i<oldforcePlotCount;i++) 
	{	
		_strokeData.forcePlotPoints[i2] =_strokeData.forcePlotPoints[i];
		i2++;
	}
	_strokeData.forcePlotCount = i2;

	
}

PM3MonitorHandler* PM3Monitor::handler()
{
	return _handler;
}

void PM3Monitor::setDeviceCount(UINT16_T value)
{
	_deviceCount = value;
}
