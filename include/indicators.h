#pragma once
#include <cstdint>
#include "order_book.h"

class Indicators {
    public: 
        void rsi(double close);
        double ema() const;
        double bollinger_bands() const;
        void get_macd() const;
        MACD get_macd_full() const;
        void macd(double close);
        Indicators();
        double get_rsi() const;
        double get_ema() const;
        void ema(int n, double close);
        void ema(int n, double close, EMA& target);
        void atr(double high, double close, double low);
        double get_atr() const;
        double tr(double high, double close, double low);
        void bb(double close);
        void get_bb() const;
        BollingerBands get_bb_full() const;
        void obv(double close, int32_t volume);
        double get_obv() const;
        bool is_rsi_ready() const;
        bool is_ema_ready() const;
        bool is_macd_ready() const;
        bool is_bb_ready() const;
        bool is_atr_ready() const;

    
    private:
        bool rsi_ready_;
        bool ema_ready_;
        bool macd_ready_;
        bool atr_ready_;
        bool bb_ready_;
        RSI curruent_rsi_;
        EMA ema_;
        MACD macd_;
        EMA ema_fast_;
        EMA ema_slow_;
        EMA ema_signal_;
        ATR atr_;
        BollingerBands bb_;
        OBV obv_;
};