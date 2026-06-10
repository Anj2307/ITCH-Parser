#pragma once
#include <cstdint>
#include <vector>
#include "message_types.h"

class Decoder{
    public:
        char get_message_type(const uint8_t* buf);
        AddOrderMsg decode_add_order(const uint8_t* buf);

        OrderExecutedMsg decode_order_executed(const uint8_t* buf);

        OrderCancelMsg decode_order_cancel(const uint8_t* buf);

        OrderDeleteMsg decode_order_delete(const uint8_t* buf);

        OrderReplaceMsg decode_order_replace(const uint8_t* buf);

        SystemEventMsg decode_system_event(const uint8_t* buf);

        StockDirectoryMsg decode_stock_directory(const uint8_t* buf);

        OrderExecutedWithPriceMsg decode_order_executed_with_price(const uint8_t* buf);

};