#include "trends.h"
#include <cstring>
#include<iostream>
#include <winsock2.h>

TrendResult Trends::calculate(const Indicators& ind, double price) {
    TrendResult result;
    result.rsi_score  = score_rsi(ind);
    result.ema_score  = score_ema(ind, price);
    result.macd_score = score_macd(ind);
    result.bb_score   = score_bb(ind, price);
    result.obv_score  = score_obv(ind);
    result.score      = result.rsi_score + result.ema_score +
                        result.macd_score + result.bb_score +
                        result.obv_score;
    return result;
}

double Trends::score_rsi(const Indicators& ind) {
    if (!ind.is_rsi_ready()) return 0;
    double rsi = ind.get_rsi();

    if (rsi > 70)       return +20;
    else if (rsi > 60)  return +10;
    else if (rsi > 50)  return +5;
    else if (rsi > 40)  return -5;
    else if (rsi > 30)  return -10;
    else                return -20;
}

double Trends::score_ema(const Indicators& ind, double price) {
    if(!ind.is_ema_ready()) return 0;
    if(!ind.is_atr_ready()) return 0;

    double ema = ind.get_ema();
    double atr=ind.get_atr();
    double diff=price-ema;

    if(diff > atr) return +20;
    else if (diff>atr*0.5) return +10;
    else if (diff>0) return +5;
    else if(diff>-atr*0.5) return -5;
    else if (diff>-atr) return -10;
    else  return -20;

}

double Trends::score_macd(const Indicators& ind) {
    return 0;
}

double Trends::score_bb(const Indicators& ind, double price) {
    return 0;
}

double Trends::score_obv(const Indicators& ind) {
    return 0;
}
