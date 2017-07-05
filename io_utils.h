#pragma once

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::string;

size_t getFileSize(const string &fname) {
    std::ifstream ifs(fname, std::ios::binary);
    ifs.seekg(0, ifs.end);
    size_t len = ifs.tellg();
    ifs.close();
    return len;
}

template <typename T>
vector<T> fetchEntries(const string& fname) {
    size_t len = getFileSize(fname);
    std::ifstream ifs(fname, std::ios::binary);

    vector<T> entries(len / sizeof(T));
    ifs.read(reinterpret_cast<char*>(entries.data()), len);

    ifs.close();
    return entries;
}

template <typename T>
void writeEntries(const string& fname, const vector<T>& entries) {
    std::ofstream ofs(fname, std::ios::binary);
    ofs.write(reinterpret_cast<const char*>(entries.data()),
              entries.size() * sizeof(T));
    ofs.close();
}

template <typename T>
T fetch(uint32_t pos, std::ifstream& ifs) {
    T ret;
    ifs.seekg(pos * sizeof(T));
    ifs.read(reinterpret_cast<char*>(&ret), sizeof(T));
    return ret;
}
