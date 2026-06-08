#include "order_book.h"
#include <cstring>
#include <winsock2.h>

void OrderBook :: add_order(const AddOrderMsg& msg) {
    Order order;
    order.order_ref_num = msg.order_reference_number;
    order.side = msg.side;
    order.shares = msg.shares;
    order.price = msg.price;

    memcpy(order.stock,msg.stock,8);
    orders_[msg.order_reference_number]=order;

    if(msg.side=='B') {
        bids_[msg.price].total_shares+=msg.shares;
        bids_[msg.price].num_orders++;
    }
    if(msg.side=='S') {
        asks_[msg.price].total_shares+=msg.shares;
        asks_[msg.price].num_orders++;
    }
}

