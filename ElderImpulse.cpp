#include "IndicatorInterfaceUnit.h"

// パラメーター設定
int EMA_Period = 13;
int MACD_Fast = 12;
int MACD_Slow = 26;
int MACD_Signal = 9;

EXPORT void __stdcall Init()
{
    IndicatorShortName("Elder Impulse System");
    SetOutputBufferCount(3);

    // バッファー0: 買い（緑）
    SetIndexBuffer(0, 0);
    SetIndexStyle(0, ds_Histogram, ps_Solid, 2, 0x0000FF00);
    SetIndexLabel(0, "Bullish");

    // バッファー1: 売り（赤）
    SetIndexBuffer(1, 1);
    SetIndexStyle(1, ds_Histogram, ps_Solid, 2, 0x000000FF);
    SetIndexLabel(1, "Bearish");

    // バッファー2: 中立（青）
    SetIndexBuffer(2, 2);
    SetIndexStyle(2, ds_Histogram, ps_Solid, 2, 0x00FF0000);
    SetIndexLabel(2, "Neutral");

    // パラメーター登録
    RegOption("EMA Period", ot_Integer, &EMA_Period);
    RegOption("MACD Fast", ot_Integer, &MACD_Fast);
    RegOption("MACD Slow", ot_Integer, &MACD_Slow);
    RegOption("MACD Signal", ot_Integer, &MACD_Signal);
}

EXPORT void __stdcall OnParamsChange()
{
}

EXPORT void __stdcall Calculate(int index)
{
    if (index >= Bars() - MACD_Slow - 1) return;

    // 13EMA
    double emaCurr = iMA(Symbol(), Period(), EMA_Period, ma_EMA, pt_Close, index);
    double emaPrev = iMA(Symbol(), Period(), EMA_Period, ma_EMA, pt_Close, index + 1);

    // MACDヒストグラム (OsMA) = MACD Main - MACD Signal
    double macdCurr = iMACD(Symbol(), Period(), MACD_Fast, MACD_Slow, MACD_Signal, pt_Close, mode_Main, index);
    double macdPrev = iMACD(Symbol(), Period(), MACD_Fast, MACD_Slow, MACD_Signal, pt_Close, mode_Main, index + 1);

    double macdSigCurr = iMACD(Symbol(), Period(), MACD_Fast, MACD_Slow, MACD_Signal, pt_Close, mode_Signal, index);
    double macdSigPrev = iMACD(Symbol(), Period(), MACD_Fast, MACD_Slow, MACD_Signal, pt_Close, mode_Signal, index + 1);

    double osmaCurr = macdCurr - macdSigCurr;
    double osmaPrev = macdPrev - macdSigPrev;

    bool emaUp = (emaCurr > emaPrev);
    bool emaDown = (emaCurr < emaPrev);
    bool osmaUp = (osmaCurr > osmaPrev);
    bool osmaDown = (osmaCurr < osmaPrev);

    // バッファー初期化
    SetIndexValue(index, 0, 0);
    SetIndexValue(index, 1, 0);
    SetIndexValue(index, 2, 0);

    // シグナル判定
    if (emaUp && osmaUp)
    {
        SetIndexValue(index, 0, High(index)); // 緑（強気）
    }
    else if (emaDown && osmaDown)
    {
        SetIndexValue(index, 1, High(index)); // 赤（弱気）
    }
    else
    {
        SetIndexValue(index, 2, High(index)); // 青（中立）
    }
}
