#include <algorithm>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "entry.h"
#include "io_utils.h"
#include "rng_utils.h"
#include "utils.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
namespace fs = std::experimental::filesystem;
constexpr uint32_t BLOCK_SIZE = 0x01000000;

void split(int step, int stage) {
    auto oldDir = fs::path("workspace");
    oldDir /= fs::path(std::to_string(step));
    oldDir /= fs::path("stage" + std::to_string(stage - 1));
    fs::directory_iterator iter(oldDir), end;
    std::vector<fs::path> paths;
    while (iter != end) {
        paths.push_back(iter->path());
        ++iter;
    }
    std::sort(paths.begin(), paths.end());

    auto newDir = fs::path("workspace");
    newDir /= fs::path(std::to_string(step));
    newDir /= fs::path("stage" + std::to_string(stage));
    fs::create_directories(newDir);

    const uint32_t AND = ((1 << (4 * stage)) - 1);
    const uint32_t SHIFT = 4 * stage - 4;
    cout << std::hex << AND << ", " << SHIFT << endl;

    for (auto path : paths) {
        std::ifstream ifs;
        auto fname = path.string();
        ifs.open(fname, ifstream::in | ifstream::binary);
        cout << "spliting: " << fname << endl;

        if (!ifs) {
            cout << "error" << endl;
            return;
        }

        string old = stage == 1 ? "" : path.stem();
        array<std::ofstream, 16> ofs;
        for (int i = 0; i < 16; i++) {
            auto file = fs::path(toHex(i, 0) + ".bin");
            auto fname =
                (newDir / fs::path(toHex(i, 0) + old + ".bin")).string();
            ofs[i].open(fname, ofstream::out | ofstream::binary);
        }
        uint64_t count = 0;

        while (1) {
            if (ifs.eof()) break;
            auto entry = Entry::load(ifs);
            auto value = entry.value;

            auto last = (value & AND) >> SHIFT;
            assert(0 <= last && last <= 15);
            entry.save(ofs[last]);
            count++;
            if (count % BLOCK_SIZE == 0) {
                cout << "spliting: " << count << endl;
            }
        }
    }
}

int main(int argc, char const* argv[]) {
    if (argc < 3) {
        std::cerr << "error" << std::endl;
        std::cerr << "usage: ./split_db STEP STAGE" << std::endl;
        return 1;
    }
    int step = std::stoi(argv[1]);
    int stage = std::stoi(argv[2]);

    split(step, stage);

    cout << "finish: stage = " << stage << endl;
    return 0;
}
