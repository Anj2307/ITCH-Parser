#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>
#include <winsock2.h>

class FileReader {
    public:
        FileReader(const char* filename);
        ~FileReader();
        bool next_message(std:: vector<uint8_t>& buf);
        bool is_open() const;

    private:
        FILE* file_;
};