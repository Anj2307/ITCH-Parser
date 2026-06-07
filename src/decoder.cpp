#include "decoder.h"
#include <vector>
#include <cstring>
#include <winsock2.h>
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


OrderExecutedMsg Decoder:: decode_order_executed(const std:: vector<uint8_t>&buf){
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


    uint32_t shares;
    memcpy(&shares, buf.data()+19,4);
    shares= ntohl(shares);

    uint64_t match_number;
    memcpy(&match_number,buf.data()+23,8);
    match_number=ntohll(match_number);
    
    OrderExecutedMsg msg;
    msg.stock_locate = stock_locate;
    msg.tracking_number = tracking_number;
    msg.timestamp = timestamp;
    msg.order_reference_number = order_reference_number;
    msg.shares = shares;
    msg.match_number = match_number;
    return msg;
}

OrderCancelMsg Decoder:: decode_order_cancel(const std:: vector<uint8_t>&buf){
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


    uint32_t shares;
    memcpy(&shares, buf.data()+19,4);
    shares= ntohl(shares);


    
    OrderCancelMsg msg;
    msg.stock_locate = stock_locate;
    msg.tracking_number = tracking_number;
    msg.timestamp = timestamp;
    msg.order_reference_number = order_reference_number;
    msg.shares = shares;
    return msg;  
}

OrderDeleteMsg Decoder:: decode_order_delete(const std:: vector<uint8_t>&buf){
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


    

    
    OrderDeleteMsg msg;
    msg.stock_locate = stock_locate;
    msg.tracking_number = tracking_number;
    msg.timestamp = timestamp;
    msg.order_reference_number = order_reference_number;
    return msg;
}


OrderReplaceMsg Decoder:: decode_order_replace(const std:: vector<uint8_t>&buf){
    uint16_t stock_locate;
    memcpy(&stock_locate, buf.data() + 1, 2);
    stock_locate = ntohs(stock_locate);

    uint16_t tracking_number;
    memcpy(&tracking_number, buf.data() + 3, 2);
    tracking_number = ntohs(tracking_number);

    uint64_t timestamp;
    memcpy(&timestamp, buf.data() + 5, 6);
    timestamp = ntohll(timestamp);
    
    uint64_t original_order_reference_number;
    memcpy(&original_order_reference_number, buf.data() + 11, 8);
    original_order_reference_number = ntohll(original_order_reference_number);

    uint64_t new_order_reference_number;
    memcpy(&new_order_reference_number, buf.data() + 19, 8);
    new_order_reference_number = ntohll(new_order_reference_number);



    uint32_t shares;
    memcpy(&shares, buf.data()+27,4);
    shares= ntohl(shares);

    uint32_t price;
    memcpy(&price,buf.data()+31,4);
    price= ntohl(price);

    
    OrderReplaceMsg msg;
    msg.stock_locate = stock_locate;
    msg.tracking_number = tracking_number;
    msg.timestamp = timestamp;
    msg.original_order_reference_number = original_order_reference_number;
    msg.new_order_reference_number=new_order_reference_number;
    msg.shares = shares;
    msg.price=price;
    return msg;


    
}

