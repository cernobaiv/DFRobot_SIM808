#pragma once

#include "sim808.h"
#include "SIM808_Net.h"

class DFRobot_SIM808_TCP_Server : public DFRobot_SIM808_Net
{
public:
    /** Binds to a port and start listening for incoming connections.
     *  @param host host (can be either an ip address or a name. If a name is provided, a dns request will be established)
     *  @param port
     *  @param timeout wait seconds till connected
     *  @param chartimeout wait milliseconds between characters from DFRobot_SIM808 module
     *  @returns true if successful
     */
    bool bind(unsigned short port, int timeout = 2 * DEFAULT_TIMEOUT, int chartimeout = 2 * DEFAULT_INTERCHAR_TIMEOUT);

    /** Check if a tcp link is active
     *  @returns true if successful
     */
    bool is_connected(void) override;
	
    /** Close the server
     *  @returns true if successful
     */
    bool close(void) override;

    /** Deactivate the PDP context and close all connections.
     *  @returns true if successful
     */
    bool deactivatePDPandCLoseAllConnections();
	
    /** check if DFRobot_SIM808 module is readable or not
     *  @returns true if readable
     */
    int readable(void);

    /** wait a few time to check if DFRobot_SIM808 module is readable or not
     *  @param socket socket
     *  @param wait_time time of waiting
     */
    int wait_readable(int wait_time);

    /** wait a few time to check if DFRobot_SIM808 module is writeable or not
     *  @param socket socket
     *  @param wait_time time of waiting
     */
    int wait_writeable(int req_size);

    /** send data to socket
     *  @param socket socket
     *  @param str string to be sent
     *  @param len string length
     *  @returns return bytes that actually been send
     */
    int send(const char * str, int len);

    /** read data from socket
     *  @param socket socket
     *  @param buf buffer that will store the data read from socket
     *  @param len string length need to read from socket
     *  @returns bytes that actually read
     */
    int recv(char* buf, int len);

    const char* getLocalIPAddr() const;

private:
    char localIP[18];
};
