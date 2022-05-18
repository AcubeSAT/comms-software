#include "bch_decoder.hpp"
#include "cstring"

bch_decoder::hash bch_decoder::syndromes_hash = {
        {98,  0},
        {49,  1},
        {122, 2},
        {61,  3},
        {124, 4},
        {62,  5},
        {31,  6},
        {109, 7},
        {84,  8},
        {42,  9},
        {21,  10},
        {104, 11},
        {52,  12},
        {26,  13},
        {13,  14},
        {100, 15},
        {50,  16},
        {25,  17},
        {110, 18},
        {55,  19},
        {121, 20},
        {94,  21},
        {47,  22},
        {117, 23},
        {88,  24},
        {44,  25},
        {22,  26},
        {11,  27},
        {103, 28},
        {81,  29},
        {74,  30},
        {37,  31},
        {112, 32},
        {56,  33},
        {28,  34},
        {14,  35},
        {7,   36},
        {97,  37},
        {82,  38},
        {41,  39},
        {118, 40},
        {59,  41},
        {127, 42},
        {93,  43},
        {76,  44},
        {38,  45},
        {19,  46},
        {107, 47},
        {87,  48},
        {73,  49},
        {70,  50},
        {35,  51},
        {115, 52},
        {91,  53},
        {79,  54},
        {69,  55},
        {64,  56},
        {32,  57},
        {16,  58},
        {8,   59},
        {4,   60},
        {2,   61},
        {1,   62}

};


void bch_decoder::decode(uint8_t *codeword) {

    static int primitive_elements_bch_63_54[64] = {1, 2, 4, 8, 16, 32, 64, 69, 79, 91, 115, 35, 70, 73, 87, 107,
                                                   19, 38, 76, 93, 127, 59, 118, 41, 82, 97, 7, 14, 28, 56, 112,
                                                   37, 74, 81, 103, 11, 22, 44, 88, 117, 47, 94, 121, 55, 110,
                                                   25, 50, 100, 13, 26, 52, 104, 21, 42, 84, 109, 31, 62, 124,
                                                   61, 122, 49, 98, 1};
    static const uint8_t generator = 197;
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
    } else if ((syndromes_hash.find(syndrome) == syndromes_hash.end())) {

    } else {
        memcpy(decoded_word, codeword, sizeof(uint8_t) * k_bch);
        decoded_word[syndromes_hash.at(syndrome)] ^= 1;
    }

    return;
}





