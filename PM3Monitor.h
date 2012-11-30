/*
 *  Pm3.h
 *  PMSDKDemo
 *
 *  Created by tijmen on 17-11-12.
 *
 *  xcode c++ project ot make concept2 PM3 monitoring easy
 *  thanks to Andrew Lewis for the great .net example. a large part of
 *  his example could be used for this project.
 */

/*
 Copyright 2012 Tijmen van Gulik
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */ 

#include <exception>
#include <Carbon/Carbon.h>
#include <string>

using namespace std;



enum StrokePhase
    {
        StrokePhase_Idle = 0,
        StrokePhase_Catch = 1,
        StrokePhase_Drive = 2,
        StrokePhase_Dwell = 3,
        StrokePhase_Recovery = 4
    };
    
//convert the phase to a display text
const char* strokePhaseToString(StrokePhase strokePhase);

class PM3Exception:public exception
{ public:
    PM3Exception(int a_errorCode,const string a_errorText);
    PM3Exception(const PM3Exception &e);
    ~PM3Exception() throw();
    const char* what() ;
    void remove();
    string errorText;
	//negative for PM3 concept2 errors positive numbers for PM3Monitor errors
    int errorCode;
};

const int CM_DATA_BUFFER_SIZE = 64;
const int MAX_PLOT_POINTS = 1024; //todo find the real limit maybe a bit largen then needed

const int ERROR_INIT_DEVICE_NOT_FOUND = 1;
const int ERROR_POINT_BUFFER_OVERFLOW = 2;
const int ERROR_INIT_NOT_CALLED =3;

struct StrokeData 
{
	unsigned short int forcePlotPoints[MAX_PLOT_POINTS];
	unsigned short int forcePlotCount;
	uint dragFactor;
	uint workDistance;
	uint workTimehours;
	uint workTimeminutes;
	uint workTimeseconds; 
	double workTime;
	double splitMinutes; 
	double splitSeconds;
	uint power;
	double strokesPerMinuteAverage;
	uint strokesPerMinute;		 
};

class PM3Monitor
{ protected:
	
    bool _initialized;
    unsigned short int _deviceCount;
	unsigned short int _deviceNumber;
	StrokeData _strokeData;
	
	StrokePhase _previousStrokePhase;
	StrokePhase _currentStrokePhase;
	
	UInt32 _cmd_data[CM_DATA_BUFFER_SIZE];
	unsigned short int _cmd_data_size;
	UInt32 _rsp_data[CM_DATA_BUFFER_SIZE];
	unsigned short int _rsp_data_size;
	
	void *_eventData;
	
	uint _nSPMReads;
	uint _nSPM;
	
    void initCommandBuffer();
	void addCSafeData(UInt32 data);
	void executeCSafeCommand(const char* description);
	
    virtual void highResolutionUpdate();
	void accumulateForceCurve();
	void lowResolutionUpdate();
	
	//events
	virtual void newStrokePhase();
	virtual void incrementalPowerCurveUpdate();
	virtual void strokeDataUpdate();
	
  public:
	//this mst be called after the create
	unsigned short int initialize();
	
	//call start to connect to the device numer and set the event data which is returned for each event
	void start(unsigned short int a_deviceNumber,void *eventData);
	
	//resets the PM3, already called in the initialize at the first time
	void reset();
	
	//updates the performance data, call this about ever 80 mili seconds ( or faster)
	void update();
	
	//return nr of devices attached to the USB
    unsigned short int deviceCount();
	
	//return current device number a_deviceNumber 
	unsigned short int deviceNumber();

	PM3Monitor();
	
  //events
  public:
    // attach to this event for live drawing of the curve. (onStrokeDataUpdate gives only the curve at the end of the stroke 
	void (*onIncrementalPowerCurveUpdate)(void *eventData,unsigned short int a_value,unsigned short int a_index);
	//called on catch,drive,dwell and recovery ( the catch is not  allways send )
	void (*onNewStrokePhase)(void *eventData,StrokePhase strokePhase);
	//strokeData contains info about the stroke. Send at the same time as the dwell
	void (*onStrokeDataUpdate)(void *eventData,StrokeData &strokeData);

	
};

