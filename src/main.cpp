#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include "file_reader.h"
#include "decoder.h"
#include "order_book.h"
#include "indicators.h"
#include "trends.h"
#include "signals.h"
int main() {
    FileReader reader("data/01302019.NASDAQ_ITCH50");
    
    if (!reader.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    Decoder decoder;
    OrderBook book;
    Indicators ind;
    TrendResult trend;
    Trends trends;
    SignalResult sig;
    Signals signals;
    book.set_symbol("AAPL    ");

    uint8_t buf[64];
    uint16_t length;
    
    int msg_count = 0;
    auto start =std:: chrono:: high_resolution_clock::now();
    FILE* csv = fopen("data/aapl_analysis.csv", "w");
    fprintf(csv, "timestamp_sec,best_bid,best_ask,spread,mid_price,vwap,volume,book_imbalance\n");


    FILE* ohlcv_csv = fopen("data/appl_ohclv.csv","w");
    fprintf(ohlcv_csv, "time,open,high,low,close,volume\n");
    uint64_t last_csv_time = 0;
    uint64_t current_timestamp = 0;
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
        }
        
        current_timestamp = book.last_timestamp();
        if(book.best_bid() > 0 && book.best_ask() > 0){
        if (current_timestamp > 0 && current_timestamp - last_csv_time >= 1000000000ULL) {
            uint64_t seconds = current_timestamp / 1000000000ULL;
            uint64_t hours   = seconds / 3600;
            uint64_t minutes = (seconds % 3600) / 60;
            uint64_t secs    = seconds % 60;

            fprintf(csv, "%02llu:%02llu:%02llu,%.4f,%.4f,%.4f,%.4f,%.4f,%u,%.4f\n",
                hours, minutes, secs,
                book.best_bid() / 10000.0,
                book.best_ask() / 10000.0,
                book.spread(),
                book.mid_price(),
                book.vwap(),
                book.volume(),
                book.book_imbalance()
            );
            last_csv_time = current_timestamp;
        }
    }
        if (book.bar_complete(book.last_timestamp())) {
            OHLCVBar bar = book.get_bar();
            if(bar.initialized){
            uint64_t seconds = bar.timestamp / 1000000000ULL;
            ind.rsi(bar.close / 10000.0);
            ind.ema(20,bar.close / 10000.0);
            ind.macd(bar.close / 10000.0);
            ind.bb(bar.close / 10000.0);
            ind.atr(bar.high/ 10000.0, bar.close/ 10000.0, bar.low/ 10000.0);
            ind.obv(bar.close/10000.0,bar.volume);
            
            trend=trends.calculate(ind,book.mid_price());
            sig=signals.calculate(trend,book.mid_price());
            fprintf(ohlcv_csv, "%02llu:%02llu,%.4f,%.4f,%.4f,%.4f,%llu\n",
                seconds / 3600, (seconds % 3600) / 60,
                bar.open / 10000.0,
                bar.high / 10000.0,
                bar.low / 10000.0,
                bar.close / 10000.0,
                (unsigned long long)bar.volume
            );
        }
            book.reset_bar(book.last_timestamp());
}

        msg_count++;
        if (msg_count % 10000000 == 0) {
            std::cout << "Messages: " << msg_count
                      << " Best Bid: " << book.best_bid() / 10000.0
                      << " Best Ask: " << book.best_ask() / 10000.0
                      << " volume: " << book.volume()
                      << " spread: " << book.spread()
                      << " mid_price: " << book.mid_price()
                      <<" book_imbalance: " << book.book_imbalance()
                      << " rsi: " << ind.get_rsi()
                      <<" ema: " <<ind.get_ema()
                      <<" atr: " << ind.get_atr()
                      <<" obv: " << ind.get_obv()
                      <<" trend_score: "<< trend.score

                      << std::endl;
            std::cout << " raw timestamp: " << book.last_timestamp() << std::endl;
            ind.get_macd();
            ind.get_bb();

            if (sig.signal == Signal::BUY)
                std::cout << "SIGNAL: BUY  at " << book.mid_price() << "\n";
            else if (sig.signal == Signal::SELL)
                std::cout << "SIGNAL: SELL at " << book.mid_price() << "\n";
            else if (sig.signal == Signal::EXIT)
                std::cout << "SIGNAL: EXIT at " << book.mid_price() << "\n";
         }
    }
    fclose(csv);
    

    auto end = std:: chrono:: high_resolution_clock:: now();

    std::cout << "Total messages: " << msg_count << std::endl;
    std::cout << "Final Best Bid: " << book.best_bid() / 10000.0 << std::endl;
    std::cout << "Final Best Ask: " << book.best_ask() / 10000.0 << std::endl;
    auto duration=std:: chrono:: duration_cast<std:: chrono::nanoseconds>(end-start);
    std:: cout<< duration.count() << " ns"<<std:: endl;
    std::cout << " VWAP: " << book.vwap();
    return 0;
}