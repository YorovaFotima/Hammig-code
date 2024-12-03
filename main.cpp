#include <iostream>
#include <string>
#include <cstdint>
#include "hamming.h"

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



