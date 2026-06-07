#pragma once
#include <cstdint>
#include <vector>
#include "message.types.h"

class Decoder{
    public:
        char get_message_type(const std:: vector<uint8_t>& buf);
        AddOrderMsg decode_add_order(const std:: vector<uint8_t>&buf);
};