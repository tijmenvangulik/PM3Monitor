/*
 *  main.cpp
 *  concept 2 PM3 monitor demo
 *
 *  Created by tijmen on 17-11-12.
 *
 *  xcode c++ project ot make concept2 PM3 monitoring easy
 *  thanks to Andrew Lewis for the great .net example. a large part of
 *  his example could be used for this project.
 *  
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

#include <iostream>
#include "PM3Monitor.h"
#include "utility.h"

using namespace std;

class HandlePM3Data : public PM3MonitorHandler {
public:
	// attach to this event for live drawing of the curve. (onStrokeDataUpdate gives only the curve at the end of the stroke 
	void onIncrementalPowerCurveUpdate(PM3Monitor &monitor,unsigned short int a_value,unsigned short int a_index)
	{ 
		//no code yet
	}
	//called on catch,drive,dwell and recovery ( the catch is not  allways send )
	void onNewStrokePhase(PM3Monitor &monitor,StrokePhase strokePhase)
	{   
		std::cout << " new stroke phase: " <<  strokePhaseToString(strokePhase) << "\n";  	
	}
	
	//strokeData contains info about the stroke. Send at the same time as the dwell
	void onStrokeDataUpdate(PM3Monitor &monitor,StrokeData &strokeData)
	{   
		cout << "----------------------------------------------------\n";
		cout << "curve :"<< strokeData.forcePlotCount<<" \n";
		for(int i=0;i<strokeData.forcePlotCount;i++)
		{   cout << strokeData.forcePlotPoints[i];
			for(int i2=0;i2<strokeData.forcePlotPoints[i];i2+=5) 
			{ cout << "*";
			}
			cout << "\n";
		}
		cout << "Drag factor: " << strokeData.dragFactor << "\n";
		cout << "Work distance: " <<  strokeData.workDistance << "\n";
        //convert a float work time to a time
        int miliseconds;int seconds;int minutes;int hours;int days;
        decodeDuration( strokeData.workTime, miliseconds, seconds, minutes, hours, days );
        
		cout << "Work time "
             <<  hours << ":"
             << minutes<< ":"
             << seconds << "\n";
		cout << "Work time: " <<  strokeData.workTime << "\n";
		cout << "Split: " <<  strokeData.splitMinutes << ":" 
             << strokeData.splitSeconds << "\n";
		cout << "Power: " <<  strokeData.power << "\n";
		cout << "Strokes Per Minute Average: " <<  strokeData.strokesPerMinuteAverage << "\n";
		cout << "Strokes Per Minute: " <<  strokeData.strokesPerMinute << "\n";
		cout << "\n";
		
	}	
};



int main (int argc, char * const argv[]) {
	
	std::cout << "Start PM3 Monitor ( press enter to stop)\n";
	
	PM3Monitor monitor = PM3Monitor();	
	
	//object for the events which prints the perforance data
	HandlePM3Data handler; 
	
	try
	{   
		//initializes the monitor, returns the nr of devices, but ignore
        monitor.initialize();  
 		
		//you write here code to ask the user to  which device he wants to connect
		
	    monitor.start(0,//device 0 assume there is only one, then it is allways one
					  handler ); // handler which handles the events 
		
		//keep on monitoring till you hit a key
		while( !keyboardHit() ) //not pressed key board
		{ 
			//update the performance info
			monitor.update();
			
			//give other processes some time and do use 100% cpu
			usleep(80);
		}
		std::cout << "Stop PM3 Monitor\n";
	}
	catch ( PM3Exception& e)
    {  
		cerr << e.errorText;
		return e.errorCode;
	} 	
	return 0;
}
