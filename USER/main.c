#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "tftlcd.h"
#include "adc.h"
#include "timer.h"
#include "sdram.h"
#include "malloc.h"
#include "touch.h"
#include "GUI.h"
#include "WM.h"
#include "graphyt.h"
#include "includes.h"
#include "DIALOG.h"
#include "dac.h"
#include "setting.h"
/*
基于STM32的示波器
作者：1805140338-蔡皓
建议使用VScode进行开发，用VScode的快捷键真的很舒服。花个30分钟看一下快捷键，有很多教程可以在网上查看，这方面我是看的B站和知乎。学习的网站很多不要在CSDN上吊死。
ctrl + / 快捷行注释，再按一下取消
ctrl + k ，ctrl + O   折叠代码
ctrl + k ，ctrl + j   展开代码
alt + shift + up（down）向上或向下复制一整行
ctrl + left（right）以单词向左向右移动
alt + shift + F  自动格式化代码   
微软在2022年3月份已经宣布将在未来进入嵌入式市场。多一嘴，MDK在3月份宣布KEIL 5有社区免费版，不再限制文件大小。
（一），做项目时注意做好备份，不是光为了防止丢失，之前我添加一个功能删改了很多，导致之前做的很多都不能用了。
找之前的版本时，发现忘了备份。导致在一个已经解决的问题上浪费了4天，真的很折磨。建议去学习以下GIT，不用2小时就会，VScode还不用手动敲命令。
（二），学习STM32时可以去看野火和正点原子的视频，不用纠结看哪个，这两个互相补充。有些虽然学了但是不一定会用，有些需要用但是不用学的那么细致。
（三），学习UcosIII不需要学习那么深入，更多的时需要会怎么用，源代码很多，学习的话需要很多时间。
（四），学习EMWIN一定要了解回调机制和通知这两个是如何实现的，过来人的眼泪。写代码时旁边务必打开EMWIN的官方手册。建议多看看官方的仿真代码很有学习意义。
（五），学习示波器可以先去看看别人的案例，我找了差不多有10多个，其中有个armfly公司做的F4的示波器很有参考意义。我定时器控制ADC的采样就是参考他们的。
他们的代码很多很多，我有些看不懂。有兴趣可以看看效果很赞。
（六），如果时间充分可以尝试用EMWIN的2D图形来做示波器界面，这样自定义程度很高。如果象我一样时间不够了，可以用窗口的控件（按钮，复选框……）速度更快。
讲一下EMWIN的几个我踩的坑，EMWIN中graph插件Y-T不可以自定义线条的粗细和长短。Y-X模式不可以自动刷新。
（七），代码要记得注释，不是为了给别人看，即使是自己写的，如果好几天不看很快就会忘的。
************************************************/

//任务优先级
#define START_TASK_PRIO				3
//任务堆栈大小	
#define START_STK_SIZE 				128
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//TOUCH任务
//设置任务优先级
#define TOUCH_TASK_PRIO				4
//任务堆栈大小
#define TOUCH_STK_SIZE				128
//任务控制块
OS_TCB TouchTaskTCB;
//任务堆栈
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch任务
void touch_task(void *p_arg);

//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO 				6
//任务堆栈大小
#define LED0_STK_SIZE				128
//任务控制块
OS_TCB Led0TaskTCB;
//任务堆栈
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
//led0任务
void led0_task(void *p_arg);

//EMWINDEMO任务
//设置任务优先级
#define EMWINDEMO_TASK_PRIO			5
//任务堆栈大小
#define EMWINDEMO_STK_SIZE			512
//任务控制块
OS_TCB EmwindemoTaskTCB;
//任务堆栈
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];


//emwindemo_task任务
void emwindemo_task(void *p_arg);


int main(void)
{
    OS_ERR err;
	CPU_SR_ALLOC();
    
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz   
    HAL_Init();                     //初始化HAL库
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART
    LED_Init();                     //初始化LED 
    KEY_Init();                     //初始化按键
	MYDMA_Config(DMA1_Stream5,DMA_CHANNEL_7);//DMA的初始化函数
	TIM2_Init(2,2);                 //初始化定时器，DMA模式下要开启外部中断触发的设置
	TIM1_Config(0);					
	DAC1_Init(); 
    Rheostat_Init();                  //初始化ADC
    SDRAM_Init();                   //SDRAM初始化
    TFTLCD_Init();  		        //LCD初始化
    TP_Init();				        //触摸屏初始化
    my_mem_init(SRAMIN);		    //初始化内部内存池
	my_mem_init(SRAMEX);		    //初始化外部内存池
	my_mem_init(SRAMCCM);		    //初始化CCM内存池
    
    OSInit(&err);		            //初始化UCOSIII
	OS_CRITICAL_ENTER();            //进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	            //退出临界区	 
	OSStart(&err);                  //开启UCOSIII
	while(1);
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
	#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&err);  	//统计任务                
	#endif
		
	#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
		CPU_IntDisMeasMaxCurReset();	
	#endif

	#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
		//使能时间片轮转调度功能,设置默认的时间片长度
		OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
	#endif		
	__HAL_RCC_CRC_CLK_ENABLE();		//使能CRC时钟
    WM_SetCreateFlags(WM_CF_MEMDEV); //启动所有窗口的存储设备
	GUI_Init();  			//STemWin初始化
	WM_MULTIBUF_Enable(1);  //开启STemWin多缓冲,RGB屏可能会用到
	OS_CRITICAL_ENTER();	//进入临界区
	//STemWin Demo任务	
	OSTaskCreate((OS_TCB*     )&EmwindemoTaskTCB,		
				 (CPU_CHAR*   )"Emwindemo task", 		
                 (OS_TASK_PTR )emwindemo_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )EMWINDEMO_TASK_PRIO,     
                 (CPU_STK*    )&EMWINDEMO_TASK_STK[0],	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE/10,	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//触摸屏任务
	OSTaskCreate((OS_TCB*     )&TouchTaskTCB,		
				 (CPU_CHAR*   )"Touch task", 		
                 (OS_TASK_PTR )touch_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TOUCH_TASK_PRIO,     
                 (CPU_STK*    )&TOUCH_TASK_STK[0],	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE/10,	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);			 
	//LED0任务
	OSTaskCreate((OS_TCB*     )&Led0TaskTCB,		
				 (CPU_CHAR*   )"Led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK*    )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);

	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//退出临界区
}

//EMWINDEMO任务
void emwindemo_task(void *p_arg)
{
	OS_ERR err;
	GUI_CURSOR_Show();
	//更换皮肤
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX); 
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	MENU_SetDefaultSkin(MENU_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	while(1)
	{
		myGraphyt_demo();
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时5ms  这个函数里有供ucosIII切换任务的标志，没有它程序会死在这里
	}
}


//TOUCH任务
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时5ms
	}
}

//LED0任务，这个可以很好的反应项目里有没有死循环，如果死循环LED灯就不会闪烁
void led0_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		LED0 = !LED0;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}



