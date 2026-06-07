#pragma once
#include <cstdint>
#include <vector>
#include "message_types.h"

class Decoder{
    public:
        char get_message_type(const std:: vector<uint8_t>& buf);
        AddOrderMsg decode_add_order(const std:: vector<uint8_t>&buf);

        OrderExecutedMsg decode_order_executed(const std:: vector<uint8_t>&buf);

        OrderCancelMsg decode_order_cancel(const std:: vector<uint8_t>&buf);

        OrderDeleteMsg decode_order_delete(const std:: vector<uint8_t>&buf);

        OrderReplaceMsg decode_order_replace(const std:: vector<uint8_t>&buf);
};