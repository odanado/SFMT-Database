#include "rng_utils.h"

void skipSFMT(sfmt_t* sfmt, int offset) {
    for (int i = 0; i < offset; i++) sfmt_genrand_uint64(sfmt);
}
uint32_t calcNeedle(uint32_t seed, int offset) {
    sfmt_t sfmt;
    sfmt_init_gen_rand(&sfmt, seed);
    skipSFMT(&sfmt, offset);
    array<uint64_t, 7> rand;
    for (int i = 0; i < 7; i++) {
        rand[i] = sfmt_genrand_uint64(&sfmt) % 17;
    }
    return encode(rand);
}
