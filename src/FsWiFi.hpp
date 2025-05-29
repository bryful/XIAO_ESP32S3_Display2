#pragma once
#ifndef FSWIFI_HPP
#define FSWIFI_HPP
#include <WiFi.h>

#include <Arduino.h>

class FsWifi
{
private:
    const char *ntpServer1 = "ntp.nict.jp";
    const char *ntpServer2 = "time.google.com";
    const char *ntpServer3 = "ntp.jst.mfeed.ad.jp";
    const long gmtOffset_sec = 9 * 3600;
    const int daylightOffset_sec = 0;

public:
    struct tm timeinfo;
    FsWifi()
    {
    }
    ~FsWifi()
    {
    }
    bool isConnected()
    {
        return (WiFi.status() == WL_CONNECTED);
    }
    bool Begin();
    bool Begin(const char *ssid, const char *password);
    bool GetNtp();
    bool UpdateTime()
    {
        return getLocalTime(&timeinfo);
    }
};

#endif