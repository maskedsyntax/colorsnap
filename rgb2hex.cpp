#include <iostream>

std::string rgb2hex(int r, int g, int b)
{
    char hexcol[8];
    snprintf(hexcol, sizeof(hexcol), "#%02X%02X%02X", r, g, b);
    return std::string(hexcol);
}