#include "setting.h"
#include "FRAMEWIN.h"
#include "GUI.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "timer.h"
#include "adc.h"
#include "RADIO.h"
#include "TEXT.h"
#include "BUTTON.h"
//#include "all_value.h"
// #include "GRAPH.h"
//#include "BUTTON.h"
#define WM_APP_SHOW_TEXT	(WM_USER +0) 
//#include "string.h"

int dropdown_flag=0;
int drop1_step_value=1;
int Single_flag=0;
int Grid_flag=1;
int Lines=1;
int Points=0;
int Hscroll=0;
int Vscroll=0;
int line_X_flag=0;
int line_Y_flag=0;
int chose_A_B=0;
static WM_HWIN settingwin;
//setting_flag  *set_flag;

static const GUI_WIDGET_CREATE_INFO _childDialogCreate[] = {
	{FRAMEWIN_CreateIndirect, "setting", 0, 80, 0, 400, 270, FRAMEWIN_CF_MOVEABLE},
	//
	// {TEXT_CreateIndirect, "Spacing X:", 0, 5, 210, 50, 20},       //
	// {TEXT_CreateIndirect, "Spacing Y:", 0, 5, 230, 50, 20},       //
	{CHECKBOX_CreateIndirect, 0, GUI_ID_CHECK1, 10, 10, 50, 0},	 // 405
	{CHECKBOX_CreateIndirect, 0, GUI_ID_CHECK2, 10, 30, 50, 0},	 //
	{CHECKBOX_CreateIndirect, 0, GUI_ID_CHECK3, 10, 50, 50, 0},	 //
	{CHECKBOX_CreateIndirect, 0, GUI_ID_CHECK4, 10, 70, 50, 0},	 //
	{CHECKBOX_CreateIndirect, 0, GUI_ID_CHECK5, 10, 90, 50, 0},	 //
	{CHECKBOX_CreateIndirect, 0, GUI_ID_CHECK6, 10, 110, 50, 0}, //
	{CHECKBOX_CreateIndirect, 0, GUI_ID_CHECK7, 290, 25, 60, 0}, //
	{CHECKBOX_CreateIndirect, 0, GUI_ID_CHECK8, 290, 65, 60, 0}, //
	{DROPDOWN_CreateIndirect, "sample", GUI_ID_DROPDOWN0, 70, 10, 90, 60, 0},
	{DROPDOWN_CreateIndirect, "line_step", GUI_ID_DROPDOWN1, 70, 40, 90, 60, 0},
	{TEXT_CreateIndirect, "Sample", 0, 170, 10, 40, 20},
	{TEXT_CreateIndirect, "line_step", 0, 170, 40, 50, 20},
	//{BUTTON_CreateIndirect, "", GUI_ID_BUTTON0, 230, 200, 36, 18}, //
	{ RADIO_CreateIndirect,0, GUI_ID_RADIO0, 230, 10,  60,   0, 0, (30 << 8) | 3 },
	//{BUTTON_CreateIndirect, "test", GUI_ID_BUTTON1, 355, 210, 36, 18},

};

// setting界面背景回调函数
static void _childbDialog(WM_MESSAGE *pMsg)
{
	WM_HWIN child_handle;
	int Id;
	int NCode;
	int Value;


	WM_HWIN c_hdlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
		case WM_PAINT:
			GUI_SetBkColor(GUI_WHITE);
			GUI_Clear();
		break;

		case WM_INIT_DIALOG:
			child_handle = WM_GetDialogItem(c_hdlg, GUI_ID_CHECK1);
			CHECKBOX_SetText(child_handle, "Single");
			// CHECKBOX_SetState(child_handle, 0);

			child_handle = WM_GetDialogItem(c_hdlg, GUI_ID_CHECK2);
			CHECKBOX_SetText(child_handle, "lines");
			CHECKBOX_SetState(child_handle, 1);

			child_handle = WM_GetDialogItem(c_hdlg, GUI_ID_CHECK3);
			CHECKBOX_SetText(child_handle, "Points");
			CHECKBOX_SetState(child_handle, 0);

			child_handle = WM_GetDialogItem(c_hdlg, GUI_ID_CHECK4);
			CHECKBOX_SetText(child_handle, "Grid");
			CHECKBOX_SetState(child_handle, 1);

			child_handle = WM_GetDialogItem(c_hdlg, GUI_ID_CHECK5);
			CHECKBOX_SetText(child_handle, "HScroll");
			CHECKBOX_SetState(child_handle, 0);

			child_handle = WM_GetDialogItem(c_hdlg, GUI_ID_CHECK6);
			CHECKBOX_SetText(child_handle, "VScroll");
			CHECKBOX_SetState(child_handle, 0);

			child_handle = WM_GetDialogItem(c_hdlg, GUI_ID_CHECK7);
			CHECKBOX_SetText(child_handle, "line_X");
			CHECKBOX_SetState(child_handle, 0);

			child_handle = WM_GetDialogItem(c_hdlg, GUI_ID_CHECK8);
			CHECKBOX_SetText(child_handle, "line_Y");
			CHECKBOX_SetState(child_handle, 0);


			child_handle = WM_GetDialogItem(pMsg->hWin, GUI_ID_DROPDOWN0);
			// DROPDOWN_SetFont(child_handle, &GUI_Font24B_ASCII);
			DROPDOWN_SetAutoScroll(child_handle, 1); //启用自动使用滚动条
			DROPDOWN_SetScrollbarWidth(child_handle, 15);
			DROPDOWN_AddString(child_handle, "1.5Mhz");
			DROPDOWN_AddString(child_handle, "1Mhz");
			DROPDOWN_AddString(child_handle, "500Khz");
			DROPDOWN_AddString(child_handle, "200Khz");
			DROPDOWN_AddString(child_handle, "100Khz");


			child_handle = WM_GetDialogItem(pMsg->hWin, GUI_ID_DROPDOWN1);
			// DROPDOWN_SetFont(child_handle, &GUI_Font24B_ASCII);
			DROPDOWN_SetAutoScroll(child_handle, 1); //启用自动使用滚动条
			DROPDOWN_SetScrollbarWidth(child_handle, 15);
			DROPDOWN_AddString(child_handle, "1");
			DROPDOWN_AddString(child_handle, "2");
			DROPDOWN_AddString(child_handle, "3");
			DROPDOWN_AddString(child_handle, "4");
			DROPDOWN_AddString(child_handle, "5");
			DROPDOWN_AddString(child_handle, "6");
			DROPDOWN_AddString(child_handle, "7");
			DROPDOWN_AddString(child_handle, "8");
			DROPDOWN_AddString(child_handle, "9");
			DROPDOWN_AddString(child_handle, "10");

			child_handle = WM_GetDialogItem(pMsg->hWin, GUI_ID_RADIO0);
			RADIO_SetText(child_handle, "line_A", 0);
			RADIO_SetText(child_handle, "line_B", 1);
			RADIO_SetText(child_handle, "A & B", 2);

			
		break;

		case WM_NOTIFY_PARENT:
			Id = WM_GetId(pMsg->hWinSrc); 
			NCode = pMsg->Data.v;
			switch (Id)
			{

				case GUI_ID_DROPDOWN0:
					switch (NCode)
					{
					case WM_NOTIFICATION_CLICKED:
						break;
					case WM_NOTIFICATION_RELEASED:
						break;
					case WM_NOTIFICATION_SEL_CHANGED:
						//Close_ADC();
						c_hdlg = WM_GetDialogItem(pMsg->hWin, GUI_ID_DROPDOWN0);
						Value = DROPDOWN_GetSel(c_hdlg);
						dropdown_flag=(int)Value;
						TIM1_Config(dropdown_flag);


						//可能有些芯片到这一步需要重新初始化一下ADC
						//Rheostat_Init();
						//GUI_DispDecAt(dropdown_flag, 460, 200, 1);
						//Open_ADC();
						//set_flag->dropdown_vlaue=Value;不能使用结构体，会卡住，不知道为什么
						break;
					}
				break;

				case GUI_ID_DROPDOWN1:
					switch (NCode)
					{
					case WM_NOTIFICATION_CLICKED:
						break;
					case WM_NOTIFICATION_RELEASED:
						break;
					case WM_NOTIFICATION_SEL_CHANGED:
						c_hdlg = WM_GetDialogItem(pMsg->hWin, GUI_ID_DROPDOWN1);
						drop1_step_value = DROPDOWN_GetSel(c_hdlg)+1;   //加一是因为得到的值是从0开始
						//GUI_DispDecAt(drop1_step_value, 460, 150, 2);
						break;
					}
				break;

				//Single
				case GUI_ID_CHECK1:
					switch (NCode)
					{
					case WM_NOTIFICATION_VALUE_CHANGED:
						Single_flag=CHECKBOX_IsChecked(WM_GetDialogItem(c_hdlg, Id));
						break;
					}
				break;

				//lines
				case GUI_ID_CHECK2:
					switch (NCode)
					{
					case WM_NOTIFICATION_VALUE_CHANGED:
						Lines=CHECKBOX_IsChecked(WM_GetDialogItem(c_hdlg, Id));
						break;
					}
				break;

				//Points
				case GUI_ID_CHECK3:
					switch (NCode)
					{
					case WM_NOTIFICATION_VALUE_CHANGED:
						Points = CHECKBOX_IsChecked(WM_GetDialogItem(c_hdlg, Id));
						break;
					}
				break;
				
				//Grid
				case GUI_ID_CHECK4:
					switch (NCode)
					{
					case WM_NOTIFICATION_VALUE_CHANGED:
						Grid_flag = CHECKBOX_IsChecked(WM_GetDialogItem(c_hdlg, Id));
						break;
					}
				break;
				
				//Hscroll
				case GUI_ID_CHECK5:
					switch (NCode)
					{
					case WM_NOTIFICATION_VALUE_CHANGED:
						Hscroll = CHECKBOX_IsChecked(WM_GetDialogItem(c_hdlg, Id));
						break;
					}
				break;
				
				//Vscroll
				case GUI_ID_CHECK6:
					switch (NCode)
					{
					case WM_NOTIFICATION_VALUE_CHANGED:
						Vscroll = CHECKBOX_IsChecked(WM_GetDialogItem(c_hdlg, Id));
						break;
					}
				break;

				//测量线X
				case GUI_ID_CHECK7:
					switch (NCode)
					{
					case WM_NOTIFICATION_VALUE_CHANGED:
						line_X_flag = CHECKBOX_IsChecked(WM_GetDialogItem(c_hdlg, Id));
						break;
					}
				break;

				//测量线Y
				case GUI_ID_CHECK8:
					switch (NCode)
					{
					case WM_NOTIFICATION_VALUE_CHANGED:
						line_Y_flag = CHECKBOX_IsChecked(WM_GetDialogItem(c_hdlg, Id));
						break;
					}
				break;


				case GUI_ID_RADIO0:
					switch (NCode)
					{
					case WM_NOTIFICATION_VALUE_CHANGED:
						chose_A_B = RADIO_GetValue(WM_GetDialogItem(c_hdlg, Id));
						GUI_DispDecAt(chose_A_B, 460, 120, 2);
						break;
					}
				break;

			}
		break;

		default:
		WM_DefaultProc(pMsg);
	}
}

//隐藏窗口
void _HideWindow(void)
{
	WM_HideWindow(settingwin);
}

//显示窗口
void _ShowWindow(void)
{
	if (settingwin != 0)
	{
		// if (WM_IsVisible(settingwin) == 0) {
		WM_ShowWindow(settingwin);
		WM_Paint(settingwin);
		//}
	}
}

void _DeleteWindow(void)
{

	WM_DeleteWindow(settingwin);
}

WM_HWIN setting_init(void)
{
	WM_HWIN winset;
	FRAMEWIN_Handle _hFrame;

	winset = GUI_CreateDialogBox(_childDialogCreate, GUI_COUNTOF(_childDialogCreate), _childbDialog, 0, 0, 0);
	_hFrame = winset;
	settingwin = winset;
	// FRAEWIN_SetClientColor(_hFrame, GUI_INVALID_COLOR);
	FRAMEWIN_AddMinButton(_hFrame, FRAMEWIN_BUTTON_RIGHT, 0);

	
	return winset;
}

//设置界面初始化
void setting_demo(void)
{
	WM_EnableMemdev(WM_HBKWIN);

	setting_init();
	GUI_Delay(5);
}
