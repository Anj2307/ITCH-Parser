#pragma once
#include "order_book.h"

struct PatternResult {
    bool doji;
    bool hammer;
    bool shooting_star;
    bool marubozu_bull;
    bool marubozu_bear;
    bool bullish_engulfing;
    bool bearish_engulfing;
    bool bullish_harami;
    bool bearish_harami;
    bool morning_star;
    bool evening_star;
};

class Patterns {
public:
    Patterns();
    PatternResult detect(OHLCVBar current, OHLCVBar previous);

private:
    double body(OHLCVBar bar);
    double upper_wick(OHLCVBar bar);
    double lower_wick(OHLCVBar bar);
    double total_range(OHLCVBar bar);
    bool is_green(OHLCVBar bar);
    bool is_red(OHLCVBar bar);

    OHLCVBar prev_prev_bar_;
    bool has_prev_prev_;
};