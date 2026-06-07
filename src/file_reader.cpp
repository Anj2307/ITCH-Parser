#include "file_reader.h"

FileReader:: FileReader(const char* filename){
    file_ = std:: fopen(filename, "rb");
}
FileReader:: ~FileReader(){
    if(file_){
        std:: fclose(file_);
    }
}

bool FileReader:: is_open() const{
    if(file_==nullptr) return false;
    else return true;
}

bool FileReader:: next_message(std:: vector<uint8_t>&buf){
    if(!is_open()) return false;
    uint16_t raw_length;
    size_t n =fread(&raw_length,2,1,file_);
    if(n !=1) return false;
    uint16_t length;
    length=ntohs(raw_length);

    buf.resize(length);

    n= fread(buf.data(),1,length,file_);
    if (n!= length) return false;

    return true;

    
}
