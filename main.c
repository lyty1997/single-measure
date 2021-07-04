/******************** (C) COPYRIGHT 2008 STMicroelectronics *********************

 * File Name          : main.c
 * Author             : 陆阳天宇， 西安交通大学机械工程
 * Version            : 
 * Date               : 2020-9-20
 * Description        : STM32F407开发板通过SPI控制读取PCap02AE
 ******************************************************************************/
  
#include "stm32f4xx.h"		
#include "gpio_init.h"
#include "bsp_led.h"
#include "bsp_debug_usart.h"
#include "SPI_PCAP02.h"		 //声明了私有函数

#include <math.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h> 


// int i;
// int j;

/**************** 定义PCAP02的私有变量 ******************************/

uint32_t   CriticalSecCntr;

bool     configured_true = false;
uint8_t  Rx_data1;
uint8_t  Rx_data2;
uint8_t  Rx_data3;

float     *sram_float_memory  = ((float *)(SRAM_BASE + 0xB00));
uint32_t  *sram_uint32_t_memory = ((uint32_t *)(SRAM_BASE + 0xB00));
uint32_t  sram_mem_offset     = 0x0;


uint8_t   PicoCap_Hex_Firmware[8192];  // 为固件库分配 8k 大小的内存
uint32_t  Cfg_Register[26];
uint16_t  Install_Byte_Limit = 4096;   // max. 4096
uint16_t  main_index = 0;
uint16_t  hex_index = 0;
uint16_t  cfg_index = 0;

//char * HexFile = "PCap02AEstandard.hex";		
//Note：hex文件放在main.c源文件同路径下，否则修改以上语句路径
/***********************************************************************************/


/********************************* 标准配置 ****************************************/
uint8_t PCap02_standard_config_reg[78] = {
          0x0F,0x00,0x01,   // Config Register  0,  1,  2,
          0x14,0x01,0x14,   // Config Register  3,  4,  5,
          0x04,0x04,0xA8,   // Config Register  6,  7,  8,
          0x02,0x10,0x00,   // Config Register  9, 10, 11,
          0x03,0x01,0x00,   // Config Register 12, 13, 14,
          0x00,0x00,0xD0,   // Config Register 15, 16, 17,
          0x07,0x00,0x00,   // Config Register 18, 19, 20,
          0x00,0x00,0x02,   // Config Register 21, 22, 23,
          0x04,0x00,0x00,   // Config Register 24, 25, 26,
          0x01,0x40,0x05,   // Config Register 27, 28, 29,
          0x01,0x00,0x00,   // Config Register 30, 31, 32,
          0x00,0x43,0x07,   // Config Register 33, 34, 35,
          0x00,0x00,0x50,   // Config Register 36, 37, 38,
          0x00,0x00,0x00,   // Config Register 39, 40, 41,
          0x00,0x44,0x00,   // Config Register 42, 43, 44,
          0x00,0xFF,0x00,   // Config Register 45, 46, 47,
          0x27,0x30,0x00,   // Config Register 48, 49, 50,
          0x00,0x00,0x00,   // Config Register 51, 52, 53,
          0x00,0x00,0x00,   // Config Register 54, 55, 56,
          0x00,0x00,0x00,   // Config Register 57, 58, 59,
          0x00,0x00,0x00,   // Config Register 60, 61, 62,
          0x00,0x00,0x00,   // Config Register 63, 64, 65,
          0x00,0x00,0x00,   // Config Register 66, 67, 68,
          0x00,0x00,0x00,   // Config Register 69, 70, 71,
          0x00,0x28,0x01,   // Config Register 72, 73, 74,
          0x00,0x00,0x01};  // Config Register 75, 76, 77
/*************************************************************************************/
					
/********************************* 套件修改配置1 ****************************************/
uint8_t PCap02_modify_config1_reg[78] = {
          0x0F,0x00,0x01,   // Config Register  0,  1,  2,
          0x94,0x80,0x05,   // Config Register  3,  4,  5,
          0x01,0x04,0xA8,   // Config Register  6,  7,  8,
          0x02,0x10,0x00,   // Config Register  9, 10, 11,
          0x05,0x01,0x00,   // Config Register 12, 13, 14,
          0x00,0x00,0x09,   // Config Register 15, 16, 17,
          0x00,0x00,0x00,   // Config Register 18, 19, 20,
          0x00,0x00,0x01,   // Config Register 21, 22, 23,
          0x08,0x00,0x00,   // Config Register 24, 25, 26,
          0x01,0x40,0x05,   // Config Register 27, 28, 29,
          0x02,0x00,0x00,   // Config Register 30, 31, 32,
          0x00,0x43,0x01,   // Config Register 33, 34, 35,
          0x00,0x00,0x50,   // Config Register 36, 37, 38,
          0x00,0x00,0x00,   // Config Register 39, 40, 41,
          0x00,0x44,0x00,   // Config Register 42, 43, 44,
          0x00,0xFF,0x00,   // Config Register 45, 46, 47,
          0x27,0x30,0x00,   // Config Register 48, 49, 50,
          0x00,0x00,0x00,   // Config Register 51, 52, 53,
          0x00,0x00,0x00,   // Config Register 54, 55, 56,
          0x00,0x00,0x00,   // Config Register 57, 58, 59,
          0x00,0x00,0x00,   // Config Register 60, 61, 62,
          0x00,0x00,0x00,   // Config Register 63, 64, 65,
          0x00,0x00,0x00,   // Config Register 66, 67, 68,
          0x00,0x00,0x00,   // Config Register 69, 70, 71,
          0x00,0x28,0x01,   // Config Register 72, 73, 74,
          0x00,0x00,0x01};  // Config Register 75, 76, 77
/*************************************************************************************/
	
/*************************** 套件修改配置2，低速时钟200kHz，单端，内部电容补偿 ****************************************/
uint8_t PCap02_modify_config2_reg[78] = {
          0x0F,0x00,0x01,   // Config Register  0,  1,  2,
          0x14,0x01,0x14,   // Config Register  3,  4,  5,
          0x04,0x04,0xA8,   // Config Register  6,  7,  8,
          0x02,0x10,0x00,   // Config Register  9, 10, 11,
          0x03,0x01,0x00,   // Config Register 12, 13, 14,
          0x00,0x00,0x28,   // Config Register 15, 16, 17,
          0x00,0x00,0x00,   // Config Register 18, 19, 20,
          0x00,0x00,0x01,   // Config Register 21, 22, 23,
          0x08,0x01,0x00,   // Config Register 24, 25, 26,
          0x01,0x40,0x05,   // Config Register 27, 28, 29,
          0x02,0x00,0x00,   // Config Register 30, 31, 32,
          0x00,0x43,0x07,   // Config Register 33, 34, 35,
          0x00,0x00,0x50,   // Config Register 36, 37, 38,
          0x00,0x00,0x00,   // Config Register 39, 40, 41,
          0x00,0x44,0x00,   // Config Register 42, 43, 44,
          0x00,0xFF,0x00,   // Config Register 45, 46, 47,
          0x27,0x30,0x00,   // Config Register 48, 49, 50,
          0x00,0x00,0x00,   // Config Register 51, 52, 53,
          0x00,0x00,0x00,   // Config Register 54, 55, 56,
          0x00,0x00,0x00,   // Config Register 57, 58, 59,
          0x00,0x00,0x00,   // Config Register 60, 61, 62,
          0x00,0x00,0x00,   // Config Register 63, 64, 65,
          0x00,0x00,0x00,   // Config Register 66, 67, 68,
          0x00,0x00,0x00,   // Config Register 69, 70, 71,
          0x00,0x28,0x01,   // Config Register 72, 73, 74,
          0x00,0x00,0x01};  // Config Register 75, 76, 77
/*************************************************************************************/
					
/********************** 套件修改配置3，低速时钟200kHz，浮动，无电容补偿 ****************************************/
uint8_t PCap02_modify_config3_reg[78] = {
          0x0F,0x00,0x01,   // Config Register  0,  1,  2,
          0x14,0x01,0x14,   // Config Register  3,  4,  5,
          0x04,0x04,0xB8,   // Config Register  6,  7,  8,
          0x03,0x01,0x00,   // Config Register  9, 10, 11,
          0x0F,0x01,0x00,   // Config Register 12, 13, 14,
          0x00,0x00,0x10,   // Config Register 15, 16, 17,
          0x00,0x00,0x00,   // Config Register 18, 19, 20,
          0x00,0x00,0x01,   // Config Register 21, 22, 23,
          0x08,
	0x01,0x00,   // Config Register 24, 25, 26,
          0x01,0x40,0x05,   // Config Register 27, 28, 29,
          0x02,0x00,0x00,   // Config Register 30, 31, 32,
          0x00,0x43,0x07,   // Config Register 33, 34, 35,
          0x00,0x00,0x50,   // Config Register 36, 37, 38,
          0x00,0x00,0x00,   // Config Register 39, 40, 41,
          0x00,0x44,0x00,   // Config Register 42, 43, 44,
          0x00,0xFF,0x00,   // Config Register 45, 46, 47,
          0x27,0x30,0x00,   // Config Register 48, 49, 50,
          0x00,0x00,0x00,   // Config Register 51, 52, 53,
          0x00,0x00,0x00,   // Config Register 54, 55, 56,
          0x00,0x00,0x00,   // Config Register 57, 58, 59,
          0x00,0x00,0x00,   // Config Register 60, 61, 62,
          0x00,0x00,0x00,   // Config Register 63, 64, 65,
          0x00,0x00,0x00,   // Config Register 66, 67, 68,
          0x00,0x00,0x00,   // Config Register 69, 70, 71,
          0x00,0x28,0x01,   // Config Register 72, 73, 74,
          0x00,0x00,0x01};  // Config Register 75, 76, 77
/*************************************************************************************/
					
/********************** 套件修改配置4，低速时钟200kHz，浮动，外部电容补偿 ****************************************/
uint8_t PCap02_modify_config4_reg[78] = {
          0x0F,0x00,0x01,   // Config Register  0,  1,  2,
          0x14,0x01,0x14,   // Config Register  3,  4,  5,
          0x04,0x04,0xB8,   // Config Register  6,  7,  8,
          0x03,0x21,0x00,   // Config Register  9, 10, 11,
          0x0F,0x01,0x00,   // Config Register 12, 13, 14,
          0x00,0x00,0x14,   // Config Register 15, 16, 17,
          0x00,0x00,0x00,   // Config Register 18, 19, 20,
          0x00,0x00,0x01,   // Config Register 21, 22, 23,
          0x08,
	0x01,0x00,   // Config Register 24, 25, 26,
          0x01,0x40,0x05,   // Config Register 27, 28, 29,
          0x02,0x00,0x00,   // Config Register 30, 31, 32,
          0x00,0x43,0x07,   // Config Register 33, 34, 35,
          0x00,0x00,0x50,   // Config Register 36, 37, 38,
          0x00,0x00,0x00,   // Config Register 39, 40, 41,
          0x00,0x44,0x00,   // Config Register 42, 43, 44,
          0x00,0xFF,0x00,   // Config Register 45, 46, 47,
          0x27,0x30,0x00,   // Config Register 48, 49, 50,
          0x00,0x00,0x00,   // Config Register 51, 52, 53,
          0x00,0x00,0x00,   // Config Register 54, 55, 56,
          0x00,0x00,0x00,   // Config Register 57, 58, 59,
          0x00,0x00,0x00,   // Config Register 60, 61, 62,
          0x00,0x00,0x00,   // Config Register 63, 64, 65,
          0x00,0x00,0x00,   // Config Register 66, 67, 68,
          0x00,0x00,0x00,   // Config Register 69, 70, 71,
          0x00,0x28,0x01,   // Config Register 72, 73, 74,
          0x00,0x00,0x01};  // Config Register 75, 76, 77
/*************************************************************************************/
					
/********************** 套件修改配置5，低速时钟200kHz，单端，无补偿 ****************************************/
uint8_t PCap02_modify_config5_reg[78] = {
          0x0F,0x00,0x01,   // Config Register  0,  1,  2,
          0x14,0x01,0x14,   // Config Register  3,  4,  5,
          0x04,0x04,0xA8,   // Config Register  6,  7,  8,
          0x02,0x00,0x00,   // Config Register  9, 10, 11,
          0x03,0x01,0x00,   // Config Register 12, 13, 14,
          0x00,0x00,0x28,   // Config Register 15, 16, 17,
          0x00,0x00,0x00,   // Config Register 18, 19, 20,
          0x00,0x00,0x01,   // Config Register 21, 22, 23,
          0x08,
	0x01,0x00,   // Config Register 24, 25, 26,
          0x01,0x40,0x05,   // Config Register 27, 28, 29,
          0x02,0x00,0x00,   // Config Register 30, 31, 32,
          0x00,0x43,0x07,   // Config Register 33, 34, 35,
          0x00,0x00,0x50,   // Config Register 36, 37, 38,
          0x00,0x00,0x00,   // Config Register 39, 40, 41,
          0x00,0x44,0x00,   // Config Register 42, 43, 44,
          0x00,0xFF,0x00,   // Config Register 45, 46, 47,
          0x27,0x30,0x00,   // Config Register 48, 49, 50,
          0x00,0x00,0x00,   // Config Register 51, 52, 53,
          0x00,0x00,0x00,   // Config Register 54, 55, 56,
          0x00,0x00,0x00,   // Config Register 57, 58, 59,
          0x00,0x00,0x00,   // Config Register 60, 61, 62,
          0x00,0x00,0x00,   // Config Register 63, 64, 65,
          0x00,0x00,0x00,   // Config Register 66, 67, 68,
          0x00,0x00,0x00,   // Config Register 69, 70, 71,
          0x00,0x28,0x01,   // Config Register 72, 73, 74,
          0x00,0x00,0x01};  // Config Register 75, 76, 77
/*************************************************************************************/
	
/********************** 套件修改配置6，低速时钟200kHz，单端，无补偿，通道6 ****************************************/
uint8_t PCap02_modify_config6_reg[78] = {
          0x0F,0x00,0x01,   // Config Register  0,  1,  2,
          0x94,0x80,0x05,   // Config Register  3,  4,  5,
          0x01,0x04,0xA8,   // Config Register  6,  7,  8,
          0x00,0x10,0x00,   // Config Register  9, 10, 11,
          0x41,0x01,0x00,   // Config Register 12, 13, 14,
          0x00,0x00,0x14,   // Config Register 15, 16, 17,
          0x00,0x00,0x00,   // Config Register 18, 19, 20,
          0x00,0x00,0x02,   // Config Register 21, 22, 23,
          0x08,0x01,0x00,   // Config Register 24, 25, 26,
          0x02,0x40,0x05,   // Config Register 27, 28, 29,
          0x01,0x00,0x00,   // Config Register 30, 31, 32,
          0x00,0x43,0x05,   // Config Register 33, 34, 35,
          0x00,0x00,0x50,   // Config Register 36, 37, 38,
          0x00,0x00,0x00,   // Config Register 39, 40, 41,
          0x00,0x44,0x00,   // Config Register 42, 43, 44,
          0x00,0xFF,0x00,   // Config Register 45, 46, 47,
          0x27,0x30,0x00,   // Config Register 48, 49, 50,
          0x00,0x00,0x00,   // Config Register 51, 52, 53,
          0x00,0x00,0x00,   // Config Register 54, 55, 56,
          0x00,0x00,0x00,   // Config Register 57, 58, 59,
          0x00,0x00,0x00,   // Config Register 60, 61, 62,
          0x00,0x00,0x00,   // Config Register 63, 64, 65,
          0x00,0x00,0x00,   // Config Register 66, 67, 68,
          0x00,0x00,0x00,   // Config Register 69, 70, 71,
          0x00,0x28,0x01,   // Config Register 72, 73, 74,
          0x00,0x00,0x01};  // Config Register 75, 76, 77
/*************************************************************************************/

					
/************************* PCap02参数寄存器配置(Reg 0 .. 77) ****************************/
//每单个寄存器配置都是高位写在前，低位写在后
					uint8_t PCap02_config_reg[78] = 
				 {
          0xFF,   
					 // 寄存器0，启动模式：
					 // 0Bxxxx，AUTOBOOT_DIS：0000 = 独立操作，设备从OTP内存引导自身；
					 // 											1111 = AutoBOOT为从模式，设备请求通过串行总线启动；
					 // 程序内存锁：
					 // 0Bxxxx，MEM_LOCK_DIS：0000 = 防止固件被读取（加密）；
					 //												1111 = 不锁内存；
	        0x00,   
					 // 寄存器1，OTP内部错误检测和修复机制：
					 // 0Bxxxxxxxx，ECC_MODE：h00 = 关闭OTP内部诊断；
					 //												h0F = double模式；
					 //												hF0 = quad模式，内存大小减半到1963byte；
	        0x41,   
					 // 寄存器2：
					 // 0Bx：空，0；
					 // 0Bx，SPI_COLLAVOID_EN：1 = 避免DSP冲突，以及允许SPI寄存器访问许可；
					 // 0Bxx，I2C_ADD：补充IIC头地址；
					 // 0Bxx，MEM_COMP_LENGTH：00 = 关闭SRAM-to-OTP比较机制；
					 // 											01 = 比较5个字节后退出；
					 //												10 = 比较任意33字节后退出；
					 //												11 = 比较所有257个字节后退出；
					 // 0Bxx，OTP_RO_SPEED：OTP晶振速度，01 = 最快；
          0x14,   
					// 寄存器3，低速时钟OLF配置：
					// 0Bxx，OLF_CTUNE：粗调低速时钟；
					//									00 = 200kHz；
					//									01 = 100kHz；
					//									10 = 50kHz（推荐）；
					//									11 = 10kHz；
					// 0Bxxxx，OLF_FTUNE：微调低速时钟，0~15；
					/* OLF_CTUNE			OLF_FTUNE			OLF Frequency
						 11:= (10kHz) 		0001 						5 kHz
						 11:= (10kHz) 		0111 						10 kHz
						 10:= (50kHz) 		0000 						28 kHz
						 10:= (50kHz) 		0011 						48 kHz
						 01:= (100kHz) 		0100 						100 kHz
						 00:= (200kHz) 		0101 						200 kHz						
					*/
					// 0Bx，OX_CLK32KHZ_EN：选择外部时钟，从4MHz or 32kHz；
					// 											0 = 4MHz；
					//											1 = 32kHz；
					// 0Bx，OLF_CLK_SEL：选择低速时钟源；
					//									0 = 选择片上低速时钟源为OLF；
					//									1 = 选择外部32kHz晶振为OLF；
/************ 自己的多路板用32.7kHz外部晶振       高频PLL可不可以到40MHz??? DSP内部最高100MHz*****/
	        0x03,   
					// 寄存器4，外部高速时钟OX/OHF配置：
					// 0Bx，OX_DIS：禁用OX时钟；
					//							0 = 开启；
					//							1 = 禁用；
					// 0Bx，OX_AMP_TRIM：调整OX时钟反馈增益；
					//									0 = 低增益；
					//									1 = 高增益；
					// 0Bx，OX_DIV4：时钟4分频；
					//							 0 = 不分频；
					//							 1 = 4分频；
					// 0Bx，OX_AUTOSTOP_DIS：0；
					// 0Bx，OX_STOP：0；
					// 0Bxxx，OX_RUN：控制脉冲模式的持久性或延迟，延迟是指测量开始前的振荡器预热时间；
					//								000 = 关闭OX时钟；
					//								001 = OX永恒运行；
					//								010 = OX延迟31/低速时钟频率（31倍低速时钟周期）；
					//								011 = OX延迟2/低速时钟频率（2倍低速时钟周期）；
					//								110 = OX延迟1/低速时钟频率（1倍低速时钟周期）；
					0x14,   // 寄存器5：
					// 0Bxx：空，00；
					// 0Bxxxxxx，OCF_TIME：控制恒定时钟OCF频率，服务于EEPROM；
					//										必须使fOCF == 5kHz
					//										fOCF = fOLF/(2*0Dxx);
          0x04,   // 寄存器6，内部高速时钟源配置：
					// 0Bxxxxx：空，00000；
					// 0Bxxx，OHF_CLK_SEL：选择内部高速时钟OHF源；
					/*										001 = 内部时钟源#1
																010 = 内部时钟源#2
																100 = 外部时钟源
					*/
					0x14,   // 寄存器7，差分电容配置：
					// 0Bxxx：空，000；
					// 0Bx，DCHG_DUM_DIS：针对测量差分电容的假充放电，使对称；
					//										0 = 差分电容
					//										1 = 普通电容
					// 0Bx，SCHMITT_CDUM_EN：更好的对称性；
					//											0 = 默认
					//											1 = 差分传感器和假测量模式
					// 0Bx，C_SENSE_INVERT：1 = 推荐使用片上施密特触发，触发器和转换器的翻转水平；
					// 0Bxx，SCHMITT_SEL：00 = 选择片上施密特触发；
					
/******************************* 放电电阻配置 *************************************/
					0x58,   // 寄存器8：
					// 0Bxx，RDCHG_INT_H：选择PC4~PC7的放电电阻；
					/*										00 = 180 kΩ
																01 = 90 kΩ
																10 = 30 kΩ
																11 = 10 kΩ
					*/
					// 0Bxx，RDCHG_INT_L：选择PC0~PC3和PC8、PC9的放电电阻；
					/*										00 = 180 kΩ
																01 = 90 kΩ
																10 = 30 kΩ
																11 = 10 kΩ				
					*/
					// 0Bx，RDCHG_INT_EN：0 = 关闭内部放电电阻；
					//										1 = 启用内部放电电阻；
					// 0Bx：空，0；
					// 0Bx，RDCHG_EXT_EN：0 = 关闭外部放电电阻；
					//										1 = 启用外部放电电阻；
					// 0Bx，RDCHG_1MEG_EN：用内部1MΩ电阻替换为PC0~PC3和PC8、PC9的放电电阻；
          0x1A,   // 寄存器9，电阻配置：
					// 0Bx：空，0；
					// 0Bx，AUX_PD_DIS：取消内部下拉电阻来启用PC_AUX；
					//									0 = 不启用
					//									1 = 启用
					// 0Bx，AUX_CINT：0
					// 0Bx，RDCHG_EARLY_OPEN：1 = 提前打开芯片内部放电电阻
					//												0 = 不打开
					// 0Bx，RDCHG_PERM_EN：1 = 保持芯片内部放电电阻永久连接
					//										0 = 不永久连接
					// 0Bx，RDCHG_EXT_PERM：1 = 启用外部放电电阻
					//											0 = 不启用
					// 0Bxx，RCHG_SEL：选择充电电阻；
					/*								00 = 180 kΩ
														01 = 90 kΩ
														10 = 30 kΩ
														11 = 10 kΩ
					*/
					0x10,   // 寄存器10，补偿配置：
					// 0Bx，C_REF_INT：在PC8和PC9上使用片上参比电容；
					//								0 = 外部参比电容，PC0/GND或PC0/PC1；
					//								1 = 内部参比电容；
					// 0Bx，COMP_R_SEL：选择电阻用于补偿时变产生的寄生并联电阻率；
					//									0 = 90 kΩ
					//									1 = 180 kΩ
					// 0Bx，C_COMP_EXT：1 = 激活片外寄生电容的补偿机制；（仅浮动接法）
					//									0 = 不激活
					// 0Bx，C_COMP_INT：1 = 激活片上寄生电容的补偿机制并进行增益补偿；
					//									0 = 不激活
					// 0Bx，C_COMP_R：1 = 启动寄生并联电阻率缓变补偿机制；
					//								0 = 不启用
					// 0Bx，C_COMP_FORCE：1 = 补偿差动传感器的机械力；（仅差动）
					//											0 = 不补偿
					// 0Bx，C_DIFFERENTIAL：0 = 单端传感器
					//											1 = 差分传感器
					// 0Bx，C_FLOATING：0 = 接地测量
					//									1 = 浮动测量
					0x0C,   // 寄存器11：
					// 0Bxx：空，00；
					// 0Bx，C_PORT_PAT：测量端口的顺序将在每个序列后颠倒；
					//									0 = 正常
					//									1 = 顺序测量后，每个端口的顺序颠倒
					// 0Bx，C_SELFTEST：仅适用于差动传感器的自检；
					//									0 = 关闭
					//									1 = 转换寄存器10中的机械力补偿值C_COMP_FORCE；
					// 0Bxx，CY_CLK_SEL：选择CDC的时钟源；
					/*									00 = OLF_CLK
															01 = 禁止
															10 = OHF_CLK/4
															11 = OHF_CLK
					*/
					// 0Bx，CY_PRE_LONG：增加内部时钟路径之间的安全延迟；
					//									0 = 关闭
					//									1 = 开启
					// 0Bx，C_DC_BALANCE：保持传感器无直流状态；（仅浮动单传感器或者差动传感器）
					//										0 = 关闭
					//										1 = 开启
          0x03,   // 寄存器12，使能CDC端口：
					// 0Hxx，C_PORT_EN：逐位启动CDC端口; 
					/*	16进制					00 = 全部关闭
														01 = PC0
														02 = PC1
														04 = PC2
														08 = PC3
														10 = PC4
														20 = PC5
														40 = PC6
														80 = PC7
						PC0~PC3低4位，PC4~PC7高4位，打开多通道相加即可
						例如，打开PC0和PC1，为01+02=03
									打开PC0,PC1,PC3,为01+02+08=0B
					*/
					0x01, 0x00,  // 寄存器13 & 寄存器14：
					// 0Bxxx：空，000；14的高3位
					// 0Bxxxxxxxxxxxxx，C_AVRG：寄存器13开始低13位设置一次平均后输出结果的采样次数；
					/*							0 or 1 = 无平均
													2 = 2个结果平均
													3 = 3个结果平均
													最多8191个采样结果取平均
					*/
          0x00, 0x00,  // 寄存器15 & 寄存器16：由DSP设置
					// 0Bxxx：空，000；16的高3位
					// 0Bxxxxxxxxxxxxx，C_AVRG_ALT：15开始低13位设置第二次平均后输出结果的采样次数；
					/*							0 or 1 = 无平均
													2 = 2个结果平均
													3 = 3个结果平均
													最多8191个采样结果取平均
					*/
/*************************************** 触发转换周期 **************************************************************/
					0x0A, 0x00, 0x00, // 寄存器17、寄存器18&寄存器19：
					// 0Bx：空，0；19的最高位
					// 0Bxxxxxxxxxx...，CONV_TIME：17开始低23位，转换触发转换周期，Tcon=2*CONV_TIME/fOLF,
					//								Tcon不可小于单次测量的周期

					0x00, 0x00, 0x00, // 寄存器20、寄存器21&寄存器22：
					// 0Bx：空，0；22的最高位；
					// 0Bxxxxxxxxxx...，CONV_TIME_ALT：20开始低23位，第二次设置转换周期，DSP设置两种操作模式，同上；
					
					0x02 ,  // 寄存器23，放电次数：续寄存器24低2位
					// 0Hxx，DISCHARGE_TIME：h00最快，不稳定就向上调整；
					//					放电时间Tdischarge = （DISCHARGE_TIME + 1）*Tcycleclock
					//					0 ： Tdischarge = 1*Tcycleclock
					0xCC,		// 寄存器24，GPIO3引脚上升沿触发CDC测量；
					// 0Bxx，C_STARTONPIN：选择允许通过硬件触发CDC的GPIO端口；
					/*										00 = PG0
																01 = PG1
																10 = PG2
																11 = PG3					
					*/
					// 0Bxx，C_TRIG_SEL_ALT：同下，由DSP控制写入寄存器；
					// 0Bxx，C_TRIG_SEL：选择可用于启动CDC的触发源；
					/*									00 = 拉伸模式
															01 = 连续模式
															10 = 转换触发计时器
															11 = GPIO上升沿触发，硬件触发
															软件触发需要些操作码，强制触发，但是不稳定
					*/
					// 0Bxx，DISCHARGE_TIME：	接寄存器23				
					0x01, 0x00,  // 寄存器25&寄存器26：
					// 0Bxx：空，00；26的高2位
					// 0Bxxxx，C_FAKE：真实采样前的预热的假采样次数，是舍弃不要的次数，设为0；
					// 0Bxxxxxxxxxx，PRECHARGE_TIME：25开始低10位，预充电次数；
					//															0 = 不限流预充电
					//				预充电时间Tprecharge =( PRECHARGE_TIME + 1) * Tcycleclock
          0x01, 0xC4,  // 寄存器27 & 寄存器28：
					// 0Bx，EE_SINGLE_WR_EN：28最高位，EEPROM单个地址写入保护，不影响读取；
					//											0 = 不允许有单独的写操作
					//											1 = 允许写入
					// 0Bx，EE_WR_EN：EEPROM连续写入保护，不影响读取；
					//								0 = 不允许写入或擦除EEPROM
					//								1 = 允许
					// 0Bx，EE_DISABLE：禁用EEPROM；
					//									0 = EEPROM使能
					//									1 = 完全禁用，无法进行读访问
					// 0Bx，EE_IFC_PRIO：内部位，预设0；
					// 0Bx，EE_WAKEUP_MODE：为EEPROM选择读写设置时间；
					//											0 = 1.5倍恒定时钟周期tOCF
					//											1 = 10微妙
					// 0Bx，EE_ON：0 = 仅访问时唤醒，然后立即将其送入睡眠状态，会产生耗时
					//						1 = 永远唤醒EEPROM
					// 0Bxxxxxxxxxx，FULLCHARGE_TIME：27开始到28低2位的10位，设置满充电时间；
					//					满充时间Tfullcharge = (FULLCHARGE_TIME + 1) * Tcycleclock
					//					0 := no fullcharge								

/********************************* RDC配置 ***********************************************/				
					
					0x00,   // 寄存器29：RDC触发；
					// 0Bxx，R_STARTONPIN：不推荐使用，00；
					// 0Bxxx，R_TRIG_SEL_ALT：同下，DSP选择两种模式；
					// 0Bxxx，R_TRIG_SEL：RDC的触发器源选择；
					/*									000 = 关闭
															001 = OLF_CLK
															011 = 硬件管脚触发
															101 = 与CDC异步触发
															110 = 在CDC转换结束后同步触发
					*/
          0xFF, 0xFF, 0x3F, // 寄存器30、寄存器31&32：RDC相对CDC触发倍率；
					// 0Dx，R_TRIG_PREDIV：从30起22位，预分频器，允许进行比电容测量更少的温度测量；
					//										如果在寄存器29设置OLF时钟为触发源，则可作为OLF的分频；
					/*										0 or 1 = 每一次CDC触发后触发
																2 = 没两次CDC触发后触发
																......
																最大2的21次方倍分频
					*/
					// 0Bxx，R_AVRG：寄存器32高2位：RDC平均采样数；
					/*							00 = 无平均
													01 = 4个采样点
													10 = 8个采样点
													11 = 16个采样点
					*/
          0x00,   // 寄存器33：RDC假采样数；
					// 0Bx，R_SENSE_INVERT：0；
					// 0Bx，R_FAKE：真实测量前的预热假测量次数；
					//							0 = 每次平均有2次假测量
					//							1 = 每次平均有8次假测量
					// 0Bxxxxxx，R_QHA_SEL：内部位，000000；
					0x40,   // 寄存器34：
					// 0Bx：空，0；
					// 0Bx，R_REF_SEL：选择RDC的参考；
					/*								0 = 外部PT0
														1 = 内部
					*/
					// 0Bx，R_3EXT_SEL：当且仅当使用内部参考时，允许测量3个外部传感器；
					/*									0 = 少于3个外部传感器，可以使用外部参考
															1 = 3个外部传感器，只能使用内部参考
					*/
					// 0Bx，R_PT1_EN：0 = 禁用PT1
					//								1 = 使能PT1
					// 0Bx，R_PT0_EN：0 = 禁用PT0
					//								1 = 使能PT10
					// 0Bx，R_PT2REF_EN：0 = 禁用PT2REF
					//									1 = 使能PT2REF
					// 0Bx，R_PORT_EN_IMES：0 = 禁用内部温度传感器
					//											1 = 使能内部温度传感器
					// 0Bx，R_PORT_EN_IREF：0 = 禁用内部参考电阻
					//											1 = 使能内部参考电阻
					
					
/****************************** 设置内部参比电容大小 *********************************/
					0x03,   // 寄存器35：
					// 0Bxxxxx，C_REF_SEL：置内部参考电容大小，步宽0.3pF~1.5pF，精度较低；
					//										0D0~0D31 = 0~31pF
					// 0Bx，R_CY：RDC部分的循环时间；
					//						0 = 140微妙
					//						1 = 280微妙
					// 0Bxx，R_OLF_DIV：RDC时钟分频，要将RDC时钟频率设置为10kHz；
					/*								00 = /1 (for OLF = 10kHz)
														01 = /4 (for OLF = 50kHz)
														10 = /8 (for OLF = 100kHz)
														11 = /16 (for OLF = 200kHz)
					*/
          0x04,   // 寄存器36：选择实时时钟源并使能；
					// 0Bxxx：空，000；
					// 0Bx，RTC_CLK_SEL：选择实时时钟源，需要外部32kHz晶振，内部时钟不精确；
					//									0 = OLF时钟
					//									1 = 外部HF
					// 0Bx，RTC_EN：0 = 关闭
					//							1 = 激活实时时钟
					// 0Bx，LBD_CLK_SEL：电池低电量检测的时钟源选择；
					//									0 = OLF clock
					//									1 = OLF clock / 16
					// 0Bx，TDC_NOISE_CY_DIS：内部位，0；
					// 0Bx：空，0；

					0x00,   // 寄存器37：内部默认00；
					
					0x34,   // 寄存器38：TDC内部默认0h34；
					// 0Bxxxxxx，TDC_QHA_SEL：内部位，0D13;
					// 0Bxx：空，00；
					
          0x00,   // 寄存器39：内部默认00；
					
/********************************* DSP设置 **************************************/
					0x00,   // 寄存器40：DSP设置，高速时钟最高运算速度；
					// 0Bxx，DSP_MOFLO_EN：在GPIO脉冲线上使能单稳态触发器；
					//										00 = 关闭
					//										11 = 开启
					// 0Bxx，DSP_CLK_MODE：选择DSP时钟源；
					/*										00 = 环形振荡器
																01 = 低速时钟
																11 = 高速时钟
					*/
					// 0Bxx，DSP_SPEED：DSP速度设置；
					//									00 = 最快
					//									11 = 最慢
					// 0Bx，PG1xPG3：0 = PDM/PWM 脉冲输出端为PG3
					//							1 = PDM/PWM 脉冲输出端为PG1
					// 0Bx，PG0xPG2：0 = PDM/PWM 脉冲输出端为PG2
					//							1 = PDM/PWM 脉冲输出端为PG0
					0x00,   // 寄存器41：看门狗时间设置；
					// 0Hxx，WD_TIME：OCF设置为5kHz，则看门狗时间为50毫秒的倍数；
					//								twatchdog = WD_TIME * 50 ms
          0x00,   // 寄存器42：DSP GPIO使能和翻转；
					// 0Bxxxx，DSP_STARTONPIN：设置GPIO用于启动DSP；
					//												逐位，从PG0~PG3
					// 0Bxxxx，DSP_FF_IN：触发寄存器；
					//									逐位，从DSP_IN_0~DSP_IN_3
					0x44,   // 寄存器43：内存设置为SRAM，DSP计时器触发；
					// 0Bx，INT2PG2：将PG2定义为一个额外的中断端口INTN；
					//							0 = 仅使用INTN引脚作为外部中断输出
					//							1 = 复用PG2作为外部中断输出
					// 0Bx，DSP_SPRAM_SEL：选择SRAM或OTP为程序存储单元；
					//										0 = OTP
					//										1 = SRAM
					// 0Bx，DSP_START：0 = 直接通过接口触发DSP
					//								1 = 外部触发DSP
					// 0Bxxxxx，DSP_START_EN：用于启动DSP的各种触发条件的掩码；
					/*												0 = 在CDC转换结束后
																		1 = 计时器
																		2 = RDC转换结束后
																		3 = 外部中断上升沿
																		4 = 前端发生错误
					*/
					0x00,   // 寄存器44：PDM/PWM调制时钟设置，关闭；
					// 0Bxxxx，PI1_CLK_SEL：脉冲成分1
					/*								4 = OHF * 2
														5 = OHF
														6 = OHF / 2
														7 = OHF / 4
														8 = OLF * 2
														9 = OLF
														10 = OLF / 2
														11 = OLF / 4
														12 = RO
														13 = RO / 2
														14 = RO / 4		
					*/
					// 0Bxxxx，PI0_CLK_SEL：脉冲成分0，同上
          0x00,   // 寄存器45：
					// 0Bxxxx，PI_EN：使能PDM/PWM调制脉冲生成方式;
					/*								b'xx01 = PWM at path 0
														b'xx10 = PDM at path 0
														b'01xx = PWM at path 1
														b'10xx = PDM at path 1
					*/
					// 0Bxx，PI1_RES：脉冲码结果位数;
					/*								0 = 10 bit
														1 = 12 bit
														2 = 14 bit
														3 = 16 bit
					*/
					// 0Bxx，PI0_RES：脉冲码结果位数，同上；
					
					
					
					0xF7,   // 寄存器46：配置PG0~PG3为输入或输出，启动内部保护上拉电阻；
					// 0Bxxxx，PG_DIR_IN：在输入和输出之间切换通用端口方向；
					/*							0 = 输出												1 = 输入
							 0000 OR 0100 = PG0输出				 1000 OR 1100 = PG0输入
							 0001 OR 0101 = PG1输出				 1001 OR 1101 = PG1输入
							 0010 OR 0110 = PG2输出				 1010 OR 1110 = PG2输入
							 0011 OR 0111 = PG3输出				 1011 OR 1111 = PG3输入
					*/
					// 0Bxxxx，PG_PULLUP：配置引脚上拉；
					/*							0 = 悬空												1 = 上拉
							 0000 OR 0100 = PG0悬空				 1000 OR 1100 = PG0上拉
							 0001 OR 0101 = PG1悬空				 1001 OR 1101 = PG1上拉
							 0010 OR 0110 = PG2悬空				 1010 OR 1110 = PG2上拉
							 0011 OR 0111 = PG3悬空				 1011 OR 1111 = PG3上拉
					*/
					
					
					
					0x00,   // 寄存器47：保持默认；
					// 0Bxx：空，00；
					// 0Bx，BG_PERM： 1 = 永久激活带隙
					//								0 = 脉冲激活带隙
					// 0Bxxxxx，BG_TRIM0：调整内部带隙电源使其正常工作；
					/*								h00 = 1.68V
														h07 = 1.8V
														h1F = 2.2V
					*/
          0x07,   // 寄存器48：保持默认；
					// 0Bx，TDC_NOISE_DIS：内部位，0；
					// 0Bxx，TDC_MUPU_SPEED：TDC特殊速度，01；
					// 0Bx，TDC_MR2：TDC具体测量范围选择；
					//							0 = MR1
					//							1 = MR2
					// 0Bxxxx，BG_TRIM1：调整内部电池低电量监测和EEPROM写入的核心带隙电压；
					//									推荐值h07
					0x30,   // 寄存器49：内部位，保持默认值h30；

					0x00,  					  // Config Register 50,
          0x00,0x00,0x00,   // Config Register 51, 52, 53,
          0x00,0x00,0x00,   // Config Register 54, 55, 56,
          0x00,0x00,0x00,   // Config Register 57, 58, 59,
          0x00,0x00,0x00,   // Config Register 60, 61, 62,
          0x00,0x00,0x00,   // Config Register 63, 64, 65,
          0x00,0x00,0x00,   // Config Register 66, 67, 68,
          0x00,0x00,				// Config Register 69, 70,
					0x00,0x00,0x28,   // Config Register 71,72, 73：增益校正因子，保持默认；
          0x01,   					// Config Register 74,
          0x00,0x00,0x01};  // Config Register 75, 76，76：RunBit；
/****************************** 方案一单路测量寄存器配置完毕 ************************************************/
/*方案一是用内部模拟电容作为C0的方案*/
					

/********************************************** 定义固件数组 *******************************************************/
uint8_t PCap02_firmware[1536] =
					{	0x00, 0x00, 0x00, 0x7A, 0xC0, 0xCF, 0xFF, 0xF0, 0xD2, 0x43, 0x7A, 0xD0, 0x34, 0x72, 0x62, 0x63, 
						0x00, 0x65, 0x7A, 0xC4, 0xD1, 0x43, 0x7A, 0xD0, 0x33, 0xAB, 0x5E, 0x42, 0x5C, 0x48, 0xB0, 0x01, 
						0x20, 0x68, 0xB1, 0x02, 0x78, 0x20, 0x60, 0xB2, 0x01, 0x20, 0x68, 0xB3, 0x1B, 0x7A, 0xC2, 0xD1, 
						0x43, 0x7A, 0xDB, 0x33, 0xAB, 0x00, 0x7F, 0x7A, 0xD4, 0x43, 0x7A, 0xE4, 0x44, 0x7A, 0xC3, 0xD1, 
						0x43, 0x7A, 0xDB, 0x33, 0xAB, 0x00, 0x78, 0x20, 0x60, 0xB5, 0x0B, 0x72, 0x62, 0x7F, 0x7A, 0xD4, 
						0x43, 0x7A, 0xE4, 0x44, 0x20, 0x60, 0x00, 0xB7, 0x02, 0x78, 0x20, 0x60, 0x84, 0x01, 0x25, 0x80, 
						0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x1B, 0x7A, 0xC0, 0xD1, 0x43, 0x7A, 0xDB, 0x33, 0xAB, 
						0x00, 0x7F, 0x23, 0x98, 0x84, 0x01, 0x25, 0x80, 0x20, 0x91, 0x43, 0x58, 0x7A, 0xC0, 0xD1, 0x43, 
						0x6A, 0xC0, 0x44, 0x7A, 0xC4, 0xD1, 0x43, 0x7A, 0xD0, 0x3A, 0x66, 0x67, 0x76, 0x77, 0x66, 0x20, 
						0x1B, 0x7A, 0xC0, 0xC0, 0xC0, 0xC8, 0xD2, 0x43, 0x7A, 0xDE, 0x44, 0x7A, 0xC0, 0xC0, 0xC1, 0xD1, 
						0xD2, 0x43, 0x7A, 0xDD, 0x44, 0x7A, 0xC0, 0xC0, 0xC0, 0xC8, 0xD2, 0x41, 0x25, 0xD1, 0x6A, 0xD0, 
						0x43, 0x7A, 0xD9, 0x44, 0x6A, 0xD1, 0x43, 0x7A, 0xDA, 0x44, 0x6A, 0xD2, 0x43, 0x7A, 0xDB, 0x44, 
						0x20, 0xD8, 0x70, 0x60, 0x71, 0x61, 0x66, 0x67, 0x76, 0x77, 0x66, 0x7B, 0x7A, 0xC1, 0xD1, 0x43, 
						0x7A, 0xDB, 0x33, 0xAB, 0x00, 0x78, 0x20, 0x60, 0x7A, 0xD5, 0x43, 0x7A, 0xC7, 0xD1, 0x41, 0x6A, 
						0xEB, 0x45, 0x5A, 0x22, 0xF1, 0x46, 0x46, 0x46, 0x46, 0x7A, 0xDD, 0x44, 0x7A, 0xC0, 0xC0, 0xC0, 
						0xC8, 0xD2, 0x43, 0x55, 0x7A, 0xDE, 0x45, 0x7A, 0xDD, 0x51, 0x7A, 0xD6, 0x41, 0x25, 0xB5, 0x6A, 
						0xEA, 0x45, 0x7A, 0xD7, 0x41, 0x25, 0xB5, 0x6A, 0xE9, 0x45, 0x1C, 0x41, 0x43, 0x58, 0x6A, 0xEA, 
						0x22, 0xAB, 0x6A, 0xE9, 0x50, 0x46, 0x6A, 0xEB, 0x44, 0xA9, 0x03, 0x6A, 0xEB, 0x59, 0x43, 0x7A, 
						0xD0, 0xD1, 0x41, 0x5C, 0xA8, 0x05, 0x6A, 0xC0, 0x5A, 0x6A, 0xEB, 0x45, 0x6A, 0xEB, 0x41, 0x7A, 
						0xE2, 0x45, 0x7A, 0xD8, 0x41, 0x25, 0xB5, 0x6A, 0xEA, 0x45, 0x7A, 0xD9, 0x41, 0x25, 0xB5, 0x6A, 
						0xE9, 0x45, 0x1D, 0x41, 0x43, 0x58, 0x6A, 0xEA, 0x22, 0xAB, 0x6A, 0xE9, 0x50, 0x46, 0x6A, 0xEB, 
						0x44, 0xA9, 0x03, 0x6A, 0xEB, 0x59, 0x43, 0x7A, 0xD0, 0xD1, 0x41, 0x5C, 0xA8, 0x05, 0x6A, 0xC0, 
						0x5A, 0x6A, 0xEB, 0x45, 0x6A, 0xEB, 0x41, 0x7A, 0xE3, 0x45, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
						0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
						0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 
						0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 
						0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 0x47, 
						0x02, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 
						0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 
						0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 
						0x4F, 0x02, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 
						0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 
						0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 0x4E, 
						0x4E, 0x4E, 0x02, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 
						0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 
						0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56, 
						0x56, 0x56, 0x56, 0x02, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 
						0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 
						0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 
						0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x02, 0x6A, 0xF5, 0x44, 0x42, 0x7A, 0xC6, 0xD1, 0x41, 0x6A, 0xF3, 
						0x45, 0x40, 0x5D, 0x22, 0xF1, 0x46, 0x46, 0x46, 0x46, 0x6A, 0xF4, 0x44, 0x59, 0x59, 0x6A, 0xF3, 
						0x45, 0x5B, 0x5B, 0xAB, 0x10, 0x6A, 0xF5, 0x43, 0x40, 0x5D, 0x6A, 0xF2, 0x22, 0xED, 0x6A, 0xF3, 
						0x13, 0xAA, 0xF6, 0x13, 0xAA, 0x04, 0x6A, 0xF5, 0x43, 0x40, 0x5D, 0x6A, 0xF4, 0x57, 0x57, 0xAB, 
						0x07, 0x6A, 0xF2, 0x5E, 0x6A, 0xF4, 0x13, 0xAA, 0xF7, 0x02, 0x6A, 0xF5, 0x45, 0x7A, 0xC6, 0xD1, 
						0x41, 0x6A, 0xF3, 0x45, 0x40, 0x5D, 0x22, 0xF1, 0x46, 0x46, 0x46, 0x46, 0x6A, 0xF4, 0x44, 0x59, 
						0x59, 0x6A, 0xF3, 0x45, 0x5B, 0x5B, 0xAB, 0x10, 0x6A, 0xF5, 0x41, 0x43, 0x58, 0x6A, 0xF2, 0x22, 
						0xBB, 0x6A, 0xF3, 0x13, 0xAA, 0xF6, 0x13, 0xAA, 0x04, 0x6A, 0xF5, 0x41, 0x43, 0x58, 0x6A, 0xF4, 
						0x57, 0x57, 0xAB, 0x07, 0x6A, 0xF2, 0x56, 0x6A, 0xF4, 0x13, 0xAA, 0xF7, 0x02, 0x6A, 0xF5, 0x45, 
						0xAB, 0x13, 0x7A, 0xC7, 0xD1, 0x5A, 0xAC, 0x08, 0x52, 0x4B, 0x12, 0x4E, 0x12, 0xA8, 0xFB, 0xA9, 
						0x05, 0x5A, 0x46, 0x12, 0xA8, 0xFB, 0x00, 0x02, 0x7A, 0xF6, 0x43, 0x40, 0x4F, 0x4F, 0x4F, 0x6A, 
						0xF6, 0x45, 0x7A, 0xDA, 0x41, 0x43, 0x58, 0x6A, 0xF6, 0x22, 0xAB, 0x6A, 0xF5, 0x44, 0x7A, 0xC0, 
						0xC0, 0xC1, 0xE6, 0xD2, 0x43, 0x7A, 0xDD, 0x44, 0x7A, 0xC0, 0xC0, 0xC0, 0xC8, 0xD2, 0x41, 0x7A, 
						0xDE, 0x45, 0x7A, 0xC0, 0xC0, 0xC1, 0xED, 0xD2, 0x43, 0x6A, 0xF4, 0x44, 0x1C, 0x43, 0xAB, 0x02, 
						0x6A, 0xF5, 0x58, 0x8C, 0x04, 0x7A, 0xDD, 0x13, 0x1C, 0x50, 0x1D, 0x44, 0x7A, 0xDD, 0x13, 0x7A, 
						0xDE, 0x13, 0x6A, 0xF4, 0x43, 0x7A, 0xDD, 0x58, 0xAC, 0xE1, 0x8C, 0x17, 0x7A, 0xEF, 0x43, 0x6A, 
						0xC8, 0x59, 0x7A, 0xF0, 0x43, 0x6A, 0xC9, 0x59, 0x7A, 0xF3, 0x43, 0x6A, 0xCA, 0x59, 0x7A, 0xF4, 
						0x43, 0x6A, 0xCB, 0x59, 0x00, 0x6A, 0xC9, 0x7A, 0xC0, 0xC0, 0xC0, 0xC9, 0xD2, 0x43, 0x7A, 0xDD, 
						0x44, 0x6A, 0xF2, 0x44, 0x6A, 0xF3, 0x44, 0x7A, 0xC0, 0xC0, 0xC0, 0xC8, 0xD2, 0x43, 0x7A, 0xDE, 
						0x44, 0x1D, 0x43, 0x4E, 0x4E, 0x4E, 0x44, 0x7A, 0xC0, 0xC0, 0xC0, 0xCF, 0xD2, 0x43, 0x6A, 0xF4, 
						0x44, 0x8C, 0x09, 0x7A, 0xC0, 0xC0, 0xC0, 0xCB, 0xD2, 0x43, 0x6A, 0xF4, 0x44, 0x00, 0x40, 0x5D, 
						0x1C, 0x43, 0x1D, 0x22, 0xDC, 0x6A, 0xF3, 0x43, 0x7A, 0xDD, 0x44, 0x1C, 0x45, 0x8D, 0x0C, 0x7A, 
						0xDE, 0x13, 0x13, 0x1D, 0x43, 0x4E, 0x4E, 0x4E, 0x44, 0x6A, 0xF2, 0x13, 0x6A, 0xF3, 0x13, 0x6A, 
						0xF2, 0x13, 0x41, 0x7A, 0xDD, 0x45, 0x6A, 0xF4, 0x43, 0x5C, 0xAC, 0xD0, 0x7A, 0xC0, 0xC0, 0xC0, 
						0xCF, 0xD2, 0x43, 0x6A, 0xF4, 0x44, 0x6A, 0xF3, 0x41, 0x6A, 0xF4, 0x43, 0x5C, 0xA8, 0x0C, 0x6A, 
						0xF3, 0x43, 0x13, 0x7A, 0xDD, 0x44, 0x1C, 0x44, 0x59, 0x43, 0xAB, 0xE9, 0x6A, 0xC8, 0x43, 0x46, 
						0x46, 0x46, 0x44, 0x8A, 0x14, 0x40, 0x5D, 0x4B, 0x7A, 0xC8, 0xD1, 0x43, 0x7A, 0xF9, 0x33, 0xAB, 
						0x1C, 0x6A, 0xC8, 0x45, 0x6A, 0xC9, 0x45, 0x8D, 0x01, 0x00, 0x02, 0x6A, 0xCA, 0x45, 0x6A, 0xCB, 
						0x45, 0x6A, 0xCC, 0x45, 0x6A, 0xCD, 0x45, 0x6A, 0xCE, 0x45, 0x6A, 0xCF, 0x45, 0x02, 0x7A, 0xC2, 
						0xD1, 0x43, 0x6A, 0xF5, 0x44, 0x6A, 0xF4, 0x44, 0x8C, 0x09, 0x7A, 0xC1, 0xD1, 0x43, 0x6A, 0xF5, 
						0x44, 0x6A, 0xF4, 0x44, 0x8D, 0x1A, 0x7A, 0xC1, 0xD1, 0x43, 0x6A, 0xF5, 0x44, 0x4E, 0x50, 0x6A, 
						0xF4, 0x44, 0x8C, 0x0C, 0x7A, 0xC0, 0xD1, 0x43, 0x6A, 0xF5, 0x44, 0x4E, 0x4E, 0x50, 0x6A, 0xF4, 
						0x44, 0x7A, 0xC0, 0xC0, 0xC0, 0xC8, 0xD2, 0x43, 0x4E, 0x4E, 0x6A, 0xF4, 0x51, 0x7A, 0xC9, 0xD1, 
						0x43, 0x7A, 0xF9, 0x33, 0xAB, 0x01, 0x25, 0x75, 0x6A, 0xF5, 0x43, 0x6A, 0xF4, 0x51, 0x7A, 0xCA, 
						0xD1, 0x43, 0x7A, 0xF9, 0x33, 0xAB, 0x01, 0x25, 0x75, 0x6A, 0xF5, 0x43, 0x6A, 0xF4, 0x51, 0x7A, 
						0xCB, 0xD1, 0x43, 0x7A, 0xF9, 0x33, 0xAB, 0x01, 0x25, 0x75, 0x6A, 0xF5, 0x43, 0x6A, 0xF4, 0x51, 
						0x7A, 0xCC, 0xD1, 0x43, 0x7A, 0xF9, 0x33, 0xAB, 0x01, 0x25, 0x75, 0x6A, 0xF5, 0x43, 0x6A, 0xF4, 
						0x51, 0x7A, 0xCD, 0xD1, 0x43, 0x7A, 0xF9, 0x33, 0xAB, 0x01, 0x25, 0x75, 0x6A, 0xF5, 0x43, 0x6A, 
						0xF4, 0x51, 0x7A, 0xCE, 0xD1, 0x43, 0x7A, 0xF9, 0x33, 0xAB, 0x01, 0x25, 0x75, 0x6A, 0xF5, 0x43, 
						0x6A, 0xF4, 0x51, 0x7A, 0xCF, 0xD1, 0x43, 0x7A, 0xF9, 0x33, 0xAB, 0x01, 0x25, 0x75, 0x6A, 0xF5, 
						0x43, 0x6A, 0xF4, 0x51, 0x02, 0x6A, 0xF4, 0x43, 0x46, 0x46, 0x7A, 0xDD, 0x44, 0x1C, 0x45, 0x02, 
						0x7A, 0xE2, 0x41, 0x4F, 0x4F, 0x4F, 0x6A, 0xF2, 0x45, 0x5A, 0x7A, 0xE3, 0x43, 0x6A, 0xF2, 0x25, 
						0xB2, 0x6A, 0xD0, 0x45, 0x5A, 0x7A, 0xE4, 0x43, 0x6A, 0xF2, 0x25, 0xB2, 0x6A, 0xD1, 0x45, 0x5A, 
						0x7A, 0xE5, 0x43, 0x6A, 0xF2, 0x25, 0xB2, 0x6A, 0xD2, 0x45, 0x7A, 0xC8, 0xD1, 0x43, 0x7A, 0xD0, 
						0x3A, 0x02, 0x22, 0xDC, 0x02, 0x7A, 0xD7, 0xD1, 0x43, 0x6A, 0xF2, 0x45, 0x5D, 0xAD, 0x01, 0x5D, 
						0x45, 0x41, 0x02, 0x7A, 0xCF, 0xD1, 0x43, 0x6A, 0xF2, 0x45, 0x5D, 0xAD, 0x01, 0x5D, 0x45, 0x41, 
						0x02, 0x4B, 0x12, 0x1D, 0x43, 0x1C, 0x44, 0x7A, 0xDD, 0x13, 0x7A, 0xDE, 0x13, 0x12, 0xAA, 0xF2, 
						0x02, 0x7A, 0xC0, 0xC0, 0xC0, 0xD0, 0xD2, 0x43, 0x58, 0x58, 0x6A, 0xF2, 0x44, 0x42, 0x46, 0x3C, 
						0x13, 0xAA, 0xFA, 0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
//-----------------------------------------------------------------------------------------------------------------------
					
/********************* 声明PCap02使用的变量 ***********************************/


// uint32_t  Res0_content;
// uint32_t  STATUS;						
uint8_t   Res1_content[3];
// uint8_t		Res_content[45];
// float     Res4_content;
// double     Capacitance_ratio;
// uint8_t   Status_content[3];
uint8_t		RunBit;
/********************************************************************************/

/********************************************* 宏定义 **********************************/

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
/********************************* 宏定义结束 ********************************/

/******************************** 主函数 **********************************/
/*************************************************************************
  * @brief  用野火STM32F407开发板控制并读写PCAP02AE电容转换芯片
  * @param  无
  * @retval 无
  ********************************************************************/
int main()
{
	/* ================================定义变量=====================================*/
	int RES;
	// int RES_Mirror=0;
	//用宏来选择要执行的动作，不用的注释掉
//	#define LOAD_FIRMWARE_ARRAY
//	#define LOAD_HEX_FIRMWARE_FILE
//	#define COMMUNICATION_TEST
//	#define Direct_R_W
	#define CLEAR_SRAM
	#define WRITE_SRAM
//	#define READ_SRAM
//	#define WRITE_EEPROM
	// 使能PCAP的SPI
	IIC_EN_Config();
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	//配置LED
	LED_GPIO_Config();
	
//	SysTick_Init();
	
	/* 配置串口1为：115200 8-N-1 */
	Debug_USART_Config();
  
//	printf("\r\n Start the test \r\n");

	//清空嵌入式SRAM
	Embed_SRAM_Init();
	//使能PCAP02_GPIO
	GPIOs_Init();
	//初始化SPI
	SPI_PCAP02_Init();
	//PCAP02电源复位
 	SPI_PCAP02_SendByte(PCap02_Power_Reset);
	
	LED_WHITE;
//	printf("Using the 0x%02X Clock source \n", RCC_GetSYSCLKSource () );
//	printf("ClocksFrequency = %s\n", RCC_GetClocksFreq( SYSCLK) );
	
	/******************************** 加载固件库hex文件 *********************************/
	// 从外部HEX文件加载固件库,打开文件并转换为int型数据
	#ifdef LOAD_HEX_FIRMWARE_FILE  
   FILE * HexFirmwareFile;
   char c;
   hex_index = 0;
   int count_byte = 0;       // 每行的字节数量
   int count_line = 0;       // 行数
   int count_ff = 0;         // 在某些数量的 0xFF 时中断
   int break_no_of_ff = 160; // 在10行160个 0xFF 后, 终止加载
    
   HexFirmwareFile = fopen ("E:\\program\\keil STM32\\v1.0\\Output\\PCap02AE.hex","r"); // 以只读模式打开文件，文件名在开头的宏里定义
   if (HexFirmwareFile==NULL) 
	 {
		 perror ("Error opening file");	
//		 printf("I couldn't open the file: (%d)%s \r\n", errno, strerror(errno) );
	 }//检查是否加载成功
   else
   {
     puts("HexFirmwareFile will be loading, max size 4k)"); 
     puts(" (loading...)"); 
     do
     {
       c = fgetc (HexFirmwareFile);
       int i1 = convert_char2int(c);
       i1 *= pow(2,4);
       c = fgetc (HexFirmwareFile);
       int i2 = convert_char2int(c);
      
       PicoCap_Hex_Firmware[hex_index] = i1 + i2;  // 给固件分配内存
      
       if (PicoCap_Hex_Firmware[hex_index]==0xFF)  // 检查是不是到一堆0xFF了
       {
         count_ff++;
         if (count_ff==break_no_of_ff) break; // 跳出循环
       }
       else count_ff=0;
        
       hex_index++;
       count_byte++;

       if (count_byte==16)                   // 检查每行结束
       { 
         fgetc (HexFirmwareFile);  // wildcard, last blank '' 0x20
         fgetc (HexFirmwareFile);  // wildcard, carriage return <CR> 0x0A
         count_byte = 0;
         count_line++;
         if (count_line==16) {printf("%u\n",hex_index);count_line=0;}
         else printf(".");
         fflush(stdout); 
       }
       else fgetc (HexFirmwareFile);  // wildcard, blank '' 0x20
     } while (hex_index != Install_Byte_Limit); // without check (c != EOF)
     fclose (HexFirmwareFile);
     hex_index -= break_no_of_ff;   // define real last byte of Firmware
     printf("\n %u Bytes of HexFirmwareFile have been loaded \n\n", hex_index+1);
   }
	#endif
	//-------------------------------------------------------------------------------
/***********************************************************************************************/	
	
	
	#ifdef   CLEAR_SRAM
      // 清除 SRAM 0..4096 Bytes 
      puts("|         | Clear complete SRAM memory (4k)");
      printf("|");
      fflush(stdout); 		//用来强制清空输出缓冲区的函数，避免输出错误
      for (uint16_t i=0; i<4096; i++) 
      {
        Send_24Bit_Opcode(PCAP02_SPI, Write_To_SRAM, i, 0x00);
        if (i==0   ||i==500 ||i==1000||i==1500||i==2000||i==2500||
            i==3000||i==3500||i==4000)
        {
          printf(".");
          fflush(stdout);
        }
      }
      printf("| SRAM address 4095 reached!\n");
      puts("Read the first 16 Bytes from SRAM...");
      for (uint16_t i=0; i<16; i++) 
      {
        printf(" %02X",Send_24Bit_Opcode(PCAP02_SPI, Read_SRAM, i, 0xFF));
        fflush(stdout); 
      }
      puts("\n SRAM address 15 reached!");
	#endif
	
	
	
	#ifdef Direct_R_W
			Direct_R_and_W ();
	
	#endif
		
		


		
	/* SRAM写入、读取测试，测试PCAP02通信是否正常 */
	#ifdef COMMUNICATION_TEST
		while(1)
	{
//		char 	SRAM_callback[1];
    // SPI通信测试
    uint16_t adr = 0x3FF;  // 地址范围从0~0xFFF,从 0x7FF (2^11-1) 2048-1 开始写入
    uint8_t dat = 0xCD;
		
		puts("Test the SRAM Communication");
		printf(" Writing 0x%02X to Address 0x%04X\n",dat,adr);

    Send_24Bit_Opcode(PCAP02_SPI, Write_To_SRAM, adr, dat);				//写入SRAM
				
		printf(" Reading Data is 0x%02X\n",Send_24Bit_Opcode(PCAP02_SPI, Read_SRAM, adr, 0xFF));	//读出SRAM
	}
	#endif

			
/***********************************************************************/			
			//调试过程中，向SRAM发送固件，调试完成可以向EEPROM存固件
	#ifdef   WRITE_SRAM
      printf("\n|      | Write the First 96 Lines of Bytes from the Firmware into SRAM (4k)\n");
      printf("|");
      fflush(stdout);
      for (uint16_t i=0; i<1536; i++) 
      {
        Send_24Bit_Opcode(PCAP02_SPI, Write_To_SRAM, i, PCap02_firmware[i]);
        if (i==0   ||i==500 ||i==1000||i==1500||i==2000||i==2500||
            i==3000||i==3500||i==4000)
        {
          printf(".");
          fflush(stdout);
        }
      }
      printf("| SRAM address 1536 reached!\n");
	#endif 
/**********************************************************************/
			

      
	#ifdef   READ_SRAM
			while(1)
{
      puts("Read the first 16Bytes from SRAM...");
      for (uint16_t i=0; i<1536; i++) 
      {
        printf(" 0x%02X",Send_24Bit_Opcode(PCAP02_SPI, Read_SRAM, i, 0xFF));
				
//-------------------------在此处设置断点在线调试----------------------------
				
        fflush(stdout); 
			}
		}
	#endif     	
/***************************************************************/
		


/****************************** 下载标准固件库至EEPROM ******************************/
	#ifdef   WRITE_EEPROM
      printf("\n| | load the Firmware into EEPROM \n");
      printf("|");
      fflush(stdout);
      for (uint16_t i=0; i<hex_index+1; i++) 
      {
        Send_24Bit_Opcode(PCAP02_SPI, Write_To_EEPROM, i, PicoCap_Hex_Firmware[i]);
        if (i==0   ||i==500 ||i==1000||i==1500||i==2000||i==2500||
            i==3000||i==3500||i==4000)
        {
          printf(".");
          fflush(stdout);
        }
      }
      printf("| EEPROM address %u reached!\n", hex_index);
	#endif
/******************************************************************************************/
	
//while(1)
//	{
//		Write_RunBit(0x01);
//SPI_PCAP02_Status();
//	}
	
/********************************* 向PCAP02写入配置 *********************************/

//while(1)
//	{
// 写入配置寄存器之前清零 RUNBIT
	puts("\nClear RUNBIT...\n");
  Write_RunBit(00);
	//                *bus_type,   command, 		address, *data, 			quantity
	
	//	读取状态寄存器
//	SPI_PCAP02_Status();	
	 
  // 写入自定义PCap02 config
//  puts("Write Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_config_reg, sizeof(PCap02_config_reg));
		
	// 写入标准PCap02 config
//  puts("Write Standard Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_standard_config_reg, sizeof(PCap02_standard_config_reg));
	
	// 写入标准修改的PCap02 config1
//  puts("Write modify Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_modify_config1_reg, sizeof(PCap02_modify_config1_reg));

	// 写入标准修改的PCap02 config2，低速时钟200kHz，单端，内部电容补偿
//  puts("Write modify Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_modify_config2_reg, sizeof(PCap02_modify_config2_reg));
	
	// 写入标准修改的PCap02 config3，低速时钟200kHz，浮动，无电容补偿
//  puts("Write modify Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_modify_config3_reg, sizeof(PCap02_modify_config3_reg));
	
	// 写入标准修改的PCap02 config4，低速时钟200kHz，浮动，外部电容补偿
//  puts("Write modify Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_modify_config4_reg, sizeof(PCap02_modify_config4_reg));

	// 写入标准修改的PCap02 config5，低速时钟200kHz，单端，无补偿
//  puts("Write modify Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_modify_config5_reg, sizeof(PCap02_modify_config5_reg));
	
	// 写入标准修改的PCap02 config6，低速时钟200kHz，单端，无补偿,通道6
  puts("Write modify Config...\n");
  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_modify_config6_reg, sizeof(PCap02_modify_config5_reg));
		
  // RUNBIT 置1
  puts("Set RUNBIT...\n");
  Write_RunBit(01);
	
	//	读取状态寄存器
//	 printf("STATUS is 0x%08X\r\n",SPI_PCAP02_Status());
	 
	//初始化PCAP02
  SPI_PCAP02_SendByte(PCap02_Initialize);
	//开始测量
	SPI_PCAP02_SendByte(PCap02_CDC_Start_Conversion);
//			Wait_For_Measurement();

	/********************** 下面开始主循环 *********************/
	while(1){
		//软件操作码触发CDC转换数据，开始测量
//		SPI_PCAP02_SendByte(PCap02_CDC_Start_Conversion);
		//硬件GPIO3上升沿触发
//		SPI_PCAP02_PG3_HIGH();
		// measure_again:
		// Wait_For_Measurement();
		// LED_CYAN;
		// printf("debug\n");
		//	读取状态寄存器
//		SPI_PCAP02_Status();	
		//读取结果
//		puts("Read Results...\n");
		/********* 1通道结果读取 **********/
		//结果寄存器1
//		Write_Incremental(PCAP02_SPI, Read_Results, 0x03, Res1_content, 3);
		//结果寄存器2
//		Write_Incremental(PCAP02_SPI, Read_Results, 0x06, Res1_content, 3);
		//结果寄存器+3
//		Write_Incremental(PCAP02_SPI, Read_Results, 0x09, Res1_content, 3);
		/********* 6通道结果读取 **********/
		// measure_again:
		Write_Incremental(PCAP02_SPI, Read_Results, 0x12, Res1_content, 3);

		// debug
		// for(int i = 0; i <3;i++) {
		// 	printf("%x", Res1_content[i]);
		// }
// LED_CYAN;
    	RES = (Res1_content[2] << 16) | (Res1_content[1] << 8) | Res1_content[0];
		// if(RES == RES_Mirror){
        //   goto measure_again;          
        // }
        // RES_Mirror = RES;

		printf("%d\n", RES);
		// 读取所有结果寄存器
//		Write_Incremental(PCAP02_SPI, Read_Results, 0x00, Res_content, 45);
//    RES = (Res_content[5] << 16) | (Res_content[4] << 8) | Res_content[3];
//		SPI_PCAP02_PG3_LOW();
		//	读取状态寄存器
//		SPI_PCAP02_Status();	
    	// Capacitance_ratio = RES / pow(2,21);
		// Capacitance_ratio = (RES / 2097152)*1000000;
		// Capacitance_ratio = RES;
	
		// printf("%d\n", RES_Mirror);
		// printf("RES/2097152 = %f\n",RES/(double)2097152);
//			Capacitance_ratio = convertBinaryToDecimal(RES);
    	// printf("Capacitance_ratio = %f\n",  Capacitance_ratio);	
		// LED_CYAN;
//		printf("  0x%08X\r, %f\n", RES, Capacitance_ratio);	
		// int res;
		// float debug1;
		// double debug2;
		// res = RES;
		// debug1 =(float) res / 1000000;
		// debug2 = res / 1000000;
		// printf("RES = %ld , res = %ld , debug1 = %f , debug2 = %f\n", RES,res, debug1, debug2);
		// printf("ceshi1 = %f\n",112.993);
		// printf("ceshi2 = %d\n",112);
		// printf("size of RES = %d, size of res = %d, size of debug1 = %d, size of debug2 = %d\n", 
		// sizeof(RES),sizeof(res),sizeof(debug1),sizeof(debug2));
		// LED_WHITE;
	}
	//查找方差最小的测量结果
//	find_var	iance_float(1000);
}
/***************************** main函数结束 **********************************/
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
  * @brief  main函数调用的子函数，SPI通信相关子函数的在另外的SPI_PCAP02.c原文件里
 **/
/*******************************************************************************
  * @brief  为了加载固件库，把char型转换为int
  * @param  16进制ASSCII码
  * @retval 整型0~15
 ******************************************************************************/
int convert_char2int(int char_input)
{
  if (char_input==48) return 0;  // char(0)
  if (char_input==49) return 1;  // char(1)
  if (char_input==50) return 2;  // char(2)
  if (char_input==51) return 3;  // char(3)
  if (char_input==52) return 4;  // char(4)
  if (char_input==53) return 5;  // char(5)
  if (char_input==54) return 6;  // char(6)
  if (char_input==55) return 7;  // char(7)
  if (char_input==56) return 8;  // char(8)
  if (char_input==57) return 9;  // char(9)
  if (char_input==65) return 10; // char(A)
  if (char_input==66) return 11; // char(B)
  if (char_input==67) return 12; // char(C)
  if (char_input==68) return 13; // char(D)
  if (char_input==69) return 14; // char(E)
  if (char_input==70) return 15; // char(F)
  else return 99; // 正常情况下不应该出现
}
/************************************************************************************/


/*******************************************************************************
  * @brief  初始化SRAM
  * @param  无
  * @retval 无
 ******************************************************************************/
void Embed_SRAM_Init(void)
{
 // Releasing reset for Embedded SRAM
  RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_BKPSRAM, ENABLE);
}


/*******************************************************************************
  * @brief  取数据求方差，单个采样可以用于求最准确值
  * @param  求方差的样本数
  * @retval 方差
 ******************************************************************************/
float find_variance_float(uint16_t sample_count)
{
  double      sum = 0;
  float       mean = 0; 
  float       diff;
  float       square;
  float       numerator = 0;
  float       variance;
  float       data;

  sram_mem_offset = 1000;  // Using values 1000-2000 for Variance calculation
  
  for(int i=0;i<sample_count;i++)
  {
    data = sram_float_memory[sram_mem_offset]; 
    sum = sum + data;
    sram_mem_offset ++; // For the sram_memory, the address increments by 4 everytime
  }
  
  mean = sum/sample_count;
  sram_mem_offset = 1000; // Using values 1000-2000 for Variance calculation
  
  for(int j=0;j<sample_count;j++)
  {
    data = sram_float_memory[sram_mem_offset];
    diff = data - mean;
    square = diff * diff;
    numerator += square;
    sram_mem_offset ++;
  }
  
  //variance = numerator/(sample_count-1); // Sample variance
  variance = numerator/(sample_count); // Population variance variance
  return variance;
}

/*---------------------------------二进制转十进制-------------------------------------*/
// void TwoToTen()
// {   
//         long long n,a;
//         int sum = 0, i = 0, m;
//         printf("输入一个二进制数: ");
//         scanf("%lld", &n);
//         a = n;
//         while (n != 0)
//         {
//             m = n % 10;
//             n /= 10;
//             sum += m*pow(2, i);
//             ++i;
//         }
//         printf("二进制数 %lld 转换为十进制为 %d\n", a, sum);
// }


