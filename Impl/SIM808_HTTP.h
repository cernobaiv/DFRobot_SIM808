#pragma once

enum class HttpAction
{
    GET  = 48,  // '0'
    POST = 49   // '1'
};

class DFRobot_SIM808_HTTP
{
public:
    bool initHttpService(const char* apn);

    bool addParameter(const char* name, const char* val);
    bool addData(const char* data);

    int sendRequest(const HttpAction& action);

    bool readData(char* response);

    bool stopHttpService();
};
