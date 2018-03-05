#include <algorithm>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

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

    auto newDir = fs::path("workspace");
    newDir /= fs::path(std::to_string(step));
    newDir /= fs::path("stage" + std::to_string(stage));
    fs::create_directories(newDir);

    fs::directory_iterator iter(oldDir), end;
    const uint32_t AND = ((1 << (4 * stage)) - 1);
    const uint32_t SHIFT = 4 * stage - 4;
    cout << std::hex << AND << ", " << SHIFT << endl;

    while (iter != end) {
        std::ifstream ifs;
        auto fname = iter->path().string();
        ifs.open(fname, ifstream::in | ifstream::binary);
        cout << "spliting: " << fname << endl;

        if (!ifs) {
            cout << "error" << endl;
            return;
        }

        string old = stage == 1 ? "" : iter->path().stem();
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
            uint64_t e;
            ifs.read((char*)&e, sizeof(e));
            uint32_t value = e & 0xFFFFFFFF;

            auto last = (value & AND) >> SHIFT;
            assert(0 <= last && last <= 15);
            ofs[last].write((char*)&e, sizeof(e));
            count++;
            if (count % BLOCK_SIZE == 0) {
                cout << "spliting: " << count << endl;
            }
        }
        ++iter;
    }
}

int main() {
    int step;
    cout << "step > ";
    cin >> step;

    int stage;
    cout << "stage > ";
    cin >> stage;

    split(step, stage);

    return 0;
}
