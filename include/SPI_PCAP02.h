/************************************************

  * @file    SPI_interface.h
  * @author  陆阳天宇
  * @version V1.0
  * @date    2020-09-23
  * @brief   SPI_PCAP02 底层应用头文件 

****************************************************/


// 避免重复定义头文件
#ifndef SPI_PCAP02_H
#define SPI_PCAP02_H

// 添加头文件
#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_rcc.h"
#include <stdio.h>


/************************ 声明PCAP02的私有变量 *************************/
#define LOOP_DLY_100US    2000
#define LOOP_DLY_250ns    2
#define NUMBER_OF_SAMPLES 100
#define FOUR_MILLISECONDS_TIMEOUT 4300
#define TWENTY_MILLISECONDS_TIMEOUT 20300

					
/****************************** PCap02 寄存器地址 + 操作码 *****************************/
//只有I2C需要，SPI不需要
// Standard Device Address Bute 0 | 1 | 0 | 1 | 0 | A1 | A0 | R/W 
				                       	//(R=read=1 and W=write=0)
#define   PicoCap_Device_Address_Read   0x41
#define   PicoCap_Device_Address_Write  0x40		// 4.2章节
					
/******************** 用宏封装操作码，配置见4.1.1 *************************************/
// 初始化
#define   PCap02_Power_Reset          0x88 // 电源整体复位
#define   PCap02_Initialize           0x8A // 复位初始化
#define   PCap02_CDC_Start_Conversion 0x8C // 触发CDC数据转换，软件触发
#define   PCap02_RDC_Start_Conversion 0x8E // 触发RDC数据转换
#define   PCap02_Terminate_Write_OTP  0x84 // 终止向OTP写入数据

// EEPROM Access
#define   Write_To_EEPROM      0xE0 // 写EEPROM
#define   Read_EEPROM          0x60 // 读EEPROM
#define   Block_Write_EEPROM   0xE1 // 块写入
#define   Block_Erase_EEPROM   0xE3 // 块擦除
#define   Erase_EEPROM         0xE2 // 擦除

// SRAM Access
#define   Write_To_SRAM   0x90 // 写SRAM			
#define   Read_SRAM       0x10 // 读SRAM

// OTP Access
#define   Write_to_OTP    0xA0 // 写OTP
#define   Read_OTP        0x20 // 读OTP  

// 写入配置&读取结果
#define   Write_Config    0xC0 // 写入配置
#define   Read_Results    0x40 // 读出结果
/*************************************************************************************/


/************************* 封装几个重要寄存器的地址 *****************************/
#define		RunBit_address					0x4D
#define		Registers_0							0x00
#define		Results_address					0x03
#define		Statu_address						0x24


/****************************** 硬件中断控制 ************************************/
//#define		


/********************** SPI接口定义-开头 ****************************/
//SPI 号及时钟初始化函数
#define PCAP02_SPI                           SPI1
#define PCAP02_SPI_CLK                       RCC_APB2Periph_SPI1
#define PCAP02_SPI_CLK_INIT                  RCC_APB2PeriphClockCmd
//SCK 引脚
#define PCAP02_SPI_SCK_PIN                   GPIO_Pin_5
#define PCAP02_SPI_SCK_GPIO_PORT             GPIOA                       
#define PCAP02_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define PCAP02_SPI_SCK_PINSOURCE             GPIO_PinSource5
#define PCAP02_SPI_SCK_AF                    GPIO_AF_SPI1
//MISO 引脚
#define PCAP02_SPI_MISO_PIN                  GPIO_Pin_6                
#define PCAP02_SPI_MISO_GPIO_PORT            GPIOA                   
#define PCAP02_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define PCAP02_SPI_MISO_PINSOURCE            GPIO_PinSource6
#define PCAP02_SPI_MISO_AF                   GPIO_AF_SPI1
//MOSI 引脚
#define PCAP02_SPI_MOSI_PIN                  GPIO_Pin_7                
#define PCAP02_SPI_MOSI_GPIO_PORT            GPIOA                     
#define PCAP02_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define PCAP02_SPI_MOSI_PINSOURCE            GPIO_PinSource7
#define PCAP02_SPI_MOSI_AF                   GPIO_AF_SPI1
//CS(NSS)引脚
#define PCAP02_CS_PIN               		   GPIO_Pin_4                
#define PCAP02_CS_GPIO_PORT          		   GPIOA                     
#define PCAP02_CS_GPIO_CLK            		 RCC_AHB1Periph_GPIOA
#define PCAP02_SPI_CS_PINSOURCE            GPIO_PinSource4					//复用SPI1模式尝试
//控制CS(NSS)引脚输出低电平
#define SPI_PCAP02_CS_LOW()      {PCAP02_CS_GPIO_PORT->BSRRH=PCAP02_CS_PIN;}
//控制CS(NSS)引脚输出高电平
#define SPI_PCAP02_CS_HIGH()     {PCAP02_CS_GPIO_PORT->BSRRL=PCAP02_CS_PIN;}

//硬件控制触发PG3引脚
#define PCAP02_PG3_PIN										 GPIO_Pin_4
#define PCAP02_PG3_GPIO_PORT           		 GPIOC                     
#define PCAP02_PG3_GPIO_CLK            		 RCC_AHB1Periph_GPIOC
#define PCAP02_PG3_PINSOURCE 		      		 GPIO_PinSource4					
//硬件触发PG3,输出低电平
#define SPI_PCAP02_PG3_LOW()		 {PCAP02_PG3_GPIO_PORT->BSRRH=PCAP02_PG3_PIN;}
//硬件触发PG3,输出高电平
#define SPI_PCAP02_PG3_HIGH()    {PCAP02_PG3_GPIO_PORT->BSRRL=PCAP02_PG3_PIN;}

//来自PCAP02的外部中断INTN引脚
#define PCAP02_INTN_PIN										 GPIO_Pin_5
#define PCAP02_INTN_GPIO_PORT           	 GPIOC                     
#define PCAP02_INTN_GPIO_CLK            	 RCC_AHB1Periph_GPIOC
#define PCAP02_INTN_PINSOURCE 		      	 GPIO_PinSource5

// SPI 使能IIC_EN引脚
#define PCAP02_IIC_EN_PIN										 GPIO_Pin_1
#define PCAP02_IIC_EN_GPIO_PORT           	 GPIOB                     
#define PCAP02_IIC_EN_GPIO_CLK            	 RCC_AHB1Periph_GPIOB
#define PCAP02_IIC_EN_PINSOURCE 		      	 GPIO_PinSource1
/************************* SPI接口定义-结尾 ****************************/


/*************************** 等待超时时间 ******************************/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/***************************** 信息输出 *******************************/
#define PCAP02_DEBUG_ON         1

#define PCAP02_INFO(fmt,arg...)           printf("<<-PCAP02-INFO->> "fmt"\n",##arg)
#define PCAP02_ERROR(fmt,arg...)          printf("<<-PCAP02-ERROR->> "fmt"\n",##arg)
#define PCAP02_DEBUG(fmt,arg...)          do{\
                                          if(PCAP02_DEBUG_ON)\
                                          printf("<<-PCAP02-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)


/******************************** 声明私有函数 ***********************************/
int convert_char2int(int char_input);
void Embed_SRAM_Init(void);
float find_variance_float(uint16_t sample_count);
int convertBinaryToDecimal(long long n);

/************************* SPI总线相关 **************************************/
void GPIOs_Init(void);
void Wait_For_Interrupt(void);
void SPI_PCAP02_Init(void);
uint8_t SPI_PCAP02_ReadByte(void); 
void SPI_PCAP02_SendByte(uint8_t byte);
uint32_t SPI_PCAP02_Status(void);
uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);
uint8_t SPI_PCAP02_Readdata(void);

//******************** 驱动函数,按字节逐步写入数据 **********************
void Write_Incremental(void* bus_type, uint8_t command,
                       uint16_t address, uint8_t *data, uint8_t quantity);
//******************** 驱动函数,按3字节24位逐步写入数据 **********************
uint8_t Send_24Bit_Opcode(void* bus_type, uint8_t command, uint16_t address, uint8_t data);
//******************** 驱动函数,写入RunBit位,调试用 **********************
void Write_RunBit(uint8_t data);
//测试用
void Direct_R_and_W (void);
uint8_t TraPar ( uint8_t PutIn);
																
#endif /* SPI_PCAP02_H */


