#include <iostream>
#include <string>
#include <vector>
#include <experimental/filesystem>

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
    FILE* fp = fopen(fname.c_str(), "wb");

    vector<uint64_t> entries(BLOCK_SIZE);
    for (uint64_t count = 0; count < MAX; count += BLOCK_SIZE) {
        if (count % 0x100000 == 0) cout << toHex(count, 8) << endl;
#pragma omp parallel for
        for (uint64_t i = 0; i < BLOCK_SIZE; i++) {
            uint64_t seed = count + i;
            entries[i] = (seed << 32) | calcNeedle(seed, step);
        }
        for (uint32_t i = 0; i < BLOCK_SIZE; i++) {
            fwrite(&entries[i], sizeof(uint64_t), 1, fp);
        }
    }
    fclose(fp);
}

int main() {
    cout << "step > ";
    int step;
    cin >> step;
    create(step);
}
