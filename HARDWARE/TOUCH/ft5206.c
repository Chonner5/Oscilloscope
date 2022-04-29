#include "ft5206.h"
#include "touch.h"
#include "ctiic.h"
#include "tftlcd.h"
#include "usart.h"
#include "delay.h" 
#include "string.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//7寸电容触摸屏-FT5206 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/28
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
 
 
//向FT5206写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	 
	CT_IIC_Send_Byte(FT_CMD_WR);	//发送写命令 	 
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//发数据
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//产生一个停止条件	    
	return ret; 
}
//从FT5206读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(FT_CMD_WR);   	//发送写命令 	 
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(FT_CMD_RD);   	//发送读命令		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
    CT_IIC_Stop();//产生一个停止条件     
} 
u8 CIP[5]; //用来存放触摸IC-GT911
//初始化FT5206触摸屏
//返回值:0,初始化成功;1,初始化失败
u8 FT5206_Init(void)
{
    u8 temp[5];
    u8 res = 1;
    GPIO_InitTypeDef GPIO_Initure;
 
    __HAL_RCC_GPIOH_CLK_ENABLE();			//开启GPIOH时钟
    __HAL_RCC_GPIOI_CLK_ENABLE();			//开启GPIOI时钟
                
    //PH7
    GPIO_Initure.Pin=GPIO_PIN_7;            //PH7
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);     //初始化
            
    //PI8
    GPIO_Initure.Pin=GPIO_PIN_8;            //PI8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);     //初始化
        
    CT_IIC_Init();      	//初始化电容屏的I2C总线  
    FT_RST=0;				//复位
    delay_ms(20);
    FT_RST=1;				//释放复位		    
    delay_ms(50);  	
    temp[0]=0;
    //读取版本号，参考值：0x3003
    FT5206_RD_Reg(FT_ID_G_LIB_VERSION, &temp[0], 2);

    if ((temp[0] == 0X30 && temp[1] == 0X03) || temp[1] == 0X01 || temp[1] == 0X02) //版本:0X3003/0X0001/0X0002
    {
        printf("CTP ID:%x\r\n", ((u16)temp[0] << 8) + temp[1]);
        FT5206_WR_Reg(FT_DEVIDE_MODE, temp, 1);	//进入正常操作模式
        FT5206_WR_Reg(FT_ID_G_MODE, temp, 1);		//查询模式
        temp[0] = 22;								//触摸有效值，22，越小越灵敏
        FT5206_WR_Reg(FT_ID_G_THGROUP, temp, 1);	//设置触摸有效值
        temp[0] = 12;								//激活周期，不能小于12，最大14
        FT5206_WR_Reg(FT_ID_G_PERIODACTIVE, temp, 1);
        res = 0;
    }
    else
    {
        GT9147_RD_Reg(GT_PID_REG, temp, 4); //读取产品ID
        temp[4] = 0;
        printf("CTP ID:%s\r\n", temp);  //打印ID
        memcpy(CIP, temp, sizeof(u8) * 4);
        temp[0] = 0X02;
        GT9147_WR_Reg(GT_CTRL_REG, temp, 1); //软复位GT9XXX
        GT9147_RD_Reg(GT_CFGS_REG, temp, 1); //读取GT_CFGS_REG寄存器
        delay_ms(10);
        temp[0] = 0X00;
        GT9147_WR_Reg(GT_CTRL_REG, temp, 1); //结束复位, 进入读坐标状态
        res = 0;
    }

    return res;
}
const u16 FT5206_TPX_TBL[5] = {FT_TP1_REG, FT_TP2_REG, FT_TP3_REG, FT_TP4_REG, FT_TP5_REG};
//GT911属于GT9xx系列，所以直接调用gt9147的相关宏定义和调用相关函数
const u16 GT911_TPX_TBL[5] = {GT_TP1_REG, GT_TP2_REG, GT_TP3_REG, GT_TP4_REG, GT_TP5_REG};
u8 g_gt_tnum = 5;    //默认支持的触摸屏点数(5点触摸)
//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
u8 FT5206_Scan(u8 mode)
{
    u8 buf[4];
    u8 i = 0;
    u8 res = 0;
    u8 temp;
    u16 tempsta;
    static u8 t = 0; //控制查询间隔,从而降低CPU占用率
    t++;

    if ((t % 10) == 0 || t < 10) //空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
    {
        if (strcmp((char *)CIP, "911") == 0) //触摸IC 911
        {
            GT9147_RD_Reg(GT_GSTID_REG, &mode, 1); //读取触摸点状态

            if ((mode & 0X80) && ((mode & 0XF) <= g_gt_tnum))
            {
                i = 0;
                GT9147_WR_Reg(GT_GSTID_REG, &i, 1); //清标志
            }
        }
        else
        {
            FT5206_RD_Reg(FT_REG_NUM_FINGER, &mode, 1); //读取触摸点的状态
        }

        if ((mode & 0XF) && ((mode & 0XF) <= g_gt_tnum))
        {
            temp = 0XFF << (mode & 0XF); //将点的个数转换为1的位数,匹配tp_dev.sta定义
            tempsta = tp_dev.sta;  //保存当前的tp_dev.sta值
            tp_dev.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;
            tp_dev.x[g_gt_tnum - 1] = tp_dev.x[0]; //保存触点0的数据,保存在最后一个上
            tp_dev.y[g_gt_tnum - 1] = tp_dev.y[0];

            for (i = 0; i < g_gt_tnum; i++)
            {
                if (tp_dev.sta & (1 << i))   //触摸有效?
                {
                    if (strcmp((char *)CIP, "911") == 0) //触摸IC 911
                    {
                        GT9147_RD_Reg(GT911_TPX_TBL[i], buf, 4); //读取XY坐标值

                        if (tp_dev.touchtype & 0X01) //横屏
                        {
                            tp_dev.y[i] = lcddev.height - (((u16)(buf[3] & 0X0F) << 8) + buf[2]);
                            tp_dev.x[i] = lcddev.width - (((u16)(buf[1] & 0X0F) << 8) + buf[0]);

                        }
                        else /*  */
                        {
                            tp_dev.x[i] = ((u16)buf[3] << 8) + buf[2];
                            tp_dev.y[i] = lcddev.height - (((u16)buf[1] << 8) + buf[0]);
                        }
                    }
                    else
                    {
                        FT5206_RD_Reg(FT5206_TPX_TBL[i], buf, 4);	//读取XY坐标值

                        if (tp_dev.touchtype & 0X01) //横屏
                        {
                            tp_dev.y[i] = ((u16)(buf[0] & 0X0F) << 8) + buf[1];
                            tp_dev.x[i] = ((u16)(buf[2] & 0X0F) << 8) + buf[3];
                        }
                        else
                        {
                            tp_dev.x[i] = lcddev.width - (((u16)(buf[0] & 0X0F) << 8) + buf[1]);
                            tp_dev.y[i] = ((u16)(buf[2] & 0X0F) << 8) + buf[3];
                        }
                    }

                    //printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
                }
            }

            res = 1;

            if (tp_dev.x[0] > lcddev.width || tp_dev.y[0] > lcddev.height) //非法数据(坐标超出了)
            {
                if ((mode & 0XF) > 1) // 有其他点有数据,则复第二个触点的数据到第一个触点.
                {
                    tp_dev.x[0] = tp_dev.x[1];
                    tp_dev.y[0] = tp_dev.y[1];
                    t = 0; // 触发一次,则会最少连续监测10次,从而提高命中率
                }
                else        // 非法数据,则忽略此次数据(还原原来的)
                {
                    tp_dev.x[0] = tp_dev.x[g_gt_tnum - 1];
                    tp_dev.y[0] = tp_dev.y[g_gt_tnum - 1];
                    mode = 0X80;
                    tp_dev.sta = tempsta; // 恢复tp_dev.sta
                }
            }
            else t = 0;    // 触发一次,则会最少连续监测10次,从而提高命中率
        }
    }

    if ((mode & 0X1F) == 0) //无触摸点按下
    {
        if (tp_dev.sta & TP_PRES_DOWN)	//之前是被按下的
        {
            tp_dev.sta &= ~(1 << 7);	//标记按键松开
        }
        else     //之前就没有被按下
        {
            tp_dev.x[0] = 0xffff;
            tp_dev.y[0] = 0xffff;
            tp_dev.sta &= 0XE0;	//清除点有效标记
        }
    }

    if (t > 240)t = 10; //重新从10开始计数

    return res;
}

