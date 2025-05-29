#include "FsWiFi.hpp"

bool FsWifi::Begin()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFi.disconnect(true, true);
        delay(500);
    }
    WiFi.begin();

    int retry = 5;
    while (!isConnected())
    {
        delay(500);
        retry--;
        if (retry <= 0)
        {
            return false;
        }
    }
    if (isConnected())
    {
        GetNtp();
    }
    return isConnected();
};
bool FsWifi::Begin(const char *ssid, const char *password)
{
    if ((strlen(ssid) == 0) || (strlen(password) == 0))
    {
        return false;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFi.disconnect(true, true);
        delay(500);
    }

    WiFi.begin(ssid, password);

    int retry = 20;
    while (!isConnected())
    {
        delay(500);
        retry--;
        if (retry <= 0)
        {
            return false;
        }
    }
    if (isConnected())
    {
        GetNtp();
    }
    return isConnected();
};
bool FsWifi::GetNtp()
{
    if (!isConnected())
    {
        return false;
    }
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2, ntpServer3);
    int retry = 20;
    bool ret = true;
    while (!getLocalTime(&timeinfo))
    {
        delay(500);
        if (retry-- <= 0)
        {
            bool ret = false;
            break;
        }
    }
    return ret;
}
