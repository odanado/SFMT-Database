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
using std::vector;
namespace fs = std::experimental::filesystem;

void sort(int step, int stage) {
    auto oldDir = fs::path("workspace");
    oldDir /= fs::path(std::to_string(step));
    oldDir /= fs::path("stage" + std::to_string(stage));
    fs::directory_iterator iter(oldDir), end;
    vector<fs::path> paths;
    while (iter != end) {
        paths.push_back(iter->path());
        ++iter;
    }
    std::sort(paths.begin(), paths.end());

    auto newDir = fs::path("dist");
    newDir /= fs::path(std::to_string(step));
    fs::create_directories(newDir);

    for (auto path : paths) {
        std::ifstream ifs;
        auto fname = path.string();
        ifs.open(fname, ifstream::in | ifstream::binary);
        cout << "sorting: " << fname << endl;

        auto newFileName =
            (newDir / fs::path(path.stem().string() + ".bin")).string();
        std::ofstream ofs;
        ofs.open(newFileName, ofstream::out | ofstream::binary);

        if (!ifs) {
            cout << "error" << endl;
            return;
        }

        vector<uint64_t> entries;
        while (1) {
            if (ifs.eof()) break;
            uint64_t e;
            ifs.read((char*)&e, sizeof(e));
            entries.push_back(e);
        }
        std::sort(entries.begin(), entries.end(), [](uint64_t e1, uint64_t e2) {
            uint32_t v1 = e1 & 0xFFFFFFFF;
            uint32_t v2 = e2 & 0xFFFFFFFF;
            return v1 < v2;
        });
        for (auto e : entries) {
            uint32_t seed = e >> 32;
            // uint32_t value = e & 0xFFFFFFFF;
            // cout << std::hex << seed << ", " << value << endl;
            ofs.write((char*)&seed, sizeof(seed));
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
