#include <iostream>
#include "utils/rgb2hex.h"

std::string rgb2hex(int r, int g, int b)
{
    char hexcol[8];
    snprintf(hexcol, sizeof(hexcol), "#%02X%02X%02X", r, g, b);
    return std::string(hexcol);
}

std::string rgb2hex(const RGB &color) 
{ 
    return rgb2hex(color.r, color.g, color.b); 
}