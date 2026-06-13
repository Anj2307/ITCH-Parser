#pragma once
#include "indicators.h"

class Trends{
    public: TrendResult calculate(const Indicators& ind, double price);
    private:
        double score_rsi(const Indicators& ind);
        double score_ema(const Indicators& ind, double price);
        double score_macd(const Indicators& ind);
        double score_bb(const Indicators& ind, double price);
        double score_obv(const Indicators& ind);
}
