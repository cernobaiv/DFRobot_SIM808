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
 
#include "sim808.h"
#include "SIM808_HTTP.h"

const char *desHTTPACTION = "+HTTPACTION: ";
const char *desHTTPREAD = "+HTTPREAD: ";


bool DFRobot_SIM808_HTTP::initHttpService()
{
    if(!sim808_check_with_cmd("AT+CGATT?\r\n", "OK\r\n",CMD))
        return false;

    if(!sim808_check_with_cmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n", "OK\r\n",CMD))
        return false;

    if(!sim808_check_with_cmd("AT+SAPBR=3,1,\"APN\",\"net\"\r\n","OK\r\n",CMD))
        return false;

    if(!sim808_check_with_cmd("AT+SAPBR=1,1\r\n", "OK\r\n",CMD))
        return false;

    if(!sim808_check_with_cmd("AT+HTTPINIT\r\n", "OK\r\n",CMD))
        return false;

    return true;
}

bool DFRobot_SIM808_HTTP::httpGET(const char *url, char *responseBody)
{
    String atRequest = String("AT+HTTPPARA=\"URL\",\"") + url + "\"\r\n";
    //const char* atRequest = "AT+HTTPPARA=\"URL\",\"api.ipify.org\"\r\n";

    Serial.print("Requesting: ");
    Serial.println(atRequest);

    if(!sim808_check_with_cmd(atRequest.c_str(), "OK\r\n",CMD))
        return false;

    char buf[128];

    if(!sim808_check_with_cmd("AT+HTTPACTION=0\r\n", "OK\r\n",CMD))
        return false;

    sim808_read_buffer(buf, sizeof(buf));

    // Action response.
    char* data = strtok(buf, "\r\n");

    if (strstr(data, desHTTPACTION))
    {
        char* tok = strtok(data + 13, ",");
        char* resultCode = strtok(NULL, ",");

        if (resultCode && strstr(resultCode, "200"))
        {
            sim808_send_cmd("AT+HTTPREAD\r\n");
            sim808_clean_buffer(buf, sizeof(buf));
            sim808_read_buffer(buf, sizeof(buf));

            // Empty line.
            tok = strtok(buf, "\r\n");

            // Num bytes read.
            tok = strtok(NULL, "\r\n");

            // Data
            data = strtok(NULL, "\r\n");

            char* res = strtok(NULL, "\r\n");

            if (res && strstr(res, "OK"))
            {
                copyWithDefault(data, responseBody, "");
                return true;
            }
        }
    }


    return false;
}

bool DFRobot_SIM808_HTTP::stopHttpService()
{
    if(!sim808_check_with_cmd("AT+HTTPTERM\r\n", "OK\r\n",CMD))
        return false;

    if(!sim808_check_with_cmd("AT+SAPBR=0,1\r\n", "OK\r\n",CMD))
        return false;

    return true;
}
