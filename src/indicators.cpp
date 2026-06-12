#include "indicators.h"
#include <winsock2.h>
#include<iostream>

Indicators :: Indicators() : rsi_ready_(false){
    curruent_rsi_.value=0;
    curruent_rsi_.avg_loss=0;
    curruent_rsi_.avg_gain=0;
    curruent_rsi_.prev_close=0;
    curruent_rsi_.num_bar=0;
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

