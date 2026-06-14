# ITCH Parser — NASDAQ Market Data Pipeline in C++

Ever wondered what's happening inside a stock exchange at the nanosecond level? This project parses raw NASDAQ ITCH 5.0 binary data straight from the source — no CSV files, no third-party data providers, just the real thing.

Built entirely in C++, this is a full market data pipeline that goes from raw binary messages all the way to trading signals.

---

## What It Does

NASDAQ publishes every single order, cancellation, and execution in a binary format called ITCH 5.0. This project reads that raw binary feed and turns it into something meaningful — a live order book, OHLCV bars, technical indicators, and ultimately buy/sell signals.

The pipeline looks like this:

```
Raw ITCH Binary File
        ↓
Message Decoder (A, E, X, D, U, C messages)
        ↓
Order Book (live bid/ask, spread, imbalance)
        ↓
OHLCV Bar Builder (1-minute bars)
        ↓
Technical Indicators (RSI, EMA, MACD, ATR, Bollinger Bands, OBV, VWAP)
        ↓
Trend Score (-100 to +100)
        ↓
Trading Signals (BUY / SELL / EXIT)
```

---

## Features

**Market Data**
- Parses all ITCH 5.0 message types including Add, Execute, Cancel, Delete, and Replace orders
- Maintains a live order book with best bid/ask tracking
- Computes spread, mid price, and book imbalance in real time
- Tracks VWAP across the full session

**Bar Builder**
- Builds 1-minute OHLCV bars from raw tick data
- Exports bars to CSV for external analysis

**Technical Indicators**
- RSI (Relative Strength Index) with Wilder's smoothing
- EMA (Exponential Moving Average)
- MACD with signal line and histogram
- ATR (Average True Range) for volatility measurement
- Bollinger Bands with rolling standard deviation
- OBV (On Balance Volume) for volume flow analysis
- VWAP (Volume Weighted Average Price)

**Trend & Signal Engine**
- Composite trend score combining all indicators (-100 to +100)
- BUY / SELL / EXIT signal generation based on score thresholds
- ATR-based stop loss and take profit levels

**Output**
- Real-time console output every 10 million messages
- CSV export for order book snapshots
- CSV export for OHLCV bars
- Signal printing when direction changes

---

## Project Structure

```
ITCH-Parser/
├── include/
│   ├── file_reader.h       # Binary file reading
│   ├── decoder.h           # ITCH message decoding
│   ├── order_book.h        # Order book management
│   ├── indicators.h        # Technical indicators
│   ├── trends.h            # Trend scoring
│   └── signals.h           # Signal generation
├── src/
│   ├── main.cpp            # Entry point
│   ├── file_reader.cpp
│   ├── decoder.cpp
│   ├── order_book.cpp
│   ├── indicators.cpp
│   ├── trends.cpp
│   └── signals.cpp
├── tests/
├── CMakeLists.txt
└── README.md
```

---

## Getting Started

### Prerequisites

- C++17 or later
- CMake 3.10+
- MinGW or GCC

### Build

```bash
mkdir build
cd build
cmake ..
make
```

### Data

Download NASDAQ ITCH 5.0 sample data from the [NASDAQ FTP server](https://itch.nasdaqtrader.com/ITCH.aspx) and place it in the `data/` folder.

### Run

```bash
./itch_parser
```

The program will process the file and output:
- Console stats every 10 million messages
- `data/aapl_analysis.csv` — order book snapshots
- `data/aapl_ohlcv.csv` — 1-minute OHLCV bars
- Trading signals printed when they change

---

## Sample Output

```
Messages: 200000000 Best Bid: 162.72 Best Ask: 162.74
spread: 0.02 mid_price: 162.73 rsi: 49.84 ema: 162.95
atr: 0.138 obv: -392741 trend_score: 25

SIGNAL: BUY  at 163.195
SIGNAL: EXIT at 163.385
SIGNAL: SELL at 163.15
SIGNAL: EXIT at 163.40
```

---

## How the Trend Score Works

Each indicator contributes up to ±20 points toward a total score between -100 and +100:

| Indicator | Bullish Condition | Bearish Condition |
|---|---|---|
| RSI | > 60 | < 40 |
| EMA | Price above EMA | Price below EMA |
| MACD | Line + histogram positive | Line + histogram negative |
| Bollinger | Price near upper band | Price near lower band |
| OBV | Rising with price | Falling with price |

Signals fire when the score crosses ±40 and reset when it returns toward zero.

---

## Why This Project

Most algorithmic trading projects start with daily CSV data from Yahoo Finance. This one starts at the lowest possible level — raw binary nanosecond-timestamped exchange data. If you want to understand how markets actually work at the microstructure level, this is where to start.

---

## Built With

- C++17
- CMake
- NASDAQ ITCH 5.0 Protocol

---

## Author

**Anj2307** — built from scratch, parsing bytes to signals.