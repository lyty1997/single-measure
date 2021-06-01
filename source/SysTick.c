/**********************************
系统滴答定时器，在每个数据帧之间中断延时
**********************************/

#include "SysTick.h"
#include "misc.h"


static u8  fac_ns=0;							//ns延时倍乘数			   

//初始化延迟函数
//SYSTICK的时钟固定为AHB时钟
//SYSCLK:系统时钟频率
void SysTick_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	fac_ns=SystemCoreClock/84000000;						//大约12ns   
}								    
 
 
//延时nus
//nus为要延时的us数.		    								   
void delay_ns(u32 nns)
{		
	u32 temp;	    	 
	SysTick->LOAD=nns*fac_ns; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	 
}


/****************
软件查询延时，参数8为一个SPI通信周期96ns
****************/
//void SysTick_Delay_Us( __IO uint32_t ns)
//{
// uint32_t i;
// SysTick_Config(2);		// 12ns

// for (i=0; i<ns; i++) 
//	{
// // 当计数器的值减小到0 的时候，CRTL 寄存器的位16 会置1
// while ( !((SysTick->CTRL)&(1<<16)) );
//  }
// // 关闭SysTick 定时器
// SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
//}

/********************* end file *******************************/

