
import argparse
import sys
import webbrowser
from pathlib import Path

import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

# ---------------------------------------------------------------- load data

def load_data(csv_path: str, resample: str | None = None) -> pd.DataFrame:
    df = pd.read_csv(csv_path)

    required = {"timestamp_sec", "best_bid", "best_ask", "spread",
                "mid_price", "vwap", "volume", "book_imbalance"}
    missing = required - set(df.columns)
    if missing:
        sys.exit(f"ERROR: CSV is missing columns: {sorted(missing)}")

    # Parse HH:MM:SS into a proper datetime (date doesn't matter, only time-of-day)
    df["time"] = pd.to_datetime(df["timestamp_sec"], format="%H:%M:%S")
    df = df.sort_values("time").set_index("time")

    if resample:
        agg = {
            "best_bid":       "last",
            "best_ask":       "last",
            "spread":         "mean",
            "mid_price":      "last",
            "vwap":           "last",
            "volume":         "sum",
            "book_imbalance": "mean",
        }
        df = df.resample(resample).agg(agg).dropna(subset=["mid_price"])

    return df

# ------------------------------------------------------------- build figure

def build_dashboard(df: pd.DataFrame, title: str) -> go.Figure:
    fig = make_subplots(
        rows=4, cols=1,
        shared_xaxes=True,
        vertical_spacing=0.04,
        row_heights=[0.45, 0.18, 0.18, 0.19],
        subplot_titles=(
            "Price (Bid / Ask / Mid / VWAP)",
            "Spread",
            "Volume",
            "Book Imbalance",
        ),
    )

    x = df.index

    # --- Row 1: prices, with bid-ask band ---------------------------------
    fig.add_trace(go.Scatter(
        x=x, y=df["best_ask"], name="Best Ask",
        line=dict(color="#ef5350", width=1),
        hovertemplate="Ask: %{y:.4f}<extra></extra>",
    ), row=1, col=1)

    fig.add_trace(go.Scatter(
        x=x, y=df["best_bid"], name="Best Bid",
        line=dict(color="#26a69a", width=1),
        fill="tonexty", fillcolor="rgba(120,120,120,0.12)",  # shaded spread band
        hovertemplate="Bid: %{y:.4f}<extra></extra>",
    ), row=1, col=1)

    fig.add_trace(go.Scatter(
        x=x, y=df["mid_price"], name="Mid Price",
        line=dict(color="#42a5f5", width=1.6),
        hovertemplate="Mid: %{y:.4f}<extra></extra>",
    ), row=1, col=1)

    fig.add_trace(go.Scatter(
        x=x, y=df["vwap"], name="VWAP",
        line=dict(color="#ab47bc", width=1.6, dash="dash"),
        hovertemplate="VWAP: %{y:.4f}<extra></extra>",
    ), row=1, col=1)

    # --- Row 2: spread ------------------------------------------------------
    fig.add_trace(go.Scatter(
        x=x, y=df["spread"], name="Spread",
        line=dict(color="#ffa726", width=1.2),
        fill="tozeroy", fillcolor="rgba(255,167,38,0.15)",
        hovertemplate="Spread: %{y:.4f}<extra></extra>",
    ), row=2, col=1)

    # --- Row 3: volume ------------------------------------------------------
    fig.add_trace(go.Bar(
        x=x, y=df["volume"], name="Volume",
        marker_color="#78909c",
        hovertemplate="Vol: %{y}<extra></extra>",
    ), row=3, col=1)

    # --- Row 4: book imbalance (green above 0, red below) -------------------
    imb_colors = ["#26a69a" if v >= 0 else "#ef5350" for v in df["book_imbalance"]]
    fig.add_trace(go.Bar(
        x=x, y=df["book_imbalance"], name="Imbalance",
        marker_color=imb_colors,
        hovertemplate="Imb: %{y:.4f}<extra></extra>",
    ), row=4, col=1)
    fig.add_hline(y=0, line_width=1, line_color="gray", row=4, col=1)

    # --- Layout -------------------------------------------------------------
    fig.update_layout(
        title=dict(text=title, x=0.5, font=dict(size=20)),
        template="plotly_dark",
        height=950,
        hovermode="x unified",
        legend=dict(orientation="h", yanchor="bottom", y=1.02,
                    xanchor="right", x=1),
        bargap=0.05,
        margin=dict(t=110, b=40, l=60, r=30),
    )
    fig.update_yaxes(title_text="Price", row=1, col=1)
    fig.update_yaxes(title_text="Spread", row=2, col=1)
    fig.update_yaxes(title_text="Shares", row=3, col=1)
    fig.update_yaxes(title_text="Imbalance", range=[-1, 1], row=4, col=1)
    fig.update_xaxes(title_text="Time", row=4, col=1)

    # Range selector buttons + slider on the bottom axis
    fig.update_xaxes(
        rangeslider=dict(visible=True, thickness=0.04),
        row=4, col=1,
    )
    return fig

# ----------------------------------------------------------- summary stats

def print_summary(df: pd.DataFrame) -> None:
    print("\n=== Session Summary ===")
    print(f"Rows:            {len(df):,}")
    print(f"Time range:      {df.index.min().time()} -> {df.index.max().time()}")
    print(f"Mid price:       {df['mid_price'].iloc[0]:.4f} -> {df['mid_price'].iloc[-1]:.4f}")
    print(f"Mid hi / lo:     {df['mid_price'].max():.4f} / {df['mid_price'].min():.4f}")
    print(f"Avg spread:      {df['spread'].mean():.4f}")
    print(f"Total volume:    {int(df['volume'].sum()):,}")
    print(f"Avg imbalance:   {df['book_imbalance'].mean():+.4f}")
    print("=======================\n")

# --------------------------------------------------------------------- main

def main() -> None:
    p = argparse.ArgumentParser(description="Interactive ITCH order book dashboard")
    p.add_argument("csv", help="Path to the CSV produced by the C++ parser")
    p.add_argument("-o", "--output", default="dashboard.html",
                   help="Output HTML file (default: dashboard.html)")
    p.add_argument("--resample", default=None, metavar="RULE",
                   help="Downsample, e.g. '5s', '1min' (recommended for big files)")
    p.add_argument("--no-open", action="store_true",
                   help="Don't auto-open the dashboard in a browser")
    args = p.parse_args()

    df = load_data(args.csv, args.resample)
    print_summary(df)

    title = f"ITCH Order Book — {Path(args.csv).stem}"
    fig = build_dashboard(df, title)

    out = Path(args.output)
    fig.write_html(out, include_plotlyjs="cdn")
    print(f"Dashboard written to: {out.resolve()}")

    if not args.no_open:
        webbrowser.open(out.resolve().as_uri())

if __name__ == "__main__":
    main()