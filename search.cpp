#include <array>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <tuple>

#include "io_utils.h"
#include "rng_utils.h"
#include "utils.h"

constexpr uint32_t SPLIT_SIZE = 100;
using Result = std::tuple<uint32_t, uint32_t>;

template <size_t N>
bool ok(uint32_t seed, array<size_t, N> rand, size_t len, size_t offset) {
    sfmt_t sfmt;
    sfmt_init_gen_rand(&sfmt, seed);
    skipSFMT(&sfmt, offset);
    for (size_t i = 0; i < len; i++) {
        uint64_t r = sfmt_genrand_uint64(&sfmt);
        if (r % 17 != rand[i]) return false;
    }
    return true;
}

template <size_t N>
std::vector<Result> search(std::array<uint64_t, N> rand, size_t len, int step) {
    string dirName = "bin/" + std::to_string(step);

    auto r = encode(rand);
    auto fname = getFilename(dirName, r % SPLIT_SIZE);
    std::ifstream ifs(fname, std::ios::binary);
    auto sz = getFileSize(fname) / sizeof(uint32_t);

    uint32_t pos = 0;
    for (int i = 31; i >= 0; i--) {
        auto nextPos = pos + (1 << i);
        if (nextPos < sz) {
            auto seed = fetch<uint32_t>(nextPos, ifs);
            if (calcNeedle(seed, step) <= r) {
                pos = nextPos;
            }
        }
    }
    std::vector<Result> results;
    while (1) {
        auto seed = fetch<uint32_t>(pos, ifs);
        auto res = calcNeedle(seed, step);
        if (res == r) {
            if (ok(seed, rand, len, step)) {
                results.emplace_back(seed, res);
            }
        } else {
            break;
        }
        if (pos == 0) break;
        pos--;
    }
    ifs.close();

    return results;
}

int main(int argc, char* argv[]) {
    std::cin.tie(0);
    std::ios::sync_with_stdio(false);
    bool isFuzzy = false;
    if (argc > 1 && std::string(argv[1]) == "fuzzy") {
        isFuzzy = true;
    }

    int step;
    std::cin >> step;
    int n;
    std::array<uint64_t, 16> needles, orgNeedles;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> orgNeedles[i];
    }

    for (int add = 0; add < 17; add++) {
        for (int S = 0; S < (1 << n); S++) {
            // 本当のあいまい検索は時間がかかるので使わない
            if (S != 0) continue;
            // あいまい検索じゃない場合は，定数を加算しない
            if (!isFuzzy && add != 0) continue;

            for (int i = 0; i < n; i++) {
                needles[i] = orgNeedles[i];
                if (S >> i & 1) {
                    needles[i]++;
                }
                needles[i] += add;
                needles[i] %= 17;
            }
            auto results = search(needles, n, step);
            for (auto&& x : results) {
                std::cout << toHex(std::get<0>(x), 8) << " ";
                std::cout << toHex(std::get<1>(x), 8) << " ";
                std::cout << (-add + 17) % 17 << std::endl;
            }
        }
    }
    return 0;
}
