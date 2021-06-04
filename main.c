/******************** (C) COPYRIGHT 2008 STMicroelectronics *********************

 * File Name          : main.c
 * Author             : ½����� ������ͨ��ѧ��е����
 * Version            : 
 * Date               : 2020-9-20
 * Description        : STM32F407������ͨ��SPI���ƶ�ȡPCap02AE
 ******************************************************************************/
  
#include "stm32f4xx.h"		//??????
// #include "stm32f4xx_rcc.h"
#include "bsp_led.h"
#include "bsp_debug_usart.h"
#include "SPI_PCAP02.h"		 //������˽�к���

#include <math.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h> 


int i;
int j;

/**************** ����PCAP02��˽�б��� ******************************/

uint32_t   CriticalSecCntr;

bool     configured_true = false;
uint8_t  Rx_data1;
uint8_t  Rx_data2;
uint8_t  Rx_data3;

float     *sram_float_memory  = ((float *)(SRAM_BASE + 0xB00));
uint32_t  *sram_uint32_t_memory = ((uint32_t *)(SRAM_BASE + 0xB00));
uint32_t  sram_mem_offset     = 0x0;


uint8_t   PicoCap_Hex_Firmware[8192];  // Ϊ�̼������ 8k ��С���ڴ�
uint32_t  Cfg_Register[26];
uint16_t  Install_Byte_Limit = 4096;   // max. 4096
uint16_t  main_index = 0;
uint16_t  hex_index = 0;
uint16_t  cfg_index = 0;

//char * HexFile = "PCap02AEstandard.hex";		
//Note��hex�ļ�����main.cԴ�ļ�ͬ·���£������޸��������·��
/***********************************************************************************/


/********************************* ��׼���� ****************************************/
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
					
/********************************* �׼��޸�����1 ****************************************/
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
	
/*************************** �׼��޸�����2������ʱ��200kHz�����ˣ��ڲ����ݲ��� ****************************************/
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
					
/********************** �׼��޸�����3������ʱ��200kHz���������޵��ݲ��� ****************************************/
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
					
/********************** �׼��޸�����4������ʱ��200kHz���������ⲿ���ݲ��� ****************************************/
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
					
/********************** �׼��޸�����5������ʱ��200kHz�����ˣ��޲��� ****************************************/
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
	
/********************** �׼��޸�����6������ʱ��200kHz�����ˣ��޲�����ͨ��6 ****************************************/
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

					
/************************* PCap02�����Ĵ�������(Reg 0 .. 77) ****************************/
//ÿ�����Ĵ������ö��Ǹ�λд��ǰ����λд�ں�
					uint8_t PCap02_config_reg[78] = 
				 {
          0xFF,   
					 // �Ĵ���0������ģʽ��
					 // 0Bxxxx��AUTOBOOT_DIS��0000 = �����������豸��OTP�ڴ���������
					 // 											1111 = AutoBOOTΪ��ģʽ���豸����ͨ����������������
					 // �����ڴ�����
					 // 0Bxxxx��MEM_LOCK_DIS��0000 = ��ֹ�̼�����ȡ�����ܣ���
					 //												1111 = �����ڴ棻
	        0x00,   
					 // �Ĵ���1��OTP�ڲ���������޸����ƣ�
					 // 0Bxxxxxxxx��ECC_MODE��h00 = �ر�OTP�ڲ���ϣ�
					 //												h0F = doubleģʽ��
					 //												hF0 = quadģʽ���ڴ��С���뵽1963byte��
	        0x41,   
					 // �Ĵ���2��
					 // 0Bx���գ�0��
					 // 0Bx��SPI_COLLAVOID_EN��1 = ����DSP��ͻ���Լ�����SPI�Ĵ���������ɣ�
					 // 0Bxx��I2C_ADD������IICͷ��ַ��
					 // 0Bxx��MEM_COMP_LENGTH��00 = �ر�SRAM-to-OTP�Ƚϻ��ƣ�
					 // 											01 = �Ƚ�5���ֽں��˳���
					 //												10 = �Ƚ�����33�ֽں��˳���
					 //												11 = �Ƚ�����257���ֽں��˳���
					 // 0Bxx��OTP_RO_SPEED��OTP�����ٶȣ�01 = ��죻
          0x14,   
					// �Ĵ���3������ʱ��OLF���ã�
					// 0Bxx��OLF_CTUNE���ֵ�����ʱ�ӣ�
					//									00 = 200kHz��
					//									01 = 100kHz��
					//									10 = 50kHz���Ƽ�����
					//									11 = 10kHz��
					// 0Bxxxx��OLF_FTUNE��΢������ʱ�ӣ�0~15��
					/* OLF_CTUNE			OLF_FTUNE			OLF Frequency
						 11:= (10kHz) 		0001 						5 kHz
						 11:= (10kHz) 		0111 						10 kHz
						 10:= (50kHz) 		0000 						28 kHz
						 10:= (50kHz) 		0011 						48 kHz
						 01:= (100kHz) 		0100 						100 kHz
						 00:= (200kHz) 		0101 						200 kHz						
					*/
					// 0Bx��OX_CLK32KHZ_EN��ѡ���ⲿʱ�ӣ���4MHz or 32kHz��
					// 											0 = 4MHz��
					//											1 = 32kHz��
					// 0Bx��OLF_CLK_SEL��ѡ�����ʱ��Դ��
					//									0 = ѡ��Ƭ�ϵ���ʱ��ԴΪOLF��
					//									1 = ѡ���ⲿ32kHz����ΪOLF��
/************ �Լ��Ķ�·����32.7kHz�ⲿ����       ��ƵPLL�ɲ����Ե�40MHz??? DSP�ڲ����100MHz*****/
	        0x03,   
					// �Ĵ���4���ⲿ����ʱ��OX/OHF���ã�
					// 0Bx��OX_DIS������OXʱ�ӣ�
					//							0 = ������
					//							1 = ���ã�
					// 0Bx��OX_AMP_TRIM������OXʱ�ӷ������棻
					//									0 = �����棻
					//									1 = �����棻
					// 0Bx��OX_DIV4��ʱ��4��Ƶ��
					//							 0 = ����Ƶ��
					//							 1 = 4��Ƶ��
					// 0Bx��OX_AUTOSTOP_DIS��0��
					// 0Bx��OX_STOP��0��
					// 0Bxxx��OX_RUN����������ģʽ�ĳ־��Ի��ӳ٣��ӳ���ָ������ʼǰ������Ԥ��ʱ�䣻
					//								000 = �ر�OXʱ�ӣ�
					//								001 = OX�������У�
					//								010 = OX�ӳ�31/����ʱ��Ƶ�ʣ�31������ʱ�����ڣ���
					//								011 = OX�ӳ�2/����ʱ��Ƶ�ʣ�2������ʱ�����ڣ���
					//								110 = OX�ӳ�1/����ʱ��Ƶ�ʣ�1������ʱ�����ڣ���
					0x14,   // �Ĵ���5��
					// 0Bxx���գ�00��
					// 0Bxxxxxx��OCF_TIME�����ƺ㶨ʱ��OCFƵ�ʣ�������EEPROM��
					//										����ʹfOCF == 5kHz
					//										fOCF = fOLF/(2*0Dxx);
          0x04,   // �Ĵ���6���ڲ�����ʱ��Դ���ã�
					// 0Bxxxxx���գ�00000��
					// 0Bxxx��OHF_CLK_SEL��ѡ���ڲ�����ʱ��OHFԴ��
					/*										001 = �ڲ�ʱ��Դ#1
																010 = �ڲ�ʱ��Դ#2
																100 = �ⲿʱ��Դ
					*/
					0x14,   // �Ĵ���7����ֵ������ã�
					// 0Bxxx���գ�000��
					// 0Bx��DCHG_DUM_DIS����Բ�����ֵ��ݵļٳ�ŵ磬ʹ�Գƣ�
					//										0 = ��ֵ���
					//										1 = ��ͨ����
					// 0Bx��SCHMITT_CDUM_EN�����õĶԳ��ԣ�
					//											0 = Ĭ��
					//											1 = ��ִ������ͼٲ���ģʽ
					// 0Bx��C_SENSE_INVERT��1 = �Ƽ�ʹ��Ƭ��ʩ���ش�������������ת�����ķ�תˮƽ��
					// 0Bxx��SCHMITT_SEL��00 = ѡ��Ƭ��ʩ���ش�����
					
/******************************* �ŵ�������� *************************************/
					0x58,   // �Ĵ���8��
					// 0Bxx��RDCHG_INT_H��ѡ��PC4~PC7�ķŵ���裻
					/*										00 = 180 k��
																01 = 90 k��
																10 = 30 k��
																11 = 10 k��
					*/
					// 0Bxx��RDCHG_INT_L��ѡ��PC0~PC3��PC8��PC9�ķŵ���裻
					/*										00 = 180 k��
																01 = 90 k��
																10 = 30 k��
																11 = 10 k��				
					*/
					// 0Bx��RDCHG_INT_EN��0 = �ر��ڲ��ŵ���裻
					//										1 = �����ڲ��ŵ���裻
					// 0Bx���գ�0��
					// 0Bx��RDCHG_EXT_EN��0 = �ر��ⲿ�ŵ���裻
					//										1 = �����ⲿ�ŵ���裻
					// 0Bx��RDCHG_1MEG_EN�����ڲ�1M�������滻ΪPC0~PC3��PC8��PC9�ķŵ���裻
          0x1A,   // �Ĵ���9���������ã�
					// 0Bx���գ�0��
					// 0Bx��AUX_PD_DIS��ȡ���ڲ���������������PC_AUX��
					//									0 = ������
					//									1 = ����
					// 0Bx��AUX_CINT��0
					// 0Bx��RDCHG_EARLY_OPEN��1 = ��ǰ��оƬ�ڲ��ŵ����
					//												0 = ����
					// 0Bx��RDCHG_PERM_EN��1 = ����оƬ�ڲ��ŵ������������
					//										0 = ����������
					// 0Bx��RDCHG_EXT_PERM��1 = �����ⲿ�ŵ����
					//											0 = ������
					// 0Bxx��RCHG_SEL��ѡ������裻
					/*								00 = 180 k��
														01 = 90 k��
														10 = 30 k��
														11 = 10 k��
					*/
					0x10,   // �Ĵ���10���������ã�
					// 0Bx��C_REF_INT����PC8��PC9��ʹ��Ƭ�ϲαȵ��ݣ�
					//								0 = �ⲿ�αȵ��ݣ�PC0/GND��PC0/PC1��
					//								1 = �ڲ��αȵ��ݣ�
					// 0Bx��COMP_R_SEL��ѡ��������ڲ���ʱ������ļ������������ʣ�
					//									0 = 90 k��
					//									1 = 180 k��
					// 0Bx��C_COMP_EXT��1 = ����Ƭ��������ݵĲ������ƣ����������ӷ���
					//									0 = ������
					// 0Bx��C_COMP_INT��1 = ����Ƭ�ϼ������ݵĲ������Ʋ��������油����
					//									0 = ������
					// 0Bx��C_COMP_R��1 = �����������������ʻ��䲹�����ƣ�
					//								0 = ������
					// 0Bx��C_COMP_FORCE��1 = ������������Ļ�е�����������
					//											0 = ������
					// 0Bx��C_DIFFERENTIAL��0 = ���˴�����
					//											1 = ��ִ�����
					// 0Bx��C_FLOATING��0 = �ӵز���
					//									1 = ��������
					0x0C,   // �Ĵ���11��
					// 0Bxx���գ�00��
					// 0Bx��C_PORT_PAT�������˿ڵ�˳����ÿ�����к�ߵ���
					//									0 = ����
					//									1 = ˳�������ÿ���˿ڵ�˳��ߵ�
					// 0Bx��C_SELFTEST���������ڲ���������Լ죻
					//									0 = �ر�
					//									1 = ת���Ĵ���10�еĻ�е������ֵC_COMP_FORCE��
					// 0Bxx��CY_CLK_SEL��ѡ��CDC��ʱ��Դ��
					/*									00 = OLF_CLK
															01 = ��ֹ
															10 = OHF_CLK/4
															11 = OHF_CLK
					*/
					// 0Bx��CY_PRE_LONG�������ڲ�ʱ��·��֮��İ�ȫ�ӳ٣�
					//									0 = �ر�
					//									1 = ����
					// 0Bx��C_DC_BALANCE�����ִ�������ֱ��״̬���������������������߲��������
					//										0 = �ر�
					//										1 = ����
          0x03,   // �Ĵ���12��ʹ��CDC�˿ڣ�
					// 0Hxx��C_PORT_EN����λ����CDC�˿�; 
					/*	16����					00 = ȫ���ر�
														01 = PC0
														02 = PC1
														04 = PC2
														08 = PC3
														10 = PC4
														20 = PC5
														40 = PC6
														80 = PC7
						PC0~PC3��4λ��PC4~PC7��4λ���򿪶�ͨ����Ӽ���
						���磬��PC0��PC1��Ϊ01+02=03
									��PC0,PC1,PC3,Ϊ01+02+08=0B
					*/
					0x01, 0x00,  // �Ĵ���13 & �Ĵ���14��
					// 0Bxxx���գ�000��14�ĸ�3λ
					// 0Bxxxxxxxxxxxxx��C_AVRG���Ĵ���13��ʼ��13λ����һ��ƽ�����������Ĳ���������
					/*							0 or 1 = ��ƽ��
													2 = 2�����ƽ��
													3 = 3�����ƽ��
													���8191���������ȡƽ��
					*/
          0x00, 0x00,  // �Ĵ���15 & �Ĵ���16����DSP����
					// 0Bxxx���գ�000��16�ĸ�3λ
					// 0Bxxxxxxxxxxxxx��C_AVRG_ALT��15��ʼ��13λ���õڶ���ƽ�����������Ĳ���������
					/*							0 or 1 = ��ƽ��
													2 = 2�����ƽ��
													3 = 3�����ƽ��
													���8191���������ȡƽ��
					*/
/*************************************** ����ת������ **************************************************************/
					0x0A, 0x00, 0x00, // �Ĵ���17���Ĵ���18&�Ĵ���19��
					// 0Bx���գ�0��19�����λ
					// 0Bxxxxxxxxxx...��CONV_TIME��17��ʼ��23λ��ת������ת�����ڣ�Tcon=2*CONV_TIME/fOLF,
					//								Tcon����С�ڵ��β���������

					0x00, 0x00, 0x00, // �Ĵ���20���Ĵ���21&�Ĵ���22��
					// 0Bx���գ�0��22�����λ��
					// 0Bxxxxxxxxxx...��CONV_TIME_ALT��20��ʼ��23λ���ڶ�������ת�����ڣ�DSP�������ֲ���ģʽ��ͬ�ϣ�
					
					0x02 ,  // �Ĵ���23���ŵ���������Ĵ���24��2λ
					// 0Hxx��DISCHARGE_TIME��h00��죬���ȶ������ϵ�����
					//					�ŵ�ʱ��Tdischarge = ��DISCHARGE_TIME + 1��*Tcycleclock
					//					0 �� Tdischarge = 1*Tcycleclock
					0xCC,		// �Ĵ���24��GPIO3���������ش���CDC������
					// 0Bxx��C_STARTONPIN��ѡ������ͨ��Ӳ������CDC��GPIO�˿ڣ�
					/*										00 = PG0
																01 = PG1
																10 = PG2
																11 = PG3					
					*/
					// 0Bxx��C_TRIG_SEL_ALT��ͬ�£���DSP����д��Ĵ�����
					// 0Bxx��C_TRIG_SEL��ѡ�����������CDC�Ĵ���Դ��
					/*									00 = ����ģʽ
															01 = ����ģʽ
															10 = ת��������ʱ��
															11 = GPIO�����ش�����Ӳ������
															���������ҪЩ�����룬ǿ�ƴ��������ǲ��ȶ�
					*/
					// 0Bxx��DISCHARGE_TIME��	�ӼĴ���23				
					0x01, 0x00,  // �Ĵ���25&�Ĵ���26��
					// 0Bxx���գ�00��26�ĸ�2λ
					// 0Bxxxx��C_FAKE����ʵ����ǰ��Ԥ�ȵļٲ�����������������Ҫ�Ĵ�������Ϊ0��
					// 0Bxxxxxxxxxx��PRECHARGE_TIME��25��ʼ��10λ��Ԥ��������
					//															0 = ������Ԥ���
					//				Ԥ���ʱ��Tprecharge =( PRECHARGE_TIME + 1) * Tcycleclock
          0x01, 0xC4,  // �Ĵ���27 & �Ĵ���28��
					// 0Bx��EE_SINGLE_WR_EN��28���λ��EEPROM������ַд�뱣������Ӱ���ȡ��
					//											0 = �������е�����д����
					//											1 = ����д��
					// 0Bx��EE_WR_EN��EEPROM����д�뱣������Ӱ���ȡ��
					//								0 = ������д������EEPROM
					//								1 = ����
					// 0Bx��EE_DISABLE������EEPROM��
					//									0 = EEPROMʹ��
					//									1 = ��ȫ���ã��޷����ж�����
					// 0Bx��EE_IFC_PRIO���ڲ�λ��Ԥ��0��
					// 0Bx��EE_WAKEUP_MODE��ΪEEPROMѡ���д����ʱ�䣻
					//											0 = 1.5���㶨ʱ������tOCF
					//											1 = 10΢��
					// 0Bx��EE_ON��0 = ������ʱ���ѣ�Ȼ��������������˯��״̬���������ʱ
					//						1 = ��Զ����EEPROM
					// 0Bxxxxxxxxxx��FULLCHARGE_TIME��27��ʼ��28��2λ��10λ�����������ʱ�䣻
					//					����ʱ��Tfullcharge = (FULLCHARGE_TIME + 1) * Tcycleclock
					//					0 := no fullcharge								

/********************************* RDC���� ***********************************************/				
					
					0x00,   // �Ĵ���29��RDC������
					// 0Bxx��R_STARTONPIN�����Ƽ�ʹ�ã�00��
					// 0Bxxx��R_TRIG_SEL_ALT��ͬ�£�DSPѡ������ģʽ��
					// 0Bxxx��R_TRIG_SEL��RDC�Ĵ�����Դѡ��
					/*									000 = �ر�
															001 = OLF_CLK
															011 = Ӳ���ܽŴ���
															101 = ��CDC�첽����
															110 = ��CDCת��������ͬ������
					*/
          0xFF, 0xFF, 0x3F, // �Ĵ���30���Ĵ���31&32��RDC���CDC�������ʣ�
					// 0Dx��R_TRIG_PREDIV����30��22λ��Ԥ��Ƶ����������бȵ��ݲ������ٵ��¶Ȳ�����
					//										����ڼĴ���29����OLFʱ��Ϊ����Դ�������ΪOLF�ķ�Ƶ��
					/*										0 or 1 = ÿһ��CDC�����󴥷�
																2 = û����CDC�����󴥷�
																......
																���2��21�η�����Ƶ
					*/
					// 0Bxx��R_AVRG���Ĵ���32��2λ��RDCƽ����������
					/*							00 = ��ƽ��
													01 = 4��������
													10 = 8��������
													11 = 16��������
					*/
          0x00,   // �Ĵ���33��RDC�ٲ�������
					// 0Bx��R_SENSE_INVERT��0��
					// 0Bx��R_FAKE����ʵ����ǰ��Ԥ�ȼٲ���������
					//							0 = ÿ��ƽ����2�μٲ���
					//							1 = ÿ��ƽ����8�μٲ���
					// 0Bxxxxxx��R_QHA_SEL���ڲ�λ��000000��
					0x40,   // �Ĵ���34��
					// 0Bx���գ�0��
					// 0Bx��R_REF_SEL��ѡ��RDC�Ĳο���
					/*								0 = �ⲿPT0
														1 = �ڲ�
					*/
					// 0Bx��R_3EXT_SEL�����ҽ���ʹ���ڲ��ο�ʱ���������3���ⲿ��������
					/*									0 = ����3���ⲿ������������ʹ���ⲿ�ο�
															1 = 3���ⲿ��������ֻ��ʹ���ڲ��ο�
					*/
					// 0Bx��R_PT1_EN��0 = ����PT1
					//								1 = ʹ��PT1
					// 0Bx��R_PT0_EN��0 = ����PT0
					//								1 = ʹ��PT10
					// 0Bx��R_PT2REF_EN��0 = ����PT2REF
					//									1 = ʹ��PT2REF
					// 0Bx��R_PORT_EN_IMES��0 = �����ڲ��¶ȴ�����
					//											1 = ʹ���ڲ��¶ȴ�����
					// 0Bx��R_PORT_EN_IREF��0 = �����ڲ��ο�����
					//											1 = ʹ���ڲ��ο�����
					
					
/****************************** �����ڲ��αȵ��ݴ�С *********************************/
					0x03,   // �Ĵ���35��
					// 0Bxxxxx��C_REF_SEL�����ڲ��ο����ݴ�С������0.3pF~1.5pF�����Ƚϵͣ�
					//										0D0~0D31 = 0~31pF
					// 0Bx��R_CY��RDC���ֵ�ѭ��ʱ�䣻
					//						0 = 140΢��
					//						1 = 280΢��
					// 0Bxx��R_OLF_DIV��RDCʱ�ӷ�Ƶ��Ҫ��RDCʱ��Ƶ������Ϊ10kHz��
					/*								00 = /1 (for OLF = 10kHz)
														01 = /4 (for OLF = 50kHz)
														10 = /8 (for OLF = 100kHz)
														11 = /16 (for OLF = 200kHz)
					*/
          0x04,   // �Ĵ���36��ѡ��ʵʱʱ��Դ��ʹ�ܣ�
					// 0Bxxx���գ�000��
					// 0Bx��RTC_CLK_SEL��ѡ��ʵʱʱ��Դ����Ҫ�ⲿ32kHz�����ڲ�ʱ�Ӳ���ȷ��
					//									0 = OLFʱ��
					//									1 = �ⲿHF
					// 0Bx��RTC_EN��0 = �ر�
					//							1 = ����ʵʱʱ��
					// 0Bx��LBD_CLK_SEL����ص͵�������ʱ��Դѡ��
					//									0 = OLF clock
					//									1 = OLF clock / 16
					// 0Bx��TDC_NOISE_CY_DIS���ڲ�λ��0��
					// 0Bx���գ�0��

					0x00,   // �Ĵ���37���ڲ�Ĭ��00��
					
					0x34,   // �Ĵ���38��TDC�ڲ�Ĭ��0h34��
					// 0Bxxxxxx��TDC_QHA_SEL���ڲ�λ��0D13;
					// 0Bxx���գ�00��
					
          0x00,   // �Ĵ���39���ڲ�Ĭ��00��
					
/********************************* DSP���� **************************************/
					0x00,   // �Ĵ���40��DSP���ã�����ʱ����������ٶȣ�
					// 0Bxx��DSP_MOFLO_EN����GPIO��������ʹ�ܵ���̬��������
					//										00 = �ر�
					//										11 = ����
					// 0Bxx��DSP_CLK_MODE��ѡ��DSPʱ��Դ��
					/*										00 = ��������
																01 = ����ʱ��
																11 = ����ʱ��
					*/
					// 0Bxx��DSP_SPEED��DSP�ٶ����ã�
					//									00 = ���
					//									11 = ����
					// 0Bx��PG1xPG3��0 = PDM/PWM ���������ΪPG3
					//							1 = PDM/PWM ���������ΪPG1
					// 0Bx��PG0xPG2��0 = PDM/PWM ���������ΪPG2
					//							1 = PDM/PWM ���������ΪPG0
					0x00,   // �Ĵ���41�����Ź�ʱ�����ã�
					// 0Hxx��WD_TIME��OCF����Ϊ5kHz�����Ź�ʱ��Ϊ50����ı�����
					//								twatchdog = WD_TIME * 50 ms
          0x00,   // �Ĵ���42��DSP GPIOʹ�ܺͷ�ת��
					// 0Bxxxx��DSP_STARTONPIN������GPIO��������DSP��
					//												��λ����PG0~PG3
					// 0Bxxxx��DSP_FF_IN�������Ĵ�����
					//									��λ����DSP_IN_0~DSP_IN_3
					0x44,   // �Ĵ���43���ڴ�����ΪSRAM��DSP��ʱ��������
					// 0Bx��INT2PG2����PG2����Ϊһ��������ж϶˿�INTN��
					//							0 = ��ʹ��INTN������Ϊ�ⲿ�ж����
					//							1 = ����PG2��Ϊ�ⲿ�ж����
					// 0Bx��DSP_SPRAM_SEL��ѡ��SRAM��OTPΪ����洢��Ԫ��
					//										0 = OTP
					//										1 = SRAM
					// 0Bx��DSP_START��0 = ֱ��ͨ���ӿڴ���DSP
					//								1 = �ⲿ����DSP
					// 0Bxxxxx��DSP_START_EN����������DSP�ĸ��ִ������������룻
					/*												0 = ��CDCת��������
																		1 = ��ʱ��
																		2 = RDCת��������
																		3 = �ⲿ�ж�������
																		4 = ǰ�˷�������
					*/
					0x00,   // �Ĵ���44��PDM/PWM����ʱ�����ã��رգ�
					// 0Bxxxx��PI1_CLK_SEL������ɷ�1
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
					// 0Bxxxx��PI0_CLK_SEL������ɷ�0��ͬ��
          0x00,   // �Ĵ���45��
					// 0Bxxxx��PI_EN��ʹ��PDM/PWM�����������ɷ�ʽ;
					/*								b'xx01 = PWM at path 0
														b'xx10 = PDM at path 0
														b'01xx = PWM at path 1
														b'10xx = PDM at path 1
					*/
					// 0Bxx��PI1_RES����������λ��;
					/*								0 = 10 bit
														1 = 12 bit
														2 = 14 bit
														3 = 16 bit
					*/
					// 0Bxx��PI0_RES����������λ����ͬ�ϣ�
					
					
					
					0xF7,   // �Ĵ���46������PG0~PG3Ϊ���������������ڲ������������裻
					// 0Bxxxx��PG_DIR_IN������������֮���л�ͨ�ö˿ڷ���
					/*							0 = ���												1 = ����
							 0000 OR 0100 = PG0���				 1000 OR 1100 = PG0����
							 0001 OR 0101 = PG1���				 1001 OR 1101 = PG1����
							 0010 OR 0110 = PG2���				 1010 OR 1110 = PG2����
							 0011 OR 0111 = PG3���				 1011 OR 1111 = PG3����
					*/
					// 0Bxxxx��PG_PULLUP����������������
					/*							0 = ����												1 = ����
							 0000 OR 0100 = PG0����				 1000 OR 1100 = PG0����
							 0001 OR 0101 = PG1����				 1001 OR 1101 = PG1����
							 0010 OR 0110 = PG2����				 1010 OR 1110 = PG2����
							 0011 OR 0111 = PG3����				 1011 OR 1111 = PG3����
					*/
					
					
					
					0x00,   // �Ĵ���47������Ĭ�ϣ�
					// 0Bxx���գ�00��
					// 0Bx��BG_PERM�� 1 = ���ü����϶
					//								0 = ���弤���϶
					// 0Bxxxxx��BG_TRIM0�������ڲ���϶��Դʹ������������
					/*								h00 = 1.68V
														h07 = 1.8V
														h1F = 2.2V
					*/
          0x07,   // �Ĵ���48������Ĭ�ϣ�
					// 0Bx��TDC_NOISE_DIS���ڲ�λ��0��
					// 0Bxx��TDC_MUPU_SPEED��TDC�����ٶȣ�01��
					// 0Bx��TDC_MR2��TDC���������Χѡ��
					//							0 = MR1
					//							1 = MR2
					// 0Bxxxx��BG_TRIM1�������ڲ���ص͵�������EEPROMд��ĺ��Ĵ�϶��ѹ��
					//									�Ƽ�ֵh07
					0x30,   // �Ĵ���49���ڲ�λ������Ĭ��ֵh30��

					0x00,  					  // Config Register 50,
          0x00,0x00,0x00,   // Config Register 51, 52, 53,
          0x00,0x00,0x00,   // Config Register 54, 55, 56,
          0x00,0x00,0x00,   // Config Register 57, 58, 59,
          0x00,0x00,0x00,   // Config Register 60, 61, 62,
          0x00,0x00,0x00,   // Config Register 63, 64, 65,
          0x00,0x00,0x00,   // Config Register 66, 67, 68,
          0x00,0x00,				// Config Register 69, 70,
					0x00,0x00,0x28,   // Config Register 71,72, 73������У�����ӣ�����Ĭ�ϣ�
          0x01,   					// Config Register 74,
          0x00,0x00,0x01};  // Config Register 75, 76��76��RunBit��
/****************************** ����һ��·�����Ĵ���������� ************************************************/
/*����һ�����ڲ�ģ�������ΪC0�ķ���*/
					

/********************************************** ����̼����� *******************************************************/
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
					
/********************* ����PCap02ʹ�õı��� ***********************************/

uint32_t  RES;
uint32_t  Res0_content;
uint32_t  STATUS;						
uint8_t   Res1_content[3];
uint8_t		Res_content[45];
float     Res4_content;
float     Capacitance_ratio;
uint8_t   Status_content[3];
uint8_t		RunBit;
/********************************************************************************/

/********************************************* �궨�� **********************************/

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
/********************************* �궨����� ********************************/

/******************************** ������ **********************************/
/*************************************************************************
  * @brief  ��Ұ��STM32F407��������Ʋ���дPCAP02AE����ת��оƬ
  * @param  ��
  * @retval ��
  ********************************************************************/
int main()
{
	//�ú���ѡ��Ҫִ�еĶ��������õ�ע�͵�
//	#define LOAD_FIRMWARE_ARRAY
//	#define LOAD_HEX_FIRMWARE_FILE
//	#define COMMUNICATION_TEST
//	#define Direct_R_W
	#define CLEAR_SRAM
	#define WRITE_SRAM
//	#define READ_SRAM
//	#define WRITE_EEPROM
	
	//����LED
	LED_GPIO_Config();
	
//	SysTick_Init();
	
	/* ���ô���1Ϊ��115200 8-N-1 */
	Debug_USART_Config();
  
//	printf("\r\n Start the test \r\n");

	//���Ƕ��ʽSRAM
	Embed_SRAM_Init();
	//ʹ��PCAP02_GPIO
	GPIOs_Init();
	//��ʼ��SPI
	SPI_PCAP02_Init();
	//PCAP02��Դ��λ
 	SPI_PCAP02_SendByte(PCap02_Power_Reset);
	
	LED_PURPLE;
//	printf("Using the 0x%02X Clock source \n", RCC_GetSYSCLKSource () );
//	printf("ClocksFrequency = %s\n", RCC_GetClocksFreq( SYSCLK) );
	
	/******************************** ���ع̼���hex�ļ� *********************************/
	// ���ⲿHEX�ļ����ع̼���,���ļ���ת��Ϊint������
	#ifdef LOAD_HEX_FIRMWARE_FILE  
   FILE * HexFirmwareFile;
   char c;
   hex_index = 0;
   int count_byte = 0;       // ÿ�е��ֽ�����
   int count_line = 0;       // ����
   int count_ff = 0;         // ��ĳЩ������ 0xFF ʱ�ж�
   int break_no_of_ff = 160; // ��10��160�� 0xFF ��, ��ֹ����
    
   HexFirmwareFile = fopen ("E:\\program\\keil STM32\\v1.0\\Output\\PCap02AE.hex","r"); // ��ֻ��ģʽ���ļ����ļ����ڿ�ͷ�ĺ��ﶨ��
   if (HexFirmwareFile==NULL) 
	 {
		 perror ("Error opening file");	
//		 printf("I couldn't open the file: (%d)%s \r\n", errno, strerror(errno) );
	 }//����Ƿ���سɹ�
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
      
       PicoCap_Hex_Firmware[hex_index] = i1 + i2;  // ���̼������ڴ�
      
       if (PicoCap_Hex_Firmware[hex_index]==0xFF)  // ����ǲ��ǵ�һ��0xFF��
       {
         count_ff++;
         if (count_ff==break_no_of_ff) break; // ����ѭ��
       }
       else count_ff=0;
        
       hex_index++;
       count_byte++;

       if (count_byte==16)                   // ���ÿ�н���
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
      // ��� SRAM 0..4096 Bytes 
      puts("|         | Clear complete SRAM memory (4k)");
      printf("|");
      fflush(stdout); 		//����ǿ���������������ĺ����������������
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
		
		


		
	/* SRAMд�롢��ȡ���ԣ�����PCAP02ͨ���Ƿ����� */
	#ifdef COMMUNICATION_TEST
		while(1)
	{
//		char 	SRAM_callback[1];
    // SPIͨ�Ų���
    uint16_t adr = 0x3FF;  // ��ַ��Χ��0~0xFFF,�� 0x7FF (2^11-1) 2048-1 ��ʼд��
    uint8_t dat = 0xCD;
		
		puts("Test the SRAM Communication");
		printf(" Writing 0x%02X to Address 0x%04X\n",dat,adr);

    Send_24Bit_Opcode(PCAP02_SPI, Write_To_SRAM, adr, dat);				//д��SRAM
				
		printf(" Reading Data is 0x%02X\n",Send_24Bit_Opcode(PCAP02_SPI, Read_SRAM, adr, 0xFF));	//����SRAM
	}
	#endif

			
/***********************************************************************/			
			//���Թ����У���SRAM���͹̼���������ɿ�����EEPROM��̼�
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
				
//-------------------------�ڴ˴����öϵ����ߵ���----------------------------
				
        fflush(stdout); 
			}
		}
	#endif     	
/***************************************************************/
		


/****************************** ���ر�׼�̼�����EEPROM ******************************/
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
	
/********************************* ��PCAP02д������ *********************************/

//while(1)
//	{
// д�����üĴ���֮ǰ���� RUNBIT
	puts("\nClear RUNBIT...\n");
  Write_RunBit(00);
	//                *bus_type,   command, 		address, *data, 			quantity
	
	//	��ȡ״̬�Ĵ���
//	SPI_PCAP02_Status();	
	 
  // д���Զ���PCap02 config
//  puts("Write Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_config_reg, sizeof(PCap02_config_reg));
		
	// д���׼PCap02 config
//  puts("Write Standard Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_standard_config_reg, sizeof(PCap02_standard_config_reg));
	
	// д���׼�޸ĵ�PCap02 config1
//  puts("Write modify Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_modify_config1_reg, sizeof(PCap02_modify_config1_reg));

	// д���׼�޸ĵ�PCap02 config2������ʱ��200kHz�����ˣ��ڲ����ݲ���
//  puts("Write modify Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_modify_config2_reg, sizeof(PCap02_modify_config2_reg));
	
	// д���׼�޸ĵ�PCap02 config3������ʱ��200kHz���������޵��ݲ���
//  puts("Write modify Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_modify_config3_reg, sizeof(PCap02_modify_config3_reg));
	
	// д���׼�޸ĵ�PCap02 config4������ʱ��200kHz���������ⲿ���ݲ���
//  puts("Write modify Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_modify_config4_reg, sizeof(PCap02_modify_config4_reg));

	// д���׼�޸ĵ�PCap02 config5������ʱ��200kHz�����ˣ��޲���
//  puts("Write modify Config...\n");
//  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_modify_config5_reg, sizeof(PCap02_modify_config5_reg));
	
	// д���׼�޸ĵ�PCap02 config6������ʱ��200kHz�����ˣ��޲���,ͨ��6
  puts("Write modify Config...\n");
  Write_Incremental(PCAP02_SPI, Write_Config, 0x00, PCap02_modify_config6_reg, sizeof(PCap02_modify_config5_reg));
		
  // RUNBIT ��1
  puts("Set RUNBIT...\n");
  Write_RunBit(01);
	
	//	��ȡ״̬�Ĵ���
//	 printf("STATUS is 0x%08X\r\n",SPI_PCAP02_Status());
	 
	//��ʼ��PCAP02
  SPI_PCAP02_SendByte(PCap02_Initialize);
	//��ʼ����
	SPI_PCAP02_SendByte(PCap02_CDC_Start_Conversion);
//			Wait_For_Interrupt();

	/********************** ���濪ʼ��ѭ�� *********************/
	while(1)
	{
		//��������봥��CDCת�����ݣ���ʼ����
//		SPI_PCAP02_SendByte(PCap02_CDC_Start_Conversion);
		//Ӳ��GPIO3�����ش���
//		SPI_PCAP02_PG3_HIGH();
		Wait_For_Interrupt();
		//	��ȡ״̬�Ĵ���
//		SPI_PCAP02_Status();	
		//��ȡ���
//		puts("Read Results...\n");
		/********* 1ͨ�������ȡ **********/
		//����Ĵ���1
//		Write_Incremental(PCAP02_SPI, Read_Results, 0x03, Res1_content, 3);
		//����Ĵ���2
//		Write_Incremental(PCAP02_SPI, Read_Results, 0x06, Res1_content, 3);
		//����Ĵ���+3
//		Write_Incremental(PCAP02_SPI, Read_Results, 0x09, Res1_content, 3);
		/********* 6ͨ�������ȡ **********/
		Write_Incremental(PCAP02_SPI, Read_Results, 0x12, Res1_content, 3);
    RES = (Res1_content[2] << 16) | (Res1_content[1] << 8) | Res1_content[0];
		
		// ��ȡ���н���Ĵ���
//		Write_Incremental(PCAP02_SPI, Read_Results, 0x00, Res_content, 45);
//    RES = (Res_content[5] << 16) | (Res_content[4] << 8) | Res_content[3];
//		SPI_PCAP02_PG3_LOW();
		//	��ȡ״̬�Ĵ���
//		SPI_PCAP02_Status();	
    Capacitance_ratio = RES / pow(2,21);
//			Capacitance_ratio = convertBinaryToDecimal(RES);
    printf("%f\n",  Capacitance_ratio);	
	LED_CYAN;
//		printf("  0x%08X\r, %f\n", RES, Capacitance_ratio);	
	}
	//���ҷ�����С�Ĳ������
//	find_var	iance_float(1000);
}
/***************************** main�������� **********************************/
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
  * @brief  main�������õ��Ӻ�����SPIͨ������Ӻ������������SPI_PCAP02.cԭ�ļ���
 **/
/*******************************************************************************
  * @brief  Ϊ�˼��ع̼��⣬��char��ת��Ϊint
  * @param  16����ASSCII��
  * @retval ����0~15
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
  else return 99; // ��������²�Ӧ�ó���
}
/************************************************************************************/


/*******************************************************************************
  * @brief  ��ʼ��SRAM
  * @param  ��
  * @retval ��
 ******************************************************************************/
void Embed_SRAM_Init(void)
{
 // Releasing reset for Embedded SRAM
  RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_BKPSRAM, ENABLE);
}


/*******************************************************************************
  * @brief  ȡ�����󷽲��������������������׼ȷֵ
  * @param  �󷽲��������
  * @retval ����
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
  
  for(i=0;i<sample_count;i++)
  {
    data = sram_float_memory[sram_mem_offset]; 
    sum = sum + data;
    sram_mem_offset ++; // For the sram_memory, the address increments by 4 everytime
  }
  
  mean = sum/sample_count;
  sram_mem_offset = 1000; // Using values 1000-2000 for Variance calculation
  
  for(j=0;j<sample_count;j++)
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



