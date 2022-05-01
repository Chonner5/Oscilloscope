#ifndef _ALL_VALUE_H
#define _ALL_VALUE_H

//这个文件存放了很多的自定义的变量可以供其他文件使用

static GUI_CONST_STORAGE GUI_COLOR Colors[] = {
    GUI_MAKE_COLOR(0xFFFFFF), GUI_MAKE_COLOR(0x0000AA)};





static GRAPH_SCALE_Handle _hScaleV; // Handle of vertical scale 网格句柄
static GRAPH_SCALE_Handle _hScaleH; // Handle of horizontal scale

static const GUI_COLOR _aColor[3] = {GUI_RED, GUI_DARKGREEN, GUI_MAGENTA};
static const U8 _aLStyle[3] = {GUI_LS_SOLID, GUI_LS_DOT, GUI_LS_DASH};

static int peroid_test; //周期测量值
static int voltage_test;//电压测量值
static int _NumPoints;//初始化的点数

static int X_step = 2, Y_step = 5;
static int start_x, signal_max;//x开始的位置，信号最大值
static int Xline_A_palce=80;  //X——line A的位置 默认值80
static int Xline_B_palce=160;//X——line B默认值

static int Yline_A_palce=30;
static int Yline_B_palce=90;
// static const int   _aNumPoints[] = { 100, 25, 13 };
static GUI_POINT _aPoint[2][400],test_lineplace[2][2];
GRAPH_DATA_Handle _ahDataXY[2],test_line[2];

static GUI_POINT test_Y_lineplace[2][2];
GRAPH_DATA_Handle  test_Y_line[2];
static WM_HWIN hWin;




#endif

