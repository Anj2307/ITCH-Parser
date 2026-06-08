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

void OrderBook :: delete_order(const OrderDeleteMsg& msg){

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

void OrderBook :: cancel_order(const OrderCancelMsg& msg){
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


