#pragma once

#include <cstdint>
#include <fstream>

class Entry {
    uint32_t seed;
    uint32_t value;
    uint16_t set;

 public:
    Entry(uint32_t seed = 0, uint32_t value = 0, uint16_t set = 0)
        : seed(seed), value(value), set(set) {}

    Entry load(std::ifstream &ifs) const {
        ifs.read((char*)&seed, sizeof(seed));
        ifs.read((char*)&value, sizeof(value));
        ifs.read((char*)&set, sizeof(set));
        return Entry(seed, value, set);
    }

    void save(std::ofstream &ofs) const {
        ofs.write((char*)&seed, sizeof(seed));
        ofs.write((char*)&value, sizeof(value));
        ofs.write((char*)&set, sizeof(set));
    }
};
