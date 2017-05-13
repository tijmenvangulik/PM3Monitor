//
//  C2DriverConcept2.hpp
//  C2Monitor
//
//  Created by tijmen on 30-07-16.
//
//

#ifndef C2DriverConcept2_hpp
#define C2DriverConcept2_hpp

#include "C2Driver.hpp"

const int CM_DATA_BUFFER_SIZE = 64;

class C2DriverConcept2 : public C2Driver
{
  protected:
    UINT32_T _cmd_data[CM_DATA_BUFFER_SIZE];
    UINT16_T _cmd_data_size;
    UINT32_T _rsp_data[CM_DATA_BUFFER_SIZE];
    UINT16_T _rsp_data_size;
    UINT16_T _rsp_data_index;
  public:
    
     ERRCODE_T init(int timeout);
     ERRCODE_T discover( UINT16_T &deviceCount);
     ERRCODE_T lastError(char *error, int bufSize);
    
     void cSafeBufferInit();
     void cSafeBufferAdd(UINT8_T data);
     int cSafeBufferCount();
    
     ERRCODE_T cSafeBufferSend(int deviceNumber);
    
     int cSafeReceiveBufferCount();
     int cSafeReceiveBufferIndex();
     UINT8_T cSafeReceiveBufferCurrent(int add_offset=0);
     void cSafeReceiveBufferNext(int add=1);
    
     ERRCODE_T close();
    
};

#endif /* C2DriverConcept2_hpp */
