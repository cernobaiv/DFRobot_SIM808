#include "SIM808_Net.h"

namespace
{
    const char OK[] =  "OK\r\n";
    const char ERROR[] =  "ERROR";
}

bool DFRobot_SIM808_Net::join(const char* apn)
{
    sim808_send_cmd("AT+CSTT=\"");

    if (apn) {
      sim808_send_cmd(apn);
    }

    if (sim808_check_with_cmd("\"\r\n", OK, CMD))
    {
        //Brings up wireless connection
        if (sim808_check_with_cmd("AT+CIICR\r\n", OK, CMD))
        {
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

            // Echo command
            const char* tok = strtok(ipAddr, "\r\n");

            // IP addr
            const char* bufIpAddr = strtok(NULL, "\r\n");

            if (strstr(bufIpAddr, ERROR) != NULL)
                return false;

            sim808_clean_buffer(localIP, sizeof(localIP));
            copyWithDefault(bufIpAddr, localIP);
            return true;
        }
    }

    return false;
}

void DFRobot_SIM808_Net::disconnect()
{
    sim808_send_cmd("AT+CIPSHUT\r\n");
}

bool DFRobot_SIM808_Net::is_connected()
{
    char resp[96];
    sim808_send_cmd("AT+CIPSTATUS\r\n");
    sim808_read_buffer(resp,sizeof(resp), DEFAULT_TIMEOUT);

    return (strstr(resp,"CONNECTED") != nullptr);
}

bool DFRobot_SIM808_Net::close()
{
    // if not connected, return
    if (!is_connected()) {
        return true;
    }
    return sim808_check_with_cmd("AT+CIPCLOSE\r\n", "CLOSE OK\r\n", CMD);
}

int DFRobot_SIM808_Net::readable(void)
{
    return sim808_check_readable();
}

int DFRobot_SIM808_Net::wait_readable(int wait_time)
{
    return sim808_wait_readable(wait_time);
}

int DFRobot_SIM808_Net::wait_writeable(int req_size)
{
    return req_size+1;
}

int DFRobot_SIM808_Net::send(const char * str, int len)
{
	char num[4];
    if(len > 0){
		sim808_send_cmd("AT+CIPSEND=");
		itoa(len, num, 10);
		sim808_send_cmd(num);

		if(!sim808_check_with_cmd("\r\n",">",CMD)) {
            return 0;
        }

        //delay(500);
        sim808_send_cmd(str);
        //delay(500);
        sim808_send_End_Mark();
        if(!sim808_wait_for_resp("SEND OK\r\n", DATA, DEFAULT_TIMEOUT * 10, DEFAULT_INTERCHAR_TIMEOUT * 10)) {
            return 0;
        }        
    }
    return len;
}
    

int DFRobot_SIM808_Net::recv(char* buf, int len)
{
    return sim808_recv(buf, len);
}

const char* DFRobot_SIM808_Net::getLocalIPAddr() const
{
    return localIP;
}

