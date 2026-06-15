#include "patterns.h"
#include <cmath>
#include <algorithm>

Patterns::Patterns() : has_prev_prev_(false) {
    prev_prev_bar_.initialized = false;
}

double Patterns::body(OHLCVBar bar) {
    return std::fabs(bar.close - bar.open);
}

double Patterns::upper_wick(OHLCVBar bar) {
    return bar.high - std::max(bar.open, bar.close);
}

double Patterns::lower_wick(OHLCVBar bar) {
    return std::min(bar.open, bar.close) - bar.low;
}

double Patterns::total_range(OHLCVBar bar) {
    return bar.high - bar.low;
}

bool Patterns::is_green(OHLCVBar bar) {
    return bar.close > bar.open;
}

bool Patterns::is_red(OHLCVBar bar) {
    return bar.close < bar.open;
}


PatternResult Patterns::detect(OHLCVBar current, OHLCVBar previous) {
    PatternResult result = {};

    double curr_body  = body(current);
    double curr_range = total_range(current);
    double curr_upper = upper_wick(current);
    double curr_lower = lower_wick(current);

    double prev_body  = body(previous);
    double prev_range = total_range(previous);

    if (curr_range == 0) return result;

    // DOJI
    result.doji = curr_body < curr_range * 0.1;

    // HAMMER
    result.hammer = curr_lower > curr_body * 2.0
                 && curr_upper < curr_body * 0.5
                 && curr_range > 0;

    // SHOOTING STAR
    result.shooting_star = curr_upper > curr_body * 2.0
                        && curr_lower < curr_body * 0.5
                        && curr_range > 0;

    // MARUBOZU BULLISH
    result.marubozu_bull = is_green(current)
                        && curr_upper < curr_range * 0.02
                        && curr_lower < curr_range * 0.02
                        && curr_body  > curr_range * 0.95;

    // MARUBOZU BEARISH
    result.marubozu_bear = is_red(current)
                        && curr_upper < curr_range * 0.02
                        && curr_lower < curr_range * 0.02
                        && curr_body  > curr_range * 0.95;

    // BULLISH ENGULFING
    result.bullish_engulfing = is_red(previous)
                            && is_green(current)
                            && current.open  < previous.close
                            && current.close > previous.open;

    // BEARISH ENGULFING
    result.bearish_engulfing = is_green(previous)
                            && is_red(current)
                            && current.open  > previous.close
                            && current.close < previous.open;

    // BULLISH HARAMI
    result.bullish_harami = is_red(previous)
                         && is_green(current)
                         && current.open  > previous.close
                         && current.close < previous.open;

    // BEARISH HARAMI
    result.bearish_harami = is_green(previous)
                         && is_red(current)
                         && current.open  < previous.close
                         && current.close > previous.open;

    // THREE BAR PATTERNS
    if (has_prev_prev_ && prev_prev_bar_.initialized) {
        // MORNING STAR
        result.morning_star = is_red(prev_prev_bar_)
                           && body(prev_prev_bar_) > prev_prev_bar_.high - prev_prev_bar_.low * 0.6
                           && result.doji
                           && is_green(current)
                           && current.close > prev_prev_bar_.open;

        result.evening_star = is_green(prev_prev_bar_)
                           && body(prev_prev_bar_) > prev_prev_bar_.high - prev_prev_bar_.low * 0.6
                           && result.doji
                           && is_red(current)
                           && current.close < prev_prev_bar_.open;
    }

    // update three bar history
    prev_prev_bar_ = previous;
    has_prev_prev_ = true;

    return result;
}