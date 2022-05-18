//
// Created by konkapo on 18/5/2022.
//

#ifndef STM32H755_BCH_DECODER_HPP
#define STM32H755_BCH_DECODER_HPP


#include <unordered_map>
#include "stdint.h"
#include "math.h"


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
    uint8_t decoded_word[56];
    typedef std::unordered_map<int, int> hash;
    static hash syndromes_hash;


public:
    /**
    * Initializes an object of the class and sets the output to be a multiple of k_bch.
     */

    bch_decoder() {

    };

    /**
    * Destroys the object of the class.
    */

    ~bch_decoder();

    void decode(uint8_t *codeword);

};


#endif //STM32H755_BCH_DECODER_HPP
