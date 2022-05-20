#ifndef STM32H755_BCH_DECODER_HPP
#define STM32H755_BCH_DECODER_HPP


#include <unordered_map>
#include "stdint.h"
#include "etl/unordered_map.h"

class bch_decoder {

private:
    /**
     * The input number of bits to the decoder.
     */
    static const uint8_t n_bch = 64;

    /**
    * The output number of bits of the decoder.
    */
    static const uint8_t k_bch = 56;

    /**
    * The hash map that assigns the syndromes to the bit error location.
    */

    etl::unordered_map<int, int, 64> hash{etl::pair{98, 0},
                                          etl::pair{49, 1},
                                          etl::pair{122, 2},
                                          etl::pair{61, 3},
                                          etl::pair{124, 4},
                                          etl::pair{62, 5},
                                          etl::pair{31, 6},
                                          etl::pair{109, 7},
                                          etl::pair{84, 8},
                                          etl::pair{42, 9},
                                          etl::pair{21, 10},
                                          etl::pair{104, 11},
                                          etl::pair{52, 12},
                                          etl::pair{26, 13},
                                          etl::pair{13, 14},
                                          etl::pair{100, 15},
                                          etl::pair{50, 16},
                                          etl::pair{25, 17},
                                          etl::pair{110, 18},
                                          etl::pair{55, 19},
                                          etl::pair{121, 20},
                                          etl::pair{94, 21},
                                          etl::pair{47, 22},
                                          etl::pair{117, 23},
                                          etl::pair{88, 24},
                                          etl::pair{44, 25},
                                          etl::pair{22, 26},
                                          etl::pair{11, 27},
                                          etl::pair{103, 28},
                                          etl::pair{81, 29},
                                          etl::pair{74, 30},
                                          etl::pair{37, 31},
                                          etl::pair{112, 32},
                                          etl::pair{56, 33},
                                          etl::pair{28, 34},
                                          etl::pair{14, 35},
                                          etl::pair{7, 36},
                                          etl::pair{97, 37},
                                          etl::pair{82, 38},
                                          etl::pair{41, 39},
                                          etl::pair{118, 40},
                                          etl::pair{59, 41},
                                          etl::pair{127, 42},
                                          etl::pair{93, 43},
                                          etl::pair{76, 44},
                                          etl::pair{38, 45},
                                          etl::pair{19, 46},
                                          etl::pair{107, 47},
                                          etl::pair{87, 48},
                                          etl::pair{73, 49},
                                          etl::pair{70, 50},
                                          etl::pair{35, 51},
                                          etl::pair{115, 52},
                                          etl::pair{91, 53},
                                          etl::pair{79, 54},
                                          etl::pair{69, 55},
                                          etl::pair{64, 56},
                                          etl::pair{32, 57},
                                          etl::pair{16, 58},
                                          etl::pair{8, 59},
                                          etl::pair{4, 60},
                                          etl::pair{2, 61},
                                          etl::pair{1, 62}

    };
    /**
    * Buffer where the decoded word is stored .
    */
    uint8_t decoded_word[k_bch];

    /**
    * The generator polynomial.
    */

    static const uint8_t generator = 197;

    /**
    * The primitive elements of BCH(63,54).
    */

    etl::array<uint8_t, n_bch> primitive_elements_bch_63_54{1, 2, 4, 8, 16, 32, 64, 69, 79, 91, 115, 35, 70, 73, 87,
                                                            107,
                                                            19, 38, 76, 93, 127, 59, 118, 41, 82, 97, 7, 14, 28, 56,
                                                            112,
                                                            37, 74, 81, 103, 11, 22, 44, 88, 117, 47, 94, 121, 55, 110,
                                                            25, 50, 100, 13, 26, 52, 104, 21, 42, 84, 109, 31, 62, 124,
                                                            61, 122, 49, 98, 1};


public:
    /**
    * Initializes an object of the class and sets the output to be a multiple of k_bch.
     */

    bch_decoder() = default;;

    /**
    * Destroys the object of the class.
    */

    ~bch_decoder() = default;;

    /**
    * Decodes a 64 bit BCH codeword.
    */

    void decode(uint8_t *codeword);

};


#endif //STM32H755_BCH_DECODER_HPP
