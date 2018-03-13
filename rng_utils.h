#pragma once

#include <array>

#define SFMT_MEXP 19937
extern "C" {
#include "./SFMT/SFMT.h"
}

using std::array;

template <size_t N>
uint32_t encode(array<uint64_t, N> rand) {
    uint32_t r = 0;
    for (int i = 0; i < 7; i++) {
        r = r * 17 + (rand[i] % 17);
    }
    return r;
}

void skipSFMT(sfmt_t* sfmt, int offset);

uint32_t calcNeedle(uint32_t seed, int offset, uint16_t S=0);
