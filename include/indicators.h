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
        double get_ema() const;
        void ema(int n, double close);
    
    private:
        double bollinger_bands_;
        double macd_;
        bool rsi_ready_;
        bool ema_ready_;
        RSI curruent_rsi_;
        EMA ema_;

};