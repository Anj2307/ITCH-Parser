#pragma once
#include <cstdint>
#include "order_book.h"

class Indicators {
    public: 
        void rsi(double close);
        double ema() const;
        double bollinger_bands() const;
        void get_macd() const;
        void macd(double close);
        Indicators();
        double get_rsi() const;
        double get_ema() const;
        void ema(int n, double close);
        void ema(int n, double close, EMA& target);

    
    private:
        bool rsi_ready_;
        bool ema_ready_;
        bool macd_ready_;
        RSI curruent_rsi_;
        EMA ema_;
        MACD macd_;
        EMA ema_fast_;
        EMA ema_slow_;
        EMA ema_signal_;
};