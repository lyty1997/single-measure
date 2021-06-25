#ifndef __GPIO_INIT
#define __GPIO_INIT

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

/* -------------------------------- 引脚定义 --------------------------------- */
//使能SPI
//IIC_EN使能
#define IIC_EN_PIN                  GPIO_Pin_1                 
#define IIC_EN_GPIO_PORT            GPIOB                      
#define IIC_EN_GPIO_CLK             RCC_AHB1Periph_GPIOB

//（0，0）
// #define LED2_PIN                  GPIO_Pin_2                 
// #define LED2_GPIO_PORT            GPIOA                      
// #define LED2_GPIO_CLK             RCC_AHB1Periph_GPIOA

//（0，1）
// #define LED3_PIN                  GPIO_Pin_3                 
// #define LED3_GPIO_PORT            GPIOA                       
// #define LED3_GPIO_CLK             RCC_AHB1Periph_GPIOA

/* -------------------------------------------------------------------------- */
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			 {p->BSRRL=i;}		//设置为高电平
#define digitalLo(p,i)			 {p->BSRRH=i;}		//输出低电平
#define digitalToggle(p,i)	 {p->ODR ^=i;}		//输出反转状态

/* -------------------------------- 引脚定义 --------------------------------- */
/* 定义控制IO的宏 */
#define IIC_EN_TOGGLE		digitalToggle(IIC_EN_GPIO_PORT,IIC_EN_PIN)
#define IIC_EN_ON			digitalHi(IIC_EN_GPIO_PORT,IIC_EN_PIN)
#define IIC_EN_OFF				digitalLo(IIC_EN_GPIO_PORT,IIC_EN_PIN)

//使能SPI1
#define SPI_EN      IIC_EN_OFF

//定义控制阵列开关的宏IO
// #define LED2_TOGGLE		digitalToggle(LED2_GPIO_PORT,LED2_PIN)
// #define LED2_OFF			digitalHi(LED2_GPIO_PORT,LED2_PIN)
// #define LED2_ON				digitalLo(LED2_GPIO_PORT,LED2_PIN)

// #define LED3_TOGGLE		digitalToggle(LED3_GPIO_PORT,LED3_PIN)
// #define LED3_OFF			digitalHi(LED3_GPIO_PORT,LED3_PIN)
// #define LED3_ON				digitalLo(LED3_GPIO_PORT,LED3_PIN)
/* -------------------------------------------------------------------------- */
void IIC_EN_Config(void);
void MULTI_GPIO_Config(void);


#endif /* __GPIO_INIT */