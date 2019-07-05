#include "sim808.h"
#include "SIM808_HTTP.h"

const char *desHTTPACTION = "+HTTPACTION: ";
const char *desHTTPREAD = "+HTTPREAD: ";


bool DFRobot_SIM808_HTTP::initHttpService(const char* apn)
{
    if(!sim808_check_with_cmd("AT+CGATT?\r\n", "OK\r\n", CMD))
        return false;

    if(!sim808_check_with_cmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n", "OK\r\n", CMD))
        return false;

    sim808_send_cmd("AT+SAPBR=3,1,\"APN\",\"");
    sim808_send_cmd(apn);

    if(!sim808_check_with_cmd("\"\r\n", "OK\r\n", CMD))
        return false;

    if(!sim808_check_with_cmd("AT+SAPBR=1,1\r\n", "OK\r\n", CMD))
        return false;

    if(!sim808_check_with_cmd("AT+HTTPINIT\r\n", "OK\r\n", CMD))
        return false;

    return true;
}

bool DFRobot_SIM808_HTTP::addParameter(const char *name, const char *val)
{
    sim808_send_cmd("AT+HTTPPARA=\"");
    sim808_send_cmd(name);
    sim808_send_cmd("\",\"");
    sim808_send_cmd(val);

    return sim808_check_with_cmd("\"\r\n", "OK\r\n", CMD);
}

bool DFRobot_SIM808_HTTP::addData(const char *data)
{
    sim808_send_cmd("AT+HTTPDATA=");

    char dataSize[6];
    itoa(strlen(data), dataSize, 10);

    sim808_send_cmd(dataSize);

    if (!sim808_check_with_cmd(",30000\r\n", "DOWNLOAD\r\n", CMD))
        return false;

    return sim808_check_with_cmd(data, "OK\r\n", CMD);
}

int DFRobot_SIM808_HTTP::sendRequest(const HttpAction& action)
{
    sim808_flush_serial();
    sim808_send_cmd("AT+HTTPACTION=");

    char httpAction[2];
    httpAction[0] = static_cast<char>(action);
    httpAction[1] = 0;

    sim808_send_cmd(httpAction);
    sim808_send_cmd("\r\n");

    char buf[64];
    sim808_clean_buffer(buf, sizeof(buf));
    sim808_read_buffer(buf, sizeof(buf));

    // Echo response.
    char* echo = strtok(buf, "\r\n");

    // Result
    char* result = strtok(nullptr, "\r\n");

    if (strstr(result, "ERROR"))
        return -1;

    // Empty line
    char* data = strtok(nullptr, "\r\n");

    if (strstr(data, desHTTPACTION))
    {
        char* resultCode = strtok(data + 15, ",");

        if (resultCode)
            return atoi(resultCode);
    }

    return -1;
}

bool DFRobot_SIM808_HTTP::readData(char* response)
{
    sim808_send_cmd("AT+HTTPREAD\r\n");

    char buf[256];
    sim808_clean_buffer(buf, sizeof(buf));
    sim808_read_buffer(buf, sizeof(buf));

    // Echo line.
    char* firstLine = strtok(buf, "\r\n");

    // Meta-data.
    char* metaData = strtok(nullptr, "\r\n");

    if (strstr(metaData, desHTTPREAD))
    {
        // Num bytes read.
        const auto nrBytesRead = atoi(metaData + 11);

        Serial.print("Num bytes read: ");
        Serial.println(nrBytesRead);

        if (nrBytesRead >= 0)
        {
            // Data
            char* data = strtok(NULL, "\r\nOK");

            strncpy(response, data, nrBytesRead);
            response[nrBytesRead] = 0;

            return true;
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
