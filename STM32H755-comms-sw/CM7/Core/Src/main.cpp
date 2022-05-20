#include "main.h"
#include "bch_decoder.hpp"

extern "C" uint32_t main_cpp() {

    uint8_t bitstream[64];
    for (int i = 0; i < 64; i++) {
        if (i % 2 == 0)
            bitstream[i] = 0;
        else
            bitstream[i] = 1;
    }

    bch_decoder bch_dec;
    uint32_t before_millis = HAL_GetTick();
    for (int i = 0; i < 100; i++) {
        bch_dec.decode(bitstream);
    }
    uint32_t decoding_time = HAL_GetTick() - before_millis;

    return decoding_time;
};
