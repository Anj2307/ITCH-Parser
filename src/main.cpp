#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include "file_reader.h"
#include "decoder.h"
#include "order_book.h"

int main() {
    FileReader reader("data/01302019.NASDAQ_ITCH50");
    
    if (!reader.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    Decoder decoder;
    OrderBook book;
    book.set_symbol("AAPL    ");

    uint8_t buf[64];
    uint16_t length;
    
    int msg_count = 0;
    auto start =std:: chrono:: high_resolution_clock::now();
    while (reader.next_message(buf,length)) {
        char type = decoder.get_message_type(buf);

        switch(type) {
            case 'S': {
                SystemEventMsg msg = decoder.decode_system_event(buf);
                if(msg.event_code=='O'){
                    book.volume_clear();
                    //std:: cout << "Market Open" << std:: endl;
                }
                if (msg.event_code == 'C') {
                    book.clear();
                    //std::cout << "Market closed" << std::endl;
                }
                break;
            }
            case 'R': {
                decoder.decode_stock_directory(buf);
                break;
            }
            case 'A': {
                AddOrderMsg msg = decoder.decode_add_order(buf);
                book.add_order(msg);
                break;
            }
            case 'F': {
                AddOrderMsg msg = decoder.decode_add_order(buf);
                book.add_order(msg);
                break;
            }
            case 'E': {
                OrderExecutedMsg msg = decoder.decode_order_executed(buf);
                book.execute_order(msg);
                break;
            }
            case 'X': {
                OrderCancelMsg msg = decoder.decode_order_cancel(buf);
                book.cancel_order(msg);
                break;
            }
            case 'D': {
                OrderDeleteMsg msg = decoder.decode_order_delete(buf);
                book.delete_order(msg);
                break;
            }
            case 'U': {
                OrderReplaceMsg msg = decoder.decode_order_replace(buf);
                book.replace_order(msg);
                break;
            }
            case 'C': {
                OrderExecutedWithPriceMsg msg=decoder.decode_order_executed_with_price(buf);
                book.execute_with_price_order(msg);
                break;
            }
            default:{
                if (length >= 19) {
                    uint64_t ref = 0;
                    memcpy(&ref, buf + 11, 8);
                    ref = ntohll(ref);
                    if (ref == 10064545ULL) {
                        std::cout << "UNHANDLED type='" << type 
                                << "' mentions stale ref" << std::endl;
                    }
                }
                break;
            }
        }

        msg_count++;
        // if (msg_count % 10000000 == 0) {
        //     std::cout << "Messages: " << msg_count
        //               << " Best Bid: " << book.best_bid() / 10000.0
        //               << " Best Ask: " << book.best_ask() / 10000.0
        //               << " volume: " << book.volume()
        //               << std::endl;
        // }
    }

    auto end = std:: chrono:: high_resolution_clock:: now();

    // std::cout << "Total messages: " << msg_count << std::endl;
    // std::cout << "Final Best Bid: " << book.best_bid() / 10000.0 << std::endl;
    // std::cout << "Final Best Ask: " << book.best_ask() / 10000.0 << std::endl;
    auto duration=std:: chrono:: duration_cast<std:: chrono::nanoseconds>(end-start);
    std:: cout<< duration.count() << " ns"<<std:: endl;
    std::cout << " VWAP: " << book.vwap();
    return 0;
}