#include "sim808.h"
#include "SIM808_TCP_Server.h"

namespace
{
    const char OK[] =  "OK\r\n";
    const char ERROR[] =  "ERROR";
}

//Here is where we ask for APN configuration, with F() so we can save MEMORY
bool DFRobot_SIM808_TCP_Server::join(const char* apn)
{
    sim808_send_cmd("AT+CSTT=\"");

    if (apn) {
      sim808_send_cmd(apn);
    }

    sim808_check_with_cmd("\"\r\n", OK, CMD);
    

    //Brings up wireless connection
    sim808_check_with_cmd("AT+CIICR\r\n", OK, CMD);

    //Get local IP address
    sim808_send_cmd("AT+CIFSR\r\n");

    char ipAddr[32];
    sim808_clean_buffer(ipAddr,32);
    sim808_read_buffer(ipAddr,32);
	//Response:
	//AT+CIFSR\r\n       -->  8 + 2
	//\r\n				 -->  0 + 2
	//10.160.57.120\r\n  --> 15 + 2 (max)   : TOTAL: 29 
	//Response error:
	//AT+CIFSR\r\n       
	//\r\n				 
	//ERROR\r\n

    Serial.print("CIFSR response: ");
    Serial.println(ipAddr);

    // Echo command
    const char* tok = strtok(ipAddr, "\r\n");

    // IP addr
    const char* bufIpAddr = strtok(NULL, "\r\n");

    if (strstr(bufIpAddr, ERROR) != NULL)
        return false;

    copyWithDefault(bufIpAddr, localIP);
    return true;
} 

void DFRobot_SIM808_TCP_Server::disconnect()
{
    sim808_send_cmd("AT+CIPSHUT\r\n");
}

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

int DFRobot_SIM808_TCP_Server::readable(void)
{
    return sim808_check_readable();
}

int DFRobot_SIM808_TCP_Server::wait_readable(int wait_time)
{
    return sim808_wait_readable(wait_time);
}

int DFRobot_SIM808_TCP_Server::wait_writeable(int req_size)
{
    return req_size+1;
}

int DFRobot_SIM808_TCP_Server::send(const char * str, int len)
{
	char num[4];
    if(len > 0){
		sim808_send_cmd("AT+CIPSEND=");
		itoa(len, num, 10);
		sim808_send_cmd(num);

		if(!sim808_check_with_cmd("\r\n",">",CMD)) {
            return 0;
        }

        delay(500);
        sim808_send_cmd(str);
        delay(500);
        sim808_send_End_Mark();
        if(!sim808_wait_for_resp("SEND OK\r\n", DATA, DEFAULT_TIMEOUT * 10, DEFAULT_INTERCHAR_TIMEOUT * 10)) {
            return 0;
        }        
    }
    return len;
}
    

int DFRobot_SIM808_TCP_Server::recv(char* buf, int len)
{
    return sim808_recv(buf, len);
}


const char *DFRobot_SIM808_TCP_Server::getLocalIPAddr() const
{
    return localIP;
}
