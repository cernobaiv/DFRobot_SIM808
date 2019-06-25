#include "sim808.h"
#include "SIM808_GPRS.h"

bool DFRobot_SIM808_GPRS::callUp(char *number)
{
    //char cmd[24];
    if(!sim808_check_with_cmd("AT+COLP=1\r\n","OK\r\n",CMD)) {
        return false;
    }
    delay(1000);
	//HACERR quitar SPRINTF para ahorar memoria ???
    //sprintf(cmd,"ATD%s;\r\n", number);
    //sim808_send_cmd(cmd);
	sim808_send_cmd("ATD");
	sim808_send_cmd(number);
	sim808_send_cmd(";\r\n");
    return true;
}

void DFRobot_SIM808_GPRS::answer(void)
{
    sim808_send_cmd("ATA\r\n");  //TO CHECK: ATA doesnt return "OK" ????
}

bool DFRobot_SIM808_GPRS::hangup(void)
{
    return sim808_check_with_cmd("ATH\r\n","OK\r\n",CMD);
}

bool DFRobot_SIM808_GPRS::disableCLIPring(void)
{
    return sim808_check_with_cmd("AT+CLIP=0\r\n","OK\r\n",CMD);
}

bool DFRobot_SIM808_GPRS::getSubscriberNumber(char *number)
{
	//AT+CNUM								--> 7 + CR = 8
	//+CNUM: "","+628157933874",145,7,4		--> CRLF + 45 + CRLF = 49
	//										-->
	//OK									--> CRLF + 2 + CRLF = 6

    byte i = 0;
    char gprsBuffer[65];
    char *p,*s;
	sim808_flush_serial();
    sim808_send_cmd("AT+CNUM\r\n");
    sim808_clean_buffer(gprsBuffer,65);
    sim808_read_buffer(gprsBuffer,65,DEFAULT_TIMEOUT);
	//Serial.print(gprsBuffer);
    if(NULL != ( s = strstr(gprsBuffer,"+CNUM:"))) {
        s = strstr((char *)(s),",");
        s = s + 2;  //We are in the first phone number character 
        p = strstr((char *)(s),"\""); //p is last character """
        if (NULL != s) {
            i = 0;
            while (s < p) {
              number[i++] = *(s++);
            }
            number[i] = '\0';
        }
        return true;
    }  
    return false;
}

bool DFRobot_SIM808_GPRS::isCallActive(char *number)
{
    char gprsBuffer[46];  //46 is enough to see +CPAS: and CLCC:
    char *p, *s;
    int i = 0;

    sim808_send_cmd("AT+CPAS\r\n");
    /*Result code:
        0: ready
        2: unknown
        3: ringing
        4: call in progress
    
      AT+CPAS   --> 7 + 2 = 9 chars
                --> 2 char              
      +CPAS: 3  --> 8 + 2 = 10 chars
                --> 2 char
      OK        --> 2 + 2 = 4 chars
    
      AT+CPAS
      
      +CPAS: 0
      
      OK
    */

    sim808_clean_buffer(gprsBuffer,29);
    sim808_read_buffer(gprsBuffer,27);
    //HACERR cuando haga lo de esperar a OK no me har閾哸 falta esto
    //We are going to flush serial data until OK is recieved
    sim808_wait_for_resp("OK\r\n", CMD);    
    //Serial.print("Buffer isCallActive 1: ");Serial.println(gprsBuffer);
    if(NULL != ( s = strstr(gprsBuffer,"+CPAS:"))) {
      s = s + 7;
      if (*s != '0') {
         //There is something "running" (but number 2 that is unknow)
         if (*s != '2') {
           //3 or 4, let's go to check for the number
           sim808_send_cmd("AT+CLCC\r\n");
           /*
           AT+CLCC --> 9
           
           +CLCC: 1,1,4,0,0,"656783741",161,""
           
           OK  

           Without ringing:
           AT+CLCC
           OK              
           */

           sim808_clean_buffer(gprsBuffer,46);
           sim808_read_buffer(gprsBuffer,45);
			//Serial.print("Buffer isCallActive 2: ");Serial.println(gprsBuffer);
           if(NULL != ( s = strstr(gprsBuffer,"+CLCC:"))) {
             //There is at least one CALL ACTIVE, get number
             s = strstr((char *)(s),"\"");
             s = s + 1;  //We are in the first phone number character            
             p = strstr((char *)(s),"\""); //p is last character """
             if (NULL != s) {
                i = 0;
                while (s < p) {
                    number[i++] = *(s++);
                }
                number[i] = '\0';            
             }
             //I need to read more buffer
             //We are going to flush serial data until OK is recieved
             return sim808_wait_for_resp("OK\r\n", CMD);
           }
         }
      }        
    } 
    return false;
}

bool DFRobot_SIM808_GPRS::getDateTime(char *buffer)
{
	//If it doesn't work may be for two reasons:
	//		1. Your carrier doesn't give that information
	//		2. You have to configurate the SIM808 IC.
	//			- First with SIM808_Serial_Debug example try this AT command: AT+CLTS?
	//			- If response is 0, then it is disabled.
	//			- Enable it by: AT+CLTS=1
	//			- Now you have to save this config to EEPROM memory of SIM808 IC by: AT&W
	//			- Now, you have to power down and power up again the SIM808 
	//			- Try now again: AT+CCLK?
	//			- It should work
	
	//AT+CCLK?						--> 8 + CR = 9
	//+CCLK: "14/11/13,21:14:41+04"	--> CRLF + 29+ CRLF = 33
	//								
	//OK							--> CRLF + 2 + CRLF =  6

    byte i = 0;
    char gprsBuffer[50];
    char *p,*s;
	sim808_flush_serial();
    sim808_send_cmd("AT+CCLK?\r");
    sim808_clean_buffer(gprsBuffer,50);
    sim808_read_buffer(gprsBuffer,50,DEFAULT_TIMEOUT);
    if(NULL != ( s = strstr(gprsBuffer,"+CCLK:"))) {
        s = strstr((char *)(s),"\"");
        s = s + 1;  //We are in the first phone number character 
        p = strstr((char *)(s),"\""); //p is last character """
        if (NULL != s) {
            i = 0;
            while (s < p) {
              buffer[i++] = *(s++);
            }
            buffer[i] = '\0';            
        }
        return true;
    }  
    return false;
}

bool DFRobot_SIM808_GPRS::getSignalStrength(int *buffer)
{
	//AT+CSQ						--> 6 + CR = 10
	//+CSQ: <rssi>,<ber>			--> CRLF + 5 + CRLF = 9						
	//OK							--> CRLF + 2 + CRLF =  6

	byte i = 0;
	char gprsBuffer[26];
	char *p, *s;
	char buffers[4];
	sim808_flush_serial();
	sim808_send_cmd("AT+CSQ\r");
	sim808_clean_buffer(gprsBuffer, 26);
	sim808_read_buffer(gprsBuffer, 26, DEFAULT_TIMEOUT);
	if (NULL != (s = strstr(gprsBuffer, "+CSQ:"))) {
		s = strstr((char *)(s), " ");
		s = s + 1;  //We are in the first phone number character 
		p = strstr((char *)(s), ","); //p is last character """
		if (NULL != s) {
			i = 0;
			while (s < p) {
				buffers[i++] = *(s++);
			}
			buffers[i] = '\0';
		}
		*buffer = atoi(buffers);
		return true;
	}
	return false;
}

bool DFRobot_SIM808_GPRS::sendUSSDSynchronous(char *ussdCommand, char *resultcode, char *response)
{
	//AT+CUSD=1,"{command}"
	//OK
	//
	//+CUSD:1,"{response}",{int}

	byte i = 0;
    char gprsBuffer[200];
    char *p,*s;
    sim808_clean_buffer(response, sizeof(response));
	
	sim808_flush_serial();
    sim808_send_cmd("AT+CUSD=1,\"");
    sim808_send_cmd(ussdCommand);
    sim808_send_cmd("\"\r");
	if(!sim808_wait_for_resp("OK\r\n", CMD))
		return false;
    sim808_clean_buffer(gprsBuffer,200);
    sim808_read_buffer(gprsBuffer,200,DEFAULT_TIMEOUT);
    if(NULL != ( s = strstr(gprsBuffer,"+CUSD: "))) {
        *resultcode = *(s+7);
		resultcode[1] = '\0';
		if(!('0' <= *resultcode && *resultcode <= '2'))
			return false;
		s = strstr(s,"\"");
        s = s + 1;  //We are in the first phone number character
        p = strstr(s,"\""); //p is last character """
        if (NULL != s) {
            i = 0;
            while (s < p) {
              response[i++] = *(s++);
            }
            response[i] = '\0';            
        }
		return true;
	}
	return false;
}

bool DFRobot_SIM808_GPRS::cancelUSSDSession(void)
{
    return sim808_check_with_cmd("AT+CUSD=2\r\n","OK\r\n",CMD);
}

bool DFRobot_SIM808_GPRS::getLocation(const char* apn, char* latitude, char* longitude)
{
    int i = 0;
    char gprsBuffer[80];
    char buffer[20];
    char *s;

    sim808_check_with_cmd("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r","OK\r\n",CMD);

    sim808_send_cmd("AT+SAPBR=3,1,\"APN\",\"");
    if (apn) {
      sim808_send_cmd(apn);
    }
    sim808_check_with_cmd("\"\r", OK, CMD);

    sim808_check_with_cmd("AT+SAPBR=1,1\r", OK, CMD);


    sim808_flush_serial();
    sim808_send_cmd("AT+CIPGSMLOC=1,1\r");
    sim808_clean_buffer(gprsBuffer,sizeof(gprsBuffer));
    sim808_read_buffer(gprsBuffer,sizeof(gprsBuffer),2*DEFAULT_TIMEOUT,6*DEFAULT_INTERCHAR_TIMEOUT);

    // Echo command.
    char* tok = strtok(gprsBuffer, "\r\n");

    // Actual data.
    char* data = strtok(NULL, "\r\n");

    if (data && strstr(data, "+CIPGSMLOC: "))
    {
        // Empty line
        tok = strtok(NULL, "\n");

        // Response
        tok = strtok(NULL, "\n");

        if (tok && strstr(tok, "OK"))
        {
            char* locationCode = strtok(data + 12, ",");

            // Zero means success.
            if(locationCode && locationCode[0] == '0')
            {
                copyWithDefault(strtok(NULL, ","), longitude);
                copyWithDefault(strtok(NULL, ","), latitude);

                return true;
            }
        }
    }

    return false;


    if(NULL != ( s = strstr(gprsBuffer,"+CIPGSMLOC:")))
    {
        s = strstr((char *)s, ",");
        s = s+1;
        //Serial.println(*s);
        i=0;
        while(*(++s) !=  ',')
            buffer[i++]=*s;
        buffer[i] = 0;

        copyWithDefault(latitude, buffer);

        i=0;
        while(*(++s) !=  ',')
            buffer[i++]=*s;
        buffer[i] = 0;

        copyWithDefault(longitude, buffer);
        return true;
    }
    return false;
}
