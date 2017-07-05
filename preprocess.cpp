#include <sys/stat.h>
#include <algorithm>
#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "io_utils.h"
#include "rng_utils.h"
#include "utils.h"

#ifdef _MSC_VER
#define __attribute__(A)
#include "./parallel-radix-sort/parallel_radix_sort.h"
#else
#include "./parallel-radix-sort/parallel_radix_sort.h"
#endif

using std::array;
using std::cin;
using std::cout;
using std::endl;
using std::vector;

constexpr uint64_t MAX = 0x100000000;
constexpr uint32_t BLOCK_SIZE = 0x01000000;
constexpr uint32_t SPLIT_SIZE = 100;

void create(int step) {
    string dirName = "bin/" + std::to_string(step);
    createDir("bin");
    createDir(dirName);

    array<FILE*, SPLIT_SIZE> fps;
    for (uint32_t i = 0; i < SPLIT_SIZE; i++) {
        auto fname = getFilename(dirName, i);
        fps[i] = fopen(fname.c_str(), "wb");
    }

    vector<uint64_t> entries;
    entries.resize(BLOCK_SIZE);
    for (uint64_t count = 0; count < MAX; count += BLOCK_SIZE) {
        if (count % 0x100000 == 0) cout << toHex(count, 8) << endl;
#pragma omp parallel for
        for (uint32_t i = 0; i < BLOCK_SIZE; i++) {
            auto seed = count + i;
            entries[i] = (seed << 32) | calcNeedle(seed, step);
        }
        for (uint32_t i = 0; i < BLOCK_SIZE; i++) {
            uint32_t last = (entries[i] & 0xFFFFFFFF) % SPLIT_SIZE;
            fwrite(&entries[i], sizeof(uint64_t), 1, fps[last]);
        }
    }
}

void sort(int step) {
    string dirName = "bin/" + std::to_string(step);

    for (uint32_t i = 0; i < SPLIT_SIZE; i++) {
        cout << "i = " << i << endl;
        auto fname = getFilename(dirName, i);
        auto entries = fetchEntries<uint64_t>(fname);
        auto sz = entries.size();

        vector<uint32_t> results(sz);
        vector<uint32_t> seeds(sz);
        for (uint32_t j = 0; j < sz; j++) {
            seeds[j] = entries[j] >> 32;
            results[j] = entries[j] & 0xFFFFFFFF;
        }
        parallel_radix_sort::PairSort<uint32_t, uint32_t> pair_sort;
        pair_sort.Init(sz);
        auto sorted = pair_sort.Sort(results.data(), seeds.data(), sz);
        for (uint32_t j = 0; j < sz; j++) {
            seeds[j] = sorted.second[j];
        }
        writeEntries<uint32_t>(fname, seeds);
    }
}
int main() {
    cout << "step > ";
    int step;
    cin >> step;
    cout << "creating..." << endl;
    create(step);
    cout << "done!" << endl;
    cout << "sorting..." << endl;
    sort(step);
    cout << "done!" << endl;
}
