#pragma once
#ifndef FSUTILS_H
#define FSUTILS_H
#include <Arduino.h>
// #include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class FsUtils
{
public:
    int split(String data, char delimiter, String *dst);
};

#endif
