#include <windows.h>

// --- FT5 API 基本定義 ---
#define ma_EMA 1
#define pt_Close 0
#define ds_Histogram 2
#define ps_Solid 0
#define ot_Integer 0
#define mode_Main 0
#define mode_Signal 1

typedef unsigned long TColor;
#define clGreen 0x0000FF00
#define clRed   0x000000FF
#define clBlue  0x00FF0000

// --- FT5 インポート関数の直接定義 ---
typedef void (__stdcall *tIndicatorShortName)(const char* name);
typedef void (__stdcall *tSetOutputBufferCount)(int count);
typedef void (__stdcall *tSetIndexBuffer)(int index, int bufferIndex);
typedef void (__stdcall *tSetIndexStyle)(int index, int style, int penStyle, int width, TColor color);
typedef void (__stdcall *tSetIndexLabel)(int index, const char* label);
typedef void (__stdcall *tRegOption)(const char* name, int type, void* param);
typedef int  (__stdcall *tBars)();
typedef PCHAR (__stdcall *tSymbol)();
typedef int  (__stdcall *tPeriod)();
typedef double (__stdcall *tHigh)(int index);
typedef double (__stdcall *tiMA)(PCHAR symbol, int timeframe, int period, int method, int price, int shift);
typedef double (__stdcall *tiMACD)(PCHAR symbol, int timeframe, int fast_ema, int slow_ema, int signal_sma, int price, int mode, int shift);
typedef void (__stdcall *tSetIndexValue)(int shift, int index, double value);

// エクスポート関数の宣言
extern "C" {
    __declspec(dllexport) void __stdcall Init();
    __declspec(dllexport) void __stdcall OnParamsChange();
    __declspec(dllexport) void __stdcall Calculate(int index);
}

// パラメーター変数
int EMA_Period = 13;
int MACD_Fast = 12;
int MACD_Slow = 26;
int MACD_Signal = 9;

// ダミー関数（コンパイルエラー回避用定義）
void __stdcall Init()
{
}

void __stdcall OnParamsChange()
{
}

void __stdcall Calculate(int index)
{
}
