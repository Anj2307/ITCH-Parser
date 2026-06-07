#include "decoder.h"
#include <vector>
#include "message_types.h"

char Decoder:: get_message_type(const std:: vector<uint8_t> & buf){
    return static_cast<char>(buf[0]);
}

AddOrderMsg Decoder:: decode_add_order(const std:: vector<uint8_t>&buf){
    uint16_t stock_locate;
    memcpy(&stock_locate, buf.data() + 1, 2);
    stock_locate = ntohs(stock_locate);

    uint16_t tracking_number;
    memcpy(&tracking_number, buf.data() + 3, 2);
    tracking_number = ntohs(tracking_number);

    uint64_t timestamp;
    memcpy(&timestamp, buf.data() + 5, 6);
    timestamp = ntohll(timestamp);
    
    uint64_t order_reference_number;
    memcpy(&order_reference_number, buf.data() + 11, 8);
    order_reference_number = ntohll(order_reference_number);

    char side=static_cast<char>(buf[19]);

    uint32_t shares;
    memcpy(&shares, buf.data()+20,4);
    shares= ntohl(shares);

    char stock[8];
    memcpy(stock, buf.data() + 24, 8);

    uint32_t price;
    memcpy(&price,buf.data()+32,4);
    price=ntohl(price);
    
    AddOrderMsg msg;
    msg.stock_locate = stock_locate;
    msg.tracking_number = tracking_number;
    msg.timestamp = timestamp;
    msg.order_reference_number = order_reference_number;
    msg.side = side;
    msg.shares = shares;
    memcpy(msg.stock, stock, 8);
    msg.price = price;
    return msg;
}