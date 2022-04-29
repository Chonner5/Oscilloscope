#ifndef _ALL_VALUE_H
#define _ALL_VALUE_H



static GUI_CONST_STORAGE GUI_COLOR Colors[] = {
    GUI_MAKE_COLOR(0xFFFFFF), GUI_MAKE_COLOR(0x0000AA)};





static GRAPH_SCALE_Handle _hScaleV; // Handle of vertical scale
static GRAPH_SCALE_Handle _hScaleH; // Handle of horizontal scale

static const GUI_COLOR _aColor[3] = {GUI_RED, GUI_DARKGREEN, GUI_MAGENTA};
static const U8 _aLStyle[3] = {GUI_LS_SOLID, GUI_LS_DOT, GUI_LS_DASH};

static int peroid_test;
static int voltage_test;
static int _NumPoints;

static int X_step = 2, Y_step = 5;
static int start_x, signal_max;
static int Xline_A_palce=80;
static int Xline_B_palce=160;

static int Yline_A_palce=30;
static int Yline_B_palce=90;
// static const int   _aNumPoints[] = { 100, 25, 13 };
static GUI_POINT _aPoint[2][400],test_lineplace[2][2];
GRAPH_DATA_Handle _ahDataXY[2],test_line[2];

static GUI_POINT test_Y_lineplace[2][2];
GRAPH_DATA_Handle  test_Y_line[2];
static WM_HWIN hWin;




#endif

