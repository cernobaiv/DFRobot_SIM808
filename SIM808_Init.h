/*!
 * @file DFRobot_sim808.h
 * @n Header file for DFRobot's SIM808 GPS/DFRobot_SIM808/GSM Shield
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
 
#ifndef __DFROBOT_SIM808_H__
#define __DFROBOT_SIM808_H__

#include "sim808.h"
#include <SoftwareSerial.h>

/** DFRobot_SIM808 class.
 *  used to realize DFRobot_SIM808 communication
 */ 

#define PWR_PIN             8
#define SIM808_RX_to_TX     9
#define SIM808_TX_to_RX     10

#define SIM808_BAUD_RATE    9600


class SIM808_Init
{
public:
    /** Create DFRobot_SIM808 instance
     *  @param number default phone number during mobile communication
     */
    SIM808_Init();
    

    void serialBegin();

    bool powerUpDown();

    bool init();

    SoftwareSerial& serial();

    void initInALoop();

private:
    bool checkSIMStatus(void);

private:
    SoftwareSerial m_mySerial;
};

#endif
