#pragma once
#include <cstdint>

struct AddOrderMsg{
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    char side;
    uint32_t shares;
    char stock[8];
    uint32_t price;
};

struct OrderExecutedMsg{
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint32_t shares;
    uint64_t match_number;
};

struct OrderCancelMsg{
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
    uint32_t shares;
};



struct OrderDeleteMsg{
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t order_reference_number;
};

struct OrderReplaceMsg{
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    uint64_t original_order_reference_number;
    uint64_t new_order_reference_number;
    uint32_t shares;
    uint32_t price;
};


struct SystemEventMsg{
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char event_code;
};

struct StockDirectoryMsg{
    uint16_t stock_locate;
    uint16_t tracking_number;
    uint64_t timestamp;
    char stock[8];
    char market_category;
    char financial_status_indicator;
    uint32_t round_lot_size;
    char round_lots_only;
    char issue_classification;
    char issue_sub_type[2];
    char authenticity;
    char short_sale_threshold_indicator;
    char ipo_flag;
    char luld_reference_price_tier;
    char etp_flag;
    uint32_t etp_leverage_factor;
    char inverse_indicator;
};

struct Order{
    uint64_t order_ref_num;
    char side;
    uint32_t shares;
    uint32_t price;
    char stock[8];
};
