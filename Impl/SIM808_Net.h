#pragma once

#include "sim808.h"

class DFRobot_SIM808_Net
{
public:
    virtual ~DFRobot_SIM808_Net() {}

    bool join(const char* apn = 0, const char* userName = 0, const char* passWord = 0);

    /** Disconnect the DFRobot_SIM808 module from the network
     *  @returns
     */
    void disconnect(void);

    /** Check if a tcp link is active
     *  @returns true if successful
     */
    virtual bool is_connected(void);
	
    /** Close a connection
     *  @returns true if successful
     */
    virtual bool close(void);
	
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
