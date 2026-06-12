#pragma once
#include <cstdint>
#include "order_book.h"

class Indicators {
    public: 
        void rsi(double close);
        double ema() const;
        double bollinger_bands() const;
        double macd() const;
        Indicators();
        double get_rsi() const;
    
    private:
        double ema_;
        double bollinger_bands_;
        double macd_;
        bool rsi_ready_;
        RSI curruent_rsi_;

};