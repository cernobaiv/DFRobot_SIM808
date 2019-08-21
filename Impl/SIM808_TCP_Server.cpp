#include "sim808.h"
#include "SIM808_TCP_Server.h"

bool DFRobot_SIM808_TCP_Server::bind(unsigned short port, int timeout, int chartimeout)
{
    char num[6];
    char resp[96];

    sim808_send_cmd("AT+CIPSERVER=1,");
    itoa(port, num, 10);
    sim808_send_cmd(num);
    sim808_send_cmd("\r\n");

    //sim808_send_cmd(cmd);
    sim808_read_buffer(resp, 96, timeout, chartimeout);

    if(NULL != strstr(resp, "SERVER OK")) {
        return true;
    }
    return false;
}

bool DFRobot_SIM808_TCP_Server::is_connected(void)
{
    char resp[96];
    sim808_send_cmd("AT+CIPSTATUS\r\n");
    sim808_read_buffer(resp,sizeof(resp),DEFAULT_TIMEOUT);

    return (strstr(resp, "LISTENING") != NULL);
}

bool DFRobot_SIM808_TCP_Server::close()
{
    // if not connected, return
    if (!is_connected()) {
        return true;
    }
    return sim808_check_with_cmd("AT+CIPSERVER=0\r\n", "SERVER CLOSE\r\n", CMD);
}

bool DFRobot_SIM808_TCP_Server::deactivatePDPandCLoseAllConnections()
{
    // if not connected, return
    if (!is_connected()) {
        return true;
    }
    return sim808_check_with_cmd("AT+CIPSHUT=0\r\n", "SHUT OK\r\n", CMD);
}
