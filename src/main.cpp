#include <iostream>
#include <vector>
#include "file_reader.h"

int main() {
    FileReader reader("data/01302019.NASDAQ_ITCH50");
    
    if (!reader.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    std::vector<uint8_t> buf;
    int count = 0;

    while (reader.next_message(buf)) {
        count++;
        if (count % 100000 == 0) {
            std::cout << "Messages read: " << count << std::endl;
        }
    }

    std::cout << "Total messages: " << count << std::endl;
    return 0;
}