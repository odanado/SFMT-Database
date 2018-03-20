#include <algorithm>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "entry.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
namespace fs = std::experimental::filesystem;

constexpr int COMP_SEQ_LEN = 16;

void one_file(int step, const string &oldFileName, const string &newFileName) {
    std::ifstream ifs;
    ifs.open(oldFileName, ifstream::in | ifstream::binary);

    std::ofstream ofs;
    ofs.open(newFileName, ofstream::out | ofstream::binary);

    std::vector<Entry> entries;
    while (1) {
        if (ifs.eof()) break;
        entries.push_back(Entry::load(ifs));
    }

    std::sort(
        entries.begin(), entries.end(),
        [](const auto &e1, const auto &e2) { return (e1.value) < (e2.value); });

    for (auto e : entries) {
        uint32_t seed = e.seed;
        ofs.write((char *)&seed, sizeof(seed));
    }
}

void sort(int step, int stage) {
    auto oldDir = fs::path("workspace");
    oldDir /= fs::path(std::to_string(step));
    oldDir /= fs::path("stage" + std::to_string(stage));
    fs::directory_iterator iter(oldDir), end;
    std::vector<fs::path> paths;
    while (iter != end) {
        paths.push_back(iter->path());
        ++iter;
    }
    std::sort(paths.begin(), paths.end());

    auto newDir = fs::path("dist");
    newDir /= fs::path(std::to_string(step));
    fs::create_directories(newDir);

    const std::size_t BLOCK_SIZE = 0x100;
    for (std::size_t i = 0; i < paths.size(); i += BLOCK_SIZE) {
        cout << std::hex << "progress: " << i << " / " << paths.size() << endl;
#pragma omp parallel for
        for (std::size_t j = 0; j < BLOCK_SIZE; j++) {
            if (i + j >= paths.size()) continue;
            auto path = paths[i + j];
            auto fname = path.string();

            auto newFileName =
                (newDir / fs::path(path.stem().string() + ".bin")).string();

            one_file(step, fname, newFileName);
        }
    }
}

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        std::cerr << "error" << std::endl;
        std::cerr << "usage: ./sort_db STEP STAGE" << std::endl;
        return 1;
    }
    int step = std::stoi(argv[1]);
    int stage = std::stoi(argv[2]);
    sort(step, stage);
    cout << "finish: stage = " << stage << endl;
}
