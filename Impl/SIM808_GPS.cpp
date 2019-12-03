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
    char buf[156];
    getCGNSINF(buf, sizeof(buf));

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
            if (strstr(data, desCGNSINF))
            {
                // Check that the GPS run status and Fix status are both 1.
                if (data[10] == '1' && data[12] == '1')
                {
                    sim808_clean_buffer(reinterpret_cast<char *>(&this->GNSSData), sizeof(GNSSData));

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

int DFRobot_SIM808_GPS::getCGNSINF(char *rawData, size_t sz)
{
    sim808_send_cmd("AT+CGNSINF\r\n");
    return sim808_recv(rawData, sz);
}
