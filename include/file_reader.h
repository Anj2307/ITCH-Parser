#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>
#include <winsock2.h>

class FileReader {
    public:
        FileReader(const char* filename);
        ~FileReader();
        bool next_message(uint8_t* buf, uint16_t& length);
        bool is_open() const;

    private:
        FILE* file_;
};