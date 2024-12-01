#include <iostream>
#include <string>
#include <set>

namespace hamming {
namespace {
const uint8_t kBlockLength = 18;
const uint8_t kPPositions[] =  {0, 1, 3, 7, 15, 23};
const uint8_t kDPositions[] =  {2, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 16, 17, 18, 19, 20, 21, 22};
}

std::vector<uint8_t> encode(std::string input) {
    if (input.size() % kBlockLength != 0) {
        input.insert(input.end(), kBlockLength - input.size() % kBlockLength, '0'); 
    }

    std::vector<uint8_t> encoded((input.size() / kBlockLength) * 3, 0);

    size_t input_pos = 0;
    size_t encoded_pos = 0;
    size_t encoded_bit_pos = 0;
    while (input_pos < input.size()) {
        if (!std::binary_search(std::begin(kPPositions), std::end(kPPositions), 
                (encoded_pos * 8 + encoded_bit_pos) % 24)) {
            encoded[encoded_pos] |= ((input[input_pos] - '0') << encoded_bit_pos);
            ++input_pos;
        }
        encoded_pos += (encoded_bit_pos + 1) / 8;
        encoded_bit_pos = (encoded_bit_pos + 1) % 8;
    }
    auto get_bit = [&encoded](size_t triplet_index, size_t i) {
        return (encoded[triplet_index * 3 + i / 8] & (1 << (i % 8))) >> (i % 8);
    };

    auto get_d_bit = [&get_bit](size_t triplet_index, size_t d_i) {
        return get_bit(triplet_index, kDPositions[d_i]);
    };

    auto get_p_bit = [&get_bit](size_t triplet_index, size_t p_i) {
        return get_bit(triplet_index, kPPositions[p_i]);
    };

    for (size_t triplet_index = 0; triplet_index < encoded.size() / 3; triplet_index++) {
        encoded[triplet_index * 3 + kPPositions[0] / 8] |= ((1 ^ get_d_bit(triplet_index, 0)
            ^ get_d_bit(triplet_index, 1) ^ get_d_bit(triplet_index, 3) ^ get_d_bit(triplet_index, 4) 
            ^ get_d_bit(triplet_index, 6) ^ get_d_bit(triplet_index, 8) ^ get_d_bit(triplet_index, 10)
            ^ get_d_bit(triplet_index, 11) ^ get_d_bit(triplet_index, 13) ^ get_d_bit(triplet_index, 15) 
            ^ get_d_bit(triplet_index, 17)) << (kPPositions[0] % 8)); 

        encoded[triplet_index * 3 + kPPositions[1] / 8] |= ((1 ^ get_d_bit(triplet_index, 0)
            ^ get_d_bit(triplet_index, 2) ^ get_d_bit(triplet_index, 3) ^ get_d_bit(triplet_index, 5) 
            ^ get_d_bit(triplet_index, 6) ^ get_d_bit(triplet_index, 9) ^ get_d_bit(triplet_index, 10)
            ^ get_d_bit(triplet_index, 12) ^ get_d_bit(triplet_index, 13) ^ get_d_bit(triplet_index, 16) 
            ^ get_d_bit(triplet_index, 17)) << (kPPositions[1] % 8)); 

        encoded[triplet_index * 3 + kPPositions[2] / 8] |= ((1 ^ get_d_bit(triplet_index, 1)
            ^ get_d_bit(triplet_index, 2) ^ get_d_bit(triplet_index, 3) ^ get_d_bit(triplet_index, 7) 
            ^ get_d_bit(triplet_index, 8) ^ get_d_bit(triplet_index, 9) ^ get_d_bit(triplet_index, 10)
            ^ get_d_bit(triplet_index, 14) ^ get_d_bit(triplet_index, 15) ^ get_d_bit(triplet_index, 16) 
            ^ get_d_bit(triplet_index, 17)) << (kPPositions[2] % 8)); 

        encoded[triplet_index * 3 + kPPositions[3] / 8] |= ((1 ^ get_d_bit(triplet_index, 4)
            ^ get_d_bit(triplet_index, 5) ^ get_d_bit(triplet_index, 6) ^ get_d_bit(triplet_index, 7) 
            ^ get_d_bit(triplet_index, 8) ^ get_d_bit(triplet_index, 9) ^ get_d_bit(triplet_index, 10)) 
                << (kPPositions[3] % 8)); 

        encoded[triplet_index * 3 + kPPositions[4] / 8] |= ((1 ^ get_d_bit(triplet_index, 11)
            ^ get_d_bit(triplet_index, 12) ^ get_d_bit(triplet_index, 13) ^ get_d_bit(triplet_index, 14) 
            ^ get_d_bit(triplet_index, 15) ^ get_d_bit(triplet_index, 16) ^ get_d_bit(triplet_index, 17))
                << (kPPositions[4] % 8)); 

        encoded[triplet_index * 3 + kPPositions[5] / 8] |= ((1 ^ get_p_bit(triplet_index, 0)
            ^ get_p_bit(triplet_index, 1) ^ get_d_bit(triplet_index, 0) ^ get_p_bit(triplet_index, 2) 
            ^ get_d_bit(triplet_index, 1) ^ get_d_bit(triplet_index, 2) ^ get_d_bit(triplet_index, 3)
            ^ get_p_bit(triplet_index, 3) ^ get_d_bit(triplet_index, 4) ^ get_d_bit(triplet_index, 5) 
            ^ get_d_bit(triplet_index, 6) ^ get_d_bit(triplet_index, 7) ^ get_d_bit(triplet_index, 8)
            ^ get_d_bit(triplet_index, 9) ^ get_d_bit(triplet_index, 10) ^ get_p_bit(triplet_index, 4)
            ^ get_d_bit(triplet_index, 11) ^ get_d_bit(triplet_index, 12) ^ get_d_bit(triplet_index, 13)
            ^ get_d_bit(triplet_index, 14) ^ get_d_bit(triplet_index, 15) ^ get_d_bit(triplet_index, 16)
            ^ get_d_bit(triplet_index, 17)) << (kPPositions[5] % 8)); 
    }

    return encoded;
}

enum Result {
    kWrongInput = 0,
    kNoError,
    kErrorInP6,
    kHasOneError,
    kHasTwoError
};

Result check(std::string& fetched_data, uint8_t& error_pos) {
    if (fetched_data.size() != 24) {
        return kWrongInput;
    }

    for (size_t triplet_index = 0; triplet_index < fetched_data.size() / 3; triplet_index++) {
    uint8_t test_A = (fetched_data[kPPositions[0]] - '0') ^ (fetched_data[kDPositions[0]] - '0') ^ (fetched_data[kDPositions[1]] - '0')
        ^ (fetched_data[kDPositions[3]] - '0') ^ (fetched_data[kDPositions[4]] - '0') ^ (fetched_data[kDPositions[6]] - '0')
        ^ (fetched_data[kDPositions[8]] - '0') ^ (fetched_data[kDPositions[10]] - '0') ^ (fetched_data[kDPositions[11]] - '0')
        ^ (fetched_data[kDPositions[13]] - '0') ^ (fetched_data[kDPositions[15]] - '0') ^ (fetched_data[kDPositions[17]] - '0');

    uint8_t test_B = (fetched_data[kPPositions[1]] - '0') ^ (fetched_data[kDPositions[0]] - '0') ^ (fetched_data[kDPositions[2]] - '0')
        ^ (fetched_data[kDPositions[3]] - '0') ^ (fetched_data[kDPositions[6]] - '0') ^ (fetched_data[kDPositions[6]] - '0')
        ^ (fetched_data[kDPositions[9]] - '0') ^ (fetched_data[kDPositions[10]] - '0') ^ (fetched_data[kDPositions[12]] - '0')
        ^ (fetched_data[kDPositions[13]] - '0') ^ (fetched_data[kDPositions[16]] - '0') ^ (fetched_data[kDPositions[17]] - '0');

    uint8_t test_C = (fetched_data[kPPositions[2]] - '0') ^ (fetched_data[kDPositions[1]] - '0') ^ (fetched_data[kDPositions[2]] - '0')
        ^ (fetched_data[kDPositions[3]] - '0') ^ (fetched_data[kDPositions[7]] - '0') ^ (fetched_data[kDPositions[8]] - '0')
        ^ (fetched_data[kDPositions[9]] - '0') ^ (fetched_data[kDPositions[10]] - '0') ^ (fetched_data[kDPositions[14]] - '0')
        ^ (fetched_data[kDPositions[15]] - '0') ^ (fetched_data[kDPositions[16]] - '0') ^ (fetched_data[kDPositions[17]] - '0');

    uint8_t test_D = (fetched_data[kPPositions[3]] - '0') ^ (fetched_data[kDPositions[4]] - '0') ^ (fetched_data[kDPositions[5]] - '0')
        ^ (fetched_data[kDPositions[6]] - '0') ^ (fetched_data[kDPositions[7]] - '0') ^ (fetched_data[kDPositions[8]] - '0')
        ^ (fetched_data[kDPositions[9]] - '0') ^ (fetched_data[kDPositions[10]] - '0');

    uint8_t test_E = (fetched_data[kPPositions[4]] - '0') ^ (fetched_data[kDPositions[11]] - '0') ^ (fetched_data[kDPositions[12]] - '0')
        ^ (fetched_data[kDPositions[13]] - '0') ^ (fetched_data[kDPositions[14]] - '0') ^ (fetched_data[kDPositions[15]] - '0')
        ^ (fetched_data[kDPositions[16]] - '0') ^ (fetched_data[kDPositions[17]] - '0');

    uint8_t test_F = (fetched_data[kPPositions[0]] - '0') ^ (fetched_data[kPPositions[1]] - '0') ^ (fetched_data[kDPositions[0]] - '0')
        ^ (fetched_data[kPPositions[2]] - '0') ^ (fetched_data[kDPositions[1]] - '0') ^ (fetched_data[kDPositions[2]] - '0')
        ^ (fetched_data[kDPositions[3]] - '0') ^ (fetched_data[kPPositions[3]] - '0') ^ (fetched_data[kDPositions[4]] - '0')
        ^ (fetched_data[kDPositions[5]] - '0') ^ (fetched_data[kDPositions[6]] - '0') ^ (fetched_data[kDPositions[7]] - '0')
        ^ (fetched_data[kDPositions[8]] - '0') ^ (fetched_data[kDPositions[9]] - '0') ^ (fetched_data[kDPositions[10]] - '0')
        ^ (fetched_data[kPPositions[4]] - '0') ^ (fetched_data[kDPositions[11]] - '0') ^ (fetched_data[kDPositions[12]] - '0')
        ^ (fetched_data[kDPositions[13]] - '0') ^ (fetched_data[kDPositions[14]] - '0') ^ (fetched_data[kDPositions[15]] - '0')
        ^ (fetched_data[kDPositions[16]] - '0') ^ (fetched_data[kDPositions[17]] - '0') ^ (fetched_data[kPPositions[5]] - '0');

    if (test_A && test_B && test_C && test_D && test_E && test_F){
        return kNoError;
    }

    else if (test_A && test_B && test_C && test_D && test_E && !test_F){
        return kErrorInP6;
    }

    else if ((!test_A || !test_B || !test_C || !test_D || !test_E) && test_F){
        return kHasTwoError;
    }

    else if ((!test_A || !test_B || !test_C || !test_D || !test_E) && !test_F){
        error_pos = 16 * !test_E + 8 * !test_D + 4 * !test_C + 2 * !test_B + 1 * !test_A;
        fetched_data[error_pos - 1] = !(fetched_data[error_pos - 1] - '0') + '0';
        return kHasOneError;
    }
    }
    return kHasOneError;
}

void print(const std::vector<uint8_t>& encoded) {
    size_t pos = 0;
    size_t bit_pos = 0;
    while (pos < encoded.size()) {
        std::cout << ((encoded[pos] & (1 << bit_pos)) >> bit_pos);
        if ((bit_pos + 1) / 8 == 1) {
            std::cout << std::endl;
        }
        pos += (bit_pos + 1) / 8;
        bit_pos = (bit_pos + 1) % 8;
    }
    std::cout << std::endl;
}

}

int main() {
    std::string input_data;
    std::cout << "Enter input data: ";
    std::cin >> input_data;
    
    hamming::print(hamming::encode(input_data));

    std::string input;
    std::cout << "Enter data to check (enter 'exit' to quit):" << std::endl;
    while (true) {
        std::cout << "Enter input data for checking: ";
        std::cin >> input;
        
        if (input == "exit") {
            break; 
        }

        uint8_t error_pos;
        auto result = hamming::check(input, error_pos);
        switch (result) {
        case hamming::Result::kWrongInput:
            std::cout << "Input data has wrong length, should be 24" << std::endl;
            break;
        case hamming::Result::kNoError:
            std::cout << "The data are valid" << std::endl;
            break;
        case hamming::Result::kErrorInP6:
            std::cout << "The data has error in p6" << std::endl;
            break;
        case hamming::Result::kHasOneError:
            std::cout << "The data has an error in " << (int)error_pos
                      << " bit. Fixed data is: " << std::endl << input << std::endl;
            break;
        case hamming::Result::kHasTwoError:
            std::cout << "The data has two errors." << std::endl;
            break;
        }
    }

    return 0;
}



