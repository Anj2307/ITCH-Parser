#include <iostream>
#include <vector>
#include "file_reader.h"
#include "decoder.h"

int main() {
    FileReader reader("data/01302019.NASDAQ_ITCH50");
    
    if (!reader.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    Decoder decoder;
    std::vector<uint8_t> buf;
    int add_order_count = 0;

    while (reader.next_message(buf)) {
        char type = decoder.get_message_type(buf);
        
        if (type == 'A') {
            AddOrderMsg msg = decoder.decode_add_order(buf);
            
            if (add_order_count < 5) {
                std::cout << "Stock Locate: " << msg.stock_locate << std::endl;
                std::cout << "Side: " << msg.side << std::endl;
                std::cout << "Shares: " << msg.shares << std::endl;
                std::cout << "Price: " << msg.price / 10000.0 << std::endl;
                std::cout << "Stock: ";
                for (int i = 0; i < 8; i++) std::cout << msg.stock[i];
                std::cout << std::endl;
                std::cout << "---" << std::endl;
            }
            add_order_count++;
        }
    }

    std::cout << "Total Add Orders: " << add_order_count << std::endl;
    return 0;
}