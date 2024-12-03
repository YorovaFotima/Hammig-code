#ifndef _HAMMING_H_
#define _HAMMING_H_

#include <vector>

namespace hamming {

enum Result {
    kWrongInput = 0,
    kNoError,
    kErrorInP6,
    kHasOneError,
    kHasTwoError
};

std::vector<uint8_t> encode(std::string input);

Result check(std::string& fetched_data, uint8_t& error_pos);

void print(const std::vector<uint8_t>& encoded);

}

#endif // _HAMMING_H_