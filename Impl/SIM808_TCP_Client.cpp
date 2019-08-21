#include "SIM808_TCP_Client.h"


bool DFRobot_SIM808_TCP_Client::connect(const char* host, int port, int timeout, int chartimeout)
{
    char num[6];
    char resp[96];

    sim808_send_cmd("AT+CIPSTART=\"TCP\",\"");
    sim808_send_cmd(host);
    sim808_send_cmd("\",");
    itoa(port, num, 10);
    sim808_send_cmd(num);
    sim808_send_cmd("\r\n");

    //sim808_send_cmd(cmd);
    sim808_read_buffer(resp, 96, timeout, chartimeout);

    if(NULL != strstr(resp, "CONNECT")) { //ALREADY CONNECT or CONNECT OK
        return true;
    }
    return false;
}

