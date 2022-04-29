#ifndef _GRAHPYT_H
#define _GRAHPYT_H



////////////////////////////////////////////////////////////////////////////////// 	
// typedef struct 
// {
// 	int *buffer;				        //AD数据存放区
// 	u8 	adflag;					        //AD采集是否完成标志
// 	int dataxsize,dataysize;	        //示波器数据区X,Y大小
// 	// u8 	linecolorindex,backcolorindex;	//线条颜色索引值和背景颜色索引值
// 	// GUI_COLOR linecolor,backcolor;		//线条颜色和背景颜色
// 	GRAPH_DATA_Handle  graphdata; 	    //GRAHP_DATA的数据句柄
// 	int div;					        //一格代表几秒
//     float resolut;                      //每个AD原始值代表多大的电压(单位为mv)
// }_oscill_dev;




//extern _oscill_dev oscilldev;

extern void myGraphyt_demo(void); 
#endif
