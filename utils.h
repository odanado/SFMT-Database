#pragma once

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using std::string;

void createDir(const string &);

string getFilename(const string &, int);

template <typename T>
string toHex(T num, uint32_t radix) {
    std::stringstream ss;
    ss << std::hex << std::setw(radix) << std::setfill('0') << num;
    return ss.str();
}
