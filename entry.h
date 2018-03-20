#pragma once

#include <cstdint>
#include <fstream>

struct Entry {
    uint32_t seed;
    uint32_t value;

    Entry(uint32_t seed = 0, uint32_t value = 0) : seed(seed), value(value) {}

    static Entry load(std::ifstream &ifs) {
        Entry entry;
        ifs.read((char *)&entry.seed, sizeof(entry.seed));
        ifs.read((char *)&entry.value, sizeof(entry.value));
        return entry;
    }

    void save(std::ofstream &ofs) const {
        ofs.write((char *)&seed, sizeof(seed));
        ofs.write((char *)&value, sizeof(value));
    }
};
