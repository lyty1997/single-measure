/************************************************

  * @file    SPI_interface.h
  * @author  ½������
  * @version V1.0
  * @date    2020-09-23
  * @brief   SPI_PCAP02 �ײ�Ӧ��ͷ�ļ� 

****************************************************/


// �����ظ�����ͷ�ļ�
#ifndef SPI_PCAP02_H
#define SPI_PCAP02_H

// ���ͷ�ļ�
#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_rcc.h"
#include <stdio.h>


/************************ ����PCAP02��˽�б��� *************************/
#define LOOP_DLY_100US    2000
#define LOOP_DLY_250ns    2
#define NUMBER_OF_SAMPLES 100
#define FOUR_MILLISECONDS_TIMEOUT 4300
#define TWENTY_MILLISECONDS_TIMEOUT 20300

					
/****************************** PCap02 �Ĵ�����ַ + ������ *****************************/
//ֻ��I2C��Ҫ��SPI����Ҫ
// Standard Device Address Bute 0 | 1 | 0 | 1 | 0 | A1 | A0 | R/W 
				                       	//(R=read=1 and W=write=0)
#define   PicoCap_Device_Address_Read   0x41
#define   PicoCap_Device_Address_Write  0x40		// 4.2�½�
					
/******************** �ú��װ�����룬���ü�4.1.1 *************************************/
// ��ʼ��
#define   PCap02_Power_Reset          0x88 // ��Դ���帴λ
#define   PCap02_Initialize           0x8A // ��λ��ʼ��
#define   PCap02_CDC_Start_Conversion 0x8C // ����CDC����ת�����������
#define   PCap02_RDC_Start_Conversion 0x8E // ����RDC����ת��
#define   PCap02_Terminate_Write_OTP  0x84 // ��ֹ��OTPд������

// EEPROM Access
#define   Write_To_EEPROM      0xE0 // дEEPROM
#define   Read_EEPROM          0x60 // ��EEPROM
#define   Block_Write_EEPROM   0xE1 // ��д��
#define   Block_Erase_EEPROM   0xE3 // �����
#define   Erase_EEPROM         0xE2 // ����

// SRAM Access
#define   Write_To_SRAM   0x90 // дSRAM			
#define   Read_SRAM       0x10 // ��SRAM

// OTP Access
#define   Write_to_OTP    0xA0 // дOTP
#define   Read_OTP        0x20 // ��OTP  

// д������&��ȡ���
#define   Write_Config    0xC0 // д������
#define   Read_Results    0x40 // �������
/*************************************************************************************/


/************************* ��װ������Ҫ�Ĵ����ĵ�ַ *****************************/
#define		RunBit_address					0x4D
#define		Registers_0							0x00
#define		Results_address					0x03
#define		Statu_address						0x24


/****************************** Ӳ���жϿ��� ************************************/
//#define		


/********************** SPI�ӿڶ���-��ͷ ****************************/
//SPI �ż�ʱ�ӳ�ʼ������
#define PCAP02_SPI                           SPI1
#define PCAP02_SPI_CLK                       RCC_APB2Periph_SPI1
#define PCAP02_SPI_CLK_INIT                  RCC_APB2PeriphClockCmd
//SCK ����
#define PCAP02_SPI_SCK_PIN                   GPIO_Pin_5
#define PCAP02_SPI_SCK_GPIO_PORT             GPIOA                       
#define PCAP02_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define PCAP02_SPI_SCK_PINSOURCE             GPIO_PinSource5
#define PCAP02_SPI_SCK_AF                    GPIO_AF_SPI1
//MISO ����
#define PCAP02_SPI_MISO_PIN                  GPIO_Pin_6                
#define PCAP02_SPI_MISO_GPIO_PORT            GPIOA                   
#define PCAP02_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define PCAP02_SPI_MISO_PINSOURCE            GPIO_PinSource6
#define PCAP02_SPI_MISO_AF                   GPIO_AF_SPI1
//MOSI ����
#define PCAP02_SPI_MOSI_PIN                  GPIO_Pin_7                
#define PCAP02_SPI_MOSI_GPIO_PORT            GPIOA                     
#define PCAP02_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define PCAP02_SPI_MOSI_PINSOURCE            GPIO_PinSource7
#define PCAP02_SPI_MOSI_AF                   GPIO_AF_SPI1
//CS(NSS)����
#define PCAP02_CS_PIN               		   GPIO_Pin_4                
#define PCAP02_CS_GPIO_PORT          		   GPIOA                     
#define PCAP02_CS_GPIO_CLK            		 RCC_AHB1Periph_GPIOA
#define PCAP02_SPI_CS_PINSOURCE            GPIO_PinSource4					//����SPI1ģʽ����
//����CS(NSS)��������͵�ƽ
#define SPI_PCAP02_CS_LOW()      {PCAP02_CS_GPIO_PORT->BSRRH=PCAP02_CS_PIN;}
//����CS(NSS)��������ߵ�ƽ
#define SPI_PCAP02_CS_HIGH()     {PCAP02_CS_GPIO_PORT->BSRRL=PCAP02_CS_PIN;}

//Ӳ�����ƴ���PG3����
#define PCAP02_PG3_PIN										 GPIO_Pin_4
#define PCAP02_PG3_GPIO_PORT           		 GPIOC                     
#define PCAP02_PG3_GPIO_CLK            		 RCC_AHB1Periph_GPIOC
#define PCAP02_PG3_PINSOURCE 		      		 GPIO_PinSource4					
//Ӳ������PG3,����͵�ƽ
#define SPI_PCAP02_PG3_LOW()		 {PCAP02_PG3_GPIO_PORT->BSRRH=PCAP02_PG3_PIN;}
//Ӳ������PG3,����ߵ�ƽ
#define SPI_PCAP02_PG3_HIGH()    {PCAP02_PG3_GPIO_PORT->BSRRL=PCAP02_PG3_PIN;}

//����PCAP02���ⲿ�ж�INTN����
#define PCAP02_INTN_PIN										 GPIO_Pin_5
#define PCAP02_INTN_GPIO_PORT           	 GPIOC                     
#define PCAP02_INTN_GPIO_CLK            	 RCC_AHB1Periph_GPIOC
#define PCAP02_INTN_PINSOURCE 		      	 GPIO_PinSource5

// SPI ʹ��IIC_EN����
#define PCAP02_IIC_EN_PIN										 GPIO_Pin_1
#define PCAP02_IIC_EN_GPIO_PORT           	 GPIOB                     
#define PCAP02_IIC_EN_GPIO_CLK            	 RCC_AHB1Periph_GPIOB
#define PCAP02_IIC_EN_PINSOURCE 		      	 GPIO_PinSource1
/************************* SPI�ӿڶ���-��β ****************************/


/*************************** �ȴ���ʱʱ�� ******************************/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/***************************** ��Ϣ��� *******************************/
#define PCAP02_DEBUG_ON         1

#define PCAP02_INFO(fmt,arg...)           printf("<<-PCAP02-INFO->> "fmt"\n",##arg)
#define PCAP02_ERROR(fmt,arg...)          printf("<<-PCAP02-ERROR->> "fmt"\n",##arg)
#define PCAP02_DEBUG(fmt,arg...)          do{\
                                          if(PCAP02_DEBUG_ON)\
                                          printf("<<-PCAP02-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)


/******************************** ����˽�к��� ***********************************/
int convert_char2int(int char_input);
void Embed_SRAM_Init(void);
float find_variance_float(uint16_t sample_count);
int convertBinaryToDecimal(long long n);

/************************* SPI������� **************************************/
void GPIOs_Init(void);
void Wait_For_Measurement(void);
void SPI_PCAP02_Init(void);
uint8_t SPI_PCAP02_ReadByte(void); 
void SPI_PCAP02_SendByte(uint8_t byte);
uint32_t SPI_PCAP02_Status(void);
uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);
uint8_t SPI_PCAP02_Readdata(void);

//******************** ��������,���ֽ���д������ **********************
void Write_Incremental(void* bus_type, uint8_t command,
                       uint16_t address, uint8_t *data, uint8_t quantity);
//******************** ��������,��3�ֽ�24λ��д������ **********************
uint8_t Send_24Bit_Opcode(void* bus_type, uint8_t command, uint16_t address, uint8_t data);
//******************** ��������,д��RunBitλ,������ **********************
void Write_RunBit(uint8_t data);
//������
void Direct_R_and_W (void);
uint8_t TraPar ( uint8_t PutIn);
																
#endif /* SPI_PCAP02_H */


