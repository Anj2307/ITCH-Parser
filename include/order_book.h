#pragma once
#include <cstdint>
#include <map>
#include <unordered_map>
#include "message_types.h"

struct PriceLevel{
    uint64_t total_shares;
    uint32_t num_orders;
};

class OrderBook {
    public:
        void add_order(const AddOrderMsg& msg);
        void execute_order(const OrderExecutedMsg& msg);
        void cancel_order(const OrderCancelMsg& msg);
        void delete_order(const OrderDeleteMsg& msg);
        void replace_order(const OrderReplaceMsg& msg);
        void execute_with_price_order(const OrderExecutedWithPriceMsg& msg);
        uint32_t best_bid() const;
        uint32_t best_ask() const;
        void bid_ask_spread(int num) const;
        int volume() const;
        void print_book(int levels) const;
        OrderBook();
        void set_symbol(const char* symbol);
        void clear();
        void volume_clear();
        double vwap() const;

        uint64_t bid_volume() const;
        uint64_t ask_volume() const;
        double spread() const;
        double mid_price() const;
        double book_imbalance() const;
    
    private:
        char symbol_[8];
        bool symbol_set_;
        std:: unordered_map<uint64_t,Order> orders_;
        std:: map<uint32_t, PriceLevel, std:: greater<uint32_t>> bids_;
        std:: map<uint32_t, PriceLevel> asks_;
        int volume_;
        uint64_t vwap_numerator_;
        uint64_t vwap_denominator_;
};


