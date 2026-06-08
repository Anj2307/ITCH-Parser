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

        uint32_t best_bid() const;
        uint32_t best_ask() const;
    
    private:
        std:: unordered_map<uint64_t,Order> orders_;
        std:: map<uint32_t, PriceLevel, std:: greater<uint32_t>> bids_;
        std:: map<uint32_t, PriceLevel> asks_;
};
