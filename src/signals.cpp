#include "signals.h"

Signals::Signals() : current_position_(Position::FLAT) , prev_score_(0), entry_price_(0) {
}

SignalResult Signals::calculate(const TrendResult& trend, double price) {
    SignalResult result;
    result.score    = trend.score;
    result.position = current_position_;
    result.signal   = Signal::HOLD;
    result.entry_price = entry_price_;

    double score = trend.score;

    if (current_position_ == Position::FLAT) {
        if (prev_score_ <= 40 && score > 40) {
            result.signal      = Signal::BUY;
            current_position_  = Position::LONG;
            entry_price_       = price;
        }
        else if (prev_score_ >= -40 && score < -40) {
            result.signal      = Signal::SELL;
            current_position_  = Position::SHORT;
            entry_price_       = price;
        }
    }
    else if (current_position_ == Position::LONG) {
        if (score < 0) {
            result.signal     = Signal::EXIT;
            current_position_ = Position::FLAT;
            entry_price_      = 0;
        }
    }
    else if (current_position_ == Position::SHORT) {
        if (score > 0) {
            result.signal     = Signal::EXIT;
            current_position_ = Position::FLAT;
            entry_price_      = 0;
        }
    }

    prev_score_ = score;
    result.position = current_position_;
    return result;
}
