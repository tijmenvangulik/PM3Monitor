//
//  C2DriverConcept2.cpp
//  C2Monitor
//
//  Created by tijmen on 30-07-16.
//
//

#include "C2DriverConcept2.hpp"
#include "PM3CsafeCP.h"
#include "PM3USBCP.h"
#include "PM3DDICP.h"
#include <iostream>
#include <sstream>

ERRCODE_T C2DriverConcept2::init(int timeout)
{
    ERRCODE_T result=tkcmdsetDDI_init();
    
    if (result==0) result=tkcmdsetCSAFE_init_protocol(1000);
    
    return result;
}

ERRCODE_T C2DriverConcept2::discover(UINT16_T &deviceCount)
{
    
    
    UINT16_T tempDeviceCount = 0;
    ERRCODE_T result = 0;
    //C2 to PM5 exist , just in case already support PM 6-8
    for(UINT16_T i =3;i<=8;i++) {
        ostringstream monitorStream;
        
        monitorStream << "Concept2 Performance Monitor "<<i<<" (PM"<<i<<")";
        tempDeviceCount=0;
        string monitorString = monitorStream.str();
        result=tkcmdsetDDI_discover_pm3s((char *)monitorString.c_str(), tempDeviceCount);
        deviceCount+=tempDeviceCount;
        
    }
    return result;
}

ERRCODE_T lastError(char *error, int bufSize)
{
    ERRCODE_T a_errorCode=0;
    tkcmdsetUSB_get_error_text(a_errorCode,error,bufSize);
    return a_errorCode;
}

void C2DriverConcept2::cSafeBufferInit()
{
    _cmd_data_size = 0;
    _rsp_data_size = CM_DATA_BUFFER_SIZE;
    
    //clear just in case this may be the problem
    for (int i=0;i<CM_DATA_BUFFER_SIZE;i++)
        _rsp_data[i] =0;
    
    _rsp_data_size=0;
    _rsp_data_index=0;
    
}

void C2DriverConcept2::cSafeBufferAdd(UINT8_T data)
{
   _cmd_data[_cmd_data_size++] = data;
}

int C2DriverConcept2::cSafeBufferCount()
{
    return _cmd_data_size;
}

ERRCODE_T C2DriverConcept2::cSafeBufferSend(int deviceNumber)
{
    return tkcmdsetCSAFE_command(deviceNumber, _cmd_data_size, _cmd_data, &_rsp_data_size, _rsp_data);
}

int C2DriverConcept2::cSafeReceiveBufferCount()
{
    return _rsp_data_size;
}

int C2DriverConcept2::cSafeReceiveBufferIndex()
{
    return _rsp_data_index;
}

UINT8_T C2DriverConcept2::cSafeReceiveBufferCurrent(int add_offset)
{
    if (_rsp_data_index+add_offset<_rsp_data_size)
      return _rsp_data[_rsp_data_index+add_offset];
    else return 0;
}

void C2DriverConcept2::cSafeReceiveBufferNext(int add)
{
    _rsp_data_index+=add;
}

ERRCODE_T C2DriverConcept2::close()
{   //todo define this
    return 0;
}
