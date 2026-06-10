#include "order_book.h"
#include <cstring>
#include<iostream>
#include <winsock2.h>


OrderBook :: OrderBook() : symbol_set_(false),volume_(0),vwap_numerator_(0),vwap_denominator_(0){
    memset(symbol_,' ',8);
}

void OrderBook:: set_symbol(const char * symbol){
    memcpy(symbol_, symbol,8);
    symbol_set_=true;
}

void OrderBook :: add_order(const AddOrderMsg& msg) {
    if (symbol_set_ && memcmp(msg.stock, symbol_, 8) != 0) return;
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

void OrderBook :: delete_order(const OrderDeleteMsg& msg){
    if (orders_.find(msg.order_reference_number) == orders_.end()) return;

    Order order;
    order=orders_[msg.order_reference_number];
    orders_.erase(msg.order_reference_number);
    char side=order.side;
    if(side=='B') 
    {
        bids_[order.price].total_shares-=order.shares;
        bids_[order.price].num_orders--;
        if(bids_[order.price].num_orders==0){
            bids_.erase(order.price);
        }
    }

    if(side=='S') 
    {
        asks_[order.price].total_shares-=order.shares;
        asks_[order.price].num_orders--;
        if(asks_[order.price].num_orders==0){
            asks_.erase(order.price);
        }
    }   
}

void OrderBook:: execute_order(const OrderExecutedMsg& msg){
    
    if (orders_.find(msg.order_reference_number) == orders_.end()) return;
    Order order;
   
    order=orders_[msg.order_reference_number];
    if (symbol_set_ && memcmp(order.stock, symbol_, 8) != 0) return;
    char side=order.side;
    volume_+=msg.shares;
    vwap_numerator_ += (uint64_t)order.price * (uint64_t)msg.shares;
    vwap_denominator_ += msg.shares;
    if(side=='B'){
        if(msg.shares==order.shares)
        {
            bids_[order.price].total_shares-=order.shares;
            bids_[order.price].num_orders--;
            orders_.erase(msg.order_reference_number);
            if(bids_[order.price].num_orders==0){
            bids_.erase(order.price);
            }
        }
        else{
            bids_[order.price].total_shares-=msg.shares;
            orders_[msg.order_reference_number].shares -= msg.shares;   
        }
        
    }

    if(side=='S'){
        if(msg.shares==order.shares)
        {
            asks_[order.price].total_shares-=order.shares;
            asks_[order.price].num_orders--;
            orders_.erase(msg.order_reference_number);
            if(asks_[order.price].num_orders==0){
            asks_.erase(order.price);
        }
    }
        else{
            asks_[order.price].total_shares-=msg.shares;
            orders_[msg.order_reference_number].shares -= msg.shares;

        }
        
    }

}

void OrderBook :: cancel_order(const OrderCancelMsg& msg){
    if (orders_.find(msg.order_reference_number) == orders_.end()) return;
    Order order;
    order=orders_[msg.order_reference_number];
    char side=order.side;

    if(side=='B'){
        if(msg.shares==order.shares)
        {
            bids_[order.price].total_shares-=order.shares;
            bids_[order.price].num_orders--;
            orders_.erase(msg.order_reference_number);
            if(bids_[order.price].num_orders==0){
            bids_.erase(order.price);
            }
        }
        else{
            bids_[order.price].total_shares-=msg.shares;
            orders_[msg.order_reference_number].shares -= msg.shares;   
        }
        
    }

    if(side=='S'){
        if(msg.shares==order.shares)
        {
            asks_[order.price].total_shares-=order.shares;
            asks_[order.price].num_orders--;
            orders_.erase(msg.order_reference_number);
            if(asks_[order.price].num_orders==0){
            asks_.erase(order.price);
        }
    }
        else{
            asks_[order.price].total_shares-=msg.shares;
            orders_[msg.order_reference_number].shares -= msg.shares;

        }
        
    }

}

void OrderBook:: replace_order(const OrderReplaceMsg& msg){
    if (orders_.find(msg.original_order_reference_number) == orders_.end()) return;
    Order order;
        order=orders_[msg.original_order_reference_number];
        orders_.erase(msg.original_order_reference_number);
        char side=order.side;

    if(side=='B') 
    {
        bids_[order.price].total_shares-=order.shares;
        bids_[order.price].num_orders--;
        if(bids_[order.price].num_orders==0){
            bids_.erase(order.price);
        }
    }

    if(side=='S') 
    {
        asks_[order.price].total_shares-=order.shares;
        asks_[order.price].num_orders--;
        if(asks_[order.price].num_orders==0){
            asks_.erase(order.price);
        }
    }
    
    order.order_ref_num = msg.new_order_reference_number;
    order.shares = msg.shares;
    order.price = msg.price;
    order.side=side;

    orders_[msg.new_order_reference_number]=order;

    if(side=='B') {
        bids_[msg.price].total_shares+=msg.shares;
        bids_[msg.price].num_orders++;
    }
    if(side=='S') {
        asks_[msg.price].total_shares+=msg.shares;
        asks_[msg.price].num_orders++;
    }

}

uint32_t OrderBook :: best_bid() const{
    if(bids_.empty()) return 0;
    return bids_.begin()->first;
}

uint32_t OrderBook :: best_ask() const{
    if(asks_.empty()) return 0;
    return asks_.begin()->first;
}

int OrderBook :: volume() const{
    return volume_;
}

void OrderBook::print_book(int levels) const {
    std::cout << "--- ASK side (top " << levels << ") ---" << std::endl;
    int count = 0;
    for (auto it = asks_.begin(); it != asks_.end() && count < levels; ++it, ++count) {
        std::cout << "  Price: " << it->first / 10000.0 
                  << " Shares: " << it->second.total_shares
                  << " Orders: " << it->second.num_orders << std::endl;
    }
    std::cout << "--- BID side (top " << levels << ") ---" << std::endl;
    count = 0;
    for (auto it = bids_.begin(); it != bids_.end() && count < levels; ++it, ++count) {
        std::cout << "  Price: " << it->first / 10000.0
                  << " Shares: " << it->second.total_shares
                  << " Orders: " << it->second.num_orders << std::endl;
    }
}

void OrderBook:: execute_with_price_order(const OrderExecutedWithPriceMsg& msg){
    
    if (orders_.find(msg.order_reference_number) == orders_.end()) return;
    Order order;
    order=orders_[msg.order_reference_number];
    if (symbol_set_ && memcmp(order.stock, symbol_, 8) != 0) return;
    char side=order.side;   

    volume_+=msg.shares;

    vwap_numerator_+=(uint64_t) msg.price*(uint64_t)msg.shares;
    vwap_denominator_+=msg.shares;

    if(side=='B'){
        if(msg.shares==order.shares)
        {
            bids_[order.price].total_shares-=order.shares;
            bids_[order.price].num_orders--;
            orders_.erase(msg.order_reference_number);
            if(bids_[order.price].num_orders==0){
            bids_.erase(order.price);
            }
        }
        else{
            bids_[order.price].total_shares-=msg.shares;
            orders_[msg.order_reference_number].shares -= msg.shares;   
        }
        
    }

    if(side=='S'){
        if(msg.shares==order.shares)
        {
            asks_[order.price].total_shares-=order.shares;
            asks_[order.price].num_orders--;
            orders_.erase(msg.order_reference_number);
            if(asks_[order.price].num_orders==0){
            asks_.erase(order.price);
        }
    }
        else{
            asks_[order.price].total_shares-=msg.shares;
            orders_[msg.order_reference_number].shares -= msg.shares;

        }
        
    }

}


void OrderBook::clear() {
    orders_.clear();
    bids_.clear();
    asks_.clear();
}

void OrderBook:: bid_ask_spread(int num) const{
    for (auto bid=bids_.begin(),ask=asks_.begin();bid!=bids_.begin()&&ask!=asks_.begin()&&num;num--,++bid,++ask){
        std:: cout<< "No. of shares";
    }
}


double OrderBook::vwap() const {
    if (vwap_denominator_ == 0) return 0;
    std::cout << "numerator: " << vwap_numerator_ 
              << " denominator: " << vwap_denominator_ << std::endl;
    return (double)vwap_numerator_ / vwap_denominator_ / 10000.0;
}

void OrderBook:: volume_clear(){
    volume_=0;
}

uint64_t OrderBook:: bid_volume() const{
    uint64_t total=0;
    for(auto& pair: bids_){
        total+=pair.second.total_shares;
    }
    return total;
}

uint64_t OrderBook:: ask_volume() const{
    uint64_t total=0;
    for(auto& pair: asks_){
        total+=pair.second.total_shares;
    }
    return total;
}

double OrderBook:: spread() const{
    return (best_ask()-best_bid())/10000.0;
}

double OrderBook:: mid_price() const{
    return (best_ask()+best_bid())/20000.0;
}

double OrderBook::book_imbalance() const {
    uint64_t bid_vol = bid_volume();
    uint64_t ask_vol = ask_volume();
    if (bid_vol + ask_vol == 0) return 0;
   return ((double)bid_vol - (double)ask_vol) / ((double)bid_vol + (double)ask_vol);
}
