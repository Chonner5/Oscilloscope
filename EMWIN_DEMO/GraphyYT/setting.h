#ifndef _SETTING_H
#define _SETTING_H

//static  int testflag=0;
extern void setting_demo(void);
extern void _ShowWindow(void);
extern void _HideWindow(void);
extern void _DeleteWindow(void);

// typedef struct //设置界面的标志位
// {
//     int check_single;
//     int check_line;
//     int check_points;
//     int check_grid;
//     int dropdown_vlaue;



//     // int dropdown_vlaue;  //

// }setting_flag;

extern int dropdown_flag;
extern int Single_flag;
extern int Grid_flag;
extern int Lines;
extern int Points;
extern int Hscroll;
extern int Vscroll;
extern int line_X_flag;
extern int line_Y_flag;
extern int chose_A_B;
extern int drop1_step_value;

//extern setting_flag   *set_flag;



#endif

