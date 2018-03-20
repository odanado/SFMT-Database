#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "entry.h"
#include "io_utils.h"
#include "rng_utils.h"
#include "utils.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
namespace fs = std::experimental::filesystem;

constexpr uint64_t MAX = 0x100000000;
constexpr uint32_t BLOCK_SIZE = 0x1000000;

void create(int step) {
    auto dir = fs::path("workspace");
    dir /= fs::path(std::to_string(step));
    dir /= fs::path("stage0");
    fs::create_directories(dir);

    auto file = dir / fs::path("0.bin");
    auto fname = string(file);
    std::ofstream ofs;
    ofs.open(fname, std::ofstream::out | std::ofstream::binary);

    vector<Entry> entries(BLOCK_SIZE);
    for (uint64_t count = 0; count < MAX; count += BLOCK_SIZE) {
        if (count % 0x100000 == 0) cout << toHex(count, 8) << endl;
#pragma omp parallel for
        for (uint64_t i = 0; i < BLOCK_SIZE; i++) {
            uint32_t seed = count + i;
            uint32_t value = calcNeedle(seed, step);
            entries[i] = Entry(seed, value);
        }
        for (uint32_t i = 0; i < BLOCK_SIZE; i++) {
            entries[i].save(ofs);
        }
    }
}

int main(int argc, char const* argv[]) {
    if (argc < 2) {
        std::cerr << "error" << std::endl;
        std::cerr << "usage: ./create_db STEP" << std::endl;
        return 1;
    }
    int step = std::stoi(argv[1]);
    create(step);

    return 0;
}
