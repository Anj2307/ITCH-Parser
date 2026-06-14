#pragma once
#include "trends.h"

enum class Signal { BUY, SELL, EXIT, HOLD };

enum class Position { FLAT, LONG, SHORT };

struct SignalResult {
    Signal signal;
    Position position;
    double entry_price;
    double score;
};

class Signals {
public:
    SignalResult calculate(const TrendResult& trend, double price);
    Signals();
private:
    Position current_position_;
    double prev_score_;
    double entry_price_;
};