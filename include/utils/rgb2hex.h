#pragma once
#include <string>

struct RGB
{
    int r, g, b;
};

std::string rgb2hex(int r, int g, int b);
std::string rgb2hex(const RGB &rgb);