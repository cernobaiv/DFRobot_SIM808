#include "SIM808_Init.h"


SIM808_Init::SIM808_Init()
    : m_mySerial(SIM808_RX_to_TX, SIM808_TX_to_RX)
{
    sim808_init(&m_mySerial, 0);
}

bool SIM808_Init::serialBegin()
{
    m_mySerial.begin(SIM808_BAUD_RATE);
    //Serial.begin(9600);

    if(!sim808_check_with_cmd("AT\r\n","OK\r\n",CMD))
        return false;

    sim808_send_cmd("AT+IPR=");

    char baudRate[8];
    itoa(SIM808_BAUD_RATE, baudRate, 10);

    sim808_send_cmd(baudRate);

    return sim808_check_with_cmd("\"\r\n", "OK\r\n", CMD);
}

bool SIM808_Init::init()
{
    if(!sim808_check_with_cmd("AT\r\n","OK\r\n",CMD))
		return false;

	// 1 : OK
    if(!sim808_check_with_cmd("AT+CFUN=1\r\n","OK\r\n",CMD))
        return false;

    if(!checkSIMStatus())
		return false;

    return true;
}

SoftwareSerial& SIM808_Init::serial()
{
    return m_mySerial;
}

void SIM808_Init::initInALoop()
{
    //******** Initialize sim808 module *************
   while(!init()) {
       delay(1000);
       //Serial.print("Sim808 init error\r\n");
   }
}

bool SIM808_Init::powerUpDown()
{
    if (init())
        return true;

    pinMode(PWR_PIN, OUTPUT);

    digitalWrite(PWR_PIN,HIGH);
    delay(1300);
    digitalWrite(PWR_PIN,LOW);
    delay(200);

    return init();
}
  
bool SIM808_Init::checkSIMStatus(void)
{
    char gprsBuffer[32];
    int count = 0;
    sim808_clean_buffer(gprsBuffer,32);
    while(count < 3) {
        sim808_send_cmd("AT+CPIN?\r\n");
        sim808_read_buffer(gprsBuffer,32,DEFAULT_TIMEOUT);
        if((NULL != strstr(gprsBuffer,"+CPIN: READY"))) {
            break;
        }
        count++;
        delay(300);
    }
    if(count == 3) {
        return false;
    }
    return true;
}
