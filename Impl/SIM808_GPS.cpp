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
#include "SIM808_GPS.h"

const char *desCGNSINF = "+CGNSINF: ";

bool DFRobot_SIM808_GPS::powerOnGNSS()
{
    return sim808_check_with_cmd("AT+CGNSPWR=1\r\n", "OK\r\n", CMD);
}

bool DFRobot_SIM808_GPS::powerOffGNSS()
{
    return sim808_check_with_cmd("AT+CGNSPWR=0\r\n", "OK\r\n", CMD);
}

bool DFRobot_SIM808_GPS::getCGNSINF()
{
    sim808_send_cmd("AT+CGNSINF\r\n");

    char buf[156];
    sim808_recv(buf, sizeof(buf));

    // Echo command.
    char* tok = strtok(buf, "\r\n");

    // Actual data.
    char* data = strtok(NULL, "\r\n");

    if (data)
    {
        // Empty line
        tok = strtok(NULL, "\n");

        char* res = strtok(NULL, "\n");

        if (res && strstr(res, "OK"))
        {
            Serial.println(data);

            if (strstr(data, desCGNSINF))
            {
                // Check that the GPS run status and Fix status are both 1.
                if (data[10] == '1' && data[12] == '1')
                {
                    // UTC date & Time
                    tok = strtok(data + 14, ",");
                    copyWithDefault(tok, GNSSData.utcTime, "");

                    // Latitude
                    tok = strtok(NULL, ",");
                    copyWithDefault(tok, GNSSData.lat);

                    // Longitude
                    tok = strtok(NULL, ",");
                    copyWithDefault(tok, GNSSData.lon);

                    // Altitude (meters)
                    tok = strtok(NULL, ",");
                    copyWithDefault(tok, GNSSData.altitude);

                    // Speed over ground (Km/h)
                    tok = strtok(NULL, ",");
                    copyWithDefault(tok, GNSSData.speed);

                    // Course (degrees)
                    tok = strtok(NULL, ",");
                    copyWithDefault(tok, GNSSData.course);


                    // Skip 7 variables.
                    tok = strtok(NULL, ",");
                    tok = strtok(NULL, ",");
                    tok = strtok(NULL, ",");
                    tok = strtok(NULL, ",");
                    tok = strtok(NULL, ",");
                    tok = strtok(NULL, ",");
                    tok = strtok(NULL, ",");

                    // gnssSatellites
                    tok = strtok(NULL, ",");
                    copyWithDefault(tok, GNSSData.gnssSatellites);

                    return true;

                }
            }
        }
    }

    return false;
}
