#include <algorithm>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

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

    std::vector<uint64_t> entries;
    while (1) {
        if (ifs.eof()) break;
        uint64_t e;
        ifs.read((char *)&e, sizeof(e));
        entries.push_back(e);
    }

    std::sort(
        entries.begin(), entries.end(),
        [](const auto &e1, const auto &e2) { return (e1 & 0xFFFFFFFF) < (e2 & 0xFFFFFFFF); });

    for (auto e : entries) {
        uint32_t seed = e >> 32;
        // uint32_t value = e & 0xFFFFFFFF;
        // cout << std::hex << seed << ", " << value << endl;
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

int main() {
    int step;
    cout << "step > ";
    cin >> step;

    int stage;
    cout << "stage > ";
    cin >> stage;
    sort(step, stage);
    cout << "finish: stage = " << stage << endl;
}
