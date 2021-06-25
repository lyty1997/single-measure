/**
  ******************************************************************************
  * @file    gpio_init.h
  * @author  Yangtianyu Lu
  * @version V1.0
  * @date    07-June-2021
  * @brief   初始化所有用到的GPIO  
  ******************************************************************************
  * @attention
  *
  * 首先是几个芯片的使能引脚，包括PCAP的SPI使能
  * 后面是阵列的使能
  *
  ******************************************************************************
  */

#include "gpio_init.h"

 /**
  * @brief  初始化控制IIC_EN的引脚PB1
  * @param  无
  * @retval 无
  */
void IIC_EN_Config(void){
    /*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启IIC_EN的GPIO外设时钟*/
		RCC_AHB1PeriphClockCmd ( IIC_EN_GPIO_CLK, ENABLE); 

		/*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = IIC_EN_PIN;	

		/*设置引脚模式为输出模式*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*设置引脚为上拉模式*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

		/*设置引脚速率为2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
		GPIO_Init(IIC_EN_GPIO_PORT, &GPIO_InitStructure);	
}

 /**
  * @brief  初始化控制阵列控制引脚
  * @param  无
  * @retval 无
  */
// void MULTI_GPIO_Config(void){
//     /*定义一个GPIO_InitTypeDef类型的结构体*/
// 		GPIO_InitTypeDef GPIO_InitStructure;

// 		/*开启LED相关的GPIO外设时钟*/
// 		RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|
// 	                           LED2_GPIO_CLK|
// 	                           LED3_GPIO_CLK, ENABLE); 

// 		/*选择要控制的GPIO引脚*/															   
// 		GPIO_InitStructure.GPIO_Pin = LED1_PIN;	

// 		/*设置引脚模式为输出模式*/
// 		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
//     /*设置引脚的输出类型为推挽输出*/
//     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
//     /*设置引脚为上拉模式*/
//     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

// 		/*设置引脚速率为2MHz */   
// 		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

// 		/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
// 		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
    
//     /*选择要控制的GPIO引脚*/															   
// 		GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
//     GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
    
//     /*选择要控制的GPIO引脚*/															   
// 		GPIO_InitStructure.GPIO_Pin = LED3_PIN;	
//     GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	
// }