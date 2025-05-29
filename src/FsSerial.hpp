#pragma once
#ifndef FSSERIAL_H
#define FSSERIAL_H

#include <Arduino.h>
// #include <M5Unified.h>

class BrySerial
{
public:
    BrySerial()
    {
    }
    ~BrySerial()
    {
    }
    bool CreateHeader(byte *head, const char *header, uint32_t size);
    void SendText(const char *str);
    void SendBin(const char *key, uint8_t *data, uint32_t size);
    bool compHeader(const char *head, const char *header);
};

#endif // FSSERIAL_H
