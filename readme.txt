ADC1_clock=(APB2/4)=22.5
t=22.5/(3+12)=1.5   ��������
������������֮��ľ���ʱ��Ϊ1/1.5=0.66***
1.5  0.67*50=33.5us  
1    1*50=50us

ADC���ʱ�Ӳ��ܳ���36M	


static int         _NumPoints;
static const int   _aNumPoints[] = { 46, 25, 13 };
static GUI_POINT   _aPoint[3][46];
GRAPH_DATA_Handle  _ahDataXY[3];

50�����ʾʱ��=(1/sample_time[i]*50)/step_x

sample_time={1500000,1000000,500000,200000,100000}