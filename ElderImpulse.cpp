#include <windows.h>

// --- FT5 API Types & Constants ---
typedef int TMovingAverageMethod;
#define ma_SMA 0
#define ma_EMA 1
#define ma_WMA 2
#define ma_SSMA 3

typedef int TAppliedPrice;
#define pt_Close 0
#define pt_Open 1
#define pt_High 2
#define pt_Low 3
#define pt_Median 4
#define pt_Typical 5
#define pt_Weighted 6

typedef int TDrawingStyle;
#define ds_Line 0
#define ds_Section 1
#define ds_Histogram 2
#define ds_Dot 3

typedef int TPenStyle;
#define ps_Solid 0

typedef int TOptionType;
#define ot_Integer 0
#define ot_Double 1
#define ot_String 2
#define ot_Boolean 3
#define ot_TimeFrame 4

#define mode_Main 0
#define mode_Signal 1

typedef unsigned long TColor;
#define clGreen 0x0000FF00
#define clRed   0x000000FF
#define clBlue  0x00FF0000

// --- FT5 API Function Declarations ---
extern "C" {
    __declspec(dllimport) void __stdcall IndicatorShortName(const char* name);
    __declspec(dllimport) void __stdcall SetOutputBufferCount(int count);
    __declspec(dllimport) void __stdcall SetIndexBuffer(int index, int bufferIndex);
    __declspec(dllimport) void __stdcall SetIndexStyle(int index, TDrawingStyle style, TPenStyle penStyle, int width, TColor color);
    __declspec(dllimport) void __stdcall SetIndexLabel(int index, const char* label);
    __declspec(dllimport) void __stdcall RegOption(const char* name, TOptionType type, void* param);
    __declspec(dllimport) int  __stdcall Bars();
    __declspec(dllimport) char* __stdcall Symbol();
    __declspec(dllimport) int  __stdcall Period();
    __declspec(dllimport) double __stdcall High(int index);
    __declspec(dllimport) double __stdcall iMA(const char* symbol, int timeframe, int period, TMovingAverageMethod method, TAppliedPrice price, int shift);
    __declspec(dllimport) double __stdcall iMACD(const char* symbol, int timeframe, int fast_ema, int slow_ema, int signal_sma, TAppliedPrice price, int mode, int shift);
    __declspec(dllimport) void __stdcall SetIndexValue(int shift, int index, double value);
}

// --- Parameters ---
int EMA_Period = 13;
int MACD_Fast = 12;
int MACD_Slow = 26;
int MACD_Signal = 9;

// --- Exported Functions ---
extern "C" __declspec(dllexport) void __stdcall Init()
{
    IndicatorShortName("Elder Impulse System");
    SetOutputBufferCount(3);

    SetIndexBuffer(0, 0);
    SetIndexStyle(0, ds_Histogram, ps_Solid, 2, clGreen);
    SetIndexLabel(0, "Bullish");

    SetIndexBuffer(1, 1);
    SetIndexStyle(1, ds_Histogram, ps_Solid, 2, clRed);
    SetIndexLabel(1, "Bearish");

    SetIndexBuffer(2, 2);
    SetIndexStyle(2, ds_Histogram, ps_Solid, 2, clBlue);
    SetIndexLabel(2, "Neutral");

    RegOption("EMA Period", ot_Integer, &EMA_Period);
    RegOption("MACD Fast", ot_Integer, &MACD_Fast);
    RegOption("MACD Slow", ot_Integer, &MACD_Slow);
    RegOption("MACD Signal", ot_Integer, &MACD_Signal);
}

extern "C" __declspec(dllexport) void __stdcall OnParamsChange()
{
}

extern "C" __declspec(dllexport) void __stdcall Calculate(int index)
{
    if (index >= Bars() - MACD_Slow - 1) return;

    double emaCurr = iMA(Symbol(), Period(), EMA_Period, ma_EMA, pt_Close, index);
    double emaPrev = iMA(Symbol(), Period(), EMA_Period, ma_EMA, pt_Close, index + 1);

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

    SetIndexValue(index, 0, 0);
    SetIndexValue(index, 1, 0);
    SetIndexValue(index, 2, 0);

    if (emaUp && osmaUp)
    {
        SetIndexValue(index, 0, High(index));
    }
    else if (emaDown && osmaDown)
    {
        SetIndexValue(index, 1, High(index));
    }
    else
    {
        SetIndexValue(index, 2, High(index));
    }
}
