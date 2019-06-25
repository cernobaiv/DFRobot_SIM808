/*!
 * @file DFRobot_sim808.cpp
 * @A library  for DFRobot's SIM808 GPS/DFRobot_SIM808/GSM Shield
 *
 * @copyright	[DFRobot](http://www.dfrobot.com), 2016
 *
 * @author [Jason](jason.ling@dfrobot.com)
 * @version  V1.0
 * @date  2016-09-23
 
 * The MIT License (MIT)
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#include "SIM808_SMS.h"


bool DFRobot_SIM808_SMS::sendSMS(char *number, char *data)
{
    //char cmd[32];
    if(!sim808_check_with_cmd("AT+CMGF=1\r\n", "OK\r\n", CMD)) { // Set message mode to ASCII
        return false;
    }
    delay(500);
	sim808_flush_serial();
	sim808_send_cmd("AT+CMGS=\"");
	sim808_send_cmd(number);
    //sprintf(cmd,"AT+CMGS=\"%s\"\r\n", number);
	//snprintf(cmd, sizeof(cmd),"AT+CMGS=\"%s\"\r\n", number);
//    if(!sim808_check_with_cmd(cmd,">",CMD)) {
    if(!sim808_check_with_cmd("\"\r\n",">",CMD)) {
        return false;
    }
    delay(1000);
    sim808_send_cmd(data);
    delay(500);
    sim808_send_End_Mark();
    return sim808_wait_for_resp("OK\r\n", CMD);
}

char DFRobot_SIM808_SMS::isSMSunread()
{
    char gprsBuffer[48];  //48 is enough to see +CMGL:
    char *s;
    
	 sim808_check_with_cmd("AT+CMGF=1\r\n","OK\r\n",CMD);
    delay(1000);

    //List of all UNREAD SMS and DON'T change the SMS UNREAD STATUS
    sim808_send_cmd(F("AT+CMGL=\"REC UNREAD\",1\r\n"));
    /*If you want to change SMS status to READ you will need to send:
          AT+CMGL=\"REC UNREAD\"\r\n
      This command will list all UNREAD SMS and change all of them to READ
      
     If there is not SMS, response is (30 chars)
         AT+CMGL="REC UNREAD",1  --> 22 + 2
                                 --> 2
         OK                      --> 2 + 2

     If there is SMS, response is like (>64 chars)
         AT+CMGL="REC UNREAD",1
         +CMGL: 9,"REC UNREAD","XXXXXXXXX","","14/10/16,21:40:08+08"
         Here SMS text.
         OK  
         
         or

         AT+CMGL="REC UNREAD",1
         +CMGL: 9,"REC UNREAD","XXXXXXXXX","","14/10/16,21:40:08+08"
         Here SMS text.
         +CMGL: 10,"REC UNREAD","YYYYYYYYY","","14/10/16,21:40:08+08"
         Here second SMS        
         OK           
    */

    sim808_clean_buffer(gprsBuffer,31); 
    sim808_read_buffer(gprsBuffer,30,DEFAULT_TIMEOUT); 
    //Serial.print("Buffer isSMSunread: ");Serial.println(gprsBuffer);

    if(NULL != ( s = strstr(gprsBuffer,"OK"))) {
        //In 30 bytes "doesn't" fit whole +CMGL: response, if recieve only "OK"
        //    means you don't have any UNREAD SMS
        delay(50);
        return 0;
    } else {
        //More buffer to read
        //We are going to flush serial data until OK is recieved
        sim808_wait_for_resp("OK\r\n", CMD);        
        //sim808_flush_serial();
        //We have to call command again
        sim808_send_cmd("AT+CMGL=\"REC UNREAD\",1\r\n");
        sim808_clean_buffer(gprsBuffer,48); 
        sim808_read_buffer(gprsBuffer,47,DEFAULT_TIMEOUT);
		//Serial.print("Buffer isSMSunread 2: ");Serial.println(gprsBuffer);       
        if(NULL != ( s = strstr(gprsBuffer,"+CMGL:"))) {
            //There is at least one UNREAD SMS, get index/position
            s = strstr(gprsBuffer,":");
            if (s != NULL) {
                //We are going to flush serial data until OK is recieved
                sim808_wait_for_resp("OK\r\n", CMD);
                return atoi(s+1);
            }
        } else {
            return -1; 

        }
    } 
    return -1;
}

bool DFRobot_SIM808_SMS::readSMS(int messageIndex, char *message, int length, char *phone, char *datetime)
{
  /* Response is like:
  AT+CMGR=2
  
  +CMGR: "REC READ","XXXXXXXXXXX","","14/10/09,17:30:17+08"
  SMS text here
  
  So we need (more or lees), 80 chars plus expected message length in buffer. CAUTION FREE MEMORY
  */

    int i = 0;
    char gprsBuffer[80 + length];
    //char cmd[16];
	char num[4];
    char *p,*p2,*s;
    
    sim808_check_with_cmd("AT+CMGF=1\r\n","OK\r\n",CMD);
    delay(1000);
	//sprintf(cmd,"AT+CMGR=%d\r\n",messageIndex);
    //sim808_send_cmd(cmd);
	sim808_send_cmd("AT+CMGR=");
	itoa(messageIndex, num, 10);
	sim808_send_cmd(num);
	sim808_send_cmd("\r\n");
    sim808_clean_buffer(gprsBuffer,sizeof(gprsBuffer));
    sim808_read_buffer(gprsBuffer,sizeof(gprsBuffer));
      
    if(NULL != ( s = strstr(gprsBuffer,"+CMGR:"))){
        // Extract phone number string
        p = strstr(s,",");
        p2 = p + 2; //We are in the first phone number character
        p = strstr((char *)(p2), "\"");
        if (NULL != p) {
            i = 0;
            while (p2 < p) {
                phone[i++] = *(p2++);
            }
            phone[i] = '\0';            
        }
        // Extract date time string
        p = strstr((char *)(p2),",");
        p2 = p + 1; 
        p = strstr((char *)(p2), ","); 
        p2 = p + 2; //We are in the first date time character
        p = strstr((char *)(p2), "\"");
        if (NULL != p) {
            i = 0;
            while (p2 < p) {
                datetime[i++] = *(p2++);
            }
            datetime[i] = '\0';
        }        
        if(NULL != ( s = strstr(s,"\r\n"))){
            i = 0;
            p = s + 2;
            while((*p != '\r')&&(i < length-1)) {
                message[i++] = *(p++);
            }
            message[i] = '\0';
        }
        return true;
    }
    return false;    
}

bool DFRobot_SIM808_SMS::readSMS(int messageIndex, char *message,int length)
{
    int i = 0;
    char gprsBuffer[100];
    //char cmd[16];
	char num[4];
    char *p,*s;
	
    sim808_check_with_cmd("AT+CMGF=1\r\n","OK\r\n",CMD);
    delay(1000);
	sim808_send_cmd("AT+CMGR=");
	itoa(messageIndex, num, 10);
	sim808_send_cmd(num);
	sim808_send_cmd("\r\n");
//  sprintf(cmd,"AT+CMGR=%d\r\n",messageIndex);
//    sim808_send_cmd(cmd);
    sim808_clean_buffer(gprsBuffer,sizeof(gprsBuffer));
    sim808_read_buffer(gprsBuffer,sizeof(gprsBuffer),DEFAULT_TIMEOUT);
    if(NULL != ( s = strstr(gprsBuffer,"+CMGR:"))){
        if(NULL != ( s = strstr(s,"\r\n"))){
            p = s + 2;
            while((*p != '\r')&&(i < length-1)) {
                message[i++] = *(p++);
            }
            message[i] = '\0';
            return true;
        }
    }
    return false;   
}

bool DFRobot_SIM808_SMS::deleteSMS(int index)
{
    //char cmd[16];
	char num[4];
    //sprintf(cmd,"AT+CMGD=%d\r\n",index);
    sim808_send_cmd("AT+CMGD=");
	itoa(index, num, 10);
	sim808_send_cmd(num);
	//snprintf(cmd,sizeof(cmd),"AT+CMGD=%d\r\n",index);
    //sim808_send_cmd(cmd);
    //return 0;
    // We have to wait OK response
	//return sim808_check_with_cmd(cmd,"OK\r\n",CMD);
	return sim808_check_with_cmd("\r","OK\r\n",CMD);	
}
