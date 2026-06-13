#include "indicators.h"
#include <winsock2.h>
#include<iostream>
#include<cmath>

Indicators :: Indicators() : rsi_ready_(false), ema_ready_(false), macd_ready_(false),atr_ready_(false),bb_ready_(false){
    curruent_rsi_.value=0;
    curruent_rsi_.avg_loss=0;
    curruent_rsi_.avg_gain=0;
    curruent_rsi_.prev_close=0;
    curruent_rsi_.num_bar=0;

    ema_.num_bar=0;
    ema_.value=0;

    ema_fast_.num_bar=0;
    ema_fast_.value=0;

    ema_slow_.value=0;
    ema_slow_.num_bar=0;

    ema_signal_.value=0;
    ema_signal_.num_bar=0;


    macd_.macd_line=0;
    macd_.signal_line=0;
    macd_. histogram=0;

    atr_.value=0;
    atr_.prev_close=0;
    atr_.num_bars=0;

    memset(bb_.buffer, 0, sizeof(bb_.buffer));
    bb_.head = 0;
    bb_.num_bar = 0;
    bb_.upper = 0;
    bb_.middle = 0;
    bb_.lower = 0;

    obv_.prev_close=0;
    obv_.value=0;
}


void Indicators:: rsi(double close){

    if(curruent_rsi_.num_bar==0) {
        curruent_rsi_.num_bar=1;
        curruent_rsi_.prev_close=close;
        return;
    }
    if(curruent_rsi_.num_bar<15)
    {
        if(close<curruent_rsi_.prev_close)
        {
            curruent_rsi_.avg_loss+=curruent_rsi_.prev_close-close;
        }
        else curruent_rsi_.avg_gain+= close-curruent_rsi_.prev_close;
        curruent_rsi_.num_bar++;
        curruent_rsi_.prev_close=close;
        return;
    }
    // smoothing phase
    if(curruent_rsi_.num_bar==15){
        curruent_rsi_.avg_gain=curruent_rsi_.avg_gain / 14;
        curruent_rsi_.avg_loss = curruent_rsi_.avg_loss / 14;
    }

    if(close<curruent_rsi_.prev_close)
        {
            curruent_rsi_.avg_loss=(curruent_rsi_.avg_loss * 13 + close-curruent_rsi_.prev_close) / 14 ;
        }
        else curruent_rsi_.avg_gain=(curruent_rsi_.avg_gain * 13 + -1 * close+curruent_rsi_.prev_close) / 14 ;

        double rs= curruent_rsi_.avg_gain / curruent_rsi_.avg_loss;
        curruent_rsi_.value=100-(100/(1+rs));
        rsi_ready_=true;

        curruent_rsi_.num_bar++;
        return;
    }

double Indicators:: get_rsi() const{
    if(!rsi_ready_) return 0;
    return curruent_rsi_.value;
}


void Indicators :: ema(int n, double close){
    if(ema_.num_bar<=n){
        ema_.num_bar++;
        ema_.value+=close;
        ema_ready_=false;
        return;
    }
    if(ema_.num_bar==n+1){
        ema_.value=ema_.value/n;
        ema_ready_=true;
        ema_.num_bar++;
        return;
    }
    ema_.value=close * double((2.0/(n+1))) + ema_.value*(1-double((2.0/(n+1))));
    ema_ready_=true;
    ema_.num_bar++;
    return;
}


void Indicators :: ema(int n, double close,EMA &target){
    if(target.num_bar<=n){
        target.num_bar++;
        target.value+=close;
        return;
    }
    if(target.num_bar==n+1){
        target.value=target.value/n;
        target.num_bar++;
        return;
    }
    target.value=close * double((2.0/(n+1))) + target.value*(1-double((2.0/(n+1))));
    target.num_bar++;
    return;
}

double Indicators :: get_ema() const{
    if(ema_ready_==false) return 0;
    return ema_.value;
}


void Indicators:: macd(double close){
    if(ema_fast_.num_bar <=13 || ema_slow_.num_bar<=27)
    {
        ema(12,close,ema_fast_);
        ema(26,close,ema_slow_);
        macd_ready_=false;
        return;
    }
    if(ema_fast_.num_bar>13 && ema_slow_.num_bar>27)
    {
        macd_.macd_line = ema_fast_.value - ema_slow_.value;
        ema(9, macd_.macd_line, ema_signal_);
        ema(12,close,ema_fast_);
        ema(26,close,ema_slow_);

        if(ema_signal_.num_bar>10)
        {
            macd_.histogram=macd_.macd_line-ema_signal_.value;
            macd_.signal_line=ema_signal_.value;
            macd_ready_=true;
        }
        return;
    }
    return;
}

void Indicators:: get_macd() const{
    if(macd_ready_==false) return ;
    else {
        std:: cout<< " macd_line: "<<macd_.macd_line
                  << " signal: " << macd_.signal_line
                  << " histogram: "<< macd_.histogram
                  << std:: endl;
    }
}

double Indicators:: tr(double high, double close, double low){
    return std::max(high-low, std::max(std::fabs(high-close), std::fabs(low-close)));
}

void Indicators:: atr(double high, double close, double low){
    if(atr_.num_bars==0)
    {
        atr_.num_bars++;
        atr_.prev_close=close;
        atr_ready_=false;
        return;
    }
    if(atr_.num_bars<15){
        atr_.value+=  tr(high,atr_.prev_close,low);
        atr_.num_bars++;
        atr_.prev_close=close;
        atr_ready_=false;
        return;
    }
    if(atr_.num_bars==15){
        atr_.value+=tr(high,atr_.prev_close,low);
        atr_.value/=14;
        atr_.prev_close=close;
        atr_ready_=true;
        atr_.num_bars++;
        return;
    }
    atr_.value= (atr_.value*13 + tr(high,atr_.prev_close,low))/14;
    atr_.prev_close=close;
    atr_ready_=true;
    atr_.num_bars++;
    return;

}

double Indicators:: get_atr() const{
    if(!atr_ready_) return 0.0;
    return atr_.value;
}

void Indicators:: bb(double close){
    bb_.buffer[bb_.head]=close;
    bb_.head=(bb_.head+1) % 20;
    bb_.num_bar++;
    if(bb_.num_bar<20) return;
    double sum=0;
    for(double i: bb_.buffer){
        sum+=i;
    }
    bb_.middle=sum/20;
    double var=0;
    for (double i: bb_.buffer){
        var+=(i-bb_.middle)*(i-bb_.middle);
    }
    var/=20;
    double stddev= std:: sqrt(var);
    bb_.upper=(bb_.middle+(2*stddev));
    bb_.lower=(bb_.middle-(2*stddev));
    bb_ready_=true;
}

void Indicators :: get_bb() const{
    if(!bb_ready_) return;

    std:: cout <<"upper: "<<bb_.upper
                <<" middle: "<<bb_.middle
                <<" lower: "<<bb_.lower
                << std:: endl;
    return;
}

void Indicators :: obv(double close, int32_t volume){
    obv_.prev_value=obv_.value;
    if(close>obv_.prev_close) {
        
        obv_.value+=(int64_t) volume;

    }
    else if(close<obv_.prev_close)
    {
        obv_.value-=(int64_t) volume;
    }

    obv_.prev_close=close;
}

double Indicators :: get_obv() const{
    return static_cast<double> (obv_.value);
}

