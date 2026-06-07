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

    int add_orders    = 0;
    int executed      = 0;
    int cancelled     = 0;
    int deleted       = 0;
    int replaced      = 0;
    int other         = 0;
    int order= 0;
    int debug_count=0;
    while (reader.next_message(buf)) {

        char type = decoder.get_message_type(buf);

        if (debug_count < 10) {
        std::cout << "buf[0] = " << (int)buf[0] 
                  << " type = " << type 
                  << " buf size = " << buf.size() << std::endl;
        debug_count++;
    }
        switch(type) {
            case 'A': decoder.decode_add_order(buf);      add_orders++; break;
            case 'F': decoder.decode_add_order(buf);      add_orders++; break;
            case 'E': decoder.decode_order_executed(buf); executed++;   break;
            case 'X': decoder.decode_order_cancel(buf);   cancelled++;  break;
            case 'D': decoder.decode_order_delete(buf);   deleted++;    break;
            case 'U': decoder.decode_order_replace(buf);  replaced++;   break;
            default:  other++; break;
        }
        // order++;
        // std::cout<<"order no. "<<order<<std::endl;
        //  std::cout << "Add Orders:  " << add_orders << std::endl;
        // std::cout << "Executed:    " << executed   << std::endl;
        // std::cout << "Cancelled:   " << cancelled  << std::endl;
        // std::cout << "Deleted:     " << deleted    << std::endl;
        // std::cout << "Replaced:    " << replaced   << std::endl;
        // std::cout << "Other:       " << other      << std::endl;
    }

    std::cout << "Add Orders:  " << add_orders << std::endl;
    std::cout << "Executed:    " << executed   << std::endl;
    std::cout << "Cancelled:   " << cancelled  << std::endl;
    std::cout << "Deleted:     " << deleted    << std::endl;
    std::cout << "Replaced:    " << replaced   << std::endl;
    std::cout << "Other:       " << other      << std::endl;

    return 0;
}