#include "FsUtils.hpp"

int FsUtils::split(String data, char delimiter, String *dst)
{
    int index = 0;
    int datalength = data.length();

    for (int i = 0; i < datalength; i++)
    {
        char tmp = data.charAt(i);
        if (tmp == delimiter)
        {
            index++;
        }
        else
            dst[index] += tmp;
    }

    return (index + 1);
}