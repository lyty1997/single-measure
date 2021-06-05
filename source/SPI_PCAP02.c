/*****************************************************************************

  * @file    SPI_interface.c
  * @author  ½������
  * @version V1.0
  * @date    2020-09-23
  * @brief   SPI_PCAP02 �ײ�Ӧ�ú��� 

******************************************************************************/


/************* ���ͷ�ļ� ******************/ 
#include "SPI_PCAP02.h"
#include "stm32f4xx_gpio.h"
#include <math.h>



/******************** ��̬Ԥ����궨�� **********************/
#define Dummy_Byte							0xFF           //Ϊ�˲���ʱ�ӷ��͵ļ�����


/****************************************************************************/
static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT; 
 
// static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

/************************** ����ȫ�ּ������� *****************************************/
int         i;
int         j;

					
		
/******************************������SPI��ص�ȫ�ֺ���*************************************/


/********************************************************
  * @brief  �ȴ���ʱ�ص�����
  * @param  None.
  * @retval None.
  *********************************************************
uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  // �ȴ���ʱ��Ĵ���,���������Ϣ 
  PCAP02_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}
*/
/**************************************************************************/



/****************************************************************************/


 /***************************************************************************
  * @brief  ��Ƭ��ʱ�ӳ�ʼ��
  * @param  ��
  * @retval ��
  ***************************************************************************/
/*
void RCC_Init (void)
{
//	uint8_t HSEStartUpStatus = 0;
		//	����ϵͳʱ�� 
	RCC_DeInit ();
	//��ձ�־λ
	RCC_ClearFlag  ();
	RCC_ClearITPendingBit  (RCC_IT_LSERDY | RCC_IT_HSERDY | RCC_IT_PLLRDY);
	RCC_ClockSecuritySystemCmd  (ENABLE); 
	
	//�����ⲿʱ��Դ
	RCC_HSEConfig (RCC_HSE_ON);
//	HSEStartUpStatus = RCC_WaitForHSEStartUp ();
//	SPITimeout = SPIT_FLAG_TIMEOUT;
//  while (HSEStartUpStatus == RESET)
//   {
//    if((SPITimeout--) == 0)  SPI_TIMEOUT_UserCallback(2);
//   }
//	printf("HSE configur %s\n!",RCC_WaitForHSEStartUp ());
	//����AHBʱ��
	
	RCC_LSEConfig (RCC_LSE_ON);
	 SPITimeout = SPIT_FLAG_TIMEOUT;
  while (RCC_GetFlagStatus  (RCC_FLAG_LSERDY) == RESET)
   {
    if((SPITimeout--) == 0)  SPI_TIMEOUT_UserCallback(2);
   }
	 
	 //ʹ�����໷
	 RCC_PLLCmd  (ENABLE);
	 	SPITimeout = SPIT_FLAG_TIMEOUT;
  while (RCC_GetFlagStatus  (RCC_FLAG_PLLRDY) == RESET)
   {
    if((SPITimeout--) == 0)  SPI_TIMEOUT_UserCallback(2);
   }
	 
	RCC_SYSCLKConfig (RCC_SYSCLKSource_PLLCLK); 
	RCC_HCLKConfig (RCC_SYSCLK_Div1);
	while(RCC_GetSYSCLKSource() != 0x08)
	 {
			SPITimeout = SPIT_FLAG_TIMEOUT;
    if((SPITimeout--) == 0)  SPI_TIMEOUT_UserCallback(2);
   }
	 RCC_PCLK2Config (RCC_HCLK_Div2);

}
*/
/****************************************************************************/

/***************************************************************************
  * @brief  GPIO��ʼ��
  * @param  ��
  * @retval ��
  ***************************************************************************/
void GPIOs_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
			 
	// GPIOʹ�� 
	RCC_AHB1PeriphResetCmd (PCAP02_SPI_SCK_GPIO_CLK | PCAP02_SPI_MISO_GPIO_CLK|
	                        PCAP02_SPI_MOSI_GPIO_CLK| PCAP02_CS_GPIO_CLK | 
													PCAP02_PG3_GPIO_CLK | RCC_AHB1Periph_GPIOD,DISABLE);
  RCC_AHB1PeriphClockCmd (PCAP02_SPI_SCK_GPIO_CLK | PCAP02_SPI_MISO_GPIO_CLK|
	                        PCAP02_SPI_MOSI_GPIO_CLK| PCAP02_CS_GPIO_CLK | 
													PCAP02_PG3_GPIO_CLK | RCC_AHB1Periph_GPIOD, ENABLE);

  //!< SPI_PCAP02_SPI ʱ��ʹ�� 
	RCC_APB2PeriphResetCmd (RCC_APB2Periph_SPI1,DISABLE);
  PCAP02_SPI_CLK_INIT(PCAP02_SPI_CLK, ENABLE);
 
  //�������Ÿ���
  GPIO_PinAFConfig(PCAP02_SPI_SCK_GPIO_PORT,PCAP02_SPI_SCK_PINSOURCE,PCAP02_SPI_SCK_AF); 
	GPIO_PinAFConfig(PCAP02_SPI_MISO_GPIO_PORT,PCAP02_SPI_MISO_PINSOURCE,PCAP02_SPI_MISO_AF); 
	GPIO_PinAFConfig(PCAP02_SPI_MOSI_GPIO_PORT,PCAP02_SPI_MOSI_PINSOURCE,PCAP02_SPI_MOSI_AF); 
//		GPIO_PinAFConfig(PCAP02_CS_GPIO_PORT,PCAP02_SPI_CS_PINSOURCE,PCAP02_SPI_CS_AF); 		//����Ӳ�������ǵ�ע�͵�
  
  //!< ���� SPI_PCAP02_SPI ����: SCK 
  GPIO_InitStructure.GPIO_Pin = PCAP02_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  GPIO_Init(PCAP02_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	//!< ���� SPI_PCAP02_SPI ����: MISO 
  GPIO_InitStructure.GPIO_Pin = PCAP02_SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(PCAP02_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
	//!< ���� SPI_PCAP02_SPI ����: MOSI 
  GPIO_InitStructure.GPIO_Pin = PCAP02_SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(PCAP02_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);  

	//!< ���� SPI_PCAP02_SPI ����: CS 
  GPIO_InitStructure.GPIO_Pin = PCAP02_CS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;				//����
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP��					//����
  GPIO_Init(PCAP02_CS_GPIO_PORT, &GPIO_InitStructure);
	
/************************************************************************/
// PG3Ӳ������ת��
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  //�����ش���������
  GPIO_InitStructure.GPIO_Pin = PCAP02_PG3_PIN;
  GPIO_Init(PCAP02_PG3_GPIO_PORT, &GPIO_InitStructure);
	
//����PCAP02���ⲿ�ж�INTN
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//��������
  GPIO_InitStructure.GPIO_Pin = PCAP02_INTN_PIN;
  GPIO_Init(PCAP02_INTN_GPIO_PORT, &GPIO_InitStructure);

// SPI ʹ��IIC_EN
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Pin = PCAP02_IIC_EN_PIN;
  GPIO_Init(PCAP02_IIC_EN_GPIO_PORT, &GPIO_InitStructure);
  // �õ�ʹ��SPI
  GPIO_WriteBit(PCAP02_IIC_EN_GPIO_PORT, PCAP02_IIC_EN_PIN, Bit_RESET); // SPI1 - PD3
	
// ֹͣ�ź� PCAP02: CS���Ÿߵ�ƽ
  SPI_PCAP02_CS_HIGH();
}
/****************************************************************************/

 /***************************************************************************
  * @brief  SPI_PCAP02��ʼ��
  * @param  ��
  * @retval ��
  ***************************************************************************/
void SPI_PCAP02_Init(void)
{
  
/********************************************************************/
	SPI_InitTypeDef  SPI_InitStructure;
  // PCAP02_SPI ģʽ���� 
  // PCAP02оƬ��SPIģʽ���ã�CPOL=0 CPHA=1 �� Mode1 , D0RD=0,MSB first
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	//���ݳ���8λ��PCAP02����Ĵ�����24λ��407����ÿ8λ��ȡ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; //����ʱ���ٶ�
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(PCAP02_SPI, &SPI_InitStructure);

	
  // ʹ�� PCAP02_SPI  
  SPI_Cmd(PCAP02_SPI, ENABLE);
}
/**********************************************************************/



/**************************************************************
  * @brief  ʹ��SPI��ȡһ���ֽڵ�����
  * @param  none
  * @retval ���ؽ��յ�������
  *************************************************************/
uint8_t SPI_PCAP02_ReadData(void)
{
	/* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE) == 0)	{}
		/* �����������ֽڲ���ʱ������ */
  SPI_I2S_SendData(PCAP02_SPI, 0x00);
  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
	return SPI_I2S_ReceiveData(PCAP02_SPI);
}


/**************************************************************
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�8λһ�ֽ�����
  * @retval none
  *************************************************************/
void SPI_PCAP02_SendByte(uint8_t byte)
{
	/* CS �õͿ�ʼͨ�� */
  SPI_PCAP02_CS_LOW();
	
  while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE) == 0)	{}
  SPI_I2S_SendData(PCAP02_SPI, byte);

  while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
	SPI_I2S_ReceiveData(PCAP02_SPI);

	/* ֹͣͨ�� CS �ø� */
		while(SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_BSY)==1){}
  SPI_PCAP02_CS_HIGH();
}



/**************************************************************
  * @brief  ʹ��SPI��ȡPCAP02��״̬��־�Ĵ���������CDC��ǰ��״̬��������
  * @param  none
  * @retval ����״̬��־λ
  *************************************************************/
uint32_t SPI_PCAP02_Status(void)
{
	uint32_t  STATUS;	
	uint8_t   Status_content[3];
//	uint8_t		RunBit;
//	��ȡ״̬�Ĵ���
	Write_Incremental(PCAP02_SPI, Read_Results, 0x24, Status_content, 3);
	STATUS = Status_content[2];
	STATUS = STATUS<<8 | Status_content[1];
	STATUS = STATUS<<8 | Status_content[0];
	//	��ʾRunBit��״̬
//	RunBit = Status_content[0];
//	 printf("STATUS is 0x%08X\r\n RunBit is 0x%02X\n", STATUS, RunBit);	
	return STATUS;
}



/**************************************************************
  * @brief  ��װһ��ʹ��SPI����24λ�������ݵĺ���
  * @param  bus_type��PCAP02_SPI���Ѿ��ں����涨��
						command��ָ�������
						address����Ӧ�ļĴ�����ַ
						data��Ҫд�������
  * @retval ���ؽ��յ�������
  *************************************************************/
uint8_t Send_24Bit_Opcode( void* bus_type, uint8_t command, uint16_t address, uint8_t data)
{ 
	uint8_t ReadData = 0;
  uint8_t Byte_0  = data;    
  uint8_t Byte_1  = address; 	//��ַ��8λ
//	uint8_t opcode = command & 0xF0;	//ָ�������
  uint8_t Byte_2  = command | (address>>8);	//ָ�������+��4λ��ַ
	
  // SPIx ���ݴ��� ------------------------------------------------------------
	SPI_PCAP02_CS_LOW();	// CS�õͣ���ʼͨ��
  //���Ͳ�����ָ��ͼĴ�����ַ����ִ�к�����д�����	 
  while (SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_TXE)==0)	{}    
		SPI_I2S_SendData(bus_type, Byte_2);   // ���� byte 2������
	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
		SPI_I2S_ReceiveData(bus_type);

  while (SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_TXE)==0) {}			 
		SPI_I2S_SendData(bus_type, Byte_1);     // ���� byte 1�Ĵ�����ַ
	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
		SPI_I2S_ReceiveData(bus_type);

	//--------------------------------------------------------------------------

	if ((command==0x90) || (command==0xA0)) // �����дָ��
		{
			while (SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_TXE)==0) {}	 
			SPI_I2S_SendData(bus_type, Byte_0);     // ���� byte 0
			while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
		  SPI_I2S_ReceiveData(bus_type);
		}
		
	if ((command==0x10) || (command==0x20)) // ����Ƕ�SRAM/OTPָ��
		{
			ReadData = SPI_PCAP02_ReadData();		//SRAM ǰ����1�����ֽ�
//			ReadData = SPI_PCAP02_ReadData();
		}
	// CS�øߣ�����ͨ�ţ���SPI��æ���ȴ���������ǰ����
	while(SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_BSY)==1){}
  SPI_PCAP02_CS_HIGH();	
		return ReadData;
}


/************************************************************
	* @brief  ʹ��SPI���ֽ��𲽷������ⳤ�ȵ�����
  * @param  bus_type��PCAP02_SPI���Ѿ��ں����涨��
						command��ָ�������
						address����Ӧ�ļĴ�����ַ
						data��Ҫд�������
						quantity��Ҫд������ݵĴ�С
  * @retval 
  *************************************************************/
void Write_Incremental(void* bus_type, uint8_t command, uint16_t address, uint8_t *data, uint8_t quantity)
{
  int8_t    i = 0;
//  uint8_t    Result_read = 0;

  // SPIx ���ݴ��� ------------------------------------------------------------
	SPI_PCAP02_CS_LOW();	// CS�õͣ���ʼͨ��
		 
	while (SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_TXE)==0)	{}    
	SPI_I2S_SendData(bus_type, command);   // ���� byte 2������
		while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
		SPI_I2S_ReceiveData(bus_type);
			 
  while (SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_TXE)==0) {}			 
	SPI_I2S_SendData(bus_type, address);     // ���� byte 1�Ĵ�����ַ
		while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
		SPI_I2S_ReceiveData(bus_type);
	
	if ((command==0x90) || (command==0xA0) || (command==0xC0)) // �����дָ��
	{
	// ��BYTE0��λ��һ�ֽ�һ�ֽڷ���data
		do{
				while (SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_TXE)==0) {}			 
				SPI_I2S_SendData(bus_type, data[i]); 
					while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
		SPI_I2S_ReceiveData(bus_type);
				i++;
			}
		while(i<quantity);
	}
//----------------------------------------------------------------------------

	if ((command==0x10) || (command==0x20)) // ����Ƕ�SRAM/OTPָ��
	{
			SPI_PCAP02_ReadData(); //  ��SRAM/OTP���ݣ�������8λ���ݣ�
			data[0] = SPI_PCAP02_ReadData();
	}

//--------------------------------------------------------------------------
	if (command==Read_Results) // ����Ƕ�ȡ�����ָ��
	{
		do{
					data[i] = SPI_PCAP02_ReadData();
				i++;
      }
		while(i<quantity);
//			while(i<12);
	}
	while(SPI_I2S_GetFlagStatus(bus_type, SPI_I2S_FLAG_BSY)==1){}
  SPI_PCAP02_CS_HIGH();	// CS�øߣ�����ͨ��
}



/************************************************************
	* @brief  ʹ��SPIд��RunBitλ
  * @param  
  * @retval none
  *************************************************************/
void Write_RunBit(uint8_t data)
{
  // SPIx ���ݴ��� ------------------------------------------------------------
	SPI_PCAP02_CS_LOW();	// CS�õͣ���ʼͨ��
		 
	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0)	{}    
	SPI_I2S_SendData(PCAP02_SPI, Write_Config);   // ���� byte 2������
	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
	SPI_I2S_ReceiveData(PCAP02_SPI);
			 
  while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {}			 
	SPI_I2S_SendData(PCAP02_SPI, 77);     // ���� byte 1�Ĵ�����ַ
	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
	SPI_I2S_ReceiveData(PCAP02_SPI);
//----------------------------------------------------------------------------

	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {}			 
	SPI_I2S_SendData(PCAP02_SPI, data); 
	while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE) == 0)	{}
	SPI_I2S_ReceiveData(PCAP02_SPI);
		
//		printf("%d\n", SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_OVR));

	while(SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_BSY)==1){}
  SPI_PCAP02_CS_HIGH();	// CS�øߣ�����ͨ��
}





/********************************************************
	* @brief INTN�жϣ�ÿ��ת����INTN���Ų���һ���½����жϣ�
					 �����ж��Ƿ�ת��
  * @param  
  * @retval Wait_For_Measurement
**********************************************************/
void Wait_For_Measurement(void)
{
	while (GPIO_ReadInputDataBit (GPIOC, GPIO_Pin_5)==1)
	{	}
}
/**************************************************************/


/********************************************************
	* @brief �����ƽ��ת��Ϊʮ����
  * @param  
  * @retval 
**********************************************************/
int convertBinaryToDecimal(long long n)
{
  int decimalNumber = 0,i=0,remainder;
  while(n!=0){
    remainder = n%10;   //ȡ������
    n /=10;    //ȡ�޳����һλ������
    decimalNumber += remainder*pow(2,i); //1111,result = 1+1*2+1*2*2+1*2*2*2
    ++i;
  }
  return decimalNumber;
}
/**************************************************************/


/********************************************************
	* @brief  ֱ��д��Ͷ�������
  * @param  
  * @retval 
**********************************************************/
void Direct_R_and_W (void)
{
			uint8_t RE=0;
			uint16_t adr = 0x3FF;
			uint8_t dat = 0xCA;
			puts("Test the SRAM Communication");
      printf(" Writing 0x%02X at Address 0x%04X\n",dat,adr);
      												
			SPI_PCAP02_CS_LOW();
				
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {} 
				SPI_I2S_SendData(PCAP02_SPI, 0x93);  
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {}				 // д��EEPROM

//    	  SPI_PCAP02_CS_HIGH();
//				 
//				SPI_PCAP02_CS_LOW();
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {} 
			  SPI_I2S_SendData(PCAP02_SPI, 0xFF); 
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {}			 // ���͵�ַ

//			 SPI_PCAP02_CS_HIGH();
//			 
//			 SPI_PCAP02_CS_LOW();
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {} 
				SPI_I2S_SendData(PCAP02_SPI, dat); 
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {}				// ��������

     
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {} 

			  SPI_PCAP02_CS_HIGH();
//			
//			
			  SPI_PCAP02_CS_LOW();
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {} 
				SPI_I2S_SendData(PCAP02_SPI, 0x13);     // ��ȡEEPROM


//   		  SPI_PCAP02_CS_HIGH();
//				
//				SPI_PCAP02_CS_LOW();
			 while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==0) {} 
		 	 SPI_I2S_SendData(PCAP02_SPI, 0xFF);     // ���͵�ַ


			SPI_PCAP02_CS_HIGH();
		
			SPI_PCAP02_CS_LOW();
				while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE)==RESET) {};

  
     //Compulsory reads to DR and SR to clear OVR,
     //so that next incoming data is saved
     SPI_I2S_ReceiveData(PCAP02_SPI);                     // To clear OVR
     SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_TXE); // To clear OVR

     //Reading byte1
     SPI_I2S_SendData(PCAP02_SPI, 0xFF);  // DUMMY WRITE
					while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE)==0) {}

     // Wait until RX buffer is not empty, then read the received data
//					SPI_PCAP02_CS_HIGH();
//					
//					SPI_PCAP02_CS_LOW();
     while (SPI_I2S_GetFlagStatus(PCAP02_SPI, SPI_I2S_FLAG_RXNE)==0) {}

     RE = SPI_I2S_ReceiveData(PCAP02_SPI); //  Read

			 	SPI_PCAP02_CS_HIGH();

      printf(" Reading at Address 0x%04X, Data is 0x%02X\n\n", adr, RE );
		
}
/*****************************************************************************/


/********************************************************
	* @brief  ��ӡ���β���
  * @param  
  * @retval 
**********************************************************/
uint8_t TraPar ( uint8_t PutIn)
{
	uint8_t test = 0;
	test = 0xEB & PutIn;
	return test;
}
/**************************************************************/


