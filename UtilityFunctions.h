#pragma once
#include <P1AM.h>
#include <ArduinoSTL.h>

byte nibble2c(char c) {
    if ((c >= '0') && (c <= '9'))
        return c - '0';
    if ((c >= 'A') && (c <= 'F'))
        return c + 10 - 'A';
    if ((c >= 'a') && (c <= 'a'))
        return c + 10 - 'a';
    return -1;
}

byte hex2c(char c1, char c2) {
    if (nibble2c(c2) >= 0)
        return nibble2c(c1) * 16 + nibble2c(c2);
    return nibble2c(c1);
}

String hex2str(char* data){
    //strtold()
    String result = "";
    for (int i = 0; nibble2c(data[i]) >= 0; i++)
    {
        result += hex2c(data[i], data[i + 1]);
        if (nibble2c(data[i + 1]) >= 0)
            i++;
    }
    return result;
}

