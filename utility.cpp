/*
 *  utility.cpp
 *  Concept2PerformanceMonitorConsole
 *
 *  Created by tijmen on 29-11-12.
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

#include "utility.h"
#include <iostream>
#include "math.h"
#include <unistd.h>


//none blocking key board hit utility function
//code copied from ##c@irc.freenode.net
int keyboardHit()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}
//convert duration in seconds to the time parts
void decodeDuration( double duration, int &miliseconds,int &seconds,int &minutes,int &hours,int &days ) {
    miliseconds = round(duration*1000);
    int durationInt = duration;
    seconds = durationInt % 60;
    durationInt = durationInt / 60;
    minutes = durationInt % 60;
    durationInt = durationInt / 60;
    hours = durationInt % 24;
    days = durationInt/24;
}
