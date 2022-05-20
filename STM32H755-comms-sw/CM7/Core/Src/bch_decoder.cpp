#include "bch_decoder.hpp"
#include "cstring"


void bch_decoder::decode(uint8_t *codeword) {

    uint8_t temp = 0;
    uint8_t syndrome = 0;
    uint8_t i = 1;
    
    while (i < n_bch) {
        for (int bytes = 0; bytes < n_bch / 8; bytes++) {
            for (int bit = 7; bit >= 0; bit--) {
                temp = codeword[n_bch - 1 - i];
                if (temp & 1 == 1) {
                    syndrome ^= primitive_elements_bch_63_54[i - 1];
                }
                i++;
            }
        }
    }
    if (syndrome > generator) {
        for (int position = 0; position < 8; position++) {
            if ((syndrome & 0x80) != 0) {
                syndrome ^= generator;
                syndrome <<= 1;
            } else {
                syndrome <<= 1;
            }
        }
    }

    if (syndrome == 0) {
        memcpy(decoded_word, codeword, sizeof(uint8_t) * k_bch);
    } else if ((hash.find(syndrome) == hash.end())) {

    } else {
        memcpy(decoded_word, codeword, sizeof(uint8_t) * k_bch);
        decoded_word[hash.at(syndrome)] ^= 1;
    }

    return;
}





