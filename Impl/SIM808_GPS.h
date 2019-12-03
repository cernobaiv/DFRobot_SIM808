#pragma once

class DFRobot_SIM808_GPS
{
public:
    //Power ON/OFF GNSS
    bool  powerOnGNSS();
    bool  powerOffGNSS();

    bool getCGNSINF();

    // Returns the amount of bytes read
    int getCGNSINF(char* rawData, size_t sz);
	
public:
    struct GNSSData {
        char utcTime[19];
        char lat[11];
        char lon[12];
        char altitude[9];
        char speed[7];
        char course[7];
        char gnssSatellites[3];
    } GNSSData;


};

