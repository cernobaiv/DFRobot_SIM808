#pragma once

#include "sim808.h"
#include "SIM808_Net.h"

class DFRobot_SIM808_UDP_Client : public DFRobot_SIM808_Net
{
public:	     
    /** Open a UDP connection with the specified host on the specified port
     *  @param host host (can be either an ip address or a name. If a name is provided, a dns request will be established)
     *  @param port
     *  @param timeout wait seconds till connected
     *  @param chartimeout wait milliseconds between characters from DFRobot_SIM808 module
     *  @returns true if successful
     */
    bool connect(const char* host, int port, int timeout = 2 * DEFAULT_TIMEOUT, int chartimeout = 2 * DEFAULT_INTERCHAR_TIMEOUT);
};
